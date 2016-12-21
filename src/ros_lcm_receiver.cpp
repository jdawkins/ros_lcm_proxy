#include <ros/ros.h>
#include <stdio.h>
#include <algorithm>

#include <lcm/lcm-cpp.hpp>
#include "lcm_navmsgs/vehicle_state.hpp"
#include <sensor_msgs/MultiDOFJointState.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/Transform.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/UInt8MultiArray.h>

sensor_msgs::MultiDOFJointState team_states;
std::vector<int> team_ids;
std_msgs::UInt8MultiArray registered_team;
ros::Publisher reg_pub;
ros::Publisher team_state_pub;

void oneHertzCallBack(const ros::TimerEvent& event){

  reg_pub.publish(registered_team);
  
}

class Handler 
{
    public:
        ~Handler() {}

        void handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const lcm_navmsgs::vehicle_state* msg)
        {
	   if(!strcmp(chan.c_str(),"vehicle_states")){	   	     
	     
	   geometry_msgs::Transform curr_pose;
	   
	   curr_pose.translation.x = msg->position[0];
	   curr_pose.translation.y = msg->position[1];
	   curr_pose.translation.z = msg->position[2];
	   
	   curr_pose.rotation.w = msg->orientation[0];
	   curr_pose.rotation.x = msg->orientation[1];
	   curr_pose.rotation.y = msg->orientation[2];
	   curr_pose.rotation.z = msg->orientation[3];
	   
	   geometry_msgs::Twist curr_vel;
	   curr_vel.linear.x = msg->velocity[0];
	   curr_vel.linear.y = msg->velocity[1];
	   curr_vel.linear.z = msg->velocity[2];
	   
	   std::vector<int>::iterator it;	   
	   it = std::find(team_ids.begin(),team_ids.end(),msg->id) ;
	  // bool exists = (indx != team_ids.end());
	    
	   if(it != team_ids.end()){
	     	     
	    // team_states.transforms[*it] = curr_pose;
	    // std::cout << *it <<std::endl;
	     int n = it-team_ids.begin();
	     std::cout << "Element Found At:" << it-team_ids.begin() << std::endl;
	     team_states.transforms[n] = curr_pose;
	     team_states.joint_names[n] = msg->name;
	     team_states.twist[n] = curr_vel;
	     
	     team_state_pub.publish(team_states);
	     
	   }
	   else{
	     //Add new agent data to list of team members
	     team_ids.push_back(msg->id);
	     team_states.transforms.push_back(curr_pose);
	     team_states.joint_names.push_back(msg->name);
	     team_states.twist.push_back(curr_vel);
	     registered_team.data.push_back(msg->id);
	   }
	   
  
	    
	  }
	    

        }
};
int main(int argc, char **argv)
{

    ros::init(argc, argv, "ros_lcm_receiver");

    ros::NodeHandle nh;
    
    lcm::LCM lcm;
      if(!lcm.good())
        return 1;

    Handler handlerObject;
    lcm.subscribe("vehicle_states", &Handler::handleMessage, &handlerObject);

     
    ros::Timer timer = nh.createTimer(ros::Duration(1),oneHertzCallBack);
    reg_pub = nh.advertise<std_msgs::UInt8MultiArray>("reg_team", 20);
    team_state_pub = nh.advertise<sensor_msgs::MultiDOFJointState>("team_states",100);
    
    ros::Rate loop_rate(200);

    while(ros::ok() && lcm.handle()==0){     
        ros::spinOnce();// Allow ROS to check for new ROS Messages
        loop_rate.sleep(); //Sleep for some amount of time determined by loop_r$
    }

    return 0;
}



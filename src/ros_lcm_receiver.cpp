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
#include <emaxx_ctrl_msgs/emaxx_status.h>
#include <emaxx_ctrl_msgs/team_state.h>

emaxx_ctrl_msgs::team_state team_states;
std::vector<int> team_ids;
std_msgs::UInt8MultiArray registered_team;
ros::Publisher reg_pub;
ros::Publisher team_state_pub;
emaxx_ctrl_msgs::emaxx_status set_emaxx_state;


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
	     
	   geometry_msgs::Pose curr_pose;
	   
	   curr_pose.position.x = msg->position[0];
	   curr_pose.position.y = msg->position[1];
	   curr_pose.position.z = msg->position[2];
	   
	   curr_pose.orientation.w = msg->orientation[0];
	   curr_pose.orientation.x = msg->orientation[1];
	   curr_pose.orientation.y = msg->orientation[2];
	   curr_pose.orientation.z = msg->orientation[3];
	   
	   geometry_msgs::Twist curr_vel;
	   curr_vel.linear.x = msg->velocity[0];
	   curr_vel.linear.y = msg->velocity[1];
	   curr_vel.linear.z = msg->velocity[2];
	   
	   std::vector<int>::iterator it;	   
	   it = std::find(team_ids.begin(),team_ids.end(),msg->id) ;
	  // bool exists = (indx != team_ids.end());
	    
	   if(it != team_ids.end()){
	     //if team id is in the list already update it
	    // team_states.transforms[*it] = curr_pose;
	    // std::cout << *it <<std::endl;
	     int n = it-team_ids.begin();
	     std::cout << "Element Found At:" << it-team_ids.begin() << std::endl;
	     team_states.poses[n] = curr_pose;
	     team_states.velocities[n] = curr_vel;
	     team_states.length = team_ids.size();
	     team_state_pub.publish(team_states);
	     
	   }
	   else{
	     //Add new agent data to list of team members
	     team_ids.push_back(msg->id);
	     team_states.poses.push_back(curr_pose);
	     team_states.velocities.push_back(curr_vel);
	     team_state_pub.publish(team_states);	     
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
    team_state_pub = nh.advertise<emaxx_ctrl_msgs::team_state>("team_states",100);
    
    
    ros::Rate loop_rate(200);

    while(ros::ok() && lcm.handle()==0){     
        ros::spinOnce();// Allow ROS to check for new ROS Messages
        loop_rate.sleep(); //Sleep for some amount of time determined by loop_r$
    }

    return 0;
}



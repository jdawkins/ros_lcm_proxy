#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>

#include <lcm/lcm-cpp.hpp>
#include "lcm_navmsgs/vehicle_state.hpp"
#include "exlcm/example_t.hpp"


lcm_navmsgs::vehicle_state my_state;
int veh_id;
std::string ugv_name;


void poseCallBack(const geometry_msgs::PoseStamped pose_msg){

  my_state.position[0] = pose_msg.pose.position.x;
  my_state.position[1] = pose_msg.pose.position.y;
  my_state.position[2] = pose_msg.pose.position.z;
  
  my_state.orientation[0] = pose_msg.pose.orientation.w;
  my_state.orientation[1] = pose_msg.pose.orientation.x;
  my_state.orientation[2] = pose_msg.pose.orientation.y;
  my_state.orientation[3] = pose_msg.pose.orientation.z;

  my_state.id = my_state.id + 1;
  
  if(my_state.id == 11)
    my_state.id = 0;
  
  //ROS_ERROR("Pose CallBack");
}
void velCallBack(const geometry_msgs::Twist vel_msg){

  my_state.velocity[0] = vel_msg.linear.x;
  my_state.velocity[1] = vel_msg.linear.y;
  my_state.velocity[2] = vel_msg.linear.z;
  
 // ROS_ERROR("Vel CallBack");
  
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "ros_lcm_sender");

    ros::NodeHandle nh;
    
    lcm::LCM lcm;

    if(!lcm.good())
        return 1;
     
    
    nh.param("veh_id",veh_id,0);
    nh.param("ugv_name",ugv_name,std::string("emaxx"));

    std::cout << ugv_name <<std::endl;
    ros::Subscriber pose_sub = nh.subscribe("/"+ugv_name+"/pose_ned",100,poseCallBack);
    ros::Subscriber vel_sub = nh.subscribe("/"+ugv_name+"/vel_ned",100,velCallBack);
        
    my_state.id = veh_id;
    my_state.name = ugv_name.c_str();
   
      
    ros::Rate loop_rate(20);
    while(ros::ok() && lcm.good()){
      ros::Time curr_time = ros::Time::now();
	my_state.timestamp = curr_time.toNSec();
		
        lcm.publish("vehicle_states", &my_state);      


        ros::spinOnce();// Allow ROS to check for new ROS Messages
        loop_rate.sleep(); //Sleep for some amount of time determined by loop_r$
    }


    return 0;
}



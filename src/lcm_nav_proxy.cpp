#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>

#include <lcm/lcm-cpp.hpp>
#include "lcm_navmsgs/vehicle_state.hpp"
#include "exlcm/example_t.hpp"


lcm_navmsgs::vehicle_state my_state;


class Handler 
{
    public:
        ~Handler() {}

        void handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const lcm_navmsgs::vehicle_state* msg)
        {
	  
	  std::cout <<"Received Message From veh: " << msg->id <<" " <<std::endl;
	  
          /*  int i;
            printf("Received message on channel \"%s\":\n", chan.c_str());
            printf("  timestamp   = %lld\n", (long long)msg->timestamp);
            printf("  position    = (%f, %f, %f)\n",
                    msg->position[0], msg->position[1], msg->position[2]);

            printf("\n");*/

        }
};

void poseCallBack(const geometry_msgs::PoseStamped pose_msg){

  my_state.position[0] = pose_msg.pose.position.x;
  my_state.position[1] = pose_msg.pose.position.y;
  my_state.position[2] = pose_msg.pose.position.z;
  
  my_state.orientation[0] = pose_msg.pose.orientation.w;
  my_state.orientation[1] = pose_msg.pose.orientation.x;
  my_state.orientation[2] = pose_msg.pose.orientation.y;
  my_state.orientation[3] = pose_msg.pose.orientation.z;

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

    ros::init(argc, argv, "lcm_nav_proxy_node");

    ros::NodeHandle nh;
    
    lcm::LCM lcm;

    if(!lcm.good())
        return 1;
     
    
    ros::Subscriber pose_sub = nh.subscribe("/emaxx/pose_ned",100,poseCallBack);
    ros::Subscriber vel_sub = nh.subscribe("/emaxx/vel_ned",100,velCallBack);
        
    my_state.id = 10;
    my_state.name = 'Right_Side';
    Handler handlerObject;
    lcm.subscribe("vehicle_states", &Handler::handleMessage, &handlerObject);
      
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



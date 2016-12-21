#include <ros/ros.h>

#include <lcm/lcm-cpp.hpp>
#include "exlcm/example_t.hpp"
#include "lcm_navmsgs/vehicle_state.hpp"

lcm_navmsgs::vehicle_state my_state;


int main(int argc, char **argv)
{

    ros::init(argc, argv, "lcm_publisher");

    ros::NodeHandle nh;
    
    lcm::LCM lcm;
      if(!lcm.good())
        return 1;

    my_state.name = "my_state_data";
    
    exlcm::example_t my_data;

    my_data.timestamp = 0;

    my_data.position[0] = 1;
    my_data.position[1] = 2;
    my_data.position[2] = 3;

    my_data.orientation[0] = 1;
    my_data.orientation[1] = 2;
    my_data.orientation[2] = 3;
    my_data.orientation[3] = 4;

    my_data.num_ranges = 20;
    my_data.ranges.resize(my_data.num_ranges);
    for(int i = 0; i < my_data.num_ranges; i++)
        my_data.ranges[i] = i;

    my_data.name = "example string";
    my_data.enabled = true;

      
    ros::Rate loop_rate(100);
    while(ros::ok()){
      ros::Time curr_time = ros::Time::now();
      my_data.timestamp = curr_time.toSec();
        lcm.publish("EXAMPLE", &my_data);      
        lcm.publish("vehicle_states", &my_state);      

        ros::spinOnce();// Allow ROS to check for new ROS Messages
        loop_rate.sleep(); //Sleep for some amount of time determined by loop_r$
    }


    return 0;
}



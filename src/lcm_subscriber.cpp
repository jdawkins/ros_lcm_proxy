#include <ros/ros.h>
#include <stdio.h>

#include <lcm/lcm-cpp.hpp>
#include "exlcm/example_t.hpp"
#include "lcm_navmsgs/vehicle_state.hpp"



class Handler 
{
    public:
        ~Handler() {}

        void handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const lcm_navmsgs::vehicle_state* msg)
        {
            int i;
            printf("Received message on channel \"%s\":\n", chan.c_str());
            printf("  timestamp   = %lld\n", (long long)msg->timestamp);
            printf("  position    = (%f, %f, %f)\n",
                    msg->position[0], msg->position[1], msg->position[2]);
            printf("  orientation = (%f, %f, %f, %f)\n",
                    msg->orientation[0], msg->orientation[1], 
                    msg->orientation[2], msg->orientation[3]);
	    printf("Veh ID	= %d", msg->id);
        }
};
int main(int argc, char **argv)
{

    ros::init(argc, argv, "lcm_subscriber");

    ros::NodeHandle nh;
    
    lcm::LCM lcm;
      if(!lcm.good())
        return 1;

    Handler handlerObject;
    lcm.subscribe("vehicle_states", &Handler::handleMessage, &handlerObject);

    //while(0 == lcm.handle());
    
    ros::Rate loop_rate(100);
    while(ros::ok() && lcm.handle()==0){     

        ros::spinOnce();// Allow ROS to check for new ROS Messages
        loop_rate.sleep(); //Sleep for some amount of time determined by loop_r$
    }


    return 0;
}



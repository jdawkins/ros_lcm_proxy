javaaddpath /home/wseuser/Libraries/lcm/lcm-java/lcm.jar
javaaddpath /home/wseuser/catkin_ws/src/ros_lcm_proxy/lcm_msgs/lcm_navmsgs/vehicle_state.jar
lc = lcm.lcm.LCM.getSingleton();

lcm_navmsgs.vehicle_state()
msg = lcm_msg.example_t();

msg.timestamp = 0;
msg.position = [1  2  3];
msg.orientation = [1 0 0 0];
msg.ranges = 1:15;
msg.num_ranges = length(msg.ranges);
msg.name = 'example string';
msg.enabled = 1;

lc.publish('EXAMPLE', msg);
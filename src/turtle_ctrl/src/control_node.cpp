#include "ros/ros.h"


#include <stdlib.h>
#include <ros/service.h>

#include "geometry_msgs/Twist.h"
#include <turtlesim/Spawn.h>
#include <turtlesim/Kill.h>

# define PI           3.14159265358979323846 

int main(int argc, char **argv)
{
  // ROS node setup
  using namespace ros;
  std::string turtle_name = "Bondar";
  init(argc, argv, "Bondar_turtle_ctrl");
  NodeHandle n;

  //Create service client and kill turtle1
  ServiceClient service = n.serviceClient<turtlesim::Kill>("kill");
  turtlesim::Kill::Request rqt1;
  turtlesim::Kill::Response resp1;
  rqt1.name = "turtle1";
  ros::service::waitForService("kill", ros::Duration(5));
  service.call(rqt1,resp1);

  //Create service client and spawn new turtle at (4; 2)
  service = n.serviceClient<turtlesim::Spawn>("spawn");
  turtlesim::Spawn::Request rqt;
  turtlesim::Spawn::Response resp;
  rqt.x = 4;
  rqt.y = 2;
  rqt.name = turtle_name;
  ros::service::waitForService("spawn", ros::Duration(5));
  service.call(rqt,resp);

  //Go to loop for controlling turtle
  Publisher pub = n.advertise<geometry_msgs::Twist>(turtle_name + "/cmd_vel", 1000);
  Rate loop_rate(2.5);
  geometry_msgs::Twist twist;
  twist.angular.z = PI / 1.2;
  while (true) 
  {
    for(int i = 0; i <= 5; i++)
    {
      twist.linear.x = 6.0;
      twist.linear.y = 0.0;
      twist.linear.z = 0.0;
      twist.angular.x = 0.0;
      twist.angular.y = 0.0;
      twist.angular.z *= -2;
      pub.publish(twist);
      spinOnce();
      loop_rate.sleep();
      if (!n.ok())
         return 0;
    }
    twist.angular.z = PI;
    pub.publish(twist);
    twist.angular.z = PI / 1.2;
    for(int i = 0; i <= 5; i++)
    {
      twist.linear.x = -6.0;
      twist.linear.y = 0.0;
      twist.linear.z = 0.0;
      twist.angular.x = 0.0;
      twist.angular.y = 0.0;
      twist.angular.z *= -2;
      pub.publish(twist);
      spinOnce();
      loop_rate.sleep();
      if (!n.ok())
        return 0;
    }
    twist.angular.z = PI;
    pub.publish(twist);
    twist.angular.z = PI / 1.2;
  }
  return 0;
}

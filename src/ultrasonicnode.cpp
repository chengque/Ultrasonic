#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "stdio.h"
extern "C" { 
#include "wiringPi.h"
}
/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */

bool rootREVOKED = 0;

static void revokeRoot(void) 
{
	if (getuid () + geteuid () == 0)
		return ;
	if (getuid () == 0)
	{
	seteuid(getuid ());
	rootREVOKED = 1;
	return ;
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "talker");

	ros::NodeHandle n;

	ros::Publisher pub = n.advertise<std_msgs::Float64>("distance", 1000);

	ros::Rate loop_rate(10);

	//int r = wiringPiSetup(); 
	
	int r = wiringPiSetup();
	revokeRoot();
	printf("setup: %d\n", r);
	// wiringPiSetup requires sudo, wiringPiSetupSys requires you add this to your /etc/rc.local:
	//
	// for example, to use pin 23 as an input:
	//
	// gpio export 23 in
	// 

	while (ros::ok())
	{
		ros::spinOnce();
		loop_rate.sleep();
	}
	
}

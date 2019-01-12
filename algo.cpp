#include <ros/ros.h>
#include <mavros_msgs/OverrideRCIn.h>
#include <mavros_msgs/RCIn.h>
#include <std_msgs/Float32.h>

std_msgs::Float32 distance1;
std_msgs::Float32 distance2;
std_msgs::Float32 distance3;
std_msgs::Float32 distance4;
mavros_msgs::RCIn over_switch;

int kill ;
int dist[4][4];
int avg_dist[4] ;

void rc_cb(const mavros_msgs::RCIn::ConstPtr& msg2){
    over_switch = *msg2;
    kill = over_switch.channels[5];
}

int push_avg(int array_number, int new_element){
    dist[array_number][0] = dist[array_number][1];
    dist[array_number][1] = dist[array_number][2];
    dist[array_number][2] = dist[array_number][3];
    dist[array_number][3] = new_element;
    int sum = dist[array_number][3] + dist[array_number][2] + dist[array_number][1] + dist[array_number][0];
    sum = int(sum/4);
    return sum;
}

void dist1_cb(const std_msgs::Float32::ConstPtr& msgd1){
    distance1 = *msgd1;
    avg_dist[0] = push_avg(0,int(distance1.data));
}

void dist2_cb(const std_msgs::Float32::ConstPtr& msgd2){
    distance2 = *msgd2;
    avg_dist[1] = push_avg(1,int(distance2.data));
}

void dist3_cb(const std_msgs::Float32::ConstPtr& msgd3){
    distance3 = *msgd3;
    avg_dist[2] = push_avg(2,int(distance3.data));
}

void dist4_cb(const std_msgs::Float32::ConstPtr& msgd4){
    distance4 = *msgd4;
    avg_dist[3] = push_avg(3,int(distance4.data));
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;
    ros::Subscriber dist1_sub = nh.subscribe<std_msgs::Float32>("/Distance1", 10, dist1_cb);
    ros::Subscriber dist2_sub = nh.subscribe<std_msgs::Float32>("/Distance2", 10, dist2_cb);
    ros::Subscriber dist3_sub = nh.subscribe<std_msgs::Float32>("/Distance3", 10, dist3_cb);
    ros::Subscriber dist4_sub = nh.subscribe<std_msgs::Float32>("/Distance4", 10, dist4_cb);
    ros::Subscriber rc_sub = nh.subscribe<mavros_msgs::RCIn>("mavros/rc/in", 10, rc_cb);
    ros::Publisher radio_pub = nh.advertise<mavros_msgs::OverrideRCIn>("mavros/rc/override", 10);

    ros::Rate rate(50.0);
    mavros_msgs::OverrideRCIn psi;
    int minimum_dist = 100;

    psi.channels[0] = 1500;
    psi.channels[1] = 1500;
    psi.channels[2] = 1500;
    psi.channels[3] = 1500;
    psi.channels[4] = 1100;
    psi.channels[5] = 1950;
    psi.channels[6] = 1000;
    psi.channels[7] = 1100;

    while (ros::ok())
    {
        if(kill <= 1100)
        {
	        if(avg_dist[0] <1minimum_dist  || avg_dist[1]<minimum_dist ||  avg_dist[2] < minimum_dist || avg_dist[3] < minimum_dist){

				//Forward Direction : avg_dist[0]
				//Backward Direction : avg_dist[2]
				//Left Direction : avg_dist[3]
				//Right Direction : avg_dist[1]

				if(avg_dist[0]<minimum_dist){
					//move backwards
					psi.channels[1] = 1800;
				}else if(avg_dist[2]<minimum_dist){
					//move forwards
					psi.channels[1] = 1200;
				}else{
					psi.channels[1] = 1500;
				}

				if(avg_dist[1]<minimum_dist){
					//move leftwards
					psi.channels[0] = 1200;
				}else if(avg_dist[3]<minimum_dist){
					//move rightwards
					psi.channels[0] = 1800;
				}else{
					psi.channels[0] = 1500;
                }
                radio_pub.publish(psi);

            }
        }

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}

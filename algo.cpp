#include <ros/ros.h>
#include <mavros_msgs/OverrideRCIn.h>
#include <mavros_msgs/RCIn.h>
#include <geometry_msgs/Quaternion.h>

geometry_msgs::Quaternion distance1;
geometry_msgs::Quaternion distance2;
geometry_msgs::Quaternion distance3;
geometry_msgs::Quaternion distance4;
mavros_msgs::RCIn over_switch;

int kill ;
int dist1[4],dist2[4],dist3[4],dist4[4] ;
int count1 = 0,count2 = 0,count3 = 0,count4 = 0;
int avg_dist[4] ;
int now1=0, now2=0, now3=0, now4=0;

void rc_cb(const mavros_msgs::RCIn::ConstPtr& msg2){
    over_switch = *msg2;
    kill = over_switch.channels[5];
}

int avg(int array[4]){
    int sum = array[0] + array[1] + array[2] + array[3]
    sum = int(sum/4)
    return sum;
}

void dist1_cb(const geometry_msgs::Quaternion::ConstPtr& msgd1){
    distance1 = *msgd1;
    dist1[count1] = int(distance1.x);
    count1++ ;
    if(count1 == 4){
    avg_dist[0] = avg(dist1;
    count1 = 0;
    now1++;
    }
}

void dist2_cb(const geometry_msgs::Quaternion::ConstPtr& msgd2){
    distance2 = *msgd2;
    dist2[count2] = int(distance2.x);
    count2++ ;
    if(count2 == 4){
    avg_dist[1] = avg(dist2);
    count2 = 0;
    now2++;
    }
}

void dist3_cb(const geometry_msgs::Quaternion::ConstPtr& msgd3){
    distance3 = *msgd3;
    dist3[count3] = int(distance3.x);
    count3++ ;
    if(count3 == 4){
    avg_dist[2] = avg(dist3);
    count3 = 0;
    now3++;
    }
}

void dist4_cb(const geometry_msgs::Quaternion::ConstPtr& msgd4){
    distance4 = *msgd4;
    dist4[count4] = int(distance4.x);
    count4++ ;
    if(count4 == 4){
    avg_dist[3] = avg(dist4);
    count4 = 0;
    now4++;
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;
    ros::Subscriber dist1_sub = nh.subscribe<geometry_msgs::Quaternion>("/Distance1", 10, dist1_cb);
    ros::Subscriber dist2_sub = nh.subscribe<geometry_msgs::Quaternion>("/Distance2", 10, dist2_cb);
    ros::Subscriber dist3_sub = nh.subscribe<geometry_msgs::Quaternion>("/Distance3", 10, dist3_cb);
    ros::Subscriber dist4_sub = nh.subscribe<geometry_msgs::Quaternion>("/Distance4", 10, dist4_cb);
    ros::Subscriber rc_sub = nh.subscribe<mavros_msgs::RCIn>("mavros/rc/in", 10, rc_cb);
    ros::Publisher radio_pub = nh.advertise<mavros_msgs::OverrideRCIn>("mavros/rc/override", 10);

    ros::Rate rate(50.0);
    mavros_msgs::OverrideRCIn psi;
	ros::Time begin = ros::Time::now();
	ros::Time end = ros::Time::now();
	ros::Duration three_seconds(3.0);

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

        if(now>0){
            if(kill <= 1100){
                if(avg_dist[0] < 70 || avg_dist[1] < 70 || avg_dist[2] < 70 || avg_dist[3] < 70){
                    begin = ros::Time::now();
                    end = ros::Time::now();
                    while((end-begin)<three_seconds && kill <= 1100){
                        end = ros::Time::now();
                        if(avg_dist[0]<70){
                        	psi.channels[1] = 1800;
                        }
                        if(avg_dist[1]<70){
                        	psi.channels[0] = 1200;
                        }
                        if(avg_dist[2]<70){
                        	psi.channels[1] = 1200;
                        }
                        if(avg_dist[3]<70){
                        	psi.channels[0] = 1800;
                        }
                        radio_pub.publish(psi);
                    }
                }
            }
            now = 0;
        }
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}

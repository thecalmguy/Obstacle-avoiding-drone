#include <ros/ros.h>
#include <mavros_msgs/OverrideRCIn.h>
#include <mavros_msgs/RCIn.h>
#include <geometry_msgs/Quaternion.h>
#define RC rc_ovrd.channels
mavros_msgs::OverrideRCIn rc_ovrd;
geometry_msgs::Quaternion distances;
mavros_msgs::RCIn over_switch;

int kill ;
int dist[4] ;
int count = 0;
int avg_dist ;
int now=0, past=0;
ros::Time begin = ros::Time::now();
ros::Time end = ros::Time::now();
ros::Duration three_seconds(3.0);

RC[0] = 1500;
RC[1] = 1200;
RC[2] = 1500;
RC[3] = 1500;
RC[4] = 1100;
RC[5] = 1000;
RC[6] = 1100;
RC[7] = 1100;

void rc_cb(const mavros_msgs::RCIn::ConstPtr& msg2){
    over_switch = *msg2;
    kill = over_switch.channels[5];
}

int avg(int array[4]){
    int sum = array[0] + array[1] + array[2] + array[3]
    sum = int(sum/4)
    return sum;
}

void dist_cb(const geometry_msgs::Quaternion::ConstPtr& msg){
    distances = *msg;
    dist[count] = int(distances.x);
    count ++ ;
    if(count == 4){
    avg_dist = avg(dist);
    count = 0;
    now ++;
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;
    ros::Subscriber dist_sub = nh.subscribe<geometry_msgs::Quaternion>("/Distances", 10, dist_cb);
    ros::Subscriber rc_sub = nh.subscribe<mavros_msgs::RCIn>("mavros/rc/in", 10, rc_cb);
    ros::Publisher radio_pub = nh.advertise<mavros_msgs::OverrideRCIn>("mavros/rc/override", 10);

    ros::Rate rate(50.0);

    while (ros::ok())
    {

        if(now>past){
            if(kill <= 1100){
                if(avg_dist < 70){
                    begin = ros::Time::now();
                    end = ros::Time::now();
                    while((end-begin)<three_seconds && kill <= 1100){
                        end = ros::Time::now();
                        radio_pub.publish(rc_ovrd);
                    }
                }
                radio_pub.publish(rc_ovrd);

            }
            now = 0;
            past = 0;
        }
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}

#!/usr/bin/env python

import rospy
import RPi.GPIO as gp
import time
from std_msgs.msg import Float32

gp.setmode(gp.BCM)

trigpin = 16
echopin = 12
gp.setup(trigpin, gp.OUT)
gp.setup(echopin, gp.IN)

dist = Float32()

def give_distance():
    gp.output(trigpin, True)
    time.sleep(0.00001)
    gp.output(trigpin, False)
    temp_start = time.time()
    start = time.time()
    end = time.time()
    if gp.input(echopin) == True :
        start = temp_start
    while gp.input(echopin) == False:
            start = time.time()
            if start - temp_start > 0.001 :
                break
    while gp.input(echopin) == True:
            end = time.time()
            if end - start > 0.017:
                break
    duration = end - start
    distance = duration/0.000058
    #print('Distance_1: {} cm '.format(distance))
    dist.data = distance
    

def proximity():
    pub = rospy.Publisher('Distance1', Float32, queue_size=10)
    rospy.init_node('proximity1', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        give_distance()
        rospy.loginfo(dist)
        pub.publish(dist)
        rate.sleep()
    gp.cleanup()

if __name__ == '__main__':
    try:
        proximity()
    except rospy.ROSInterruptException:
        pass

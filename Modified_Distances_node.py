#!/usr/bin/env python

import rospy
import RPi.GPIO as gp
import time
from geometry_msgs.msg import Quaternion

gp.setmode(gp.BCM)

trigpin1 = 16
echopin1 = 12
trigpin2 = 20
echopin2 = 21
trigpin3 = 23
echopin3 = 22
trigpin4 = 17
echopin4 = 27
gp.setup(trigpin1, gp.OUT)
gp.setup(echopin1, gp.IN)
gp.setup(trigpin2, gp.OUT)
gp.setup(echopin2, gp.IN)
gp.setup(trigpin3, gp.OUT)
gp.setup(echopin3, gp.IN)
gp.setup(trigpin4, gp.OUT)
gp.setup(echopin4, gp.IN)

dist = Quaternion()

def give_distance():
    gp.output(trigpin1, True)
    time.sleep(0.00001)
    gp.output(trigpin1, False)
    start = time.time()
'''    temp_start = time.time()
    if gp.input(echopin1) == True :
    	start = temp_start
    while gp.input(echopin1) == False:
            start = time.time()
            if start - temp_start > 0.001 :
            	break
   '''
    while gp.input(echopin1) == True:
            end = time.time()
            if end - start > 0.017:
                break
    duration = end - start
    distance = duration/0.000058
    #print('Distance_1: {} cm '.format(distance))
    dist.x = distance
    gp.output(trigpin2, True)
    time.sleep(0.00001)
    gp.output(trigpin2, False)
	start = time.time()

'''    temp_start = time.time()
    if gp.input(echopin2) == True :
    	start = temp_start
    while gp.input(echopin2) == False:
            start = time.time()
            if start - temp_start > 0.001 :
            	break'''
    while gp.input(echopin2) == True:
            end = time.time()
            if end - start > 0.017:
                break
    duration = end - start
    distance = duration/0.000058
    #print('Distance_2: {} cm '.format(distance))
    dist.y = distance
    gp.output(trigpin3, True)
    time.sleep(0.00001)
    gp.output(trigpin3, False)
	start = time.time()
   ''' temp_start=time.time()
    if gp.input(echopin3) == True :
    	start = temp_start
    while gp.input(echopin3) == False:
            start = time.time()
            if start - temp_start > 0.001 :
            	break'''
    while gp.input(echopin3) == True:
            end = time.time()
            if end - start > 0.017:
                break
    duration = end - start
    distance = duration/0.000058
    #print('Distance_3: {} cm '.format(distance))
    dist.z = distance
    gp.output(trigpin4, True)
    time.sleep(0.00001)
    gp.output(trigpin4, False)
	start = time.time()
'''    temp_start=time.time()
    if gp.input(echopin4) == True :
    	start = temp_start
    while gp.input(echopin4) == False:
            start = time.time()
            if start - temp_start >0.001 :
            	break'''
    while gp.input(echopin4) == True:
            end = time.time()
            if end - start > 0.017:
                break
    duration = end - start
    distance = duration/0.000058
    #print('Distance_4: {} cm '.format(distance))
    dist.w = distances

def proximity():
    pub = rospy.Publisher('Distances', Quaternion, queue_size=10)
    rospy.init_node('proximity', anonymous=True)
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

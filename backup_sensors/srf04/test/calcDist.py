#!/usr/bin/env python

import mraa
import time

trigpin = mraa.Gpio(13)
echopin = mraa.Gpio(20)
trigpin.dir(mraa.DIR_OUT)
echopin.dir(mraa.DIR_IN)


time.sleep(.3)

for i in range(10):
    trigpin.write(0)
    time.sleep(0.3)
    trigpin.write(1)
    time.sleep(.00001)
    trigpin.write(0)
    #print echopin.read()
    #off = time.time()
    while echopin.read() == 0:
        off = time.time()
    #print echopin.read()
    while echopin.read() == 1:
    #if echopin.read() == 1:
        on = time.time()
    distance = on - off
    distance = distance/58
    print distance
    print echopin.read()
    time.sleep(.00005)

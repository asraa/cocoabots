#!/usr/bin/env python

import mraa
import time

trigpin = mraa.Gpio(13)
echopin = mraa.Gpio(20)
#echopin.enable(True)
#echopin.enable(True)

for i in range(10):
    trigpin.write(0)
    time.sleep(0.000002)
    trigpin.write(1)
    time.sleep(.00001)
    trigpin.write(0)
    #off = time.time()
    while echopin.read() == 0:
        off = time.time()
    while echopin.read() == 1:
    #if echopin.read() == 1:
        on = time.time()
    distance = on - off
    distance = distance/58
    print distance
    print echopin.read()
    time.sleep(.00005)

#!/usr/bin/env python

import mraa, time

# Setting pin numbers
srf04_echo = mraa.Gpio(2)
srf04_trigger = mraa.Gpio(3)

# Setting IN/OUT modes
srf04_echo.dir(mraa.DIR_OUT)
srf04_trigger.dir(mraa.DIR_IN)

srf04_trigger.write(0)
time.sleep(2e-6)
srf04_trigger.write(1)
time.sleep(10e-6)
srf04_trigger.write(0)

dist = 0
while (srf04_echo.read() == 1):
	dist += 10e-6
	time.sleep(10e-6)

dist = dist/58

print dist

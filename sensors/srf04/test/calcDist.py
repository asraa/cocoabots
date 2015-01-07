import mraa
import time

echopin = mraa.Pwm(2)
trigpin = mraa.Pwm(3)
echopin.enable(True)
trigpin.enable(True)

for i in range(10):
    trigpin.write(0)
    time.sleep(0.000002)
    trigpin.write(1)
    time.sleep(.00001)
    trigpin.write(0)
    distance = echopin.pulsewidth_us()
    distance = distance/58
    print(distance)
    time.sleep(.00005)

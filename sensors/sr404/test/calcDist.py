import mraa
import time

echopin = mraa.Gpio(2)
trigpin = mraa.Gpio(3)

for i in range(10):
    trigpin.write(0)
    time.sleep(0.000002)
    trigpin.write(1)
    time.sleep(.00001)
    trigpin.write(0)
    while echopin.read() == 0:
        off = time.time()
    while echopin.read() == 1:
        on = time.time()
    distance = on - off
    distance = distance/58
    print(distance)
    time.sleep(.00005)

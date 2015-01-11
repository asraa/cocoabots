import cv2
import numpy as np
filename = raw_input('Enter a file name and path, e.g. ./balls1.jpg: ')
img=cv2.imread(filename)

hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

lower_green = np.array([45,50,50])
upper_green = np.array([75,250,250])

mask = cv2.inRange(hsv, lower_green, upper_green)
res = cv2.bitwise_and(img,img, mask= mask)
cv2.imshow('mask',mask)
cv2.imshow('img',img)
cv2.imshow('res',res)

cv2.waitKey()

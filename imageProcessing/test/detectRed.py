import cv2
import numpy as np
filename = raw_input('Enter a file name and path, e.g. ./balls1.jpg: ')
img=cv2.imread(filename)

hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

lower_red1 = np.array([0,30,30])
upper_red1 = np.array([15,255,255])
lower_red2 = np.array([160,30,30])
upper_red2 = np.array([178,255,255])


mask1 = cv2.inRange(hsv, lower_red1, upper_red1)
mask2 = cv2.inRange(hsv, lower_red2, upper_red2)
mask = cv2.bitwise_or(mask1,mask2)

res = cv2.bitwise_and(img,img, mask= mask)
cv2.imshow('mask',mask)
cv2.imshow('img',img)
cv2.imshow('res',res)

cv2.waitKey()

#!/usr/bin/env python

import time, cv2

def takePic(filename):
	cap = cv2.VideoCapture(0)
	s,img = cap.read()
	cv2.imwrite(filename,img)
	cap.release()

if __name__ == '__main__':
	f = time.strftime('%d-%m-%H%M%S.jpg')
	takePic(f)

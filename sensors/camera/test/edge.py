import numpy as np
import cv2

img = cv2.imread(raw_input('filename'), 0)
edges = cv2.Canny(img,100,200)

cv2.imshow('first', img)
cv2.imshow('edges',edges)


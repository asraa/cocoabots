from cv2.cv import *
filename = raw_input('Enter a file name and path, e.g. ./balls1.jpg: ')
img = LoadImage(filename)
NamedWindow("opencv")
ShowImage("opencv",img)
WaitKey(0)

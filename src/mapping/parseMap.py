#!/usr/bin/env python

'''
use this script with:
	./parseMap.py <inputmap.txt>


outputs: map.txt, stacks.txt

**
map.txt output is a grid
both axes are zero-indexed
x-axis goes top-down
y-axis goes left-right
format is given below

empty = 0
wall = 1
platform = 2
stack = 3
homebase = 4
startloc = 5


**
stacks.txt output has one stack per line
each line has the format: xyccc

x = xpos in the map
y = ypos in the map
c = cube, where:
		  0 = red cube
		  1 = green cube
	  and c1 = bottom
	  	  c2 = middle
		  c3 = top
'''

import sys

def parseMap(inf,outmapf,outstackf):
	input_file = open(inf,'r')
	outputMap_file = open(outmapf,'w')
	outputStack_file = open(outstackf,'w')
	posDict = {}
	stackDict = {}
	maxX = 0
	maxY = 0
	options = {'W': wall,
				'P': platform,
				'S': stack,
				'H': homeBase,
				'L': startLoc}
	for line in input_file:
		splitLine = line.split(',')
		obj = splitLine[0]
		posDict, stackDict = options[obj](splitLine[1:],posDict,stackDict)
	for key in posDict.keys():
		if (int(key[0]) > maxX):
			maxX = int(key[0])
		if (int(key[1]) > maxY):
			maxY = int(key[1])
	for x in range(maxX+1):
		line = []
		for y in range(maxY+1):
			key = (str(x),str(y))
			if key in posDict:
				line.append(posDict[key])
			else:
				line.append('0')
		outputMap_file.write(''.join(line))
		outputMap_file.write('\n') if (x != maxX) else None
	for key in stackDict.keys():
		val = stackDict[key]
		line = str(key[0])+str(key[1])
		line += str(stackDict[key][0])
		line += str(stackDict[key][1])
		line += str(stackDict[key][2])
		outputStack_file.write(line+'\n')
	outputStack_file.close()
	outputMap_file.close()
	input_file.close()

def wall(inpList, posDict, stackDict):
	pos1 = (inpList[0],inpList[1])
	pos2 = (inpList[2],inpList[3])
	posDict[pos1] = '1'
	posDict[pos2] = '1'
	return posDict, stackDict

def platform(inpList, posDict, stackDict):
	pos1 = (inpList[0],inpList[1])
	pos2 = (inpList[2],inpList[3][0])
	posDict[pos1] = '2'
	posDict[pos2] = '2'
	return posDict, stackDict

def stack(inpList, posDict, stackDict):
	pos1 = (inpList[0],inpList[1])
	posDict[pos1] = '3'
	cubeList = []
	for item in inpList[2:]:
		if (item[0] == 'R'):
			cubeList.append(0)
		elif (item[0] == 'G'):
			cubeList.append(1)
	stackDict[pos1] = cubeList
	return posDict, stackDict

def homeBase(inpList, posDict, stackDict):
	num = int(inpList[0])
	inpList = inpList[1:]
	posList = []
	for i in range(num):
		posList.append((inpList[2*i],inpList[2*i+1][0]))
	for pos in posList:
		posDict[pos] = '4'
	return posDict, stackDict

def startLoc(inpList, posDict, stackDict):
	pos1 = (inpList[0],inpList[1])
	posDict[pos1] = '5'
	return posDict, stackDict

if __name__ == '__main__':
	outMapFilename = 'map.txt'
	outStackFilename = 'stacks.txt'
	parseMap(sys.argv[1],outMapFilename,outStackFilename)
	

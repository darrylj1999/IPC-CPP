from __future__ import print_function
import sys
from collections import defaultdict
import matplotlib
matplotlib.rcParams.update({ 'lines.linewidth': 0.5 })
import matplotlib.pyplot as plot

# Input: List of pairs
# Output: First element of each pair
def getX( l ):
	a = []
	for item in l:
		a.append( item[0] )
	return a

# Output: Second element of each pair
def getY( l ):
	a = []
	for item in l:
		a.append( item[1] )
	return a
	

infilename = sys.argv[1]

dataDict = defaultdict(list)

# RUNTIME vs W
# Reading in data
with open(infilename, 'r') as infile:
	for line in infile:
		data = line.strip().split()
		# Queue Size
		z = int(data[0])
		# Number of Workers
		x = int(data[1])
		# Runtime
		y = float(data[2])
		
		dataDict[z].append( (x,y) )

# Sorting x values
for key in dataDict.keys():
	dataDict[key].sort(key=lambda x: x[0])

for key in sorted(dataDict.keys()):
	x = getX( dataDict[key] )
	y = getY( dataDict[key] )
	plot.scatter(x, y, marker='x')
	plot.plot(x, y, label='b = ' + str(key))
plot.legend()
plot.ylabel("Runtime (s)")
plot.xlabel("Number of Worker Channels (w)")
plot.savefig("out.pdf")

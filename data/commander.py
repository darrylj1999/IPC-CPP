from __future__ import print_function
b = 5
n = 10e3

while ( b <= 200 ) :
	w = 5
	while ( w <= 500 ):
		print("./client -n", n, "-b", b, "-w", w, " >> ./data/result_1.txt")
		print("rm fifo_*")
		w += 15
	b += 20

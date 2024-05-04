import sys
import os
 

if len(sys.argv) >= 2:
	#print(sys.argv)
	url=sys.argv;
	path=str(url[1])
	f2 = open(path, "a")
	mem=620000
	size=os.path.getsize(path)
	print(size)
	byte=0

	for x in range(mem):
		f2.write("%x" % 0)	
		pass
	f2.close()



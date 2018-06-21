    #!/usr/bin/python
from ctypes import *
cfun=CDLL("./test_aspect_ratio.so")
aspect=cfun.start()
#print bright
if aspect==1:
	print "PASS"
else:
	print "FAIL"

#!/usr/bin/python
from ctypes import *
cfun=CDLL("./test_aspectratio.so")
aspect=cfun.start()
#print bright
if aspect==1:
	print "Pass"
else:
	print "Fail"

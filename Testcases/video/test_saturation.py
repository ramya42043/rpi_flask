#!/usr/bin/python
from ctypes import *
cfun=CDLL("./test_saturation.so")
satur=cfun.start()
#print bright
if satur==1:
	print "Pass"
else:
	print "Fail"

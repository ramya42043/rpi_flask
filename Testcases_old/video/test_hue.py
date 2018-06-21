#!/usr/bin/python
from ctypes import *
cfun=CDLL("./test_hue.so")
hue=cfun.start()
#print bright
if hue==1:
	print "PASS"
else:
	print "FAIL"

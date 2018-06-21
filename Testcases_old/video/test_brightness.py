#!/usr/bin/python
from ctypes import *
cfun=CDLL("./test_brightness.so")
bright=cfun.start()
#print bright
if bright==1:
	print "PASS"
else:
	print "FAIL"

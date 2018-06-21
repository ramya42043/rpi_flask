#!/usr/bin/python
from ctypes import *
cfun=CDLL("./test_brightness_mp4.so")
bright=cfun.start()
#print bright
if bright==1:
	print "Pass"
else:
	print "Fail"

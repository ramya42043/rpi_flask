#!/usr/bin/python
from ctypes import *

cfun=CDLL("./test_pitch.so")
pitch=cfun.start()
#print bright
if pitch==1:
	print "Pass"
else:
	print "Fail"

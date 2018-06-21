#!/usr/bin/python
from ctypes import *
cfun=CDLL("./test_contrast.so")
contr=cfun.start()
#print bright
if contr==1:
	print "Pass"
else:
	print "Fail"

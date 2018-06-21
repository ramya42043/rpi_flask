#!/usr/bin/python
from ctypes import *
cfun=CDLL("./test_wmv.so")
bright=cfun.start()
if bright==1:
	print "Pass"
else:
	print "Fail"

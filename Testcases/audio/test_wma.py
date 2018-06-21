#!/usr/bin/python
from ctypes import *
cfun=CDLL("./test_wma.so")
foo=cfun.start()
if foo ==1:
	print "PASS"
else:
	print "FAIL"

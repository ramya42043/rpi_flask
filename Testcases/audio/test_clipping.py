#!/usr/bin/python
from ctypes import *
cfun=CDLL("./clipping.so")
foo=cfun.start()
if foo ==1:
	print "PASS"
else:
	print "FAIL"

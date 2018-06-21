#!/usr/bin/python
from ctypes import *
cfun=CDLL("./amplification.so")
foo=cfun.start()
if foo ==1:
	print "PASS"
else:
	print "FAIL"

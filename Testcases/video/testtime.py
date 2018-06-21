#!/usr/bin/python
from ctypes import *
#import ctypes
cfun=CDLL("./time.so")
foo=cfun.start()
if foo ==1:
	print "PASS"
else:
	print "FAIL"

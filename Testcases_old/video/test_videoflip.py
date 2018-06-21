#!/usr/bin/python
from ctypes import *
cfun=CDLL("./flip.so")
foo=cfun.start()
if foo==1:
	print "PASS"
else:
	print "FAIL"
#print ctypes.c_char_p(foo).value

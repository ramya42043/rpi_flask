#!/usr/bin/python
from ctypes import *
import ctypes
cfun=CDLL("./hardwaredec.so")
foo=cfun.start()
#print ctypes.c_char_p(foo).value
if foo==1:
    print "PASS"
else:
    print "FAIL"

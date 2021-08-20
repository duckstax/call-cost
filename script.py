#import sys, os
#from importlib import import_module

from fast_calc import  rbp

def run(a,b):
    return rbp(a, b)

def fcn2( a  ):
    a.id  = a.id2
    return a

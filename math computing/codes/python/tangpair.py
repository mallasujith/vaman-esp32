#Code by GVV Sharma
#June 12, 2022
#Revised November 14, 2023
#released under GNU GPL
#https://www.gnu.org/licenses/gpl-3.0.en.html

#Drawing a pair of tangents to a conic

import sys                                          #for path to external scripts
sys.path.insert(0, '/home/gadepall/github/geometry/codes/CoordGeo')        #path to my scripts
import numpy as np
import matplotlib.pyplot as plt
from numpy import linalg as LA

#local imports
from line.funcs import *
from triangle.funcs import *
from conics.funcs import *

#if using termux
import subprocess
import shlex
#end if



#Triangle vertices
A = np.array([1,-1]).reshape(-1,1)
B = np.array([-4,6]).reshape(-1,1) 
C = np.array([-3,-5]).reshape(-1,1) 

#Incircle parameters

h  = A
V = np.eye(2)
[I,r] = icircle(A,B,C)
u = -I
f = LA.norm(I)**2-r**2

[x1,x2] = contact(V,u,f,h)
print(x1,x2)


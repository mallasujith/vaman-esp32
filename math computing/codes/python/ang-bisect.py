#Code by GVV Sharma
#October 2, 2023
#released under GNU GPL
#Angle Bisectors of a triangle
#Incentre and Incircle


import sys                                          #for path to external scripts
sys.path.insert(0, '/sdcard/github/geometry/codes/CoordGeo')        #path to my scripts
import numpy as np
import scipy.linalg as SA
import mpmath as mp
import numpy.linalg as LA
import matplotlib.pyplot as plt
import matplotlib.image as mpimg

#local imports
from line.funcs import *
from triangle.funcs import *
from conics.funcs import circ_gen


#if using termux
import subprocess
import shlex
#end if

#Triangle vertices
A = np.array([1,-1]).reshape(-1,1)
B = np.array([-4,6]).reshape(-1,1) 
C = np.array([-3,-5]).reshape(-1,1) 

#Tangent sides
sides = tri_sides(A,B,C)
print(sides)


#Generating m,n,p
I_circ_mat= SA.circulant([1,1,0]).T
insides = LA.inv(I_circ_mat)@sides
print(sides,insides)

#Points of contact
F = (insides[1]*A+ insides[0]*B)/(insides[1]+ insides[0])
D = (insides[2]*B+ insides[1]*C)/(insides[2]+ insides[1])
E = (insides[0]*C+ insides[2]*A)/(insides[0]+ insides[2])
print(D,E,F)

#Generating the circumcircle
[O,R] = ccircle(D,E,F)
x_circ= circ_gen(O,R)

#angle bisector verification
m1 = dir_vec(A,B)
m2 = dir_vec(B,C)
m3 = dir_vec(C,A)
v1 = dir_vec(O,A)
v2 = dir_vec(O,B)
v3 = dir_vec(O,C)

'''
print(ang_vec(m1,-v1), ang_vec(m3,v1))
print(ang_vec(m2,-v2), ang_vec(m1,v2))
print(ang_vec(m3,-v3), ang_vec(m2,v3))
'''

#Plotting the incircle
plt.plot(x_circ[0,:],x_circ[1,:],label='$circumcircle$')

#Generating all lines
x_AB = line_gen(A,B)
x_BC = line_gen(B,C)
x_CA = line_gen(C,A)
x_OA = line_gen(O,A)
x_OB = line_gen(O,B)
x_OC = line_gen(O,C)

#Plotting all lines
plt.plot(x_AB[0,:],x_AB[1,:])
plt.plot(x_BC[0,:],x_BC[1,:])
plt.plot(x_CA[0,:],x_CA[1,:])
plt.plot(x_OA[0,:],x_OA[1,:],'--',label='$OA$')
plt.plot(x_OB[0,:],x_OB[1,:],'--',label='$OB$')
plt.plot(x_OC[0,:],x_OC[1,:],'--',label='$OC$')

#Labeling the coordinates
tri_coords = np.block([[A,B,C,D,E,F,O]])
plt.scatter(tri_coords[0,:], tri_coords[1,:])
vert_labels = ['A','B','C','D','E','F','I']
for i, txt in enumerate(vert_labels):
    plt.annotate(txt, # this is the text
                 (tri_coords[0,i], tri_coords[1,i]), # this is the point to label
                 textcoords="offset points", # how to position the text
                 xytext=(0,10), # distance from text to points (x,y)
                 ha='center') # horizontal alignment can be left, right or center
plt.xlabel('$x$')
plt.ylabel('$y$')
plt.legend(loc='best')
plt.grid() # minor
plt.axis('equal')

#if using termux
plt.savefig('figs/triangle/ang-bisect.pdf')
#subprocess.run(shlex.split("termux-open ./figs/tri_sss.pdf"))
#else
plt.show()

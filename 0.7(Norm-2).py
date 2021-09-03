# -*- coding: utf-8 -*-
"""
Created on Mon Nov 16 15:08:18 2020

@author: Buklya
"""

import numpy as np
import matplotlib as mp
from matplotlib import pyplot as plt
from scipy.stats import norm
import math

def decomposition(Q):
    ro = Q [0] [1] / ((Q[0][0])**(0.5) * (Q[1][1])**(0.5))
    a1 = (0.5*(1 + (1 - ro**2)**(0.5)))**(0.5)
    a2 = ro/(2*a1)
    A1 = np.array([ [a1,a2], [a2,a1] ])
    A2 = np.array([[(Q[0][0])**(0.5),0],[0,(Q[1][1])**(0.5)]])
    A = A2.dot(A1)
    return A

def elips(c, A):
    xx0 = [(-1)*(c**(0.5)) + t*0.01 for t in range(math.trunc(200*c**(0.5)))]
    xx1 = [c**(0.5) - t*0.01 for t in range(math.trunc(200*c**(0.5)))]
    yy0 = [(c - x**2)**(0.5) for x in xx0]
    yy1 = [(-1)*((c - x**2)**(0.5)) for x in xx1]
    xx = np.append(np.append(xx0, xx1),(-1)*(c**(0.5)))
    yy = np.append(np.append(yy0, yy1),0)
    for t in range(2 * math.trunc(200*c**(0.5)) + 1):
        vector = np.array([[xx[t]], [yy[t]]])
        result = A.dot(vector)
        xx[t] =result[0] [0]
        yy[t] = result [1] [0]
    xy = [xx, yy]
    return xy

def check(m_c, sample, Q, A, size):
    col = ['plum', 'royalblue','lightseagreen', 'mediumspringgreen','gold']
    total = [0, 0, 0, 0]
    for v in sample:
        dist = v.transpose().dot(np.linalg.inv(Q).dot(v))
        for i in range(size):
            if dist < m_c[i]:
                total[i]+=1
                plt.scatter(v[0] [0], v[1] [0], color = col[i], marker = 'o', s = 10); 
                break;
        if dist >= m_c[size-1]:
            plt.scatter(v[0] [0], v[1] [0], color = col[size], marker = 'o', s = 10); 
    for i in range(size):
        xy = elips(m_c[i], A)
        label = "c =" + str(m_c[i])
        plt.plot([x for x in xy[0]], [y for y in xy[1]], color = col[i],label = label )
    for i in range(size-1):
        total[i+1]+=total[i]
    return total
        
Q = np.array([[16,9],[9,25]])
A = decomposition(Q)
sample = [A.dot(np.array([np.random.normal(0,1,1), np.random.normal(0,1,1)])) for t in range(100)]
m_c = [1,2,3,4]
total = check(m_c, sample, Q, A, 4)
print("Amount dots in elips is ")
print(total)
plt.legend();
plt.axis('equal');

fig = plt.figure(2)
for i in range(9):
    ax = fig.add_subplot(3,3,i+1)
    ro = -1 + i/4
    if ro == -1:
        ro = -0.999
    if ro == 1:
        ro = 0.999
    Q = np.array([[1,ro],[ro,1]])
    A = decomposition(Q)
    sample = [A.dot(np.array([np.random.normal(0,1,1), np.random.normal(0,1,1)])) for t in range(40)]
    m_c = [1,4]
    check(m_c, sample, Q, A, 2)
    ax.axis('equal')

plt.show()
    


# -*- coding: utf-8 -*-
"""
Created on Wed Feb 10 09:38:57 2021

@author: Buklya
"""
import numpy as np
import math
from matplotlib import pyplot as plt
import random
import matplotlib.ticker as ticker
from matplotlib.patches import FancyBboxPatch


def generate_color():
    color = '#{:02x}{:02x}{:02x}'.format(*map(lambda x: random.randint(0, 255), range(3)))
    return color

def one_trajectory(lambd, T, flag):
    Time = 0
    t_k = [0]
    k = 0
    t0 = random.expovariate(lambd)
    while Time + t0 < T:
        k+=1
        Time+= t0
        t_k.append(Time)
        t0 = random.expovariate(lambd)
    t_k.append(T)   
    f = 1
    color = generate_color()
    if (flag and f) :
        for k0 in range(k):
            if k0 > 0:
                plt.plot([t_k[k0],t_k[k0]],[k0-1, k0], color = color)
            plt.plot([t_k[k0],t_k[k0+1]],[k0, k0], color = color)
        if k != 0:
            plt.plot([Time,Time],[k-1, k], color = color)
            plt.plot([Time, T],[k,k], color = color)
    else:
        if(flag):
            yy =[x for x in range(k+1)]
            yy.append(k)
            plt.plot(t_k, yy, color = color)
    return k


def hi_2(res, size, lamb, n):
    s = 0
    p = 0
    for i in range(size - 1):
        n_k = n * math.pow(lamb, i) * math.exp((-1)*lamb) /(math.factorial(i))
        p+=n_k
        s+=((res[i] - n_k )**2) / n_k
    n_size = n - p
    s+=((res[size - 1] - n_size )**2 ) / n_size
    return s

def toFixed(numObj, digits=0):
    return f"{numObj:.{digits}f}"

amount = 1000
size = 15
results = np.zeros(size)
T = 3
lam = 2

fig = plt.figure(figsize = (10,10))
ax = fig.add_subplot(1, 1, 1)
ax.set_xlim([0, 2*T])
ax.set_ylim([0, size])
ax.yaxis.set_major_locator(ticker.MultipleLocator(1))
ax.xaxis.set_major_locator(ticker.MultipleLocator(T))
ax.grid(which='major',color = 'lightgrey')

for n in range(amount):
    fl = 0
    if n % 40 == 0:
        fl = 1
    k = one_trajectory(lam, T, fl)
    if k >= size :
        k = size - 1
    results[k] = results[k]+1
lamb = T*lam


for i in range(size):
    plt.plot([T,T + results[i]*0.01],[i,i],  color ='lime', linewidth = 3)
    plt.text(T + results[i]*0.01 + 0.2, i - 0.1, int(results[i]) , fontsize=12, color = 'green' )
    ##plt.text(T - 0.2, i - 0.1, i , fontsize=10, color = 'k' )
    n_k = amount * math.pow(lamb, i) * math.exp((-1)*lamb) /(math.factorial(i))
    plt.plot([T,T + n_k*0.01],[i-0.1,i - 0.1],  color ='red')
    
plt.plot([T,T + results[0]*0.01],[0,0],  color ='lime', linewidth = 3, label = 'experemental')
plt.plot([T,T + n_k*0.01],[size - 1 -0.1,size - 1 - 0.1],  color ='red', label = 'theoretical')
print(hi_2(results, size, T*lam, amount))
plt.legend();

plt.text(2*T - 1, size - 1.5, 'hi^2 is '+ toFixed(hi_2(results, size, T*lam, amount), 2), fontsize=12, color = 'red' )

T0 = 2
lam = 0.6
fig = plt.figure(figsize = (10,10))
ax = fig.add_subplot(1, 1, 1)
ax.set_xlim([0, 5*T0])
ax.set_ylim([0, size])
ax.yaxis.set_major_locator(ticker.MultipleLocator(1))
ax.xaxis.set_major_locator(ticker.MultipleLocator(T0))
ax.grid(which='major',color = 'lightgrey')

for T in [T0 + T0*t for t in range(4)] :
    for i in range(size) :
        results[i] = 0
    for n in range(amount):
        fl = 0
        if T == T0 and n%40 == 0:
            fl = 1
        k = one_trajectory(lam, T, fl)
        if k >= size :
            k = size - 1
        results[k] = results[k]+1
    lamb = T*lam

    for i in range(size):
        plt.plot([T,T + results[i]*0.005],[i,i],  color ='lime', linewidth = 3)
        if i > 0 and results[i-1]!= 0:
          plt.plot([T + results[i-1]*0.005,T + results[i]*0.005],[i-1,i],  color ='plum', linewidth = 1)  
        ##plt.text(T - 0.2, i - 0.1, i , fontsize=10, color = 'k' )
    plt.text(T-1, 14, 'lambda is ' + toFixed(T*lam, 2) , fontsize=12, color = 'k' )

    
    
# coding: utf-8
# %load plot_xbj.py
import matplotlib.pyplot as plt
import pandas as pd
table1= pd.read_table('../xbj/D2_kin1.dat')
table2= pd.read_table('../xbj/D2_kin3.dat')
table3= pd.read_table('../xbj/D2_kin3.dat')
table5= pd.read_table('../xbj/D2_kin5.dat')
table=[table1,table2,table3,table5]
t=pd.concat(table)
ax = table1.plot(x='Xbj',y='Yield',yerr='Error',kind='scatter',label='Kin1')
table2.plot(ax=ax,x='Xbj',y='Yield',yerr='Error',kind='scatter',marker='*',color='r',label='Kin2')
table3.plot(ax=ax,x='Xbj',y='Yield',yerr='Error',kind='scatter',marker='1',color='m',label='Kin3')
table5.plot(ax=ax,x='Xbj',y='Yield',yerr='Error',kind='scatter',marker='4',color='g',label='Kin5')
plt.show()

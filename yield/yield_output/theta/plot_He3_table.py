# coding: utf-8
import matplotlib.pyplot as plt
import pandas as pd
table3= pd.read_table('He3_kin3.dat')
table1= pd.read_table('He3_kin1.dat')
table2= pd.read_table('He3_kin2.dat')
table3= pd.read_table('He3_kin3.dat')
table4= pd.read_table('He3_kin4.dat')
table5= pd.read_table('He3_kin5.dat')
table=[table1,table2,table3,table4,table5]
t=pd.concat(table)
t.plot(x='theta',y='Yield')
plt.show()

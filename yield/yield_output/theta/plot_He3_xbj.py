# coding: utf-8
import matplotlib.pyplot as plt
import pandas as pd
import glob
import sys
tgt='He3'
search='../xbj/{}_kin*.dat'.format(tgt)
files=glob.glob(search)
kins=[]
kinnames=[]
for file in files:
    beg=file.find('kin')
    end=file.find('.dat')
    kinnames.append(file[beg:end])
    kins.append(pd.read_table(file))
        
colors=['#0000FF','#DC143C','#00FFFF','#8B008B','#483D8B','#FF1493','#FF00FF','#F08080','#FFA500','#87CEEB','#EE82EE','#708090']
marker=['o','v','^','*','1,','2','3','h','+','D','p','s','4']

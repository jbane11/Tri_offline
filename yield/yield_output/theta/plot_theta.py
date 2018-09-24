import matplotlib.pyplot as plt
import pandas as pd
import glob
import sys
import numpy as py
import matplotlib._color_data as mcd
overlap = [name for name in mcd.CSS4_COLORS if "xkcd:" + name in mcd.XKCD_COLORS]
tgt=''
if len(sys.argv) > 1:
    tgt=sys.argv[1]
else : 
    exit
search='./{}_kin*.dat'.format(tgt)
files=glob.glob(search)
kins={}
kins_short={}
kinnames=[]
i=0
for file in files:
    beg=file.find('kin')
    end=file.find('.dat')
    kinnames.append(file[beg:end])
    kins[i]=pd.read_table(file)
    median = kins[i]['Yield'].mean()
    kins_short[i] = kins[i][kins[i].Yield > median*.07]
    #kins_short[i] = kins_short[i]['Yield']*1000
    #kins_short[i] = kins_short[i]['Error']*1000
	 
    i=i+1
        
col=('#0000FF','#DC143C','#00FFFF','#8B008B','#483D8B','#FF1493','#FF00FF','#F08080','#FFA500','#87CEEB','#EE82EE','#708090')
mark=('o','v','^','*','1','2','3','h','+','D','p','s','4')
ax = kins[0].plot(x='theta',y='Yield',yerr='Error',kind='scatter',label=kinnames[0])
for j in range(1,len(files),1):
    xkcd = mcd.XKCD_COLORS["xkcd:" + overlap[j]].upper()
    kins[j].plot(ax=ax,x='theta',y='Yield',yerr='Error',kind='scatter',color=xkcd, 
    marker=mark[j],label=kinnames[j])
last=len(kins)-1
plt.ylim(ymin=kins[last].Yield.min()*0.9,ymax=kins[0].Yield.max()*1.3)
print(kins[last].Yield.min()*0.9,kins[0].Yield.max()*1.3)
plt.grid(True)
ax.set_yscale("log")
       
ax2 = kins_short[0].plot(x='theta',y='Yield',yerr='Error',kind='scatter',label=kinnames[0])
for j in range(1,len(files),1):
    xkcd = mcd.XKCD_COLORS["xkcd:" + overlap[j]].upper()
    kins_short[j].plot(ax=ax2,x='theta',y='Yield',yerr='Error',kind='scatter',color=xkcd, 
    marker=mark[j],label=kinnames[j])
last=len(kins_short)-1
plt.ylim(ymin=kins_short[last].Yield.min()*0.9,ymax=kins_short[0].Yield.max()*1.3)    
ax2.set_yscale("log")
plt.grid(True)
plt.show()

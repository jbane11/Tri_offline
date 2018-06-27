debug=1


import math
from math import *
import matplotlib.gridspec as gridspec
import plot_header
import csv
import sys
import os
import numpy as np
import pandas as pd
from pandas import read_csv
import matplotlib.pyplot as plt
import itertools
from matplotlib.ticker import MultipleLocator, FormatStrFormatter
from matplotlib.backends.backend_pdf import PdfPages

def rvl(the_list,val):
	while val in the_list:
		the_list.remove(val)

def first_4chars(x):
    return(x[:4])

num_of_files=0
Labels = []
plots = []

in_arrays =[]
runs = []
s0L =[]
s0R =[]
s2L =[]
s2R =[]
cal1=[]
cal2=[]
cer =[] 
s0Ls =[]
s0Rs =[]
s2Ls =[]
s2Rs =[]
cal1s=[]
cal2s=[]
cers =[] 

s0Lt =[]
s0Rt =[]
s2Lt =[]
s2Rt =[]
cal1t=[]
cal2t=[]
cert =[] 
s0Lst =[]
s0Rst =[]
s2Lst =[]
s2Rst =[]
cal1st=[]
cal2st=[]
cerst =[] 
files_in=[]
files=[]
#for root, dirs, files in os.walk("./"):
#	if(debug):
#		 print('There are {} files in this dir'.format(len(files) ))

files_list = os.listdir("./")

for i in range(0,len(files_list),1):
	if '.csv' in files_list[i] and 'adc' not in files_list[i]:
		files_in.append(files_list[i].replace('.csv',''))
		
files_in = list(map(int,files_in))
files_in.sort()
files= list(map(str,files_in))
for filename in files:
	filename="./"+filename+'.csv'
	run=filename.replace('./','')
	run=int(run.replace('.csv',''))
	statinfo = os.stat(filename)
	size=statinfo.st_size
		
	if '.csv' in filename and size>=2000 and run > 1000 and run < 3500 : 
		runs.append(run)
		print('Looking in file {} with run {}'.format(filename,run))
		inpu=read_csv(filename,index_col=0)			
		in_arrays.append(inpu)
		in_arrays[num_of_files].drop(in_arrays[num_of_files].columns[len(in_arrays[num_of_files].columns)-1], axis=1, inplace=True)
			#print(in_array)
			
		if num_of_files==0:
			for lab in range (7):			
				Labels.append(in_arrays[num_of_files].index[lab])
			print(Labels)
			
		num_of_files=num_of_files+1
	#end of the read in files loop
data=pd.concat(in_arrays)
	
for r in range(0,data.index.size,7):
	for c in range(0,data.columns.size,2):
		cols=c+1
		#Sinicaltors
		s0Lt.append(data.iat[r,c])
		s0Lst.append(data.iat[r,cols])
		s0Rt.append(data.iat[r+1,c])
		s0Rst.append(data.iat[r+1,cols])
		s2Lt.append(data.iat[r+2,c])
		s2Lst.append(data.iat[r+2,cols])
		s2Rt.append(data.iat[r+3,c])
		s2Rst.append(data.iat[r+3,cols])
		#caloremitors
		cal1t.append(data.iat[r+4,c])
		cal1st.append(data.iat[r+4,cols])
		cal2t.append(data.iat[r+5,c])
		cal2st.append(data.iat[r+5,cols])
		#cer
		cert.append(data.iat[r+6,c])
		cerst.append(data.iat[r+6,cols])
	
	rvl(s0Lt,0.0)
	rvl(s0Lst,0.0)
	rvl(s0Rt,0.0)
	rvl(s0Rst,0.0)
	rvl(s2Lt,0.0)
	rvl(s2Lst,0.0)
	rvl(s2Rt,0.0)
	rvl(s2Rst,0.0)
	rvl(cal1t,0.0)
	rvl(cal1st,0.0)
	rvl(cal2t,0.0)
	rvl(cal2st,0.0)
	rvl(cert,0.0)
	rvl(cerst,0.0)
	
	#s0
	s0L.append(s0Lt)
	s0Ls.append(s0Lst)
	s0Lt = []
	s0Lst= []	
	s0R.append(s0Rt)
	s0Rs.append(s0Rst)
	s0Rt = []
	s0Rst= []	
	#s2
	s2L.append(s2Lt)
	s2Ls.append(s2Lst)
	s2Lt = []
	s2Lst= []	
	s2R.append(s2Rt)
	s2Rs.append(s2Rst)
	s2Rt = []
	s2Rst= []	
	#cal1
	cal1.append(cal1t)
	cal1s.append(cal1st)
	cal1t = []
	cal1st= []
	#cal2
	cal2.append(cal2t)
	cal2s.append(cal2st)
	cal2t = []
	cal2st= []
	#cer
	cer.append(cert)
	cers.append(cerst)
	cert = []
	cerst= []
s0L1= np.empty((1,len(runs)))
s0L1s=np.empty((1,len(runs)))
s0R1= np.empty((1,len(runs)))
s0R1s=np.empty((1,len(runs)))


S2_L=np.empty((16,len(runs)))
S2_L_sigma=np.empty((16,len(runs)))
S2_R=np.empty((16,len(runs)))
S2_R_sigma=np.empty((16,len(runs)))
cal_1=np.empty((len(cal1[1]),len(runs)))
cal_1_sigma=np.empty((len(cal1[1]),len(runs)))
cal_2=np.empty((len(cal2[1]),len(runs)))
cal_2_sigma=np.empty((len(cal2[1]),len(runs)))
cher=np.empty((len(cer[1]),len(runs)))
cher_sigma=np.empty((len(cer[1]),len(runs)))


for i in range(0,len(runs),1):

	s0L1[0][i]=(s0L[i][0])
	s0L1s[0][i]=(s0Ls[i][0])	

	s0R1[0][i]=(s0R[i][0])
	s0R1s[0][i]=(s0Rs[i][0])	

	
	for j in range(16):
		S2_L[j][i]=(s2L[i][j])
		S2_L_sigma[j][i]=(s2Ls[i][j])
		S2_R[j][i]=(s2R[i][j])
		S2_R_sigma[j][i]=(s2Rs[i][j])
	for j in range(len(cal1[1])):
		cal_1[j][i]=(cal1[i][j])
		cal_1_sigma[j][i]=(cal1s[i][j])

	for j in range(len(cal2[1])):
		cal_2[j][i]=(cal2[i][j])
		cal_2_sigma[j][i]=(cal2s[i][j])

	for j in range(len( cer[1])):
		cher[j][i]=(cer[i][j])
		cher_sigma[j][i]=(cers[i][j])
#######################################################
# Read in the pedestal from the db
ped_csv = read_csv("./adc_peds.csv",index_col=0)

#Now time to plot!!!
#S0!!!
S0_fig=plt.figure(figsize = (18.5,10.5),dpi=200)
grids = gridspec.GridSpec(2,1)
grids.update(hspace=0.05)
axs=[plt.subplot(grids[0]),plt.subplot(grids[1])]
	
minorLocator = MultipleLocator(5)

plt_s0L1=axs[0]
plt_s0L1.axhline(y=ped_csv.loc['L.S0.L'][1],xmin=0.0,xmax=1.0,c="black",linewidth=1.5,zorder=0, label="DB")
plt_s0L1.errorbar(runs,s0L1[0],yerr=s0L1s[0],fmt="o",label="S0 L")
plt_s0L1.legend()
plt_s0L1.grid(True)
plt_s0L1.set_xticklabels([])
#plt_s0L1.set_xlabel("Run Number")
plt_s0L1.set_ylabel("ADC channel")
plt_s0L1.set_title("S0 Pedestals")
plt_s0L1.xaxis.set_minor_locator(minorLocator)
plt_s0L1.grid(True,which='both')

plt_s0R1=axs[1]
plt_s0R1.axhline(y=ped_csv.loc['L.S0.R'][1],xmin=0.0,xmax=1.0,c="black",linewidth=1.5,zorder=0,label="DB")
plt_s0R1.errorbar(runs,s0R1[0],yerr=s0R1s[0],color='r',fmt="1", label="S0 R")
plt_s0R1.legend()
plt_s0R1.set_xlabel("Run Number")
plt_s0R1.set_ylabel("ADC channel")
plt_s0R1.xaxis.set_minor_locator(minorLocator)

#plt_s0R1.xaxis.set_major_locator(
plt_s0R1.grid(True,which='both')
S0_fig.savefig("graphs/S0_peds_runs.pdf")
#######################################################
print("s0")
### S2_L
color=['b','g','r','c','b','g','r','c','b','g','r','c','b','g','r','c']
form= ['o','1','2','3','4','s','p','+','x','D','<','>','v','*','h','_']
plt_S2L=[]
lbl_str=[]
S2_L_fig=[]
Multipage = PdfPages('graphs/S2_L_peds_runs.pdf')

for i in range(4):
	S2_L_fig.append(plt.figure(figsize = (18.5,10.5),dpi=200)) 	#set the canvases
	S2_L_fig[i].suptitle("S2 L pedestals")				#set title

	grids = gridspec.GridSpec(4,1)				#Set the number of subpads
	grids.update(hspace=0.05)
	pad=[]
	for j in range(4):
		pad.append(plt.subplot(grids[j]))		#Make plots in each grid
		plt_S2L.append(pad[j])
	for k in range(4):
		pmt=int(i*4+k)
		lbl_str.append("pmt {}".format(pmt))
		pad_num= k
		location="L.s2.L {}".format(pmt)
		plt_S2L[pmt].axhline(y=ped_csv.loc[location][1],xmin=0.0,xmax=1.0,c="black",linewidth=1.5,zorder=0, label="DB")
		plt_S2L[pmt].errorbar(runs,S2_L[pmt],S2_L_sigma[pmt],color=color[pmt],fmt=form[pmt], label = lbl_str[pmt])
		plt_S2L[pmt].legend()
		plt_S2L[pmt].grid(True,which='both')
		if k == 3:
			plt_S2L[pmt].set_xlabel("Run Number")
		if pmt/2.0 == floor(pmt/2):
			plt_S2L[pmt].set_ylabel("ADC channel")
		plt_S2L[pmt].xaxis.set_minor_locator(minorLocator)
		mean=np.mean(S2_L[pmt])
		plt_S2L[pmt].set_ylim(0.95*mean, 1.05*mean)
	#S2_L_fig[i].tight_layout()
	S2_L_fig[i].savefig(Multipage,format='pdf')
	plt.close()
Multipage.close()

### S2_R

color=['b','g','r','c','b','g','r','c','b','g','r','c','b','g','r','c']
form= ['o','1','2','3','4','s','p','+','x','D','<','>','v','*','h','_']
plt_S2R=[]
lbl_str=[]
S2_R_fig=[]
Multipage = PdfPages('graphs/S2_R_peds_runs.pdf')

for i in range(4):
	S2_R_fig.append(plt.figure(figsize = (18.5,10.5),dpi=200)) 	#set the canvases
	S2_R_fig[i].suptitle("S2 R pedestals")				#set title

	grids = gridspec.GridSpec(4,1)				#Set the number of subpads
	grids.update(hspace=0.05)
	pad=[]
	for j in range(4):
		pad.append(plt.subplot(grids[j]))		#Make plots in each grid
		plt_S2R.append(pad[j])
	for k in range(4):
		pmt=int(i*4+k)
		lbl_str.append("pmt {}".format(pmt))
		pad_num= k
		location="L.s2.R {}".format(pmt)
		plt_S2R[pmt].axhline(y=ped_csv.loc[location][1],xmin=0.0,xmax=1.0,c="black",linewidth=1.5,zorder=0, label="DB")
		plt_S2R[pmt].errorbar(runs,S2_R[pmt],S2_R_sigma[pmt],color=color[pmt],fmt=form[pmt], label = lbl_str[pmt])
		plt_S2R[pmt].legend()
		plt_S2R[pmt].grid(True,which='both')
		if k == 3:
			plt_S2R[pmt].set_xlabel("Run Number")
		if pmt/2.0 == floor(pmt/2):
			plt_S2R[pmt].set_ylabel("ADC channel")
		plt_S2R[pmt].xaxis.set_minor_locator(minorLocator)
		mean=np.mean(S2_R[pmt])
		plt_S2R[pmt].set_ylim(0.95*mean, 1.05*mean)
	#S2_R_fig[i].tight_layout()
	S2_R_fig[i].savefig(Multipage,format='pdf')
	plt.close()
Multipage.close()

print("s2")
### cal1

color=['b','g','r','c','b','g','r','c','b','g','r','c','b','g','r','c']
form= ['o','1','2','3','4','s','p','+','x','D','<','>','v','*','h','_']
plt_cal1=[]
lbl_str=[]
cal1_fig=[]
Multipage = PdfPages('graphs/Cal1_peds_runs.pdf')
num_of_pages = int(ceil(len(cal_1)/4.0))
clr_sel=0
fmt_sel=0

for i in range(num_of_pages):
	cal1_fig.append(plt.figure(figsize = (18.5,10.5),dpi=200)) 	#set the canvases
	cal1_fig[i].suptitle("{}  pedestals".format(Labels[4]))				#set title

	grids = gridspec.GridSpec(4,1)				#Set the number of subpads
	grids.update(hspace=0.05)
	pad=[]
	for k in range(4):
		pad.append(plt.subplot(grids[k]))		#Make plots in each grid
		plt_cal1.append(pad[k])
		pmt=int(i*4+k)
		lbl_str.append("pmt {}".format(pmt))
		pad_num= k
		location="L.prl1 {}".format(pmt)
		plt_cal1[pmt].axhline(y=ped_csv.loc[location][0],xmin=0.0,xmax=1.0,c="black",linewidth=1.5,zorder=0, label="DB")
		plt_cal1[pmt].errorbar(runs,cal_1[pmt],cal_1_sigma[pmt],color=color[clr_sel],fmt=form[fmt_sel], label = lbl_str[pmt])
		plt_cal1[pmt].legend()
		plt_cal1[pmt].grid(True,which='both')
		if k == 4:
			plt_cal1[pmt].set_xlabel("Run Number")
		if pmt/2.0 == floor(pmt/2):
			plt_cal1[pmt].set_ylabel("ADC channel")
		plt_cal1[pmt].xaxis.set_minor_locator(minorLocator)
		clr_sel += 1
		fmt_sel += 1
		if clr_sel >= 15:
			clr_sel=2
			fmt_sel=0
		mean=np.mean(cal_1[pmt])
		plt_cal1[pmt].set_ylim(0.9*mean, 1.1*mean)
		if pmt == (len(cal_1)-1):
			break
#	cal1_fig[i].tight_layout()
	cal1_fig[i].savefig(Multipage,format='pdf')
	plt.close()
Multipage.close()

print("cal1")
### cal2

color=['b','g','r','c','b','g','r','c','b','g','r','c','b','g','r','c']
form= ['o','1','2','3','4','s','p','+','x','D','<','>','v','*','h','_']
plt_cal2=[]
lbl_str=[]
cal2_fig=[]
Multipage = PdfPages('graphs/Cal2_peds_runs.pdf')
num_of_pages = int(ceil(len(cal_2)/4.0))
clr_sel=0
fmt_sel=0
for i in range(num_of_pages):
	cal2_fig.append(plt.figure(figsize = (18.5,10.5),dpi=200)) 	#set the canvases
	cal2_fig[i].suptitle("{}  pedestals".format(Labels[5]))				#set title

	grids = gridspec.GridSpec(4,1)				#Set the number of subpads
	grids.update(hspace=0.05)
	pad=[]
	for k in range(4):
		pad.append(plt.subplot(grids[k]))		#Make plots in each grid
		plt_cal2.append(pad[k])
		pmt=int(i*4+k)
		lbl_str.append("pmt {}".format(pmt))
		pad_num= k
		location="L.prl2 {}".format(pmt)
		plt_cal2[pmt].axhline(y=ped_csv.loc[location][0],xmin=0.0,xmax=1.0,c="black",linewidth=1.5,zorder=0, label="DB")
		plt_cal2[pmt].errorbar(runs,cal_2[pmt],cal_2_sigma[pmt],color=color[clr_sel],fmt=form[fmt_sel], label = lbl_str[pmt])
		plt_cal2[pmt].legend()
		plt_cal2[pmt].grid(True,which='both')
		if k == 4:
			plt_cal2[pmt].set_xlabel("Run Number")
		if pmt/2.0 == floor(pmt/2):
			plt_cal2[pmt].set_ylabel("ADC channel")
		plt_cal2[pmt].xaxis.set_minor_locator(minorLocator)
		clr_sel += 1
		fmt_sel += 1
		if clr_sel >= 15:
			clr_sel=2
			fmt_sel=0
		mean=np.mean(cal_2[pmt])
		plt_cal2[pmt].set_ylim(0.9*mean, 1.10*mean)
		if pmt == (len(cal_1)-1):
			break
#	cal2_fig[i].tight_layout()
	cal2_fig[i].savefig(Multipage,format='pdf')
	plt.close()
Multipage.close()
print("cal2")
### cer

color=['b','g','r','c','b','g','r','c','b','g','r','c','b','g','r','c']
form= ['o','1','2','3','4','s','p','+','x','D','<','>','v','*','h','_']
plt_cher=[]
lbl_str=[]
cher_fig=[]
Multipage = PdfPages('graphs/Cer_peds_runs.pdf')
num_of_pages = int(ceil(len(cher)/4.0))
clr_sel=0
fmt_sel=0

for i in range(num_of_pages):
	cher_fig.append(plt.figure(figsize = (18.5,10.5),dpi=72)) 	#set the canvases
	cher_fig[i].suptitle("{} pedestals".format(Labels[6]))		#set titl
	grids = gridspec.GridSpec(4,1)				#Set the number of subpads
	grids.update(hspace=0.05)
	pad=[]
	for k in range(4):
		pad.append(plt.subplot(grids[k]))		#Make plots in each grid
		plt_cher.append(pad[k])
		pmt=int(i*4+k)
		lbl_str.append("pmt {}".format(pmt))
		pad_num= k
		location="L.cer {}".format(pmt)
		plt_cher[pmt].axhline(y=ped_csv.loc[location][1],xmin=0.0,xmax=1.0,c="black",linewidth=1.5,zorder=0, label="DB")
		plt_cher[pmt].errorbar(runs,cher[pmt],cher_sigma[pmt],color=color[clr_sel],fmt=form[fmt_sel], label = lbl_str[pmt])
		plt_cher[pmt].legend()
		plt_cher[pmt].grid(True,which='both')
		if k == 4:
			plt_cher[pmt].set_xlabel("Run Number")
		if pmt/2.0 == floor(pmt/2):
			plt_cher[pmt].set_ylabel("ADC channel")
		plt_cher[pmt].xaxis.set_minor_locator(minorLocator)
		clr_sel += 1
		fmt_sel += 1
		if clr_sel >= 15:
			clr_sel=2
			fmt_sel=0
		mean=np.mean(cher[pmt])
		plt_cher[pmt].set_ylim(0.975*mean, 1.027*mean)
		if pmt == (len(cher)-1):
			break
#	cher_fig[i].tight_layout()
	cher_fig[i].savefig(Multipage,format='pdf')
	plt.close()
Multipage.close()


##analysis!!





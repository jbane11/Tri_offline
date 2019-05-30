
# coding: utf-8

# In[50]:
import uproot
import os,sys
import pandas as pd
import numpy as np

path_list=[]
import scipy.constants as sc


    #############
import mysql.connector
db_user= 'triton-user'
db_pswd= '3He3Hdata'
db_name= 'triton-work'
db_host= 'halladb'
a=[]

paths = [
  "/storage/MARATHONRootfiles/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin1/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin4/",
  "/run/media/jbane/Slim/MARATHONRoots/",
  "/storage/MARATHONRootfiles/",
  "/storage/MARATHONRootfiles/pass1/kin0/",
  "/storage/MARATHONRootfiles/pass1/kin1/",
  "/storage/MARATHONRootfiles/pass1/kin2/",
  "/storage/MARATHONRootfiles/pass1/kin3/",
  "/storage/MARATHONRootfiles/pass1/kin4/",
  "/storage/MARATHONRootfiles/pass1/kin5/",
  "/storage/MARATHONRootfiles/pass1/kin7/",
  "/storage/MARATHONRootfiles/pass1/kin9/",
  "/storage/MARATHONRootfiles/pass1/kin11/",
  "/storage/MARATHONRootfiles/pass1/kin13/",
  "/storage/MARATHONRootfiles/pass1/kin15/",
  "/storage/MARATHONRootfiles/pass1/kin16/",
  "/storage/MARATHONRootfiles/pass1/optics/",
  "/storage/MARATHONRootfiles/pass1/positron/",
  "/volatile/halla/triton/Marathon_Rootfiles/optics_trial/",
  "./tmproot/",
  "/home/shujie/jlab/MyTritium/Rootfiles/",
  "/volatile/halla/triton/nathaly/Rootfiles/",
  "/volatile/halla/triton/eep_Rootfiles/pass1/",
  "/volatile/halla/triton/Marathon_Rootfiles/pass1_calibration/",
  "/volatile/halla/triton/Marathon_Rootfiles/pass1/",
  "/volatile/halla/triton/Marathon_Rootfiles/pass1_fix/",
  "/chafs1/work1/tritium/Tritium_Rootfiles/",
  "/chafs1/work1/tritium/Rootfiles/",
  "/volatile/halla/triton/shujie/replay/Rootfiles/",
  "/volatile/halla/triton/Tritium_Rootfiles/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin1/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin2/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin3/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin4/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin5/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin7/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin9/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin11/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin13/"
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin15/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin16/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin0/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin1/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin2/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin3/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin4/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin5/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin7/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin9/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin11/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin13/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin15/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin16/",
  "./"]

debug=False

def DEBUG(d):
    global debug
    if d == 1:
        debug=True 
    else:
        debug=False
    print("switching debug to {}".format(debug))
    

def GetPaths():
    return paths


def buildpaths(paths,subdir):
    
    ###############
    
    roots={"/storage/MARATHONRootfiles/",
       "/v/lustre2/expphy/cache/halla/triton/prod/marathon/"}
    for root in roots:
        ddirs = os.listdir(root)
        for ddir in ddirs:
            if( os.path.isdir(root+ddir)):
                for subdir in os.listdir(root+ddir):
                    path = root+ddir+"/"+subdir
                    path_list.append("{}{}/{}".format(root,ddir,subdir))

    
    

def findroot(run):
    
    #############################
    for j,i in enumerate(paths):
        name=i+"tritium_"+str(run)+".root"
        if os.path.exists(name):
            break
        else :
            name=""
    return name


def loadrun(run):
    
    ############
    filename=findroot(run)
    rootfile=uproot.rootio.open(filename)
    return rootfile

def CalcZoff(run):
    ###################
    Zoff=0.0
    rootfile = loadrun(run)
    
    return Zoff

def SQL_whatkin(run):
    
    ###################3
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select Kinematic from MARATHONrunlist where run_number="{}"'.format(run)
    cursor.execute(query1)
    results= cursor.fetchall()
    rl={}
    
    if len(results) < 1:
        return "123"

    for i,row in enumerate(results):
        rl[i]=(row[0])

    cnx.close()
    kin=""
    if rl[0] is None:
        kin="123"
    else :
        kin=rl[0]
    return kin

def kinparse(fullkin):
    underpos = fullkin.find('_')
    ##########
    if underpos >= 0 :
        kinsub = fullkin[0:underpos]
        sufsub = fullkin[underpos+1:len(fullkin)]
        
    else :
        kinsub=fullkin
        sufsub=""
        
    return kinsub,sufsub            


    
def SQLRuns(itgt,kin,suf):


    ##########################################
    #C12 = ["Carbon", "CF","C12"]
    #H3  = ["Tritium" ,"T", "T3", "H3" ]
    #D2  = ["Deuterium","D2", "D", "H2" ]
    #H   = ["Hydrogen","H1", "H" ]
    #He3 = ["Helium-3","Helium","He3"]
    #if itgt in C12:
    #    TGT = C12[0]
    #if itgt in H3 :
    #     TGT = H3[0]
    #if itgt in D2 :
    #     TGT = D2[0]
    #if itgt in H :
    #     TGT = H[0]
    #if itgt in He3 :
    #     TGT = He3[0]
            
    tgt=gettgtlong(itgt)         

    #tgt=TGT
    #########################################        
    if suf == 'all':
        suf='%'
    if suf != 'i%':
        suf = suf + '%'
        
        Like='= "{}"'.format(kin)
    if int(kin) >= 7:
        Like= 'like "{}/_{}" ESCAPE "/"'.format(kin,suf)
    #############
    #print("check!!!", Like)
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    
   # select run_number from MARATHONrunlist where ( kinematic = '5' or kinematic like '5/_1st%' ESCAPE '/' ) and target='Carbon' and prescale_T2=1 order by run_number asc
    
    query1 = 'select run_number from MARATHONrunlist where (kinematic {}) and target="{}" and kinematic not like "%pos" and prescale_T2=1 order by run_number asc'.format(Like,tgt)
    cursor.execute(query1)
    results= cursor.fetchall()
    if debug:
        print(query1)    
    if len(results) is 0:
        query = 'select run_number from MARATHONrunlist where kinematic = "{}" and target="{}" and kinematic not like "%pos" and prescale_T2=1 order by run_number asc'.format(kin,tgt)    
        cursor.execute(query)
        results= cursor.fetchall()
       

    rl={}
    for i,row in enumerate(results):
        rl[i]=(row[0])
    cnx.close()
    
    return rl
    ####################


# In[62]:


def SQLangle(run):
    
    angle=0.0
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select angle from MARATHONrunlist where run_number={};'.format(run)
    cursor.execute(query1)
    results=cursor.fetchall()
    rl={}
    if len(results)>0 :
        for i,row in enumerate(results):
            i=i
        angle=row[0]
    cnx.close()
    
    return angle


# In[63]:

    


def SQLPS(run,trig):
    
    ps=0
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select prescale_T{} from MARATHONrunlist where run_number={};'.format( trig, run)
    cursor.execute(query1)
    results=cursor.fetchall()
    rl={}
    if len(results)>0 :
        for i,row in enumerate(results):
            i=i
        ps=row[0]
    cnx.close()
    

    if ps != ps :
        print("hmm", ps)
    return ps


# In[65]:


def SQLCharge(run,threshold):

    charge=0.0
    current=0.0
    
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select charge{} ,current{} from MARATHONchargelist where run_number={};'.format( threshold, threshold, run)
    
    cursor.execute(query1)
    results=cursor.fetchall()
    rl={}
    if len(results)>0 :
        for i,row in enumerate(results):
            i=i
        charge=row[0]
        current=row[1]
            
    cnx.close()        
    
    return charge, current
  


# In[67]:
def SQL_CorList(run):
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select * from MARATHONanalysis where run_number={};'.format(run)
    cursor.execute(query1)
    results=cursor.fetchall()
    cnx.close()        


def SQL_LT(run):
    LT=1
    
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select livetime from MARATHONanalysis where run_number={};'.format(run)
    
    cursor.execute(query1)
    results=cursor.fetchall()
    rl={}
    if len(results)>0 :
        for i,row in enumerate(results):
            i=i
        LT=row[0]
    cnx.close()        
    
    
    
    return LT


# In[81]:


def SQLtar(run):
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select target from MARATHONrunlist where run_number={};'.format(run)
    cursor.execute(query1)
    results=cursor.fetchall()
    rl={}
    if len(results)>0 :
        for i,row in enumerate(results):
            i=i
        tar=row[0]
    else :
        tar="bad"
    cnx.close()        
    
    
    return tar


def SQL_beaminfo(run):
    
        #############
    import mysql.connector
    db_user= 'triton-user'
    db_pswd= '3He3Hdata'
    db_name= 'triton-work'
    db_host= 'halladb'

    
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select beam_x_pos, beam_y_pos, beam_x_size, beam_y_size from MARATHONanalysis where run_number={};'.format(run)
    cursor.execute(query1)
    results=cursor.fetchall()

    rl=[]
    bad=0
    j=0
    if len(results)>0 :
        for i,row in enumerate(results):
            for j in range(len(row)):
                
                if row[j] is None:
                        bad=1
                rl.append(row[j])
            if j >=3:
                break
                    
    if bad == 1:
        print("bad run")
        rl[0] = 0.0
        rl[1] = 0.0
        rl[2] = 0.20
        rl[3] = 0.20
        print(rl)
    if len(rl) is not 4:
        rl.append(0.0)
        rl.append(0.0)
        rl.append(0.2)
        rl.append(0.2)
    cnx.close()        
    
    
    return rl   


def SQL_CorList(run):
    comps=["eff","err"]
    allnames=[]
    names=[]
    nums=[]
    effs=[]
    eff={}
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select * from MARATHONanalysis where run_number={};'.format(run)
    cursor.execute(query1)
    des =cursor.description
    results=cursor.fetchall()
    cnx.close()  

    if len(results) >= 1:
        for i in range(len(results[0])):
            allnames.append(des[i][0])
        
        for i in range(len(results[0])):
            if comps[0] in des[i][0] :
                if "NE"  in des[i][0] :
                    continue
                names.append(des[i][0])
                nums.append(results[0][i])
                shrt = des[i][0].replace("_eff","")
                lng="{}_err".format(shrt)
                loc = allnames.index(lng)
                effs.append(results[0][loc])
                print(shrt,results[0][i],results[0][loc]  )
                eff[i]={"name":shrt, "eff": results[0][i],"err":results[0][loc] }
         
                
    return names,nums,effs,allnames,eff


def SQL_CorListNE(run):
    comps=["eff","err"]
    allnames=[]
    names=[]
    nums=[]
    effs=[]
    eff={}
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select * from MARATHONanalysis where run_number={};'.format(run)
    cursor.execute(query1)
    des =cursor.description
    results=cursor.fetchall()
    cnx.close()  

    if len(results) >= 1:
        for i in range(len(results[0])):
            allnames.append(des[i][0])
        
        for i in range(len(results[0])):
            if comps[0] in des[i][0] :
               # if "NE"  in des[i][0] :
               #     continue
                names.append(des[i][0])
                nums.append(results[0][i])
                shrt = des[i][0].replace("_eff","")
                lng="{}_err".format(shrt)
                loc = allnames.index(lng)
                effs.append(results[0][loc])
                #print(shrt,results[0][i],results[0][loc]  )
                if "NE" in shrt:
                    eff[i]={"name":shrt, "eff": results[0][i],"err":results[0][loc] }        
                else:
                    eff[i]={"name":shrt, "eff": results[0][i],"err":results[0][loc] }
         
                
    return names,nums,effs,allnames,eff

def listmult(A,name):
    B=1.0
    index=A.columns
    for i in index:
        if "NE" in A.loc["name"][i]:
            B=B*(1/A.loc[name][i])
        else:
            B=B*A.loc[name][i]
    return B

def inquad(A,name,name1):
    B=0.0
    index=A.columns
    for i in index:
        #if "NE" in A.loc["name"][i]:
        #    B=B+( (1-A.loc[name1][i]) * A.loc[name][i] )**2
        #else :    
        B=B+(A.loc[name][i])**2
    B=np.sqrt(B)
    return B


def get_runcor(run):
    names,nums,effs,allnames,eff =SQL_CorListNE(run)
    DF1=pd.DataFrame.from_dict(eff)
    eff[len(allnames)]={"name": "Total" , "eff" :listmult(DF1,"eff"), "err" :inquad(DF1,"err","eff")}
    DF1=pd.DataFrame.from_dict(eff)
    DF1=DF1.reindex(['name','eff','err'])
    c=DF1[len(allnames)]
    effnm=c['eff']
    errnm=c['err']
    return effnm,errnm,DF1
    
def gettgtshrt(itgt):
    ##########################################
    C12 = ["Carbon", "CF","C12","Carbon "]
    H3  = ["Tritium" ,"T", "T3", "H3", "Tritium " ]
    D2  = ["Deuterium","D2", "D", "H2", "Deuterium " ]
    H   = ["Hydrogen","H1", "H", "Hydrogen " ]
    He3 = ["Helium-3","Helium","He3","Helium-3 "]
    if itgt in C12:
        TGT = C12[2]
    if itgt in H3 :
         TGT = H3[3]
    if itgt in D2 :
         TGT = D2[1]
    if itgt in H :
         TGT = H[2]
    if itgt in He3 :
         TGT = He3[2]
    return TGT
def gettgtlong(itgt):
    ##########################################
    C12 = ["Carbon", "CF","C12","Carbon "]
    H3  = ["Tritium" ,"T", "T3", "H3", "Tritium " ]
    D2  = ["Deuterium","D2", "D", "H2", "Deuterium " ]
    H   = ["Hydrogen","H1", "H", "Hydrogen " ]
    He3 = ["Helium-3","Helium","He3","Helium-3 "]
    if itgt in C12:
        TGT = C12[0]
    elif itgt in H3 :
         TGT = H3[0]
    elif itgt in D2 :
         TGT = D2[0]
    elif itgt in H :
         TGT = H[0]
    elif itgt in He3 :
         TGT = He3[0]
    else :
        print(itgt,"Not found")
        TGT=itgt
    return TGT

def Dens_Cor(run):   
    tgt=SQLtar(run)  
    charge,current = SQLCharge(run,0)
    target=gettgtlong(tgt)

    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select density_par_0,density_err_0,density_par_1,density_err_1,density_par_2,density_err_2,density_CV_0,density_CV_1,density_CV_2 from MARATHONTargetInfo where name=\"{}\"'.format(target)
    cursor.execute(query1)
    results= cursor.fetchall()

    dens_par0=results[0][0]
    dens_err0=results[0][1]
    dens_par1=results[0][2]
    dens_err1=results[0][3]
    dens_par2=results[0][4]
    dens_err2=results[0][5]
    dens_CV0 =results[0][6]
    dens_CV1 =results[0][7]
    dens_CV2 =results[0][8]

    err0 = dens_err0
    err1 = dens_err1*pow(current,2)
    err2 = dens_err2*pow(current,4)
    CV0 = 2*current*dens_CV0
    CV1 = 2*current*current*dens_CV1
    CV2 = 2*pow(current,3)*dens_CV2

    dens_cor = dens_par0 + dens_par1*current + dens_par2*pow(current,2)
    Cerr1 = np.sqrt(err0 + err1 + err2 + CV0+CV1+CV2)
    Cerr1 = Cerr1 * dens_cor
    if debug:
        print(" correction factor = %.4f  with error of +- %.5f."%(dens_cor,Cerr1))
    return dens_cor,Cerr1

def Calc_Lum(run):
    tgt=SQLtar(run)  
    charge,current = SQLCharge(run,0)
    den_cor,den_cor_err=Dens_Cor(run)
    tgt=gettgtlong(tgt)
    
    if tgt == "Tritium" or tgt == "Tritium ":
        atomicMass = 3.016;
    elif(tgt == "Helium-3"):
        atomicMass = 3.016;
    elif(tgt == "Deuterium"):
        atomicMass = 2.014102;
    elif(tgt == "Hydrogen"):
        atomicMass = 1.007947;
    elif(tgt=="Carbon"):
        atomicMass = 12.01;

    Qe= sc.e
    Na=sc.N_A
    CMtoNB=1.0e33;

    target=gettgtlong(tgt)

    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select Thickness, Thickness_err from MARATHONTargetInfo where name=\"{}\"'.format(target)
    cursor.execute(query1)
    results= cursor.fetchall()

    tgt_thick=results[0][0]
    thick_err=results[0][1]

    charge_E = charge / ( Qe*1e6)
    charge_err = 0.005
    
    lumin = (charge_E*tgt_thick*den_cor*Na/atomicMass)/CMtoNB
    lum_err = np.sqrt( pow(charge_err,2) +pow((thick_err/(tgt_thick*1.0)),2)  +pow(den_cor_err,2) )
    lum_err=lum_err*lumin
    if debug :
        print(" Luminosity = %.2f  with error of +- %.3f abs."%(lumin,lum_err))
    return lumin,lum_err

def total_lum(tgt,kin,suf):
    runlum={}
    runlist=SQLRuns(tgt,kin,suf)
    if debug:
        print(runlist)
    total_lum=0.0
    total_err=0.0
    for i in runlist:
        if debug:
            print(i,runlist[i])
        runlum[i]=Calc_Lum(runlist[i])
        total_lum= total_lum + runlum[i][0]
        total_err = np.sqrt(  pow(total_err,2) + pow(runlum[i][1],2))
    return total_lum,total_err
    
def PC(tgt,xbj):
    target=gettgtlong(tgt)
    sht=gettgtshrt(tgt)
    if sht=="C12":
        PCfact=0.0*xbj
        PC_error=0.0*xbj
        return PCfact, PC_error
    
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select positron_par_1, positron_err_1, positron_par_2, positron_err_2, positron_err_covariance from MARATHONTargetInfo where name=\"{}\"'.format(target)
    cursor.execute(query1)
    results= cursor.fetchall()

    par1=results[0][0]
    err1=results[0][1]
    par2=results[0][2]
    err2=results[0][3]
    CV=results[0][4]
    PCfact = np.exp(par1+par2*xbj)
    PC_error = PCfact*np.sqrt(err1+err2*xbj*xbj +2.0*CV*xbj)
    
    return PCfact, PC_error

def ECC(tgt,kin):
    if tgt == "C12":
        return 0.0
    filen="/home/jbane/Documents/ECC_table.csv"
    ECCdf=pd.read_csv(filen)
    ECCdf=ECCdf.fillna(0.0)
    ECCdf=ECCdf.rename(index=str, columns={"Unnamed: 0":"Kin"})
    ECCf=ECCdf[tgt][ECCdf["Kin"]==int(kin)]
    a=ECCf[0]
    return a

def total_eff(tgt,kin,suf):
    runlist=SQLRuns(tgt,kin,suf)
    corl=[]
    luml=[]
    totlum=0
    totf=0
    werr=0
    for i in runlist:

        run=runlist[i]
        cor,corerr,corDF=get_runcor(run)
        lum,lumerr=Calc_Lum(run)
        corl.append(cor)
        luml.append(lum)
        werr = werr + corerr*lum

        totlum=totlum+lum
        totf=cor*lum + totf
        #print(totf/totlum , corerr,  werr/totlum)
    return totf/totlum, werr/totlum



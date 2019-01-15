
# coding: utf-8

# In[50]:
import uproot
import os,sys
path_list=[]

from IPython.core.display import display, HTML
display(HTML("<style>.container { width:100% !important; }</style>"))

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
        return ""

    for i,row in enumerate(results):
        rl[i]=(row[0])

    cnx.close()
    kin=""
    if rl[0] is None:
        kin=""
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
    C12 = ["Carbon", "CF","C12"]
    H3  = ["Tritium" ,"T", "T3", "H3" ]
    D2  = ["Deuterium","D2", "D", "H2" ]
    H   = ["Hydrogen","H1", "H" ]
    He3 = ["Helium-3","Helium","He3"]
    if itgt in C12:
        TGT = C12[0]
    if itgt in H3 :
         TGT = H3[0]
    if itgt in D2 :
         TGT = D2[0]
    if itgt in H :
         TGT = H[0]
    if itgt in He3 :
         TGT = He3[0]

    tgt=TGT
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
    query1 = 'select run_number from MARATHONrunlist where (kinematic {}) and target="{}" order by run_number asc'.format(Like,tgt)
    cursor.execute(query1)
    results= cursor.fetchall()
        
    if len(results) is 0:
        query = 'select run_number from MARATHONrunlist where kinematic = "{}" and target="{}" order by run_number asc'.format(kin,tgt)    
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

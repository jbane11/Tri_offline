
# coding: utf-8

# In[50]:


from IPython.core.display import display, HTML
display(HTML("<style>.container { width:100% !important; }</style>"))

    #############
import mysql.connector
db_user= 'triton-user'
db_pswd= '3He3Hdata'
db_name= 'triton-work'
db_host= 'halladb'
    
def SQLRuns(tgt,kin,suf):


    ##########################################
    H3  = ["Tritium" ,"T", "T3", "H3" ]
    D2  = ["Deuterium","D2", "D", "H2" ]
    H   = ["Hydrogen","H1", "H" ]
    He3 = ["Helium-3","Helium","He3"]
    if tgt in H3 :
         TGT = H3[0]
    if tgt in D2 :
         TGT = D2[0]
    if tgt in H :
         TGT = H[0]
    if tgt in He3 :
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
    print("check!!!", Like)
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select run_number from MARATHONrunlist where (Kinematic {}) and target="{}" order by run_number asc'.format(Like,tgt)

    print(query1)

    query = 'select run_number from MARATHONrunlist where Kinematic = "{}" and target="{}" order by run_number asc'.format(kin,kin,tgt)
    cursor.execute(query1)
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

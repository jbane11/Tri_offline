
# coding: utf-8

# In[1]:


from IPython.core.display import display, HTML
display(HTML("<style>.container { width:100% !important; }</style>"))


# In[32]:



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
    import mysql.connector
    db_user= 'triton-user'
    db_pswd= '3He3Hdata'
    db_name= 'triton-work'
    db_host= 'halladb'
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
    return rl
    ####################


# In[23]:


def SQLCharge(run,threshold):
    #############
    import mysql.connector
    db_user= 'triton-user'
    db_pswd= '3He3Hdata'
    db_name= 'triton-work'
    db_host= 'halladb'
    
    charge=0.0
    current=0.0
    
    cnx = mysql.connector.connect(user=db_user,host=db_host,database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    query1 = 'select charge{} ,current{} from MARATHONchargelist where run_number={};'.format( threshold, threshold, run)
    
    cursor.execute(query1)
    results=cursor.fetchall()
    rl={}
    for i,row in enumerate(results):
        i=i
        
    charge=row[0]
    current=row[1]
    
    return charge, current
    
    
    


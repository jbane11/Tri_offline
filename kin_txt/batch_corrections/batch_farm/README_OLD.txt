=======================================================================
==  Batch Farm Scripts to perform full replay for Tritium Experiment
==  -- by Zhihong Ye, Feb 20 2018
==
=======================================================================

There are several bash-shell scripts that perform different functions 
to create jobs on batch-farm, which allow you to use many computer nodes
to run mulitple jobs at the same time. I am trying to explain the function
of each script or files below:

1) --- scan_raw_files ---
   Scan the raw-data directory on /mss/ to check what runs are saved on silo.
   Then create a list of runs that contain the run numbers, for future applications.
   Please make sure you put the correct range of run numbers for left and right arms.
 
 Usague:  ./scan_raw_files L 
            ./scan_raw_files R 
  It will create file like, tritium_L.dat for futher application (or reference)

2) --- replay_watcher ---
   Check if the new raw data has been transferred from the adaq local disc 
   to the silo or not. If it does, start a new replay. Mainly useful during 
   the experiment. Not test yet

  Usague:  ./replay_watcher L 
            ./replay_watcher R 

3) --- farm_cache ---
    With a range of run-numbers for left or right arm (or with a input file that contains 
   the list of runs), the code can check if the raw-data file of a run is on /cache/ or not.
   If not, submit the "jcache" request for this run.

   Usague:  ./farm_cache, then type in "tritium_L.dat" or "tritium_R.dat", or whatever run list file

5) --- gofarm ---
   With a range of run-numbers for left or right arm (or with a input file that contains 
   the list of runs), the code can check if the raw-data file of a run is on /cache/ or not. If 
   yes, run "genjob" to submit the job for this run. If not, submit the "jcache" request for it.

   Usague:  ./gofarm, then type in "tritium_L.dat" or "tritium_R.dat", or whatever run list file

4) --- genjob ---
   With a given run-number, the code create an executable command that will tell the 
   batch-farm PC what to do, and a batch-farm job submission script that you can submit 
   the job. e.g., RunNo=1214, the code will create two files in batch_files:
     batch_1214 -- 
     jscript_1214 --
  Then run a command "jsub ./batch_files/jscript_1214" to submit the job. You can use, 
  and I highly recommend, to use "swif" to submit and manage the job. I also have the 
  example inside the code. 

  Usague:  ./genjob RunNo 
         ./genjob RunNo 
  Then use "jobstat -u [your CUE name]" to check the job status. Or use the webpage which is much better:
  https://scicomp.jlab.org/scicomp/#/auger/jobs

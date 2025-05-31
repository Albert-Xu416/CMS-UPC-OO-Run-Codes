# Crab Submission Process

## To submit the process, we need to split the output file from STARlight to many (Usually thousands) of output files

/Users/albertxu/Desktop/STARlight_Folder/starlightLHE-master/generateLHE/splitSTARlight.sh (# The folder that contains the code to split the .out files)

/Users/albertxu/Desktop/STARlight_Folder/starlight_r313/build/slight.out (# This is the .out file that you have when you first finish the run of STARlight)

## Make sure you look into the code split_STARlight.sh, there is a copy in the folder if needed.
gsplit -l ${totalLines} ${dir}/slight.${specName}.out  --numeric-suffixes=1  -a 4   ${dir}/splitFiles/slight_${specName}_  --additional-suffix=.out
(In the code there is this line, meaning each event takes 4 lines, if you fill in 100000 lines, then there is going to be 25000 events per file. Usually you want 1000 events else it would take too long, then you should fill in 4000 events.)

./splitSTARlight.sh 4000 CohJpsi (Run it in generateLHE, it creates 4000 lines per file, and store them in file CohJpsi)

/Users/albertxu/Desktop/STARlight_Folder/starlightLHE-master/generateLHE/CohJpsi/splitFiles (Now by the code, all the splitted files are given at this location)

## Now you have these files, convert them into .lhe files individually
(First go to directory generateLHE (where the conversion code exists convert_SL2LHE.C))

./convertLHE.sh CohJpsi

(Now copy all the events to your lxplus, and you are going to see this folder in your lxplus desktop)

scp -r /Users/albertxu/Desktop/STARlight_Folder/starlightLHE-master/generateLHE/InCohJpsi/Inc_lheFiles xuha@lxplus.cern.ch:~

cd /afs/cern.ch/user/x/xuha/Inc_lheFiles

(Now copy them into the eos address)

for file in *.lhe; do
  xrdcp "$file" root://eosuser.cern.ch//eos/cms/store/group/phys_heavyions/xuha/lheFiles_0531/
done

## Now we are ready submit the crab job
(First you need to make a filelist.txt with the names that is needed filelist.txt, below is an examplar code)

for i in $(seq -w 1 2000); do

  echo "/store/group/phys_heavyions/xuha/lheFiles_0531/slight_InCohJpsi_$i.lhe"
  
done > filelist.txt

(Next we need to adjust this crab configuration file, an example is in the file, we need different one for different jobs, note make sure the python file you are running is in your current directory and also the filelist you produced. You are ready to submit the crabjob)

scram b -j8
cmsenv
crab submit --config=crabConfig1.py

## Likely Output
Success: Your task has been delivered to the prod CRAB3 server.
Task name: 250531_192006:xuha_crab_STARlight_CohJpsiToMuMu_GenSim_132X_250531_211918
Project dir: crab_projects/crab_STARlight_CohJpsiToMuMu_GenSim_132X_250531_211918
Please use ' crab status -d crab_projects/crab_STARlight_CohJpsiToMuMu_GenSim_132X_250531_211918 ' to check how the submission process proceeds.

## Now for step2/step3

(First find the generated root files address, an example is the following code, and you generate the filelist likewise)

LFN_BASE=/store/group/phys_heavyions/xuha/1_output_files_0531/STARlight_CohJpsiToMuMu_GenSim_132X_250531_211918/CRAB_UserFiles/STARlight_CohJpsiToMuMu_GenSim_132X_250531_211918

for i in $(seq 1 2000); do

  if [ $i -le 999 ]; then
  
    echo "${LFN_BASE}/0000/step1_STARlight_LHE_GenSim_${i}.root"
    
  elif [ $i -le 1999 ]; then
  
    echo "${LFN_BASE}/0001/step1_STARlight_LHE_GenSim_${i}.root"
    
  else
  
    echo "${LFN_BASE}/0002/step1_STARlight_LHE_GenSim_${i}.root"
    
  fi
  
done > 2_output_filelist.txt

crab submit --config=crabConfig2.py

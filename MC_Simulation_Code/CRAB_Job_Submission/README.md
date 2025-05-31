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

(Next we need to adjust this crab configuration file, an example is in the file), we need different one for different jobs


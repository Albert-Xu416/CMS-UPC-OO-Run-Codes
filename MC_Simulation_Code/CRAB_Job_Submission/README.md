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


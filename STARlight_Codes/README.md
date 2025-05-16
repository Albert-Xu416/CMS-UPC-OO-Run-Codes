* Every time you run STARlight, you need to rebuild a new directory "build", so first remove the old onecd 
cd starlight_r313/build

rm -rf build

mkdir build

cd build

cmake .. (This line make a whole new starlight system)

* Then after you build a new starlight system, you fill out the input
vim slight.in (Adjust the channels to what we need, an example is in the STARlight folder)

make clean

make -j4 (this line makes a new starlight)

./starlight

* Then you will see a starlight output file -- slight.out usually -- you need to convert this to LHE file
cd starlightLHE-master (go to this folder/generateLHE)

vim convert_SL2LHE.C (This is the code that converts the .out file to .lhe file, make sure you adjust the pdgid selection line)

root -l -b -q 'convert_SL2LHE.C("/Users/albertxu/Desktop/STARlight_Folder/starlight_r313/build/slight.out")' (This converts the .lhe file)

* Then you are going to see a .lhe file -- starlight_LHEtest.lhe for me -- now put it into lxplus, then see the MC simulation part
scp /Users/albertxu/Desktop/STARlight_Folder/starlightLHE-master/generateLHE xuha@lxplus.cern.ch:~/private/

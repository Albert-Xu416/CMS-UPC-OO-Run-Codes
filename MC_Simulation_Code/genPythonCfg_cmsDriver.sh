#!/bin/bash
date

cmsDriver.py Configuration/GenProduction/python/HINPbPbAutumn18GS_STARlight_fragment.py --filein /store/group/phys_heavyions/dyano/Albert_OOrun/lhefiles_05_15/starlight_LHEtest.lhe --filetype LHE --fileout file:step1_STARlight_LHE_GenSim.root --mc --eventcontent RAWSIM --no_exec --datatier GEN-SIM --conditions 141X_mcRun3_2024_realistic_HI_v14 --beamspot DBrealistic --step GEN,SIM --nThreads 1 --scenario HeavyIons --geometry DB:Extended --era Run3_2024_UPC --python_filename step1_STARlight_LHE_GenSim_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 800

cmsDriver.py step2 --filein file:step1_STARlight_LHE_GenSim.root --fileout step2_STARlight_Digi.root --mc --eventcontent RAWSIM --pileup HiMixNoPU --datatier GEN-SIM-RAW --conditions 141X_mcRun3_2024_realistic_HI_v14 --step DIGI:pdigi_hi_nogen,L1,DIGI2RAW,HLT:HIon --nThreads 2 --geometry DB:Extended --era Run3_2024_UPC --python_filename step2_STARlight_Digi_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 800

cmsDriver.py step3 --filein file:step2_STARlight_Digi.root --fileout step3_STARlight_Reco.root --mc --eventcontent AODSIM --runUnscheduled --datatier AODSIM --conditions 141X_mcRun3_2024_realistic_HI_v14 --step RAW2DIGI,L1Reco,RECO --nThreads 2 --era Run3_2024_UPC --python_filename step3_STARlight_Reco_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 800

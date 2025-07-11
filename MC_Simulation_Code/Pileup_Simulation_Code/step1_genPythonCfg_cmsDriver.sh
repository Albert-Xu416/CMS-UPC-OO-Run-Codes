cmsDriver.py Configuration/GenProduction/python/HINPbPbAutumn18GS_STARlight_fragment.py \
--filein /store/group/phys_heavyions/xuha/0619_PU_Simulation/CohJpsi_lheFiles/slight_CohJpsi_0010.lhe \
--filetype LHE \
--fileout file:/afs/cern.ch/user/x/xuha/CMSSW_15_0_8/src/PU_Output_Files/Step1/step1_PU_0010_OO.root \
--pileup HiMixGEN \
--pileup_input /store/group/phys_heavyions/xuha/Pileup_Events/MinBias_Hijing_OO_5362GeV_10.root \
--mc \
--eventcontent RAWSIM \
--datatier GEN-SIM \
--conditions 150X_mcRun3_2025_forOO_realistic_v5 \
--beamspot DBrealistic \
--step GEN,SIM \
--nThreads 1 \
--scenario HeavyIons \
--geometry DB:Extended \
--era Run3_2025_OXY \
--python_filename /afs/cern.ch/user/x/xuha/CMSSW_15_0_8/src/PU_RunFiles/step1_STARlight_LHE_GenSim_cfg.py \
--no_exec \
--customise Configuration/DataProcessing/Utils.addMonitoring \
-n 500

cmsDriver.py \
--filein file:/afs/cern.ch/user/x/xuha/CMSSW_15_0_8/src/PU_Output_Files/Step1/step1_PU_0010_OO.root \
--fileout file:/afs/cern.ch/user/x/xuha/CMSSW_15_0_8/src/PU_Output_Files/Step2/step2_PU_0010_OO.root \
--mc \
--eventcontent RAWSIM \
--pileup HiMix \
--pileup_input /store/group/phys_heavyions/xuha/Pileup_Events/MinBias_Hijing_OO_5362GeV_10.root \
--datatier GEN-SIM-DIGI-RAW \
--conditions 150X_mcRun3_2025_forOO_realistic_v5 \
--step DIGI:pdigi_hi_nogen,L1,DIGI2RAW,HLT:@fake2 \
--geometry DB:Extended \
--era Run3_2025_OXY \
--customise_commands "process.RAWSIMoutput.outputCommands.extend(['keep *_mix_MergedTrackTruth_*', 'keep *Link*_simSiPixelDigis__*', 'keep *Link*_simSiStripDigis__*'])" \
--python_filename /afs/cern.ch/user/x/xuha/CMSSW_15_0_8/src/PU_RunFiles/step2_STARlight_Digi_cfg.py \
--no_exec \
-n 500 \
--nThreads 8

cmsDriver.py \
--filein file:/afs/cern.ch/user/x/xuha/CMSSW_15_0_8/src/PU_Output_Files/Step2/step2_PU_0010_OO.root \
--fileout file:/afs/cern.ch/user/x/xuha/CMSSW_15_0_8/src/PU_Output_Files/Step3/step3_PU_0010_OO.root \
--mc \
--eventcontent AODSIM \
--datatier AODSIM \
--conditions 150X_mcRun3_2025_forOO_realistic_v5 \
--step RAW2DIGI,L1Reco,RECO \
--era Run3_2025_OXY \
--customise_commands "process.AODSIMoutput.outputCommands.extend(['keep *_mix_MergedTrackTruth_*', 'keep *Link*_simSiPixelDigis__*', 'keep *Link*_simSiStripDigis__*', 'keep *_generalTracks__*', 'keep *_hiConformalPixelTracks__*', 'keep *_siPixelClusters__*', 'keep *_siStripClusters__*'])" \
--python_filename /afs/cern.ch/user/x/xuha/CMSSW_15_0_8/src/PU_RunFiles/step3_STARlight_Reco_cfg.py \
--no_exec \
-n 500 \
--nThreads 8

cmsDriver.py step4 \
--filein file:/afs/cern.ch/user/x/xuha/CMSSW_15_0_8/src/PU_Output_Files/Step3/step3_PU_0010_OO.root \
--fileout file:/afs/cern.ch/user/x/xuha/CMSSW_15_0_8/src/PU_Output_Files/Step4/step4_PU_0010_OO.root \
--mc \
--eventcontent AODSIM \
--datatier AODSIM \
--conditions 150X_mcRun3_2025_forOO_realistic_v5 \
--step RAW2DIGI,L1Reco,RECO \
--era Run3_2025_OXY \
--customise_commands "process.AODSIMoutput.outputCommands.extend(['keep *_mix_MergedTrackTruth_*', 'keep *Link*_simSiPixelDigis__*', 'keep *Link*_simSiStripDigis__*', 'keep *_generalTracks__*', 'keep *_hiConformalPixelTracks__*', 'keep *_siPixelClusters__*', 'keep *_siStripClusters__*'])" \
--python_filename /afs/cern.ch/user/x/xuha/CMSSW_15_0_8/src/PU_RunFiles/step4_STARlight_PAT_cfg.py \
--no_exec \
-n 500 \
--nThreads 8 \

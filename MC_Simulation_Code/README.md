## This is a summary steps to run the MC simulation after you run STARlight
ssh xuha@lxplus@cern.ch
ssh lxplus9
cd /afs/cern.ch/user/x/xuha/private/CMSSW_14_1_7/src
scram b -j8
cmsenv # builds the environment

# Do it only once
+chmod genPythonCfg_cmsDriver.sh (do it only once)

./genPythonCfg_cmsDriver.sh
cmsRun step1_STARlight_LHE_GenSim_cfg.py
cmsRun step2_STARlight_Digi_cfg.py
cmsRun step3_STARlight_Reco_cfg.py

cd /afs/cern.ch/user/x/xuha/private/CMSSW_14_1_7/src/VertexCompositeAnalysis/VertexCompositeProducer/test
cmsRun PbPbSkimAndTree2023_DiMu_MC_ParticleAnalyzer_cfg.py

# Then you should see something like
diMu_ana_mc.root 

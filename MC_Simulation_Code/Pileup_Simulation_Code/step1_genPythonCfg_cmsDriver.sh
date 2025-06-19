cmsDriver.py Configuration/GenProduction/python/HINPbPbAutumn18GS_STARlight_fragment.py \
--filein /store/group/phys_heavyions/xuha//0619_PU_Simulation/CohJpsi_lheFiles/slight_CohJpsi_0006.lhe \
--filetype LHE \
--fileout file:step1_output_files/step1_0006_OO.root \
--mc \
--eventcontent RAWSIM \
--no_exec \
--datatier GEN-SIM \
--conditions 150X_mcRun3_2025_forOO_realistic_v5 \
--beamspot DBrealistic \
--step GEN,SIM \
--nThreads 1 \
--scenario HeavyIons \
--geometry DB:Extended \
--era Run3_2025_OXY \
--python_filename step1_STARlight_LHE_GenSim_cfg_PU.py \
--no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 1000

cmsDriver.py Configuration/GenProduction/python/MinBias_Hijing_OO_5362GeV.py \
  --fileout file:step2_output_files/step2_0006_OO.root \
  --mc --eventcontent RAWSIM \
  --datatier GEN-SIM \
  --conditions 150X_mcRun3_2025_forOO_realistic_v5 \
  --beamspot MatchHI \
  --step GEN,SIM \
  --nThreads 4 \
  --scenario HeavyIons \
  --geometry DB:Extended \
  --era Run3_2025_OXY \
  --pileup_input "dbs:MinBias_Hijing_b015_OO_5362GeV/wangj-GENSIM_250526_1506p1_Nominal2025OOCollision-4895bd9726fb18692563bbed400e94f9/USER" \
  --pileup HiMixGEN \
  --python_filename step2_STARlight_GENSIM_PU_cfg.py \
  --customise Configuration/DataProcessing/Utils.addMonitoring \
  --no_exec \
  -n 1000

  cmsDriver.py step3 \
  --mc \
  --eventcontent RAWSIM \
  --pileup HiMix \
  --datatier GEN-SIM-DIGI-RAW \
  --conditions 150X_mcRun3_2025_forOO_realistic_v5 \
  --step DIGI:pdigi_hi_nogen,L1,DIGI2RAW,HLT:@fake2 \
  --geometry DB:Extended \
  --era Run3_2025_OXY \
  --pileup_input "file:step2_PU_output_files/step2_0006_OO.root"  \
  --customise_commands "process.RAWSIMoutput.outputCommands.extend(['keep *_mix_MergedTrackTruth_*', 'keep *Link*_simSiPixelDigis__*', 'keep *Link*_simSiStripDigis__*'])" \
  --filein "file:step1_output_files/step1_0006_OO.root" \
  -n 1000 \
  --nThreads 8

from datetime import datetime
from CRABClient.UserUtilities import config


config = config()


config.section_("General")
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True
request_name = "STARlight_CohJpsiToMuMu_GenSim_132X"
request_name += "_%s" % datetime.now().strftime("%y%m%d_%H%M%S")
config.General.requestName = request_name


#############################################################################################
config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'step1_STARlight_LHE_GenSim_cfg.py' # The file you are using
config.JobType.numCores = 1 # The number of CPU you want to use to run.
config.JobType.allowUndistributedCMSSW = True


#############################################################################################
config.section_('Data')
with open('/afs/cern.ch/user/x/xuha/private/CMSSW_14_1_7/src/filelist.txt') as f:
    config.Data.userInputFiles = f.read().splitlines()
config.Data.splitting   = 'FileBased' #用每个file为单位,不要用eventbased因为我不会
config.Data.unitsPerJob = 1 #每个job跑一个file
NJOBS = 2000 #总共有多少个job
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS 
config.Data.publication = False
# config.JobType.maxMemoryMB = 2500 #每个job申请多少的memory, 1的时候不用动
# config.JobType.maxJobRuntimeMin = 1000
# config.Data.outputPrimaryDataset = "STARlight"
config.Data.outputDatasetTag = config.General.requestName
config.Data.outLFNDirBase = '/store/group/phys_heavyions/xuha/output_files_0527/%s' %  config.General.requestName #你想要outputfile放在哪里


config.section_('Site')
config.Data.ignoreLocality = True 
config.Site.whitelist = ['T2_CH_CERN'] #这个和下一个地址最好保持一致,不然transfer的过程会非常慢
config.Site.storageSite = 'T2_CH_CERN'


#############################################################################################

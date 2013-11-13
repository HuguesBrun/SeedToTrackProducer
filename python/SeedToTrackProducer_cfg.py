import FWCore.ParameterSet.Config as cms


process = cms.Process("ALZ")
process.load("FWCore.MessageService.MessageLogger_cfi")


process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'PRE_ST62_V8::All'


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
                                                              #'file:/sps/cms/hbrun/CMSSW_6_2_0_patch1_L2seedingDev/src/files/oldGeometry_RECO.root'
                                                              'file:/sps/cms/hbrun/CMSSW_6_2_0_patch1_L2seedingDev/src/files/NewGeometry_RECO.root' #attention Pt 100
                                                              )
                            )


process.myProducerLabel = cms.EDProducer('SeedToTrackProducer',
                L2seedsCollection = cms.InputTag("ancientMuonSeed")
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('testOutput.root'),
    outputCommands = cms.untracked.vstring('drop *'
                                           ,'keep *_*_*_ALZ'  )
)

  
process.p = cms.Path(process.myProducerLabel)

process.e = cms.EndPath(process.out)

import FWCore.ParameterSet.Config as cms

process = cms.Process("ALZ")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'PRE_ST62_V8::All'


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
                                                              #'/store/relval/CMSSW_6_2_0/RelValSingleMuPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_ST62_V8-v3/00000/EEFF6FEA-40EC-E211-94EB-003048F1C496.root'
                                                              '/store/relval/CMSSW_6_2_0/RelValSingleMuPt10/GEN-SIM-RECO/PRE_ST62_V8-v3/00000/FEB7D35C-5CEC-E211-80AA-003048FEB8EE.root')
                            )






process.useMatching = cms.EDAnalyzer('GetMuonAssocationExample',
                                   trackingParticlesCollection = cms.InputTag("mix","MergedTrackTruth"),
                                   standAloneAssociator = cms.InputTag("muonAssociatorByHits"),
                                   L2associator = cms.InputTag("muonAssociatorByHitsL2seeds"),
                                   MuonRecHitBuilder = cms.string("MuonRecHitBuilder"),
                                   StandAloneTrackCollectionLabel = cms.untracked.string("standAloneMuons"),
                                   recoMuonsCollection = cms.VInputTag(cms.InputTag("muons")),
                                   associationToSTA = cms.InputTag("classByHitsSTA","prodZ")
                                    )




#process.p = cms.Path(process.muonAssociatorByHits+process.runL2seed)


process.out = cms.OutputModule("PoolOutputModule",
                               outputCommands = cms.untracked.vstring('keep *',
                                                                      'keep *_*_*_ALZ'),
                               fileName = cms.untracked.string('testRECOouput.root')
                               )

process.TrackToMuon = cms.EDProducer('TrackToMuonProducer',
                                         TracksLabel = cms.InputTag("standAloneMuons")
                                         )

process.p = cms.Path(process.TrackToMuon)


process.outpath = cms.EndPath(process.out)






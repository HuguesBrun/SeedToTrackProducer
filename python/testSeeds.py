import FWCore.ParameterSet.Config as cms

process = cms.Process("ALZ")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'PRE_ST62_V8::All'

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
				 '/store/user/hbrun/recup_620MuSimsRAWRECO_v2/filesRecup/theFileRAW.root'
#                                '/store/user/hbrun/samplesForSeedingStudies/CMSSW620/RelValSingleMuPt100_PRE_ST62_V8/reco_1.root'
#				'/store/relval/CMSSW_6_2_0_patch1/RelValSingleMuPt100/GEN-SIM-RECO/POSTLS162_V1_UPG2015-v1/00000/DE749C75-E3FA-E211-BC47-0026189437E8.root'
#                                 '/store/user/hbrun/recup_620MuSimsRAWRECO_v2/filesRecup/theRECOfile.root'                            
    ),
)



process.load("SimMuon.MCTruth.MuonAssociatorByHits_cfi")
process.muonAssociatorByHits.tracksTag = cms.InputTag("standAloneMuons")
process.muonAssociatorByHits.UseTracker = cms.bool(False)
process.muonAssociatorByHits.UseMuon = cms.bool(True)
process.muonAssociatorByHits.PurityCut_muon = cms.double(0.01)
process.muonAssociatorByHits.EfficiencyCut_muon = cms.double(0.01)

import SimMuon.MCTruth.MuonAssociatorByHits_cfi
process.muonAssociatorByHitsL2seeds = process.muonAssociatorByHits.clone() #don't use it, it is not working ;)
process.muonAssociatorByHitsL2seeds.tracksTag = cms.InputTag("myProducerLabel")
process.muonAssociatorByHitsL2seeds.UseTracker = cms.bool(False)
process.muonAssociatorByHitsL2seeds.UseMuon = cms.bool(True)
process.muonAssociatorByHitsL2seeds.PurityCut_muon = cms.double(0.01)
process.muonAssociatorByHitsL2seeds.EfficiencyCut_muon = cms.double(0.01)


process.load("SimTracker.TrackAssociation.TrackAssociatorByPosition_cff")
process.TrackAssociatorByPosition.ComponentName = cms.string('TrackAssociatorByDeltaR')
process.TrackAssociatorByPosition.method = cms.string('momdr')
process.TrackAssociatorByPosition.QCut = cms.double(0.5)
process.TrackAssociatorByPosition.ConsiderAllSimHits = cms.bool(True)

process.tpToGlbMuAssociation = cms.EDProducer('TrackAssociatorEDProducer',
                                              associator = cms.string('TrackAssociatorByDeltaR'),
                                              label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
                                              label_tr = cms.InputTag('globalMuons')
                                              )


process.load("SimMuon.MCTruth.MuonAssociatorByHitsESProducer_NoSimHits_cfi") 
process.runL2seed = cms.EDAnalyzer('L2seedsAnalyzer',
                              isMC                    = cms.bool(True),
                              muonProducer 		= cms.VInputTag(cms.InputTag("muons")),
                              primaryVertexInputTag   	= cms.InputTag("offlinePrimaryVertices"),
                              StandAloneTrackCollectionLabel = cms.untracked.string("standAloneMuons"),
                              trackingParticlesCollection = cms.InputTag("mix","MergedTrackTruth"),
                              standAloneAssociator = cms.InputTag("muonAssociatorByHits"),
                              L2seedsCollection = cms.InputTag("ancientMuonSeed"),
                              L2seedTrackCollection = cms.InputTag("myProducerLabel"),
                              L2associator = cms.InputTag("muonAssociatorByHitsL2seeds"),
                              MuonRecHitBuilder = cms.string("MuonRecHitBuilder"),
			      associatorLabel = cms.string("muonAssociatorByHits_NoSimHits"),
                              outputFile = cms.string("muonSeedTree.root")
)




#process.out = cms.OutputModule("PoolOutputModule",
#                               outputCommands = cms.untracked.vstring(
#                                                                      'drop *',
#                                                                      'keep *_*_*_ALZ'),
#                               fileName = cms.untracked.string('testRECOouput.root')
#                               )

#process.load("hugues.SeedToTrackProducer.SeedToTrackProducer_cfg")
process.myProducerLabel = cms.EDProducer('onlineToOfflineSeed',
                                         L2seedsCollection = cms.InputTag("hltL2MuonSeeds")#ancientMuonSeed")
                                         )


process.out = cms.OutputModule("PoolOutputModule",
                               outputCommands = cms.untracked.vstring(
                                                                      'drop *',
                                                                      'keep *_classByHitsTM_*_ALZ',
                                                                      'keep *_myProducerLabel_*_ALZ',
                                                                      'keep *_mix_*_ALZ'),
                               fileName = cms.untracked.string('testRECOouput.root')
                               )

#process.p = cms.Path(process.myProducerLabel*process.muonAssociatorByHitsL2seeds*process.muonAssociatorByHits+process.runL2seed)
process.p = cms.Path(process.myProducerLabel)
process.outpath = cms.EndPath(process.out)






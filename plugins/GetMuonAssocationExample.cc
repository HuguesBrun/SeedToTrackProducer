#include "GetMuonAssocationExample.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
GetMuonAssocationExample::GetMuonAssocationExample(const edm::ParameterSet& iConfig)

{
    standAloneAssociatorTag_ = iConfig.getParameter<edm::InputTag>("standAloneAssociator");
    trackingParticlesTag_ =  iConfig.getParameter<edm::InputTag>("trackingParticlesCollection");
    L2associatorTag_ = iConfig.getParameter<edm::InputTag>("L2associator");
    theMuonRecHitBuilderName_ = iConfig.getParameter<std::string>("MuonRecHitBuilder");
}


GetMuonAssocationExample::~GetMuonAssocationExample()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
GetMuonAssocationExample::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
    using namespace reco;
    using namespace std;
    
    edm::ESHandle<TransientTrackingRecHitBuilder> theMuonRecHitBuilder;
    iSetup.get<TransientRecHitRecord>().get(theMuonRecHitBuilderName_,theMuonRecHitBuilder);
    
    // tracking particles collection
    edm::Handle<TrackingParticleCollection>  TPCollectionH ;
    TrackingParticleCollection tPC;
    iEvent.getByLabel(trackingParticlesTag_,TPCollectionH);
    if (TPCollectionH.isValid()) tPC   = *(TPCollectionH.product());
    else cout << "not found tracking particles collection" << endl;
    

    //sim to RECO tracks associator
    edm::Handle<reco::SimToRecoCollection> simRecoHandle;
    iEvent.getByLabel(standAloneAssociatorTag_,simRecoHandle);
    
    reco::SimToRecoCollection simRecColl;
    if (simRecoHandle.isValid()) {
        simRecColl = *(simRecoHandle.product());
    } else {
        cout << "no valid sim RecHit product found ! " << endl;
        return;
    }
    
    //RECO to sim tracks associator
    edm::Handle<reco::RecoToSimCollection> recoSimHandle;
    iEvent.getByLabel(standAloneAssociatorTag_,recoSimHandle);
    
    reco::RecoToSimCollection recSimColl;
    if (recoSimHandle.isValid()) {
        recSimColl = *(recoSimHandle.product());
    } else {
        cout << "no valid sim RecHit product found ! " << endl;
        return;
    }
    

    //sim to L2 seeds associator
    edm::Handle<reco::SimToRecoCollection> L2simRecoHandle;
    iEvent.getByLabel(L2associatorTag_,L2simRecoHandle);
    
    reco::SimToRecoCollection L2simRecColl;
    if (L2simRecoHandle.isValid()) {
        L2simRecColl = *(L2simRecoHandle.product());
    } else {
        cout << "no valid sim RecHit product found ! " << endl;
        return;
    }
    
    //RECO to sim L2 seeds associator
    edm::Handle<reco::RecoToSimCollection> L2recoSimHandle;
    iEvent.getByLabel(L2associatorTag_,L2recoSimHandle);
    
    reco::RecoToSimCollection L2recSimColl;
    if (L2recoSimHandle.isValid()) {
        L2recSimColl = *(L2recoSimHandle.product());
    } else {
        cout << "no valid sim RecHit product found ! " << endl;
        return;
    }

    // loop on tracking particle
    for (TrackingParticleCollection::size_type i=0; i<tPC.size(); i++) {
        TrackingParticleRef trpart(TPCollectionH, i);
        if (trpart->pt()<1) continue;
        cout << "tracking particle : Pt=" << trpart->pt() << " eta=" << trpart->eta() << " phi=" << trpart->phi() << endl;
        bool isTrackFound = false;
        float matchQuality, matchPurity;
        edm::RefToBase<reco::Track> theSTAMuon = findArecoCandidate(trpart, simRecColl, recSimColl, &isTrackFound, &matchQuality, &matchPurity);
        if (isTrackFound) {
            cout << "found a STA muon matching with quality " << matchQuality << " and purity " << matchPurity << endl;
            cout << "pt=" << theSTAMuon->pt() << " eta=" << theSTAMuon->eta() << " phi=" << theSTAMuon->phi() << endl;
        }
        
        bool isTrackFoundL2 = false;
        float matchQualityL2, matchPurityL2;
        edm::RefToBase<reco::Track> theL2seed = findArecoCandidate(trpart, L2simRecColl, L2recSimColl, &isTrackFoundL2, &matchQualityL2, &matchPurityL2);
        if (isTrackFoundL2) {
            cout << "found a L2 seed matching with quality " << matchQuality << " and purity " << matchPurity << endl;
            TrajectorySeed theSeed = (*theL2seed->seedRef());
            const TrackingRecHit *seghit = &(*(theSeed.recHits().first));
            TransientTrackingRecHit::ConstRecHitPointer ttrh(theMuonRecHitBuilder->build(seghit));
            cout <<  " eta=" << ttrh->globalPosition().eta() << " phi=" << ttrh->globalPosition().phi() << endl;
        }
    }
    edm::RefToBase<reco::Track> test;
}


// ------------ method called once each job just before starting event loop  ------------
void 
GetMuonAssocationExample::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GetMuonAssocationExample::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
GetMuonAssocationExample::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
GetMuonAssocationExample::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
GetMuonAssocationExample::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
GetMuonAssocationExample::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GetMuonAssocationExample::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

edm::RefToBase<reco::Track>
GetMuonAssocationExample::findArecoCandidate(TrackingParticleRef trpart, reco::SimToRecoCollection simRecColl, reco::RecoToSimCollection recSimColl, bool *trackFound, float *theMatchQuality, float *theMatchPurity){
    
    //1) find the STA muons if there is.
    bool foundAmatch=false;
    edm::RefToBase<reco::Track> theBestQualitySTA; //will store the STA with the best quality !
    float bestQuality=0; //initial value
    std::vector<std::pair<edm::RefToBase<reco::Track>, double> > simRecAsso;
    if(simRecColl.find(trpart) != simRecColl.end()) {
        simRecAsso = (std::vector<std::pair<edm::RefToBase<reco::Track>, double> >) simRecColl[trpart];
        //2) loop on the STA muons matched
        for (std::vector<std::pair<edm::RefToBase<reco::Track>, double> >::const_iterator IT = simRecAsso.begin();
             IT != simRecAsso.end(); ++IT) {
            // cout << "inside !! " << endl;
            edm::RefToBase<reco::Track> track = IT->first;
            double quality = IT->second;
            if (quality>bestQuality){
                bestQuality=quality;
                theBestQualitySTA = track;
            }
            foundAmatch = true;
        }
        
        //3) now that we have the STA with the best quality, check its purity
        double purity = -1.;
        if(recSimColl.find(theBestQualitySTA) != recSimColl.end()) {
            std::vector<std::pair<TrackingParticleRef, double> > recSimAsso = recSimColl[theBestQualitySTA];
            for (std::vector<std::pair<TrackingParticleRef, double> >::const_iterator ITS = recSimAsso.begin();
                 ITS != recSimAsso.end(); ++ITS) {
                TrackingParticleRef tp = ITS->first;
                if (tp == trpart) purity = ITS->second;
            }
        }

        *theMatchPurity = purity;
    }
    *trackFound = foundAmatch;
    *theMatchQuality = bestQuality;
    return theBestQualitySTA;
}

//define this as a plug-in
DEFINE_FWK_MODULE(GetMuonAssocationExample);

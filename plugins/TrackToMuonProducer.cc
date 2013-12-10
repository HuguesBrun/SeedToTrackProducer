// -*- C++ -*-
//
// Package:    TrackToMuonProducer
// Class:      TrackToMuonProducer
// 
/**\class TrackToMuonProducer TrackToMuonProducer.cc hugues/TrackToMuonProducer/plugins/TrackToMuonProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Hugues Brun
//         Created:  Tue, 05 Nov 2013 13:42:04 GMT
// $Id$
//
//




#include "TrackToMuonProducer.h"

//
// class declaration
//



//
// constants, enums and typedefs
//
typedef std::vector<std::pair<trackingRecHit_iterator, trackingRecHit_iterator> > TrackHitsCollection;

//
// static data member definitions
//

//
// constructors and destructor
//
TrackToMuonProducer::TrackToMuonProducer(const edm::ParameterSet& iConfig)
{
    
    TracksTag_ =  iConfig.getParameter<edm::InputTag>("TracksLabel");

    
    
   produces<reco::MuonCollection>();
  /*  produces<reco::TrackCollection>();
    produces<reco::TrackExtraCollection>();
    produces<TrackingRecHitCollection>();*/
    
  
}


TrackToMuonProducer::~TrackToMuonProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
TrackToMuonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    using namespace std;

    std::auto_ptr<reco::MuonCollection> TracksInMuons(new reco::MuonCollection);
    
    edm::Handle<reco::TrackCollection> theTracks;
    iEvent.getByLabel(TracksTag_, theTracks);
    
    cout << "event number=" << iEvent.id().event() << endl;
    unsigned int refToTrack = 0;
    for (reco::TrackCollection::const_iterator theTrack = theTracks->begin(); theTrack != theTracks->end(); ++theTrack, ++refToTrack){
        cout << "track Pt=" << theTrack->pt() << endl;
        reco::TrackRef trackref(theTracks, refToTrack);
        int nbRHinTrack = theTrack->recHitsSize();
	cout << "test" << nbRHinTrack << endl; 
	for (int i = 0 ; i < nbRHinTrack ; i++){
		TrackingRecHitRef RecHitRef = theTrack->recHit(i);
		const TrackingRecHit *theRH = &(*RecHitRef);
		cout << "x=" << theRH->localPosition().x() << endl;
	//	cout << "inner Phi=" << theRH->innerPosition().phi() << endl;
	}
        //TrackingRecHit::iterator test;
	//for(TrackHitsCollection::const_iterator itRecHits = theTrack->recHitsBegin(), itRecHits!=theTrack->recHitsEnd() ; ++itRecHits){
		
	//}
        float EmuonLike = sqrt(theTrack->px()*theTrack->px()+theTrack->py()*theTrack->py()+theTrack->pz()*theTrack->pz());
        //const Math::LorentzVector theQuadMom(theTrack->px(), theTrack->py(), theTrack->pz(), EmuonLike);
        //cout << "P4=" << theQuadMom->Perp() << endl;
        
        reco::Muon* muon = new reco::Muon(theTrack->charge(), reco::Particle::LorentzVector(theTrack->px(), theTrack->py(), theTrack->pz(), EmuonLike));
	cout << "muon Pt=" << muon->pt() << endl;
        muon->setStandAlone(reco::TrackRef(theTracks, refToTrack));
        muon->setOuterTrack(reco::TrackRef(theTracks, refToTrack));
	TracksInMuons->push_back(*muon);
    }
    iEvent.put(TracksInMuons); 
}

// ------------ method called once each job just before starting event loop  ------------
void 
TrackToMuonProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TrackToMuonProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
TrackToMuonProducer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
TrackToMuonProducer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
TrackToMuonProducer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
TrackToMuonProducer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TrackToMuonProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackToMuonProducer);

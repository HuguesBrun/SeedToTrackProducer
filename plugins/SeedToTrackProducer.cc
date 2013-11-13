// -*- C++ -*-
//
// Package:    SeedToTrackProducer
// Class:      SeedToTrackProducer
// 
/**\class SeedToTrackProducer SeedToTrackProducer.cc hugues/SeedToTrackProducer/plugins/SeedToTrackProducer.cc

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




#include "SeedToTrackProducer.h"

//
// class declaration
//



//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
SeedToTrackProducer::SeedToTrackProducer(const edm::ParameterSet& iConfig)
{
    
    L2seedsTag_ =  iConfig.getParameter<edm::InputTag>("L2seedsCollection");

    
    
    produces<reco::TrackCollection>();
    produces<reco::TrackExtraCollection>();
    produces<TrackingRecHitCollection>();
    
  
}


SeedToTrackProducer::~SeedToTrackProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
SeedToTrackProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    using namespace std;

    std::auto_ptr<reco::TrackCollection> selectedTracks(new reco::TrackCollection);
    std::auto_ptr<reco::TrackExtraCollection> selectedTrackExtras( new reco::TrackExtraCollection() );
    std::auto_ptr<TrackingRecHitCollection> selectedTrackHits( new TrackingRecHitCollection() );
    
    reco::TrackRefProd rTracks = iEvent.getRefBeforePut<reco::TrackCollection>();
    reco::TrackExtraRefProd rTrackExtras = iEvent.getRefBeforePut<reco::TrackExtraCollection>();
    TrackingRecHitRefProd rHits = iEvent.getRefBeforePut<TrackingRecHitCollection>();
    
    edm::Ref<reco::TrackExtraCollection>::key_type hidx = 0;
    edm::Ref<reco::TrackExtraCollection>::key_type idx = 0;

    // now read the L2 seeds collection :
    edm::Handle<TrajectorySeedCollection> L2seedsCollection;
    iEvent.getByLabel(L2seedsTag_,L2seedsCollection);
    const std::vector<TrajectorySeed>* L2seeds = 0;
    if (L2seedsCollection.isValid()) L2seeds = L2seedsCollection.product();
    else cout << "L2 seeds collection not found !! " << endl;
    
    edm::Handle<edm::View<TrajectorySeed> > seedHandle;
    iEvent.getByLabel(L2seedsTag_, seedHandle);
    
    
   // cout << "size seeds=" << L2seeds->size() << endl;
    int countRH = 0;

    // now try a loop on the seeds :
    for (unsigned int i = 0; i < L2seeds->size() ; i++){
        //cout << "seed nb " << i << endl;
        //cout << "nHits=" << (L2seeds->at(i)).nHits() << endl;
       // unsigned int index_hit = 0;
        // try to create a track from the seed
        //reco::TrackC seedTrack;
        //reco::TrackExtra* newTrkExtra;
        math::XYZPoint initPoint(0,0,0);
        math::XYZVector initMom(10,10,0);
        math::XYZPoint finalPoint(1,1,1);
        math::XYZVector finalMom(0,0,10);
        edm::RefToBase<TrajectorySeed> seed(seedHandle, i);
        CovarianceMatrix matrix1 = ROOT::Math::SMatrixIdentity();
        CovarianceMatrix matrix2 = ROOT::Math::SMatrixIdentity();
        reco::Track theTrack(0.1, 0.1,
                             initPoint, initMom,
                             1, matrix1,
                             reco::TrackBase::TrackAlgorithm::globalMuon, reco::TrackBase::TrackQuality::tight);
        reco::TrackExtra theTrackExtra(finalPoint, finalMom, true, initPoint, initMom, true,
                                  matrix1, 1,
                                  matrix2, 2,
                                  (L2seeds->at(i)).direction(), seed);
        theTrack.setExtra( reco::TrackExtraRef( rTrackExtras, idx++ ) );
        unsigned int index_hit = 0;
        for(TrajectorySeed::recHitContainer::const_iterator itRecHits=(L2seeds->at(i)).recHits().first; itRecHits!=(L2seeds->at(i)).recHits().second; ++itRecHits, ++countRH) {
            TrackingRecHit* hit = (itRecHits)->clone();
            /*cout << "x=" << (*itRecHits).localPosition().x() << endl;
            cout << "y=" << (*itRecHits).localPosition().y() << endl;
            cout << "z=" << (*itRecHits).localPosition().z() << endl;*/
            theTrack.setHitPattern( *hit, index_hit);
            selectedTrackHits->push_back(hit);
            index_hit++;
            theTrackExtra.add(TrackingRecHitRef( rHits, hidx ++ ) );
        }
        selectedTracks->push_back(theTrack);
        selectedTrackExtras->push_back(theTrackExtra);

    }
   // cout << "count RH =" << countRH << endl;
    iEvent.put(selectedTracks);
    iEvent.put(selectedTrackExtras);
    iEvent.put(selectedTrackHits);
    
}

// ------------ method called once each job just before starting event loop  ------------
void 
SeedToTrackProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SeedToTrackProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
SeedToTrackProducer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
SeedToTrackProducer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
SeedToTrackProducer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
SeedToTrackProducer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SeedToTrackProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SeedToTrackProducer);

// -*- C++ -*-
//
// Package:    EmergingJetGenAnalysis/EMJGenAnalyzer
// Class:      EMJGenAnalyzer
//
/**\class EMJGenAnalyzer EMJGenAnalyzer.cc EmergingJetGenAnalysis/EMJGenAnalyzer/plugins/EMJGenAnalyzer.cc

 Description: Analyzer for Emerging Jet analysis, supercedes EmergingJetGenAnalyzer

 Implementation:
     Re-write of EmergingJetGenAnalyzer to be more modular.
*/
//
// Original Author:  Young Ho Shin
//         Created:  Tue, 05 Apr 2016 19:37:25 GMT
//
//

// Useful keywords
// :MCONLY: Code that applies only when processing MC events
// :CUT: Code that applies cuts to objects/events
// :EVENTLEVEL: Code that calculates event-level quantities
// :JETLEVEL: Code that calculates jet-level quantities
// :JETTRACKLEVEL: Code that calculates jet-track-level quantities
// :JETSOURCE: Code that assigns "source" variable for a jet
// :TRACKSOURCE: Code that assigns "source" variable for a track
// :VERTEXSOURCE:


// system include files
#include <memory>
#include <stdlib.h> // For rand()

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "TrackingTools/IPTools/interface/IPTools.h"
#include "DataFormats/GeometrySurface/interface/Line.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "MagneticField/Engine/interface/MagneticField.h"

#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h"
#include "RecoVertex/TrimmedKalmanVertexFinder/interface/KalmanTrimmedVertexFinder.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexSmoother.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"

#include "DataFormats/JetReco/interface/GenJet.h"

// Hit pattern
#include "PhysicsTools/RecoUtils/interface/CheckHitPattern.h"

// track association
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"

// Jet Tracks Association
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"

#include "TTree.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"
#include "TParameter.h"

#include "EmergingJetGenAnalysis/EMJGenAnalyzer/interface/OutputTree.h"
#include "EmergingJetGenAnalysis/EMJGenAnalyzer/interface/EMJGenEvent.h"


//
// class declaration
//

using namespace EMJGen;

class EMJGenAnalyzer : public edm::EDFilter {
  public:
    explicit EMJGenAnalyzer(const edm::ParameterSet&);
    ~EMJGenAnalyzer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob() override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

  // ----------member data ---------------------------                                                            
  bool idbg_;

  edm::Service<TFileService> fs;

  EMJGen::OutputTree otree_ ; // OutputTree object        
  TTree* tree_;

  EMJGen :: Event event_; // current event
  EMJGen:: GenPart    genpart_    ; // Current jet
  int genpart_index_    ; // Current jet index  



  edm::Handle<reco::GenParticleCollection> genParticlesH_;
  edm::Handle<reco::GenJetCollection> genJets_;

};


//
// constructors and destructor
//
EMJGenAnalyzer::EMJGenAnalyzer(const edm::ParameterSet& iConfig) {
  {
    // Initialize tree                                                                                             
    std::string modulename = iConfig.getParameter<std::string>("@module_label");
    tree_           = fs->make<TTree>("emJetTree","emJetTree");
    otree_.Branch(tree_);
  }

  idbg_ = iConfig.getUntrackedParameter<int>("idbg");

  //  edm::ConsumesCollector iC = consumesCollector();
  consumes<std::vector<reco::GenMET> > (edm::InputTag("genMetTrue"));
  consumes<reco::GenParticleCollection> (edm::InputTag("genParticles"));
  consumes<reco::GenJetCollection> (edm::InputTag("ak4GenJets"));



}


EMJGenAnalyzer::~EMJGenAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}

bool
EMJGenAnalyzer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Reset output tree to default values                                                                            
  otree_.Init();
  // Reset Event variables 
  event_.Init();
  // Reset object counters 
  genpart_index_=0;

  event_.run   = iEvent.id().run();
  event_.event = iEvent.id().event();

  if(idbg_>0) {
    std::cout<<std::endl<<std::endl<<"********************************************"<<std::endl;
    std::cout<<"starting event "<<iEvent.id().event()<<std::endl;
  }

  // store gen particle information                                     
  iEvent.getByLabel("genParticles", genParticlesH_);                                            
  
  for (reco::GenParticleCollection::const_iterator  igen = genParticlesH_->begin(); igen != genParticlesH_->end(); igen++) {
    genpart_.Init();
    genpart_.index = genpart_index_;
    genpart_.pt=(*igen).pt();
    event_.genpart_vector.push_back(genpart_);
    genpart_index_++;
  }
  

  // Write current Event to OutputTree                                                                              
  WriteEventToOutput(event_, &otree_);
  // Write OutputTree to TTree                                                                                      
  tree_->Fill();


  return true;
}



// ------------ method called once each job just before starting event loop  ------------                           
void
EMJGenAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------                          
void
EMJGenAnalyzer::endJob() {
}



// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------

void EMJGenAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(EMJGenAnalyzer);

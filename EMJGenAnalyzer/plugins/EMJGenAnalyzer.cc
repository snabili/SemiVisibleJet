xc//
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


  const int npart=26;
  const char *partNames[npart] = {
    "pi0",
    "rho0",
    "pi+-",
    "rho+-",
    "eta",
    "omega",
    "K0",
    "K*",
    "K+-",
    "K*+-",
    "etaprime",
    "phi",
    "Delta-",
    "n",
    "Delta0",
    "p",
    "Delta+",
    "Delta++",
    "Sigma-",
    "Sigma*-",
    "Lambda",
    "Sigma0",
    "Sigma*0",
    "Sigma+",
    "Omega*0",
    "unknown"
  };


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

    edm::EDGetTokenT< reco::GenJetCollection > jetCollectionToken_;

  //computation functions
  double compute_impact(double pt, double phi, double charge, double vx, double vy ) const;


  // ----------member data ---------------------------                                                            
  int idbg_;
  float minPt_=20;
  int icnt_=0;

  std::unordered_map<std::string, TH1*> histoMap1D_;
  std::unordered_map<int,std::string> pdgName;
  std::unordered_map<int,std::string> darkName;
  std::unordered_map<int,int> pdgNum;



  EMJGen::OutputTree otree_ ; // OutputTree object        
  TTree* tree_;


  edm::Service<TFileService> fs;
  std::string namea;



  EMJGen :: Event event_; // current event
  EMJGen:: GenPart    genpart_    ; // Current jet
  int genpart_index_    ; // Current jet index  
  EMJGen:: Jet    jet_    ; // Current jet
  int jet_index_    ; // Current jet index  
  EMJGen::Track track_; // current track
  int track_index_; //current track index




  edm::Handle<reco::GenParticleCollection> genParticlesH_;
  edm::Handle<reco::GenJetCollection> genJetsH_;

};


//
// constructors and destructor
//
EMJGenAnalyzer::EMJGenAnalyzer(const edm::ParameterSet& iConfig) {
  {

    darkName.emplace(4900111,"dark pion");
    darkName.emplace(4900113,"dark rho");
    darkName.emplace(4900101,"dark quark");
    darkName.emplace(4900001,"dark mediator");

    pdgName.emplace(111,"pi0");
    pdgName.emplace(113,"rho0");
    pdgName.emplace(211,"pi+-");
    pdgName.emplace(213,"rho+-");
    pdgName.emplace(221,"eta");
    pdgName.emplace(223,"omega");
    pdgName.emplace(311,"K0");
    pdgName.emplace(313,"K*");
    pdgName.emplace(321,"K+-");
    pdgName.emplace(323,"K*+-");
    pdgName.emplace(331,"etaprime");
    pdgName.emplace(333,"phi");
    pdgName.emplace(1114,"Delta-");
    pdgName.emplace(2112,"n");
    pdgName.emplace(2114,"Delta0");
    pdgName.emplace(2212,"p");
    pdgName.emplace(2214,"Delta+");
    pdgName.emplace(2224,"Delta++");
    pdgName.emplace(3112,"Sigma-");
    pdgName.emplace(3114,"Sigma*-");
    pdgName.emplace(3122,"Lambda");
    pdgName.emplace(3212,"Sigma0");
    pdgName.emplace(3214,"Sigma*0");
    pdgName.emplace(3222,"Sigma+");
    pdgName.emplace(3324,"Omega*0");

    pdgNum.emplace(111,0);
    pdgNum.emplace(113,1);
    pdgNum.emplace(211,2);
    pdgNum.emplace(213,3);
    pdgNum.emplace(221,4);
    pdgNum.emplace(223,5);
    pdgNum.emplace(311,6);
    pdgNum.emplace(313,7);
    pdgNum.emplace(321,8);
    pdgNum.emplace(323,9);
    pdgNum.emplace(331,10);
    pdgNum.emplace(333,11);
    pdgNum.emplace(1114,12);
    pdgNum.emplace(2112,13);
    pdgNum.emplace(2114,14);
    pdgNum.emplace(2212,15);
    pdgNum.emplace(2214,16);
    pdgNum.emplace(2224,17);
    pdgNum.emplace(3112,18);
    pdgNum.emplace(3114,19);
    pdgNum.emplace(3122,20);
    pdgNum.emplace(3212,21);
    pdgNum.emplace(3214,22);
    pdgNum.emplace(3222,23);
    pdgNum.emplace(3324,24);


    std::unordered_map<int,std::string>::iterator got;
    std::unordered_map<int,int>::iterator got2;
    for(int hh=0;hh<10000000;hh++) {
      got = pdgName.find(hh);
      if(got == pdgName.end()) pdgName.emplace(hh,"unknown");
      got2 = pdgNum.find(hh);
      if(got2 == pdgNum.end()) pdgNum.emplace(hh,25);
    }





    // Initialize tree                                                                             
  namea="count"  ; histoMap1D_.emplace( namea , fs->make<TH1D>(namea.c_str() , namea.c_str(), 100 , 0., 100.) );
  namea="darkpionmass"  ; histoMap1D_.emplace( namea , fs->make<TH1D>(namea.c_str() , namea.c_str(), 100 , 0., 50.) );
  namea="darkdaughters"  ; histoMap1D_.emplace( namea , fs->make<TH1D>(namea.c_str() , namea.c_str(), 2000 , 0., 2000.) );
  namea="decays"  ; histoMap1D_.emplace( namea , fs->make<TH1F>(namea.c_str() , namea.c_str(), 3,0,3) );




  for ( auto const & it : histoMap1D_ ) {
    if(it.first!="decays") {
      it.second->Sumw2();
    } else {
      it.second->SetStats(0);
      it.second->SetCanExtend(TH1::kAllAxes);
    }
  }
  
  for(int ij=0;ij<npart;ij++) {           
    histoMap1D_["decays"]->Fill(partNames[ij],1);
  }

   
    std::string modulename = iConfig.getParameter<std::string>("@module_label");
    tree_           = fs->make<TTree>("emJetTree","emJetTree");
    otree_.Branch(tree_);
  }

  //  idbg_ = iConfig.getUntrackedParameter<int>("idbg");
  idbg_=0;
  std::cout<<"idbg_ is "<<idbg_<<std::endl;

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
  icnt_=1;
  histoMap1D_["count"]->Fill(icnt_);



  // Reset output tree to default values                                                                     

       
  otree_.Init();
  // Reset Event variables 
  event_.Init();
  // Reset object counters 
  genpart_index_=0;
  jet_index_=0;
  track_index_=0;

  event_.run   = iEvent.id().run();
  event_.event = iEvent.id().event();

  if(idbg_>0) {
    std::cout<<std::endl<<std::endl<<"********************************************"<<std::endl;
    std::cout<<"starting event "<<iEvent.id().event()<<std::endl;
  }

  // store gen particle information                                     
  iEvent.getByLabel("genParticles", genParticlesH_);                                            
  
  int icntg=0;
  for (reco::GenParticleCollection::const_iterator  igen = genParticlesH_->begin(); igen != genParticlesH_->end(); igen++) {
    genpart_.Init();
    genpart_.index = genpart_index_;
    int iid = (*igen).pdgId();
    int iiid=abs(iid);
    float xdecay=0;
    float ydecay=0;
    float zdecay=0;
    int ndau=igen->numberOfDaughters();
    int ndauch=0;
    if(ndau>0) {
      xdecay=(igen->daughter(0))->vx();
      ydecay=(igen->daughter(0))->vy();
      zdecay=(igen->daughter(0))->vz();
    }

    int icho=0; // see if it is one we want to save

    //look for dark pions or dark rhos that decay to stable particles
    if( (iiid==4900111)||(iiid==4900113)) { 
	if(iiid==4900111) {
	  histoMap1D_["darkpionmass"]->Fill((*igen).mass());
	}


      if(idbg_>0) {
	if(iiid==4900111) {
	  std::cout<<"dark pion"<<std::endl;
	}
	else if(iiid==4900113) std::cout<<"dark rho"<<std::endl;
	else std::cout<<" should not be here"<<std::endl;
      }

      if(ndau>0 ) {  // has at least one daughter   
	for(int jj=0;jj<ndau;jj++) {  // loop over daughters    
	  if((igen->daughter(jj))->status()==1 ) { // stable daughter
	    if(idbg_>0) std::cout<<"       "
	     <<std::setw(8)<<std::setprecision(4)<<iid
	     <<std::setw(8)<<std::setprecision(4)<<ndau
	     <<std::setw(8)<<std::setprecision(4)<<(igen->daughter(jj))->pt()
	     <<std::setw(8)<<std::setprecision(4)<<(igen->daughter(jj))->eta()
	     <<std::setw(8)<<std::setprecision(4)<<(igen->daughter(jj))->phi()
	     <<std::setw(8)<<std::setprecision(4)<<igen->status()
	     <<std::setw(8)<<std::setprecision(4)<<(igen->daughter(jj))->vx()
	     <<std::setw(8)<<std::setprecision(4)<<(igen->daughter(jj))->vy()
	     <<std::setw(8)<<std::setprecision(4)<<sqrt( ((igen->daughter(jj))->vx())*((igen->daughter(jj))->vx()) + ((igen->daughter(jj))->vy())*((igen->daughter(jj))->vy()))
	      <<std::setw(8)<<std::setprecision(4)<<(igen->daughter(jj))->vz()
	      <<std::endl;
	    icho=1;
	    if((igen->daughter(jj))->charge()!=0 ) { // charged stable daughter
	      ndauch+=1;
	    }
	  }
	}
      }
    }

    //look for dark quarks that have mediator for mother
    if( (iiid==4900101)) { 
      if(idbg_>0) {
	std::cout<<" dark quark"<<std::endl;
      }

      int nmoth=igen->numberOfMothers();
      if(nmoth>0 ) {  // has at least one mother
	for(int jj=0;jj<nmoth;jj++) {  // loop over mothers
	  if(abs((igen->mother(jj))->pdgId())==4900001 ) { // mother is mediator
	    if(idbg_>0) std::cout<<"       "
		     <<std::setw(8)<<std::setprecision(4)<<iid
		     <<std::setw(8)<<std::setprecision(4)<<igen->pt()
		     <<std::setw(8)<<std::setprecision(4)<<igen->eta()
		     <<std::setw(8)<<std::setprecision(4)<<igen->phi()
		     <<std::setw(8)<<std::setprecision(4)<<igen->status()
	             <<std::endl;
	    icho=1;
	  }
	}
      } 
      else {
	std::cout<<" should not be here dark quark with no mother"<<std::endl;
      }
    }


    //look for down quarks that have mediator for mother
    if( (iiid==1)) { 
      if(idbg_>0) {
	std::cout<<" down quark"<<std::endl;
      }

      int nmoth=igen->numberOfMothers();
      if(nmoth>0 ) {  // has at least one mother
	for(int jj=0;jj<nmoth;jj++) {  // loop over mothers
	  if(abs((igen->mother(jj))->pdgId())==4900001 ) { // mother is mediator
	    if(idbg_>0) std::cout<<"       "
		     <<std::setw(8)<<std::setprecision(4)<<iid
		     <<std::setw(8)<<std::setprecision(4)<<igen->pt()
		     <<std::setw(8)<<std::setprecision(4)<<igen->eta()
		     <<std::setw(8)<<std::setprecision(4)<<igen->phi()
		     <<std::setw(8)<<std::setprecision(4)<<igen->status()
	             <<std::endl;
	    icho=1;
	  }
	}
      } 
      else {
	std::cout<<" should not be here down quark with no mother"<<std::endl;
      }
    }

    //look for mediators with dark quarks as daughters
    if( (iiid==4900001)) { 
      if(idbg_>0) {
	std::cout<<" dark mediator"<<std::endl;
      }

      int ndau=igen->numberOfDaughters();
      if(ndau>0 ) {  // has at least one daughter   
	for(int jj=0;jj<ndau;jj++) {  // loop over daughters    
	  if(fabs((igen->daughter(jj))->pdgId())==4900101 ) { // dark quark daughter
	    if(idbg_>0) std::cout<<"       "
		     <<std::setw(8)<<std::setprecision(4)<<iid
		     <<std::setw(8)<<std::setprecision(4)<<ndau
		     <<std::setw(8)<<std::setprecision(4)<<igen->pt()
		     <<std::setw(8)<<std::setprecision(4)<<igen->eta()
		     <<std::setw(8)<<std::setprecision(4)<<igen->phi()
		     <<std::setw(8)<<std::setprecision(4)<<igen->status()
	             <<std::endl;
	    icho=1;
	  }
	}
      }
    }


	// write chosen ones to ntuple
    if(icho>0) {
      genpart_.pt=(*igen).pt();
      genpart_.eta=(*igen).eta();
      genpart_.phi=(*igen).phi();
      genpart_.pid=iid;
      genpart_.ndau=ndau;
      genpart_.ndauch=ndauch;
      genpart_.xdecay=xdecay;
      genpart_.ydecay=ydecay;
      genpart_.zdecay=zdecay;
 
    //add stable charged daughters
      int aiiid;
      if(ndau>0 ) {  // has at least one daughter   
	if(idbg_>0) {
	  std::cout<<std::endl;
	  std::cout<<" preparing stable daughters "<<ndauch<<std::endl;
	  std::cout<<" for a "<<darkName[iiid]<<std::endl;
	  std::cout<<" id  pt  charge stable? name "<<std::endl;
	}
	if(iiid==4900111) {
	  icnt_=2;
          histoMap1D_["count"]->Fill(icnt_);
	}
	for(int jj=0;jj<ndau;jj++) {  // loop over daughters    
	  aiiid=abs((igen->daughter(jj))->pdgId());

	  // histogram for dark pions of daughter ids if has at least one stable daughter
	  if(iiid==4900111) {
	    histoMap1D_["darkdaughters"]->Fill(aiiid);
	    histoMap1D_["decays"]->Fill(partNames[pdgNum[aiiid]],1);
	  }
	  if( (idbg_>0) && (iiid==4900111) ) {
	    std::cout
  	     <<std::setw(8)<<std::setprecision(4)<<aiiid
  	     <<std::setw(8)<<std::setprecision(4)<<(igen->daughter(jj))->pt()
  	     <<std::setw(8)<<std::setprecision(4)<<(igen->daughter(jj))->charge()
  	     <<std::setw(8)<<std::setprecision(4)<<(igen->daughter(jj))->status()
	     <<" "<<pdgName[aiiid]
	     <<std::endl;
	      }

	  if((igen->daughter(jj))->status()==1 ) { // stable daughter
	    if((igen->daughter(jj))->charge()!=0 ) { // charged daughter
	      track_.Init();
	      track_.index=track_index_;
	      track_.genpart_index = genpart_index_;
	      track_.pt = (igen->daughter(jj))->pt();
	      track_.eta = (igen->daughter(jj))->eta();
	      track_.phi = (igen->daughter(jj))->phi();
	      track_.impact = compute_impact((igen->daughter(jj))->pt(),(igen->daughter(jj))->phi(),(igen->daughter(jj))->charge(),xdecay,ydecay);
	      genpart_.track_vector.push_back(track_);
	      track_index_++;
	    }
	  }
        }
      }
      event_.genpart_vector.push_back(genpart_);
      genpart_index_++;
    }


    icntg++;
  }

  histoMap1D_["decays"]->LabelsDeflate();
  histoMap1D_["decays"]->LabelsOption("v");
  histoMap1D_["decays"]->LabelsOption("a");
  

  // store gen jet information
  iEvent.getByLabel("ak4GenJets", genJetsH_);                                            

  if(idbg_>0) {
    std::cout<<std::endl;
    std::cout<<" gen JETS in event"<<std::endl;
  }

  for (reco::GenJetCollection::const_iterator  igen = genJetsH_->begin(); igen != genJetsH_->end(); igen++) {
    jet_.Init();
    jet_.index = jet_index_;

    if ( (*igen).pt() > minPt_ ) {
      if(idbg_>0) {
	std::cout<<" jet pT is "<<(*igen).pt()<<std::endl;
      }
      jet_.pt=(*igen).pt();
      jet_.eta=(*igen).eta();
      jet_.phi=(*igen).phi();
      event_.jet_vector.push_back(jet_);
      jet_index_++;

    }
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

double EMJGenAnalyzer::compute_impact(double pt, double phi, double charge, double vx, double vy ) const {
  double imp=0.;
  if(idbg_>99) std::cout<<" pt phi charge vx vy are"<<" "<<pt<<" "<<(180./3.1415)*phi<<" "<<charge<<" "<<vx<<" "<<vy<<std::endl;

  double Bmag=3.8;
  // calculate radius of curvature in cm                             
  float Rg=100.*pt/0.3/Bmag;
  if(idbg_>99) std::cout<<"radius is "<<Rg<<std::endl;

  // calculate x and y at center of circle                           

  float xcenterg = -(vx+charge*Rg*sin(phi));
  float ycenterg = -(vy-charge*Rg*cos(phi));
  float rgen=sqrt(vx*vx+vy*vy);



  if(idbg_>99) std::cout<<" center gen is "<<xcenterg<<" "<<ycenterg<<" rad gen is "<<rgen<<std::endl;

  // calculate phi at distance of closest approach                   
  float phig=0.;
  if(charge<0) {
    phig = atan2(xcenterg,-ycenterg);
  } else {
    phig = atan2(-xcenterg,ycenterg);
  }
  if(idbg_>99) std::cout<<"phi gen is "<<(180./3.1415)*phig<<std::endl;
  // calculate impact parameter                                      
  float dxyg=sqrt(pow(xcenterg,2)+pow(ycenterg,2))-Rg;
  if(charge>0) dxyg=-1.*dxyg;

  imp=dxyg;

  if(idbg_>99) std::cout<<"impact is "<<imp<<std::endl;

  return imp;
}

//define this as a plug-in
DEFINE_FWK_MODULE(EMJGenAnalyzer);

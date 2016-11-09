#ifndef EmergingJetGenAnalysis_EMJGenAnalyzer_EMJGenEvent_h
#define EmergingJetGenAnalysis_EMJGenAnalyzer_EMJGenEvent_h

// Class describing the content of output for EmergingJetAnalyzer

#include <vector>
#include <functional>

#include "TLorentzVector.h"

#include "EmergingJetGenAnalysis/EMJGenAnalyzer/interface/OutputTree.h"
#define DEFAULTVALUE -1

using std::vector;

namespace EMJGen
{
  class Track;
  class Jet;
  class GenPart;
  class Event {
  public:
    Event() {}
    ~Event(){}
    void Init() {
      run                  = DEFAULTVALUE;
      lumi                 = DEFAULTVALUE;
      event                = DEFAULTVALUE;

      genpart_vector.clear();
      jet_vector.clear();
    };
    int    run                 ;
    int    lumi                ;
    int    event               ;


    vector<GenPart> genpart_vector;
    vector<Jet> jet_vector;
  }; 

  class Track {
  public:
    Track(){}
    ~Track(){}
    void Init(){
      index                = DEFAULTVALUE;
      genpart_index = DEFAULTVALUE;
      pt                   = DEFAULTVALUE;
      eta                   = DEFAULTVALUE;
      phi                   = DEFAULTVALUE;
      impact                = DEFAULTVALUE;
    }
    int    index               ;
    int genpart_index;
    float  pt                  ;
    float  eta                  ;
    float  phi                  ;
    float  impact                  ;
  };


  class GenPart {
  public:
    GenPart(){}
    ~GenPart(){}
    void Init(){
      index                = DEFAULTVALUE;
      pt                   = DEFAULTVALUE;
      eta                   = DEFAULTVALUE;
      phi                   = DEFAULTVALUE;
      pid                   = DEFAULTVALUE;
      ndau                   = DEFAULTVALUE;
      ndauch                   = DEFAULTVALUE;
      xdecay                = DEFAULTVALUE;
      ydecay                = DEFAULTVALUE;
      zdecay                = DEFAULTVALUE;

      track_vector.clear();
    }
    int    index               ;
    float  pt                  ;
    float  eta                  ;
    float  phi                  ;
    int  pid                  ;
    int  ndau                  ;
    int  ndauch                  ;
    float  xdecay                  ;
    float  ydecay                  ;
    float  zdecay                  ;

    vector<Track> track_vector;
  };


  class Jet {
  public:
    Jet(){}
    ~Jet(){}
    void Init(){
      index                = DEFAULTVALUE;
      pt                   = DEFAULTVALUE;
      eta                   = DEFAULTVALUE;
      phi                   = DEFAULTVALUE;
    }
    int    index               ;
    float  pt                  ;
    float  eta                  ;
    float  phi                  ;
  };

// Turn vector of objects, into vector of member variable by calling func(object)
// Output is placed in provided vector reference
template <typename Object, typename T>
void
vectorize(const vector<Object>& input, std::function<T (const Object &)> func, vector<T>& output)
{
  output.clear();
  output.reserve(input.size()); // Doesn't reduce capacity
  for (const auto& obj : input) {
    output.push_back( func(obj) );
  }
}

// Version of vectorize() that returns a vector object
template <typename Object, typename T>
vector<T>
vectorize_new(const vector<Object>& input, std::function<T (const Object &)> func)
{
  vector<T> output;
  vectorize(input, func, output);
  return output;
}

using EMJGen::Track;
using EMJGen::Event;
using EMJGen::GenPart;
 using EMJGen::Jet;

void
WriteEventToOutput(const Event& event, EMJGen::OutputTree* otree)
{
  otree->Init(); // Reset all values and clear all vectors
  // Event-level variables, e.g. int, float, etc.
  {
    otree->run                  = event.run                 ;
    otree->lumi                 = event.lumi                ;
    otree->event                = event.event               ;
  }
  // generator-level variables
  {
    vectorize<GenPart, int   >(event.genpart_vector, [](const EMJGen::GenPart& obj ){return obj.index               ;}, otree->genpart_index               );
    vectorize<GenPart, float >(event.genpart_vector, [](const EMJGen::GenPart& obj ){return obj.pt                  ;}, otree->genpart_pt                  );
    vectorize<GenPart, float >(event.genpart_vector, [](const EMJGen::GenPart& obj ){return obj.eta                  ;}, otree->genpart_eta                  );
    vectorize<GenPart, float >(event.genpart_vector, [](const EMJGen::GenPart& obj ){return obj.phi                  ;}, otree->genpart_phi                  );
    vectorize<GenPart, int >(event.genpart_vector, [](const EMJGen::GenPart& obj ){return obj.pid                  ;}, otree->genpart_pid                  );
    vectorize<GenPart, int >(event.genpart_vector, [](const EMJGen::GenPart& obj ){return obj.ndau                  ;}, otree->genpart_ndau                  );
    vectorize<GenPart, int >(event.genpart_vector, [](const EMJGen::GenPart& obj ){return obj.ndauch                  ;}, otree->genpart_ndauch                  );
    vectorize<GenPart, float >(event.genpart_vector, [](const EMJGen::GenPart& obj ){return obj.xdecay                  ;}, otree->genpart_xdecay                  );
    vectorize<GenPart, float >(event.genpart_vector, [](const EMJGen::GenPart& obj ){return obj.ydecay                  ;}, otree->genpart_ydecay                  );
    vectorize<GenPart, float >(event.genpart_vector, [](const EMJGen::GenPart& obj ){return obj.zdecay                  ;}, otree->genpart_zdecay                  );

  }

  // jet variables
  {
    vectorize<Jet, int   >(event.jet_vector, [](const EMJGen::Jet& obj ){return obj.index               ;}, otree->genjet_index               );
    vectorize<Jet, float >(event.jet_vector, [](const EMJGen::Jet& obj ){return obj.pt                  ;}, otree->genjet_pt                  );
    vectorize<Jet, float >(event.jet_vector, [](const EMJGen::Jet& obj ){return obj.eta                  ;}, otree->genjet_eta                  );
    vectorize<Jet, float >(event.jet_vector, [](const EMJGen::Jet& obj ){return obj.phi                  ;}, otree->genjet_phi                  );
  }

  // for stable daughters of gen particles
  {
    for (const auto genpart : event.genpart_vector) {
      auto index = vectorize_new<Track, int   >(genpart.track_vector, [](const Track& obj ){return obj.index;}); otree->track_index.push_back(index);
      auto genpart_index = vectorize_new<Track, int   >(genpart.track_vector, [](const Track& obj ){return obj.genpart_index;}); otree->track_genpart_index.push_back(genpart_index);
      auto pt = vectorize_new<Track, float   >(genpart.track_vector, [](const Track& obj ){return obj.pt;}); otree->track_pt.push_back(pt);
      auto eta = vectorize_new<Track, float   >(genpart.track_vector, [](const Track& obj ){return obj.eta;}); otree->track_eta.push_back(eta);
      auto phi = vectorize_new<Track, float   >(genpart.track_vector, [](const Track& obj ){return obj.phi;}); otree->track_phi.push_back(phi);
      auto impact = vectorize_new<Track, float   >(genpart.track_vector, [](const Track& obj ){return obj.impact;}); otree->track_impact.push_back(impact);
    }
  }



}

}
#endif

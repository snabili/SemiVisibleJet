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
    }
    int    index               ;
    float  pt                  ;
    float  eta                  ;
    float  phi                  ;
    int  pid                  ;
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
  }

  // jet variables
  {
    vectorize<Jet, int   >(event.jet_vector, [](const EMJGen::Jet& obj ){return obj.index               ;}, otree->genjet_index               );
    vectorize<Jet, float >(event.jet_vector, [](const EMJGen::Jet& obj ){return obj.pt                  ;}, otree->genjet_pt                  );
    vectorize<Jet, float >(event.jet_vector, [](const EMJGen::Jet& obj ){return obj.eta                  ;}, otree->genjet_eta                  );
    vectorize<Jet, float >(event.jet_vector, [](const EMJGen::Jet& obj ){return obj.phi                  ;}, otree->genjet_phi                  );
  }

}

}
#endif

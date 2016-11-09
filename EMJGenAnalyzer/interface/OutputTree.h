#ifndef EmergingJetGenAnalysis_EMJGenAnalyzer_OutputTree_h
#define EmergingJetGenAnalysis_EMJGenAnalyzer_OutputTree_h

#include <vector>

#include "TTree.h"

using std::vector;

namespace EMJGen
{
  class OutputTree {
  public:
    OutputTree() { Init(); }
    void Init();

    void Branch(TTree* tree);

    int                     run                 ;
    int                     lumi                ;
    int                     event               ;

    vector<int>             genpart_index               ;
    vector<float>           genpart_pt                  ;
    vector<float>           genpart_eta;
    vector<float>           genpart_phi;
    vector<int>           genpart_pid;
    vector<int>           genpart_ndau;
    vector<int>           genpart_ndauch;
    vector<float>           genpart_xdecay;
    vector<float>           genpart_ydecay;
    vector<float>           genpart_zdecay;
    //for dark pions include list of charged decay tracks
    vector<vector<int>> track_index;
    vector<vector<int>> track_genpart_index;
    vector<vector<float>> track_pt;
    vector<vector<float>> track_eta;
    vector<vector<float>> track_phi;
    vector<vector<float>> track_impact;

    vector<int>             genjet_index               ;
    vector<float>           genjet_pt                  ;
    vector<float>           genjet_eta;
    vector<float>           genjet_phi;
  };
}

void
EMJGen::OutputTree::Init() {
  run                 = -1;
  lumi                = -1;
  event               = -1;

  genpart_index               .clear();
  genpart_pt                  .clear();
  genpart_eta                  .clear();
  genpart_phi                  .clear();
  genpart_pid                  .clear();
  genpart_ndau                  .clear();
  genpart_ndauch                  .clear();
  genpart_xdecay                  .clear();
  genpart_ydecay                  .clear();
  genpart_zdecay                  .clear();
  track_index.clear();
  track_genpart_index.clear();
  track_pt.clear();
  track_eta.clear();
  track_phi.clear();
  track_impact.clear();

  genjet_index               .clear();
  genjet_pt                  .clear();
  genjet_eta                  .clear();
  genjet_phi                  .clear();
}

void
EMJGen::OutputTree::Branch(TTree* tree) {
#define BRANCH(tree, branch) (tree)->Branch(#branch, &branch);
  BRANCH(tree, run                 );
  BRANCH(tree, lumi                );
  BRANCH(tree, event               );

  BRANCH(tree, genpart_index               );
  BRANCH(tree, genpart_pt                  );
  BRANCH(tree, genpart_eta                  );
  BRANCH(tree, genpart_phi                  );
  BRANCH(tree, genpart_pid                  );
  BRANCH(tree, genpart_ndau                  );
  BRANCH(tree, genpart_ndauch                  );
  BRANCH(tree, genpart_xdecay                  );
  BRANCH(tree, genpart_ydecay                  );
  BRANCH(tree, genpart_zdecay                  );
  BRANCH(tree, track_index);
  BRANCH(tree, track_genpart_index);
  BRANCH(tree, track_pt);
  BRANCH(tree, track_eta);
  BRANCH(tree, track_phi);
  BRANCH(tree, track_impact);

  BRANCH(tree, genjet_index               );
  BRANCH(tree, genjet_pt                  );
  BRANCH(tree, genjet_eta                  );
  BRANCH(tree, genjet_phi                  );
}

// Insert new empty element in nested vector and returns pointer to the added element
template <typename T>
vector<T>&
make_new_element (vector< vector<T> > & vec) {
  vec.push_back( vector<T>() );
  return vec.back();
}


#endif

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
    vector<int>             genpart_source              ;
    vector<float>           genpart_pt                  ;
    vector<float>           genpart_eta;
    vector<float>           genpart_phi;
    vector<int>           genpart_pid;
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
}

// Insert new empty element in nested vector and returns pointer to the added element
template <typename T>
vector<T>&
make_new_element (vector< vector<T> > & vec) {
  vec.push_back( vector<T>() );
  return vec.back();
}


#endif

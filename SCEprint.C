#include <iostream>
#include <iomanip>
#include <locale>



void SCEprint(){

  // read the Tree generated by tree1w and fill two histograms
  // note that we use "new" to create the TFile and TTree objects,
  // to keep them alive after leaving this function.


  TFile *f = new TFile("/data/users/eno/output/X_d_1000_pi_5_lt150_0.histo.root");
  TTree *tt = (TTree*)f->Get("GenAnalyzer/emJetTree");

  Int_t event;

  vector<int> *genpart_index=new vector<int>;
  vector<float> *genpart_pt = new vector<float>;
  vector<float> *genpart_eta = new vector<float>;
  vector<float> *genpart_phi = new vector<float>;
  vector<int> *genpart_pid = new vector<int>;
  vector<int> *genpart_ndau = new vector<int>;
  vector<int> *genpart_ndauch = new vector<int>;
  vector<float> *genpart_xdecay = new vector<float>;
  vector<float> *genpart_ydecay = new vector<float>;
  vector<float> *genpart_zdecay = new vector<float>;

  vector<vector<float> > *track_pt = 0;
  vector<vector<float> > *track_eta = 0;
  vector<vector<float> > *track_phi = 0;
  vector<vector<float> > *track_impact = 0;
  vector<vector<int> > *track_index = 0;
  vector<vector<int> > *track_genpart_index = 0;



  vector<int> *genjet_index=new vector<int>;
  vector<float> *genjet_pt = new vector<float>;
  vector<float> *genjet_eta = new vector<float>;
  vector<float> *genjet_phi = new vector<float>;



  tt->SetBranchAddress("event",&event);

  tt->SetBranchAddress("genpart_index",&genpart_index);
  tt->SetBranchAddress("genpart_pt",&genpart_pt);
  tt->SetBranchAddress("genpart_eta",&genpart_eta);
  tt->SetBranchAddress("genpart_phi",&genpart_phi);
  tt->SetBranchAddress("genpart_pid",&genpart_pid);
  tt->SetBranchAddress("genpart_ndau",&genpart_ndau);
  tt->SetBranchAddress("genpart_ndauch",&genpart_ndauch);
  tt->SetBranchAddress("genpart_xdecay",&genpart_xdecay);
  tt->SetBranchAddress("genpart_ydecay",&genpart_ydecay);
  tt->SetBranchAddress("genpart_zdecay",&genpart_zdecay);

  tt->SetBranchAddress("track_pt",&track_pt);
  tt->SetBranchAddress("track_eta",&track_eta);
  tt->SetBranchAddress("track_phi",&track_phi);
  tt->SetBranchAddress("track_impact",&track_impact);
  tt->SetBranchAddress("track_index",&track_index);
  tt->SetBranchAddress("track_genpart_index",&track_genpart_index);


  tt->SetBranchAddress("genjet_index",&genjet_index);
  tt->SetBranchAddress("genjet_pt",&genjet_pt);
  tt->SetBranchAddress("genjet_eta",&genjet_eta);
  tt->SetBranchAddress("genjet_phi",&genjet_phi);


  // create a histograms
  TH1F *hptdp   = new TH1F("hptdp","dark pion pt distribution",100,0.,70.);
  TH1F *hptchdau = new TH1F("hptchdau","pt of charged daughters of dark pions",100,0.,50.);
  TH1F *himp = new TH1F("himp","impact parameter charged daughter of dark pions",100,-200.,200.);
  TH2F *himp2 = new TH2F("himp2","impact parameter charged daughter of dark pions versus r of decay",10000,0.,5000.,500,-200.,200.);
  TH1F *hrdecaydp   = new TH1F("hrdecaydp","dark decay radius distribution",100,0.,150.);
  TH1F *hldecaydp   = new TH1F("hldecaydp","dark decay length distribution",100,0.,150.);
  TH1F *hndaudp = new TH1F("hndaudp"," number dark pion daughters",20,0.,20.);
  TH1F *hndp = new TH1F("hndp"," number dark pion ",100,0.,100.);
  TH1F *hndauchdp = new TH1F("hndauchdp"," number charged dark pion daughters",20,0.,20.);
  TH1F *hptdq   = new TH1F("hptdq","dark quark pt distribution",100,0.,500.);
  TH1F *hptjet   = new TH1F("hptjet","gen jet pt distribution",100,0.,500.);


  //read all entries and fill the histograms
  Int_t nentries = (Int_t)tt->GetEntries();
  for (Int_t i=0; i<nentries; i++) {
    tt->GetEntry(i);
    cout<<"event number is "<<event<<endl;

    //gen particles variables
    Int_t ndark=0;
    for(Int_t j=0; j<(*genpart_index).size(); j++) {
      if((std::abs((*genpart_pid)[j]))>4900000) {
	  cout<<" genpart "<<(*genpart_pt)[j]<<" "<<(*genpart_pid)[j]<<endl;
	}
      if(abs((*genpart_pid)[j])==4900111){ // dark pion
	ndark=ndark+1;
	hptdp->Fill((*genpart_pt)[j]);
	float xx =(*genpart_xdecay)[j];
	float yy =(*genpart_ydecay)[j];
	float zz =(*genpart_zdecay)[j];
	hrdecaydp->Fill(xx*xx+yy*yy);
	hldecaydp->Fill(xx*xx+yy*yy+zz*zz);
	hndaudp->Fill((*genpart_ndau)[j]);
	hndauchdp->Fill((*genpart_ndauch)[j]);

	vector<float> track_pts = track_pt->at(j);
	vector<float> track_etas = track_eta->at(j);
	vector<float> track_phis = track_phi->at(j);
	vector<float> track_impacts = track_impact->at(j);
	vector<int> track_indexs = track_index->at(j);
	vector<int> track_genpart_indexs = track_genpart_index->at(j);

	cout<<" number charged daughter is "<<(*genpart_ndauch)[j]<<endl;
	cout<<" tracks size is "<<track_pts.size()<<endl;
	cout<< "pT         eta      "<<endl;
	for (unsigned itrack=0; itrack<track_pts.size(); itrack++) {
          cout
          <<setw(8)<<setprecision(3)<< track_pts[itrack]<<" "
          <<setw(8)<<setprecision(3)<< track_etas[itrack]<<" "
          <<setw(8)<< track_indexs[itrack]<<" "
	  <<endl;
	  hptchdau->Fill(track_pts[itrack]);
	  himp->Fill(track_impacts[itrack]);
	  himp2->Fill(xx*xx+yy*yy,track_impacts[itrack]);
	}



      }
      if(abs((*genpart_pid)[j])==4900101){  // dark quark
	hptdq->Fill((*genpart_pt)[j]);
      }

    }
    hndp->Fill(ndark);

    // gen jets
    for(Int_t j=0; j<(*genjet_index).size(); j++) {
      cout<<" genjet "<<(*genjet_pt)[j]<<endl;
      hptjet->Fill((*genjet_pt)[j]);
    }


  }
  // We do not close the file. We want to keep the generated
  // histograms we open a browser and the TreeViewer
  if (gROOT->IsBatch()) return;
  TCanvas *c1 = new TCanvas("c1","Vertex Plots",200,10,700,500);
  hptdp->Draw();
  //hptdq->Draw();
  c1->Modified();
  c1->Update();
  // t1->StartViewer();

  TFile myfile("haha.root","RECREATE");
  hptdp->Write();
  himp->Write();
  himp2->Write();
  hndp->Write();
  hptchdau->Write();
  hrdecaydp->Write();
  hldecaydp->Write();
  hndaudp->Write();
  hndauchdp->Write();
  hptdq->Write();
  hptjet->Write();


  tt->ResetBranchAddresses();


  //In the browser, click on "ROOT Files", then on "tree1.root"
  //You can click on the histogram icons in the right panel to draw
  //them in the TreeViewer, follow the instructions in the Help.
}

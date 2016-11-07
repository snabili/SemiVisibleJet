{ 
  // red file: new one
  TFile *f1 = new TFile("haha1.root");
  // black file : old one
  TFile *f2 = new TFile("haha2.root");  




  gStyle->SetOptStat(111111);

  // 
  TCanvas *c1 = new TCanvas("c1","1",50,10,700,800);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(1);
  c1->SetFrameBorderMode(0);
  float titleSize = 0.03;
  float titleAreaHeight = 1.2 * titleSize;
  _mainPade = new TPad("dq", "dq", 0.0, titleAreaHeight, 1.0, 1.0);
  _mainPade->Divide(2, 3, 0.0, 0.0, 0);
  _mainPade->Draw();

  f1->cd();
  _mainPade->cd(1);
  hptdq->SetLineWidth(2);
  hptdq->SetLineColor(2);
  hptdq->Draw();
  f2->cd();
  hptdq->SetLineWidth(1);
  hptdq->SetLineColor(1);
  hptdq->Draw("same");

}

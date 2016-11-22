from ROOT import TFile 

lumi = 4631 # TopBSM v9, GOLDEN JSON recovered runs
lumiPlot = 4.63


RootFiles = {}

RootFiles['DM_A'] = TFile("/home/jabeen/CMSLPC/CMSSW_8_1_0_pre9/src/ntuple-A.root")
RootFiles['DM_B'] = TFile("/home/jabeen/CMSLPC/CMSSW_8_1_0_pre9/src/ntuple-B.root")


Trees = {}
Trees['DM_A'] = RootFiles['DM_A'].GetDirectory("GenAnalyzer").Get("emJetTree")
Trees['DM_B'] = RootFiles['DM_B'].GetDirectory("GenAnalyzer").Get("emJetTree")

Nevents = {}


Nevents['DM_A'] = 100.0
Nevents['DM_B'] = 100.0



factor = 1

xsec = {}

xsec['DM_A'] =  1.6697
xsec['DM_B'] =  3.9983

xsec_norm = {}

xsec_norm['DM_A'] = 1.0
xsec_norm['DM_B'] = 1.0

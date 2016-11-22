import ROOT, sys, os, re, string
from ROOT import TCanvas, TFile, TProfile, TNtuple, TH1F, TH1D, TH2F,TF1, TPad, TPaveLabel, TPaveText, TLegend, TLatex, THStack
from ROOT import gROOT, gBenchmark, gRandom, gSystem, Double, gPad

from array import array

from LoadData_DM import *


List       = ['DM_A',
              'DM_B'
              ]


global txtfile
txtfile = open('DM_Yields_Table.txt', 'w') 

CutList = ['noselection',
           'preselection1',
           'preselection2'
            ]

def FillTables(varName, bin, xlow, high, ylabel, xlabel, save , chan):

    EventCountPre = {}
    EventCountPre1 = {}
    EventCountPre2 = {}
    VariablesPre = {}
    VariablesPre1 = {}
    VariablesPre2 = {}
    
    
    for cutlabel in CutList:       
        cut = 'genjet_pt >= 0'       
        cut1 = 'genjet_pt >= 100'
        cut2 = 'fabs(genjet_eta) < 0.1'
        
        j = 0
        for Type in List:
            histName = "genjet_pt"
            print histName
            SF = 1.0
            if cutlabel == 'noselection': 
                Newcut = cut 
                print cutlabel
                VariablesPre[Type] = TH1D(histName, histName, bin, array('d',xlow))
                Trees[Type].Draw(var + " >> " + histName, "(" + Newcut + ")", 'goff')
                print VariablesPre[Type].Integral()            
                EventCountPre[Type] = VariablesPre[Type].Integral()
            
            elif cutlabel == 'preselection1':
                Newcut = cut1 
                print cutlabel
                VariablesPre1[Type] = TH1D(histName, histName, bin, array('d',xlow))
                Trees[Type].Draw(var + " >> " + histName, "(" + Newcut + ")", 'goff')
                print VariablesPre1[Type].Integral()            
                EventCountPre1[Type] = VariablesPre1[Type].Integral()
                
            else:
                cutlabel == 'preselection2'
                Newcut = cut1 + "&& " + cut2 
                print cutlabel
                VariablesPre2[Type] = TH1D(histName, histName, bin, array('d',xlow))
                Trees[Type].Draw(var + " >> " + histName, "(" + Newcut + ")", 'goff')
                print VariablesPre2[Type].Integral()            
                EventCountPre2[Type] = VariablesPre2[Type].Integral()
                
            j=j+1
        
            
     
    txtfile.write("\\begin{table}[!h!tb]\n")
    txtfile.write("\\begin{center}\n")
    txtfile.write("\small\n")
    txtfile.write("     \caption{\n")
    txtfile.write("       Number of selected  signal events.\n")
    txtfile.write("")
    txtfile.write("")
    txtfile.write("\label{tab:cut_flow}\n")
    txtfile.write("        }\n")
    txtfile.write("\\begin{tabular}{l|c|c} \n")
    txtfile.write("\hline \n")
    txtfile.write("Cut    & \multicolumn{2}{c}{Number of Events} \\\ \hline\hline \n")
    txtfile.write("                         &  Model A & Model B }  \\\ \n")
    txtfile.write(" Total  & "+str(int(round(EventCountPre['DM_A']))) +"  & "+ str(int(round(EventCountPre['DM_B'])))+"  \\\ \n")
 
    
    txtfile.write("$p_{T}^{jet}>$ 100 GeV  & "+str(int(round(EventCountPre1['DM_A']))) +"  & "+ str(int(round(EventCountPre1['DM_B'])))+"  \\\ \n")
 
    txtfile.write("$|\eta|^{jet}<$0.1  & "+str(int(round(EventCountPre2['DM_A']))) +"  & "+ str(int(round(EventCountPre2['DM_B'])))+"  \\\ \n")
 
    txtfile.write("\hline\n")
    txtfile.write("\end{tabular}\n")
    txtfile.write("\\normalsize\n")
    txtfile.write("\end{center}\n")
    txtfile.write("\end{table}\n")




a = "smallestPz_"
b = "bestTop_"
chan = 'sig'

var = 'genjet_pt'; bin =30; low = 0; high = 1500; xaxis = "Jet pT [GeV]"; yaxis = 'Events / 50 GeV'; save = 'Jet-pT'
xlow  = [ 0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1050, 1100, 1150, 1200, 1250, 1300, 1350, 1400,1450, 1500]

bin = len(xlow) - 1


#if chan == 'mu':

#from LoadData_DM Import *
    
FillTables(var, bin, xlow, high, yaxis, xaxis , save, chan)

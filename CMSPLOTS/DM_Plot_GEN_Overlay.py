import  sys
#sys.argv.append( '-b-' )
import ROOT,os, re, string, csv, math, sys, gc
from ROOT import TCanvas, TFile, TProfile, TNtuple, TH1F, TH1D, TH2F,TF1, TGaxis, TPad, TPaveLabel, TPaveText, TLegend, TLatex, THStack, TLine, TMath, TGraph, TGraphErrors, TMultiGraph, TStyle
from ROOT import gROOT, gBenchmark, gRandom, gSystem, Double, gPad, gStyle
from subprocess import call
from array import array
ROOT.gROOT.SetBatch()
#from tdrStyle import *
#setTDRStyle()
#gROOT.ProcessLine(".L ./tdrstyle.C")
#gROOT.ProcessLine("setTDRStyle()")
gROOT.ProcessLine(".L ./rootlogon.C")
gStyle.SetPalette(1);
GenRootFiles = {}
GenTrees = {}
xmass = ['1000', '2000', '1500', '800', '400']
pimass = [ '1', '2', '5', '10']
plife = ['0p001', '0p1', '1', '5', '150', '300']

for xm in xmass:
    for pm in pimass:
        for pl in plife:

            #inf =  '../Configuration/test_' + xm +'_' + pm + '_' + pl + '/output.root'
            
            inf =  '../Configuration/test_' + xm +'_' + pm + '_' + pl + '/MCHist_mass_X_d_' + xm + '_mass_pi_d_' + pm + '_tau_pi_d_' + pl + '.root'
            sig = '_' + xm +'_' + pm + '_' + pl + '_'
            GenRootFiles[ sig ] = TFile( inf )
            print sig 
        

List_0 = ['_400_10_0p001_',
          '_400_10_150_',
          '_2000_10_0p001_',
          '_2000_10_150_'      
          ]
List_1 = ['_400_10_5_',
          '_800_10_5_',
          '_1000_10_5_',
          '_1500_10_5_',
          '_2000_10_5_'     
        ]
List_2 = ['_1000_5_0p001_',
          '_1000_5_5_',
          '_1000_5_150_'
        ]
List_3 = ['_1000_1_5_',
          '_1000_5_5_',
          '_1000_10_5_',
          ]
List_4 = ['_400_10_150_',
          '_800_10_150_',
          '_1000_10_150_',
          '_1500_10_150_',
          '_2000_10_150_',
           ]



List_C = [ 1,2,3,4, ROOT.kOrange,6,7, 8, ROOT.kYellow+1,  ROOT.kCyan-4, ROOT.kMagenta, ROOT.kYellow, ROOT.kYellow-4, ROOT.kYellow-6, ROOT.kYellow-7, ROOT.kYellow-9,
             ROOT.kYellow-10,  ROOT.kOrange-2,  ROOT.kOrange-3, ROOT.kOrange-4, ROOT.kOrange-5,
             ROOT.kCyan+1, ROOT.kCyan,ROOT.kCyan-6, ROOT.kCyan-7, ROOT.kCyan-9,
             ROOT.kCyan-10, ROOT.kAzure+1, ROOT.kAzure+2,  ROOT.kAzure+6, ROOT.kAzure+6, ROOT.kAzure+8,
             ROOT.kMagenta+1,  ROOT.kMagenta-4, ROOT.kMagenta-6, ROOT.kMagenta-7, ROOT.kMagenta-9,
             ROOT.kMagenta-10, ROOT.kPink+2, ROOT.kPink+5,  ROOT.kPink+6, ROOT.kPink+7, ROOT.kPink+8,
             ROOT.kGreen+1, ROOT.kGreen, ROOT.kGreen-4, ROOT.kGreen-6, ROOT.kGreen-7, ROOT.kGreen-9,
             ROOT.kGreen-10, ROOT.kSpring-1, ROOT.kSpring-2,  ROOT.kSpring-3, ROOT.kSpring-4, ROOT.kSpring-5
             ]

#ROOT.kAzure+2,  ROOT.kAzure+6,  ROOT.kAzure+8,
#ROOT.kGreen+1, ROOT.kGreen, ROOT.kGreen-6,
                                   

List_lists = [List_0, List_1, List_2, List_3,  List_4]            

#List = ['Data', 'QCD', 'DM_A', 'DM_B']
#List_Vars = [ 'genjet_pt', 'genpart_pt', 'genjet_eta', 'genpart_eta']
#hptdp;1	dark pion pt distribution
#hrdecaydp;1	dark decay radius distribution
#hndaudp;1	number dark pion daughters
#Hndauchdp;1	number charged dark pion daughters
#hptdq;1	dark quark pt distribution
#hptjet;1	gen jet pt distribution           
             
######################################################

#####################################################################
def Plot_Overlay_one(  var, bin, low, high, ylabel, xlabel, Outdir, save, dolog, rebin, setx):
    lst = 0 
    lists = {}
    for lists in List_lists:
    #    List = {}
        List = lists
        lstn =lst
        #List_C = []
        save4 = save + "_" + str(lstn)
        
        c4 = TCanvas("c4","Signal MC Models", 1000, 800)
    
        #c4.SetRightMargin(0.06)
        #c4.SetLogy()
    
        Variables = {}
        print Variables
        legend = TLegend(.5,.75,.9,.9," X mass, DPion mass, DPion life time ")
        legend.SetBorderSize(0)
        legend.SetTextSize(0.025) 
        legend.SetTextFont(62)
        legend.SetFillColor(0);
        maxheight = 0
        j = 0
        Type = ""
        #print "before type" +  Type
        for Type in List:
            print "Low "
            print   low
            hclr = List_C[j]
            histname = Type + var + "_" + str(lst)
            Variables[Type] = TH1F(histname, histname, bin, low, high)
            h_old = GenRootFiles[Type].Get(var)
            for hbin in range(bin+1):
                Variables[Type].SetBinContent(hbin, h_old.GetBinContent(hbin) )
                #Variables[Type].SetBinError(hbin, 0.0  )

            
            print Variables[Type].GetNbinsX()
            numbinx = Variables[Type].GetNbinsX()
            print "number of bins " + str(numbinx) 
            Variables[Type].Rebin(rebin)
            print "after rebin " + str(Variables[Type].GetNbinsX())
            print  Type  
            print Variables[Type].Integral()
            print "Low again "
            print   low
            scaletounity = 1.0
            if (Variables[Type].Integral()):
                scaletounity = 1000.0/Variables[Type].Integral()
            Variables[Type].Scale(scaletounity)
           
            print Variables[Type].Integral()
            maxh = Variables[Type].GetMaximum() + 0.2*Variables[Type].GetMaximum()
            if (maxh > maxheight):
                maxheight = maxh
            legend . AddEntry( Variables[Type], Type , "l")       
            #
            Variables[Type].SetAxisRange(low, setx, "X")
 	    if (bin<25):
                draw0 = "Ep"
            else:
                draw0 = "h" 
            if j == 0:
                Variables[Type].Draw(draw0)   
            Variables[List[0]].SetMaximum(maxheight)
            Variables[List[0]].SetMinimum(0)
            #lineclr = List_C[j]
            #Variables[Type].SetLineStyle(j-7)
            Variables[Type].SetLineColor(hclr)
            Variables[Type].SetMarkerSize(1)
            Variables[Type].SetMarkerStyle(8)
            Variables[Type].SetMarkerColor(hclr)
            Variables[Type].Draw("SAME" + draw0)
            Variables[Type].GetYaxis().CenterTitle()
            Variables[Type].GetYaxis().SetTitle(ylabel)
            Variables[Type].GetXaxis().SetTitle(xlabel)
            print Variables[Type].GetXaxis().GetTitle()
            Variables[Type].SetLineWidth(4)
            j = j + 1
    
        latex = TLatex()
        latex.SetNDC()
        latex.SetTextSize(0.04)
        latex.SetTextAlign(31) # 31 align right
        latex.DrawLatex(0.42, 0.92, "CMS Preliminary");
        
        latex2 = TLatex()
        latex2.SetNDC()
        latex2.SetTextSize(0.04)
        latex2.SetTextAlign(31) # align right
        #latex2.DrawLatex(0.87, 0.95, str(lumiPlot) + " fb^{-1} at #sqrt{s} = 7 TeV");
        legend.Draw('same')        
        
        if (dolog):
            c4.SetLogy()
            Variables[List[0]].SetMaximum(maxheight)
            Variables[List[0]].SetMinimum(0.1)
            c4.SaveAs(Outdir + save4 + '_log.gif')
            c4.SaveAs(Outdir + save4 + '_log.pdf')
            c4.SaveAs(Outdir + save4 + '_log.C')
            c4.SaveAs(Outdir + save4 + '_log.root')
        else:
            c4.SaveAs(Outdir + save4 + '.gif')
            c4.SaveAs(Outdir + save4 + '.pdf')
            c4.SaveAs(Outdir + save4 + '.C')
            c4.SaveAs(Outdir + save4 + '.root')
        #pnum = pnum + 1
        del Type
        del histname
        del c4
        del Variables
        del legend
        del List
        lst = lst +1 
        save4 = save
    gc.collect()

#####################################################################

xsec = {}
pnum = 0
Outdir = "PLOTS-GEN/" 
var = "varnam"
bin = 100
low = 100
high = 2000
xaxis = "title"
yaxis = "title"
save  = "nameeps"

rebin = 1
setx = 100
dolog = 0

List_Vars = [ 'hptdp', 'hptdq', 'hptjet', 'hrdecaydp', 'hndaudp', 'hndauchdp']


for vars in List_Vars:
    dolog = 0
    print vars
    if (vars ==  'hrdecaydp'):
        var = vars
        bin = 1000
        low = 0
        high = 1000 
        xaxis = "Dark Pion decay radius"
        yaxis = 'Events / 1 mm'
        save = var + '_' 
        rebin = 1
        setx = 50
    if (vars == 'hndauchdp'):
        var = vars
        bin = 20
        low = 0
        high = 20 
        xaxis = "Charged Dark Pion daughters"
        yaxis = 'Events'
        save = "x_" + var + '_' 
        rebin = 1
        setx = 15
    if (vars == 'hndaudp'):
        var = vars
        bin = 20
        low = 0
        high = 20 
        xaxis = "Charged Dark Pion daughters"
        yaxis = 'Events'
        save = "x_" + var + '_' 
        rebin = 1
        setx = 15
    if (vars == 'hptdq' ):
        var = vars
        bin = 200
        low = 0
        high = 2000
        xaxis = "Dark Quark pT [GeV]"
        yaxis = 'Events / 10 GeV'
        save = var + '_'
        rebin = 5
        setx = 2000
    if (vars == 'hptjet'):
        var = vars
        bin = 1500
        low = 0
        high = 1500
        xaxis = "Jet pT  [GeV]"
        yaxis = 'Events / 10 GeV'
        save = var + '_'
        rebin = 5
        setx = 300
    if (vars == 'hptdp'):
        var = vars
        bin = 1500
        low = 0
        high = 1500
        xaxis = "Dark Pion pT [GeV]"
        yaxis = 'Events / 100 GeV'
        save = var + '_' 
        rebin = 5
        setx = 100
        
    Plot_Overlay_one( var, bin, low, high, yaxis, xaxis, Outdir, save, dolog, rebin, setx)

print "now do log plots -----------------------------------"

for vars in List_Vars:
    dolog = 1
    print vars
    if (vars == 'hptjet'):
        var = vars
        bin = 1500
        low = 0
        high = 1500
        xaxis = "Jet pT  [GeV]"
        yaxis = 'Events / 10 GeV'
        save = var + '_'
        rebin = 5
        setx = 1000
    if (vars ==  'hrdecaydp'):
        var = vars
        bin = 1000
        low = 0
        high = 1000 
        xaxis = "Dark Pion decay radius"
        yaxis = 'Events / 1 mm'
        save = var + '_' 
        rebin = 1
        setx = 150
    if (vars == 'hptdp'):
        var = vars
        bin = 1500
        low = 0
        high = 1500
        xaxis = "Dark Pion pT [GeV]"
        yaxis = 'Events / 100 GeV'
        save = var + '_' 
        rebin = 15
        setx = 1000
            
    Plot_Overlay_one( var, bin, low, high, yaxis, xaxis, Outdir, save, dolog, rebin, setx)
    
os.system("./html_gif.sh  PLOTS-GEN gif" )
os.system("cp -rf   PLOTS-GEN ~/public_html/Analysis/" )
gc.collect()


#Plot_Overlay_one( 'hptdp', 1500, 0, 1500, 'Events / 100 GeV', "Dark Pion pT [GeV]" ,Outdir,  'hptdp_', 1, 5, 500)





###Plot_Overlay_one( 'hptdp', 1500, 0, 1500, 'Events / 100 GeV', "Dark Pion pT [GeV]" ,Outdir,  'hptdp_', dolog, rebin, setx)
#Plot_Overlay_one( 'hptdp', 1500, 0, 1500, 'Events / 100 GeV', "Dark Pion pT [GeV]" ,Outdir,  'hptdp_', 0, 1, 50)
#lst = 0
##for lists in List_lists:
#    Lists = lists
   # Plot_Overlay_one( 'hptdp', 1500, 0, 1500, 'Events / 100 GeV', "Dark Pion pT [GeV]" ,Outdir,  'hptdp_', 1, 5, 500, Lists, lst)

#Plot_Overlay_one( 'hptdp', 1500, 0, 1500, 'Events / 100 GeV', "Dark Pion pT [GeV]" ,Outdir,  'hptdp_', 1, 5, 500, List_01, 1)
#Plot_Overlay_one( 'hptdp', 1500, 0, 1500, 'Events / 100 GeV', "Dark Pion pT [GeV]" ,Outdir,  'hptdp_', 1, 5, 500, List_4, 4)
#Plot_Overlay_one( 'hptdp', 1500, 0, 1500, 'Events / 100 GeV', "Dark Pion pT [GeV]" ,Outdir,  'hptdp_', 1, 5, 500, List_5, 5)
 # Plot_Overlay_one( 'hptdp', 1500, 0, 1500, 'Events / 100 GeV', "Dark Pion pT [GeV]" ,Outdir,  'hptdp_', 1, 5, 500, List_4, 4)
     

#lst = lst + 1




'''
for vars in List_Vars:
    print vars
    if (vars == 'hptdp'):
        var = vars
        bin = 1500
        low = 0
        high = 1500
        xaxis = "Dark Pion pT [GeV]"
        yaxis = 'Events / 100 GeV'
        save = var + '_' 
        dolog = 0
        rebin = 1
        setx = 70
    if (vars == 'hptdq' ):
        var = vars
        bin = 1500
        low = 0
        high = 1500
        xaxis = "Dark Quark pT [GeV]"
        yaxis = 'Events / 10 GeV'
        save = var + '_' 
    if (vars == 'hptjet'):
        var = vars
        bin = 1500
        low = 0
        high = 1500
        xaxis = "Jet pT  [GeV]"
        yaxis = 'Events / 10 GeV'
        save = var + '_' 
    if (vars == 'hndaudp'):
        var = vars
        bin = 20
        low = 0
        high = 20 
        xaxis = "Dark Pion daughters"
        yaxis = 'Events'
        save = var + '_' 
        dolog = 0
        rebin = 1
    if (vars == 'hndaudp'):
        var = vars
        bin = 20
        low = 0
        high = 20 
        xaxis = "Dark Pion daughters"
        yaxis = 'Events'
        save = var + '_' 
        dolog = 0
        rebin = 1
    if (vars == 'hndauchdp'):
        var = vars
        bin = 20
        low = 0
        high = 20 
        xaxis = "Charged Dark Pion daughters"
        yaxis = 'Events'
        save = var + '_' 
    if (vars ==  'hrdecaydp'):
        var = vars
        bin = 1000
        low = 0
        high = 1000 
        xaxis = "Dark Pion decay radius"
        yaxis = 'Events / 1 mm'
        save = var + '_' 
            
    print " Outdir,  var, bin, low, high, yaxis, xaxis , save" , Outdir,  var, bin, low, high, yaxis, xaxis , save 
    Plot_Overlay_one( var, bin, low, high, yaxis, xaxis ,Outdir,  save, dolog, rebin, setx)
   '''         

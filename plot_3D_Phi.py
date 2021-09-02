import sys
sys.path.append("JPyPlotRatio");

import numpy as np
import os
import ROOT
import pickle
import JPyPlotRatio
import matplotlib.ticker as plticker
from matplotlib.colors import LogNorm

rootfiles=[
"AnalysisResults.root",
"PhiWeights_LHC15o_Error_pt02_s_hybrid.root"];
#rootfiles="PhiWeights_LHC15o_Error_pt02_s_hybrid.root"

labelList=["simple test 3D hist","1D hist with 8$<z<$10cm","local-101219-ROOT5-NoChange","local-150121-ROOT5-2020","local-151219-ROOT5-MultBinModes","local-231219-ROOT5-MultBinModes2-MerryXmas","local-011219-ROOT5-CatalystTrackCuts","local-081019-ROOT5-EffParams","local-100621-zvtx8.root","new MAP"];

toptitle = " ROOT/v6-20-08-alice1-79 "; # need to add on the top
dataDetail = " AliRoot/v5-09-57d-1";

# setting the ranges for eta and zvtx
binWzvtx = 2.; #2cm bin
binWeta  = 0.2; # eta bin width
zranges =  [ 8.+ i * binWzvtx   for i in range(int(2. / binWzvtx ) +1) ];
etaranges = [ 0.6+ i * binWeta   for i in range(int(0.2 / binWeta ) +1) ];
cent = 0;

print(zranges);
print(etaranges);

# Global variables:::
nRow = len(zranges)-1;
nCol = len(etaranges)-1;
plabels = {i:"{:.01f}$<z<${:.01f} \n {:.01f}$<\eta<${:.01f} ".format(zranges[iz],zranges[iz+1],etaranges[ie],etaranges[ie+1]) for i,(iz,ie) in enumerate(np.ndindex(nRow,nCol))};
styleList=["solid","dotted","dashed","dashdot","solid","dotted","dashed","dashdot","solid","dotted","dashed","dashdot"];

f = ROOT.TFile("{}".format(rootfiles[0]),"read");
f2 = ROOT.TFile("{}".format(rootfiles[1]),"read");			
t = f.Get("MyTask");
c = t.Get("MyOutputContainer");
h = c.FindObject("fHistZetaPhi");
h.Print();
#t = f.Get("PhiWeights"); # TList
#h = t.FindObject("PhiWeights_246087_00").Clone();

t2 = f2.Get("PhiWeights"); # TList
h2 = t2.FindObject("PhiWeights_246087_00").Clone();

#Getting 1D histograms from file
hPhi810 = c.FindObject("fHistPhiCut810");
hPhi610 = c.FindObject("fHistPhiCut610");
hPhiLess8 = c.FindObject("fHistPhiCutLess8");

def FileDep():
	print("FileDep...");
	rlimits = [(0.0,1.2) for i in range(nRow)];
	plot = JPyPlotRatio.JPyPlotRatio(panels=(nRow,nCol),
		disableRatio=[],
		panelLabel=plabels,
		legendPanel=0,
		#ratioBounds=rlimits,
		#rowBounds={0:(0,20.)},
		panelLabelLoc=(0.3,0.99),panelLabelSize=10,panelLabelAlign="left",legendSize=5);

	#creating an empty mult. array
	plots=np.empty(3,dtype=int);
	ir = 0; #since I only have one run
	ic = 0;
	#for-loop for zvtx
	for i in range(0,nRow):
		#for-loop for eta
		for j in range(0,nCol):
			iPad = nCol*i+j; #calculate pad
			


			h.GetZaxis().SetRange(h.GetZaxis().FindBin(zranges[i]),h.GetZaxis().FindBin(zranges[i+1]-1));
			h2.GetZaxis().SetRange(h2.GetZaxis().FindBin(zranges[i]),h2.GetZaxis().FindBin(zranges[i+1]-1));
			#Bin1 = h.GetZaxis().FindBin(zranges[i]);
			#Bin2 = h.GetZaxis().FindBin(zranges[i+1]-1);
			#BinLower=h.GetZaxis().GetBinLowEdge(Bin1);
			#BinUpper=h.GetZaxis().GetBinUpEdge(Bin2);
			#print(BinLower,BinUpper);
			p2d = h.Project3D("z_{}_{}{:02d}_proj2_yx".format(zranges[i],zranges[i+1],cent));
			p2d2 = h2.Project3D("z_{}_{}{:02d}_proj2_yx".format(zranges[i],zranges[i+1],cent));
			p1d = p2d.ProjectionX("etaz_proj",p2d.GetYaxis().FindBin(etaranges[j]),p2d.GetYaxis().FindBin(etaranges[j+1]-0.1),"N");
			p1d2 = p2d2.ProjectionX("etaz_proj",p2d2.GetYaxis().FindBin(etaranges[j]),p2d2.GetYaxis().FindBin(etaranges[j+1]-0.1),"N");
			Bin3 = p2d.GetYaxis().FindBin(etaranges[j]);
			Bin4 = p2d.GetYaxis().FindBin(etaranges[j+1]-0.1);
			Bin3Low = p2d.GetYaxis().GetBinLowEdge(Bin3);
			Bin4Up = p2d.GetYaxis().GetBinUpEdge(Bin4);
			print(Bin3Low,Bin4Up)
			p1derr = ROOT.TGraphErrors(p1d);
			p1derr2 = ROOT.TGraphErrors(p1d2);
			x1,y1,_,yerr1 = JPyPlotRatio.TGraphErrorsToNumpy(p1derr);
			x2,y2,_,_ = JPyPlotRatio.TGraphErrorsToNumpy(p1derr2);
			plots[0]=plot.Add(iPad,(x1,y1,yerr1),label=labelList[0], linestyle=styleList[0]);
			#plots[1]=plot.Add(iPad,(x2,y2),label="JaspersMAP",linestyle="dashed");
			
	p1 = plot.Add(0,hPhi810,color="red",label="1D hist 8$<$zvtx$<$10cm", linestyle="dotted");
	plot.Ratio(p1,plots[0]);
	f.Close();
	plot.Plot();
	plot.GetPlot().text(0.3,0.58,toptitle,fontsize=9);
	plot.GetPlot().text(0.3,0.52,dataDetail,fontsize=9);
	plot.Save("figs/phiMap_From3D_Less8.pdf");
	plot.Show();

#######################################################

FileDep();
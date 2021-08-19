import numpy as np
import ROOT

import sys
sys.path.append("JPyPlotRatio");
#https://github.com/jaelpark/JPyPlotRatio

import JPyPlotRatio

alitags = [
		"vAN-20210404_ROOT6-1",
		"vAN-20190930-1"
];

rootfile = "AnalysisResults.root";


f = ROOT.TFile("{}".format(rootfile),"read");
t = f.Get("MyTask");
h = t.Get("MyOutputContainer");
hPhi = h.FindObject("fHistPhi");
hPhiCut = h.FindObject("fHistPhiCut");

histList = ["hPhi","hPhiCut"];

#rlimits = [(0.0,1.2)];
plot = JPyPlotRatio.JPyPlotRatio(panels=(1,1),
	disableRatio=[0],
	legendPanel=1,
	#ratioBounds=rlimits,
	#panelLabelLoc=(0.3,0.99),
	#panelLabelSize=10,
	#panelLabelAlign="left",
	#legendSize=5
	);


p1 = plot.Add(0,hPhi,color="red",label="Phi");
p2 = plot.Add(0,hPhiCut,color="orange",label="PhiCut");
#plot.Ratio(p1,p2);

plot.Plot();

plot.Save("figs/Phi.pdf");
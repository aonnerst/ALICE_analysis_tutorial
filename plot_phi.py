import numpy as np
import ROOT

import sys
sys.path.append("JPyPlotRatio");
#https://github.com/jaelpark/JPyPlotRatio

import JPyPlotRatio


rootfile = "AnalysisResults.root";

toptitle = " ROOT/v6-20-08-alice1-79 "; # need to add on the top
dataDetail = " AliRoot/v5-09-57d-1";
dataDetail2 = " 0.6$<|\eta|<$0.8";

f = ROOT.TFile("{}".format(rootfile),"read");
t = f.Get("MyTask");
h = t.Get("MyOutputContainer");
hPhi810 = h.FindObject("fHistPhiCut810");
hPhi610 = h.FindObject("fHistPhiCut610");
hPhiLess8 = h.FindObject("fHistPhiCutLess8");


#rlimits = [(0.0,1.2)];
plot = JPyPlotRatio.JPyPlotRatio(panels=(1,1),
	disableRatio=[0],
	legendPanel=0,
	legendLoc=(0.45,0.40),
	#ratioBounds=rlimits,
	#panelLabelLoc=(0.3,0.99),
	#panelLabelSize=10,
	#panelLabelAlign="left",
	legendSize=6
	);
'''
x1,y1,_,_ = JPyPlotRatio.TGraphErrorsToNumpy(hPhi810);
x2,y2,_,_ = JPyPlotRatio.TGraphErrorsToNumpy(hPhi610);
x3,y3,_,_ = JPyPlotRatio.TGraphErrorsToNumpy(hPhiLess8);

p1 = plot.Add(0,(x1,y1),color="red",label="810");
p2 = plot.Add(0,(x2,y2),color="orange",label="610");
p3 = plot.Add(0,(x3,y3),color="green",label="Less than 8");
'''
p1 = plot.Add(0,hPhi810,color="red",label="8$<|zvtx|<$10cm");
p2 = plot.Add(0,hPhi610,color="orange",label="6$<|zvtx|<$10cm");
p3 = plot.Add(0,hPhiLess8,color="green",label="$|zvtx|<$8cm");

#plot.Ratio(p1,p2);

plot.Plot();
plot.GetPlot().text(0.3,0.66,toptitle,fontsize=8);
plot.GetPlot().text(0.3,0.61,dataDetail,fontsize=8);
plot.GetPlot().text(0.3,0.56,dataDetail2,fontsize=8);
plot.Show();

plot.Save("figs/Phi.pdf");
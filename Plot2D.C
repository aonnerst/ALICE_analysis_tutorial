#include "TMath.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TH3.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TString.h"
#include <TStopwatch.h>
#include <TComplex.h>
#include <vector>
#include "include/rootcommon.h"

using namespace std;

TH3F *h3d;
TH2D *h2d;

void LoadData(TString);
void DrawZEta2D();

//Main Function
void Plot2D(TString inputfile="NUAError/PhiWeights_LHC15o_Error_pt02_s_hybrid.root")
{
	LoadData(inputfile);
	DrawZEta2D();
}

// Member Functions
void LoadData(TString inputfile)
{
	TFile *fIn = TFile::Open(inputfile,"read");
	TList *f=(TList*)fIn->Get("PhiWeights");

	h3d=(TH3F*)f->FindObject("PhiWeights_245683_00"); // Finding the 3D histogram.
	h3d->Print();
}

void DrawZEta2D()
{
	gStyle->SetOptStat(0);
	TCanvas *can = new TCanvas("canvas","canvas",1024,740);

	TH2D *h2D = (TH2D*)h3d->Project3D("yz");

	//For editing canvas #include "include/rootcommon.h"
   	double lowx = -0.8,highx=0.8;
  	double ly=-10.,hy=10.;
  	//Double_t ly = 0, hy=565;
  	TH2F *hfr = new TH2F("hfr"," ", 100,lowx, highx, 10, ly, hy); // numbers: tics x, low limit x, upper limit x, tics y, low limit y, upper limit y
  	hset( *hfr, "#eta", "Z-vertex",0.9,0.9, 0.15,0.05, 0.01,0.01, 0.03,0.03, 510,505);//settings of the upper pad: x-axis, y-axis
  	hfr->Draw();
  	h2D->Draw("colz same");

  	gPad->GetCanvas()->SaveAs("figs/2DPlot_Z_vs_eta.pdf");
}
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

//Declaring variables and arrays
const int NHist = 3;
const int NCan = 3;
TH1 *hPhi[NHist];
TH3F *h3d;
TH1D *h1d;
Double_t etamin = 0.6;
Double_t etamax = 0.8;
Double_t zmin[] = {8.0,6.0,0.0};
Double_t zmax[] = {10.0,10.0,8.0};
TString names[]={
	"fHistPhiCut810",
	"fHistPhiCut610",
	"fHistPhiCutLess8",
	"fHistZetaPhi"};


//Declaring functions
void LoadData(TString);
void DrawPhi();

//Main Function
void PlotPhi(TString inputfile="AnalysisResults.root")
{
	LoadData(inputfile);
	DrawPhi();
}

// Member Functions
void LoadData(TString inputfile)
{
	TFile *fIn = TFile::Open(inputfile,"read");
	TList *f=(TList*)fIn->Get("MyTask/MyOutputContainer");

	for(Int_t ih=0; ih<NHist; ih++)
	{
		hPhi[ih]=(TH1F*)f->FindObject(Form("%s",names[ih].Data()));
	}
	h3d=(TH3F*)f->FindObject("fHistZetaPhi");
	h3d->Print();
}

void DrawPhi()
{
	for(Int_t ic=0; ic<NCan; ic++)
	{
	
		gStyle->SetOptStat(0);
		TCanvas *can = new TCanvas("canvas","canvas",1024,740);

		// (Phi, eta, zvtx), Different from python plotting
		double zminBin = h3d->GetZaxis()->FindBin(zmin[ic]);
		double zmaxBin = h3d->GetZaxis()->FindBin(zmax[ic]);
		double etaminBin = h3d->GetYaxis()->FindBin(etamin);
		double etamaxBin = h3d->GetYaxis()->FindBin(etamax);
		Int_t Bin1 = h3d->GetZaxis()->FindBin(zmin[ic]);
		Int_t Bin2 = h3d->GetZaxis()->FindBin(zmax[ic]);
		double BinLower=h3d->GetZaxis()->GetBinLowEdge(Bin1);
		double BinUpper=h3d->GetZaxis()->GetBinUpEdge(Bin2);
		cout<< h3d->GetNbinsZ() << endl;
		cout<< h3d->GetNbinsY()<< endl;
		//cout << Form()

		cout<< BinLower << endl;
		cout<<BinUpper<<endl;


		cout <<"Same as Python way" << endl;
		// The Python  way of projection
		h3d->GetZaxis()->SetRange(h3d->GetZaxis()->FindBin(zmin[ic]),h3d->GetZaxis()->FindBin(zmax[ic]));
		//TH2D *py2d = (TH2D*)h3d->Project3D(Form("z_%.1f_%.1f_proj2_yx",zmin[ic],zmax[ic]));
		TH2D *py2d = (TH2D*)h3d->Project3D("yx");
		
		TH1D *py1d = (TH1D*)py2d->ProjectionX("etaz_proj",py2d->GetYaxis()->FindBin(etamin),py2d->GetYaxis()->FindBin(etamax));

		//------------------------------

		h1d = h3d->ProjectionX("_px",etaminBin,etamaxBin,zminBin,zmaxBin,"");
		
		//For editing canvas #include "include/rootcommon.h"
	   	double lowx = -TMath::Pi(),highx=TMath::Pi();
	  	double ly=hPhi[ic]->GetMinimum()*0.1,hy=hPhi[ic]->GetMaximum()*1.1;
	  	//Double_t ly = 0, hy=565;
	  	TH2F *hfr = new TH2F("hfr"," ", 100,lowx, highx, 10, ly, hy); // numbers: tics x, low limit x, upper limit x, tics y, low limit y, upper limit y
	  	hset( *hfr, "#phi", "",0.9,0.9, 0.15,0.05, 0.01,0.01, 0.03,0.03, 510,505);//settings of the upper pad: x-axis, y-axis
	  	
	  	
	  	TLegend *legend = new TLegend(0.3,0.2,0.5,0.3,"","brNDC");
	    legend->SetTextSize(0.04);legend->SetBorderSize(0);legend->SetFillStyle(0);//legend settings;
	  	legend->AddEntry(hPhi[ic],Form("1D, %.1f<zvtx<%.1f",zmin[ic],zmax[ic]));
		legend->AddEntry(h1d,Form("3D, %.1f<zvtx<%.1f",zmin[ic],zmax[ic]));
		legend->AddEntry(py1d,Form("3D Python, %.1f<zvtx<%.1f",zmin[ic],zmax[ic]));

	  	hPhi[ic]->SetLineStyle(2);
		hPhi[ic]->SetLineWidth(2);
		//hPhi[ic]->SetLineColor(kRed+1);
		//hPhi[ic]->SetAxisRange(hPhi[ic]->GetMinimum()*0.1,hPhi[ic]->GetMaximum()*1.1,"Y");
		
		py1d -> SetLineStyle(4);
		py1d -> SetLineWidth(2);
		//py1d -> SetLineColor(kBlue+1);

		TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
		pad1->SetBottomMargin(0);
		pad1->Draw();
		pad1->cd();
		hfr->Draw();
		hPhi[ic]->Draw("e,same");
		h1d -> Draw("e,same");
		py1d -> Draw("same");
		legend -> Draw();
		can->cd();

		TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.3);
		pad2->SetTopMargin(0);
		pad2->Draw();
		pad2->cd();
		TH1D *hr = (TH1D*)h1d->Clone();
		TH1D *pyr = (TH1D*)py1d->Clone();
		hr->Divide(hPhi[ic]);
		pyr->Divide(h1d);
		hr->Draw("E");
		pyr->Draw("e");
		can->cd();
		
		
		gPad->GetCanvas()->SaveAs(Form("figs/Phi_%.1f_%.1f.pdf",zmin[ic],zmax[ic]));
	}
}
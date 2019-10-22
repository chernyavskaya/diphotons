
#include <iostream>
#include <fstream>
#include <TH1F.h>
#include <TTree.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TFile.h"
#include "TROOT.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TGraph.h"
#include <algorithm>    // std::min_element, std::max_element

using namespace std;

//g++ optimize_cats.C -g -o opt `root-config --cflags --glibs` -lMLP -lXMLIO

int main(int argc, char* argv[]){
	gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
	

	TString path="/afs/cern.ch/work/n/nchernya/ETH/DiHiggs/optimization_files/20191008/";
	
	TString what_to_opt = "ttHScore";
	double xmin = 0.;
	double xmax = 1.;
	Double_t precision=0.02;  

	TString Mgg_window = "*((Mgg>122)&&(Mgg<128))"; // this narrow window only for ttH
	TString Mgg_sideband = "*((Mgg<=115)||(Mgg>=135))";
	TString selection_sig = "weight*lumi*eventTrainedOn*0.587";   ///0.587fb expected limit for sideband run II, divide by 3 if using mix of SM,3 and box
	TString selection_bg = "weight*lumi*overlapSave";
	TString selection_diphoton = "*1.5"; //SF needed to match data normalization

	TString subcategory = "*((MVAOutputTransformed>0.29)&&(MVAOutputTransformed<.51))";
	TString outstr = "_MVA2";
	double minevents = 40; //

	selection_sig += subcategory;
	selection_bg += subcategory;


	TString date = "22_10_2019";
	TString s; TString sel; 
	TString outname = s.Format("scan_%s_minevents%.0f_%s_ttH",what_to_opt.Data(),minevents,outstr.Data());

	TFile *file_s =  TFile::Open(path+"Total_sig_bkg_tth.root");
	TFile *file_bg =  TFile::Open(path+"Total_sig_bkg_tth.root");
	TTree *tree_sig = (TTree*)file_s->Get("reducedTree_sig");
	TTree *tree_bg = (TTree*)file_bg->Get("reducedTree");

double tth_cut = 0.1;
TString tth_cut_s;
double tth_scan[100] = {};
double S2B_scan[100] = {};
double significance_scan[100] = {};
double Bsideband_scan[100] = {};
int counter= 0;
do {
	tth_scan[counter] = tth_cut;
	tth_cut_s.Form("*(ttHScore>%.3f)",tth_cut);

	TH1F *hist_S = new TH1F("hist_S","hist_S",int((xmax-xmin)/precision),xmin,xmax);
   s.Form("%s>>hist_S",what_to_opt.Data());
   sel.Form("%s",(selection_sig+Mgg_window+tth_cut_s).Data());
	tree_sig->Draw(s,sel,"goff");

	TH1F *hist_B = new TH1F("hist_B","hist_B",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B",what_to_opt.Data());
   sel.Form("%s",(selection_bg+selection_diphoton+Mgg_window+tth_cut_s).Data());
	tree_bg->Draw(s,sel,"goff");
	cout<<"BG integral under Mgg "<<hist_B->Integral()<<endl;

	TH1F *hist_B_sideband = new TH1F("hist_B_sideband","hist_B_sideband",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B_sideband",what_to_opt.Data());
   sel.Form("%s",(selection_bg+selection_diphoton+Mgg_sideband+tth_cut_s).Data());
	tree_bg->Draw(s,sel,"goff");
	cout<<"BG integral sidebands "<<hist_B_sideband->Integral()<<endl;

	TTree *tree_bg_tth = (TTree*)file_bg->Get("reducedTree_bkg_0");
	TH1F *hist_B_cut_tth = new TH1F("hist_B_cut_tth","hist_B_cut_tth",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B_cut_tth",what_to_opt.Data());
   sel.Form("%s",(selection_bg+Mgg_window+tth_cut_s).Data());
	tree_bg_tth->Draw(s,sel,"goff");
	
	hist_B->Add(hist_B_cut_tth);
//	hist_B = hist_B_cut_tth; //only ttH
 
	if (hist_B->Integral()!=0) {
		S2B_scan[counter] = (pow(hist_S->Integral(),2)/hist_B->Integral());
	} else { 
		S2B_scan[counter] = 0.;
	}
	significance_scan[counter] = sqrt(S2B_scan[counter]);
	Bsideband_scan[counter] =  (hist_B_sideband->Integral());

	tth_cut+=precision;
	counter++;
} while (tth_cut<0.4);


// CMS info
	float left2 = gStyle->GetPadLeftMargin();
	float right2 = gStyle->GetPadRightMargin();
	float top2 = gStyle->GetPadTopMargin();
	float bottom2 = gStyle->GetPadBottomMargin();
	TPaveText pCMS1(left2,1.-top2,0.4,1.,"NDC");
	pCMS1.SetTextFont(62);
	pCMS1.SetTextSize(top2*0.75);
	pCMS1.SetTextAlign(12);
	pCMS1.SetFillStyle(-1);
	pCMS1.SetBorderSize(0);
	pCMS1.AddText("CMS");
	TPaveText pCMS12(left2+0.1,1.-top2*1.1,0.6,1.,"NDC");
	pCMS12.SetTextFont(52);
	pCMS12.SetTextSize(top2*0.75);
	pCMS12.SetTextAlign(12);
	pCMS12.SetFillStyle(-1);
	pCMS12.SetBorderSize(0);
	pCMS12.AddText("Preliminary");
	TPaveText pCMS2(0.5,1.-top2,1.-right2*0.5,1.,"NDC");
	pCMS2.SetTextFont(42);
	pCMS2.SetTextSize(top2*0.75);
	pCMS2.SetTextAlign(32);
	pCMS2.SetFillStyle(-1);
	pCMS2.SetBorderSize(0);
	pCMS2.AddText("(13 TeV)");
	TPaveText pave22(0.2,0.8,0.4,1.-top2*1.666,"NDC");
	pave22.SetTextAlign(11);
	pave22.SetFillStyle(-1);
	pave22.SetBorderSize(0);
	pave22.SetTextFont(62);
	pave22.SetTextSize(top2*0.5);
	pave22.AddText("HHbbgg");
	TPaveText pave33(0.2,0.75,0.4,0.8,"NDC");
	pave33.SetTextAlign(11);
	pave33.SetFillStyle(-1);
	pave33.SetBorderSize(0);
	pave33.SetTextFont(42);
	pave33.SetTextColor(kBlue);
	pave33.SetTextSize(top2*0.5);



TGraph *gr =new TGraph(counter,tth_scan,significance_scan);

//	float ymin=std::min_element( S2B_scan.begin(), S2B_scan.end()) *0.7;
//	float ymax=std::min_element( S2B_scan.begin(), S2B_scan.end() )*1.3;

	double ymin = *std::min_element(significance_scan,significance_scan+counter) * 0.9;
	double ymax = *std::max_element(significance_scan,significance_scan+counter) * 1.1;
	int max_pos = std::distance(significance_scan, std::max_element(significance_scan,significance_scan+counter));

	TCanvas *c1 = new TCanvas("Fit","",800,800);
	c1->cd();
	TH1F *frame2 = new TH1F("frame2","",50,xmin,xmax);
	frame2->GetXaxis()->SetNdivisions(505);
//	frame2->GetYaxis()->SetRangeUser(80,150);
   frame2->SetStats(0);
	frame2->SetYTitle("S/#sqrt{B_{#gamma#gamma}+B_{ttH}}");
//	frame2->SetYTitle("S/#sqrt{B_{ttH}}");
	frame2->SetXTitle(s.Format("%s",what_to_opt.Data()));	
	frame2->SetMinimum(ymin);
	frame2->SetMaximum(ymax);
	frame2->Draw();

	gr->Draw("Psame");


	TLine* line = new TLine(tth_scan[max_pos],ymin,tth_scan[max_pos],ymax);
	line->SetLineStyle(9);
	line->SetLineColor(1);
	line->SetLineWidth(2);
	line->Draw("same");
	cout<<"maximum pos,val : "<<tth_scan[max_pos]<<" , "<<ymax;

	gPad->Update();
	pCMS1.Draw("same");
	pCMS2.Draw("same");
	pCMS12.Draw("same");
	pave22.Draw("same");
	pave33.Draw("same");
	gPad->RedrawAxis();
	c1->Print(s.Format("plots/%s/%s.png",date.Data(),outname.Data()));
	c1->Print(s.Format("plots/%s/%s.pdf",date.Data(),outname.Data()));

/*
TGraph *gr_sideband =new TGraph(counter,tth_scan,Bsideband_scan);
	ymin = *std::min_element(Bsideband_scan,Bsideband_scan+counter) * 0.9;
	ymax = *std::max_element(Bsideband_scan,Bsideband_scan+counter) * 1.1;
	TCanvas *c2 = new TCanvas("B","",800,800);
	c2->cd();
	TH1F *frame3 = new TH1F("frame3","",50,xmin,xmax);
	frame3->GetXaxis()->SetNdivisions(505);
   frame3->SetStats(0);
	frame3->SetYTitle("Expected BG in sidebands");
	frame3->SetXTitle(s.Format("%s",what_to_opt.Data()));	
	frame3->SetMinimum(ymin);
	frame3->SetMaximum(ymax);
	frame3->Draw();
	gr_sideband->Draw("Psame");
	gPad->Update();
	pCMS1.Draw("same");
	pCMS2.Draw("same");
	pCMS12.Draw("same");
	pave22.Draw("same");
	pave33.Draw("same");
	gPad->RedrawAxis();
	c2->Print(s.Format("plots/%s/sidebands_%s.png",date.Data(),outname.Data()));
*/

return 0;

}


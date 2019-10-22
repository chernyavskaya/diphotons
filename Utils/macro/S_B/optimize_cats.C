
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
#include <vector>
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
	
	const int NCAT=2;

/*	TString what_to_opt = "MVAOutputTransformed";
	double xmin = 0.;
	double xmax = 1.;
	Double_t precision=0.01;  //0.01 for MVA, 5 for MX
*/
/*
	TString what_to_opt = "MX";
	double xmin = 250;
	double xmax = 800;
	Double_t precision=5;  //0.01 for MVA, 5 for MX
*/

	TString what_to_opt = "ttHScore";
	double xmin = 0.;
	double xmax = 1.;
	Double_t precision=0.02;  



	TString Mgg_window = "*((Mgg>122)&&(Mgg<128))"; // this narrow window only for ttH
//	TString Mgg_window = "*((Mgg>115)&&(Mgg<135))";
	TString Mgg_sideband = "*((Mgg<=115)||(Mgg>=135))";
	TString selection_sig = "weight*lumi*eventTrainedOn*0.587";   ///0.587fb expected limit for sideband run II, divide by 3 if using mix of SM,3 and box
	TString selection_bg = "weight*lumi*overlapSave";
	TString selection_diphoton = "*1.5"; //SF needed to match data normalization
/*
	TString subcategory = "";
	TString outstr = "";
	double minevents = 60; //
*/
/*	TString subcategory = "*((MVAOutputTransformed>0.29)&&(MVAOutputTransformed<.51))";
	TString outstr = "_MVA2";
	double minevents = 8; //
*/
//borders of categories : 0	0.29	0.51	0.68	1

//	TString subcategory = "*((MVAOutputTransformed>0.68)&&(MVAOutputTransformed<1.))";
	TString subcategory = "*((MVAOutputTransformed>0.70)&&(MVAOutputTransformed<1.))";
	TString outstr = "_simultMVA0";
	double minevents = 45; 


	selection_sig += subcategory;
	selection_bg += subcategory;


	TString date = "22_10_2019";
	TString s; TString sel; 
	TString outname = s.Format("output_SB_%s_cat%d_minevents%.0f_%s",what_to_opt.Data(),NCAT,minevents,outstr.Data());


	TFile *file_s =  TFile::Open(path+"Total_sig_bkg_tth.root");
	TTree *tree_sig = (TTree*)file_s->Get("reducedTree_sig");
	TH1F *hist_S = new TH1F("hist_S","hist_S",int((xmax-xmin)/precision),xmin,xmax);
   s.Form("%s>>hist_S",what_to_opt.Data());
   sel.Form("%s",(selection_sig+Mgg_window).Data());
	tree_sig->Draw(s,sel,"goff");

	TFile *file_bg =  TFile::Open(path+"Total_sig_bkg_tth.root");
	TTree *tree_bg = (TTree*)file_bg->Get("reducedTree");
	TH1F *hist_B = new TH1F("hist_B","hist_B",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B",what_to_opt.Data());
   sel.Form("%s",(selection_bg+selection_diphoton+Mgg_window).Data());
	tree_bg->Draw(s,sel,"goff");
	cout<<"BG integral under Mgg "<<hist_B->Integral()<<endl;

	TH1F *hist_B_sideband = new TH1F("hist_B_sideband","hist_B_sideband",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B_sideband",what_to_opt.Data());
   sel.Form("%s",(selection_bg+selection_diphoton+Mgg_sideband).Data());
	tree_bg->Draw(s,sel,"goff");
	cout<<"BG integral sidebands "<<hist_B_sideband->Integral()<<endl;

	TTree *tree_bg_tth = (TTree*)file_bg->Get("reducedTree_bkg_0");
	TH1F *hist_B_cut_tth = new TH1F("hist_B_cut_tth","hist_B_cut_tth",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B_cut_tth",what_to_opt.Data());
   sel.Form("%s",(selection_bg+Mgg_window).Data());
	tree_bg_tth->Draw(s,sel,"goff");

	

	double END = hist_B->GetBinCenter(hist_B->FindLastBinAbove(0.))+hist_B->GetBinWidth(1)/2.; //right end of BDT distibution
	double START = hist_B->GetBinCenter(hist_B->FindFirstBinAbove(0.))-hist_B->GetBinWidth(1)/2.; //right end of BDT distibution
	cout<<"start = "<<START<<" , end = "<<END<<endl;


	hist_S->SetFillStyle(4050);
	hist_S->SetLineColor(kRed);
	hist_S->SetFillColor(kRed-7);
	hist_S->SetLineWidth(2);
	hist_B->SetFillStyle(4050);
	hist_B->SetLineColor(kBlue+1);
	hist_B->SetFillColor(kBlue-10);
	hist_B->SetLineWidth(2);
//	hist_B_cut_tth->SetFillStyle(4050);
	hist_B_cut_tth->SetLineColor(kGreen+1);
//	hist_B_cut_tth->SetFillColor(kGreen-6);
	hist_B_cut_tth->SetLineWidth(2);
	TH1F *hist_B2 = (TH1F*)hist_B->Clone("b_new");
	hist_B2->Rebin(1); //4
	TH1F *hist_S2 = (TH1F*)hist_S->Clone("s_new");
	hist_S2->Rebin(1); //4
	hist_S2->Scale();


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
	TLegend *leg = new TLegend(0.72,0.78,0.85,0.9);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(42);
	leg->SetTextSize(0.025);
	leg->AddEntry(hist_S2,"Sig (exp. exclusion)","F");
	leg->AddEntry(hist_B2,"BG","F");
	leg->AddEntry(hist_B_cut_tth,"ttH","L");



	double bin=0.;
	double s1=0; double b1=0;
	int i=0;
	float max_all=0;	
		do	{
			s1=hist_S2->GetBinContent(i+1);
			b1=hist_B2->GetBinContent(i+1);
			bin=(double) hist_S2->GetBinCenter(i+1+1);
			if ((b1)!=0) max_all += pow(s1,2)/(b1);
			i++;
		} while (bin < END);



double max = 0;
double borders[10] = {};   // including START and END
borders[0] = START;
double sig_n[10] = {0,0,0,0,0,0,0,0,0,0};
double bkg_n[10] = {0,0,0,0,0,0,0,0,0,0};
double bkg_sideband_n[10] = {0,0,0,0,0,0,0,0,0,0};
double max_n[10] = {0,0,0,0,0,0,0,0,0,0};
double max_final[10] = {0,0,0,0,0,0,0,0,0,0};
double max_total = 0;
double start_n[10] = {0,0,0,0,0,0,0,0,0,0};
double bkg_yields[10] = {0,0,0,0,0,0,0,0,0,0};
double bkg_yields_sideband[10] = {0,0,0,0,0,0,0,0,0,0};
double sig_yields[10] = {0,0,0,0,0,0,0,0,0,0};


for (int index=0;index<NCAT;index++)
	start_n[index]=START+(index+1)*precision; 
int minevt_cond_n[10] = {};


double bkg_n_tth[10] = {0,0,0,0,0,0,0,0,0,0};
double max_final_tth[10] = {0,0,0,0,0,0,0,0,0,0};
double max_n_tth[10] = {0,0,0,0,0,0,0,0,0,0};

std::vector<double> categories_scans0;
std::vector<double> categories_scans1;
std::vector<double> categories_scans2;
std::vector<double> categories_scans3;
std::vector<double> significance_scans0;

/////////////////////// Do you need ttH ???///////////////
hist_B->Add(hist_B_cut_tth);
//////////////////////////////////////////////////////////
	do {
		max_n[0]=0;
		sig_n[0] = hist_S->Integral(1,hist_S->FindBin(start_n[0])-1);
		bkg_n[0] = hist_B->Integral(1,hist_B->FindBin(start_n[0])-1);
		bkg_n_tth[0] = hist_B_cut_tth->Integral(1,hist_B_cut_tth->FindBin(start_n[0])-1);
		bkg_sideband_n[0] = hist_B_sideband->Integral(1,hist_B_sideband->FindBin(start_n[0])-1);
		if (bkg_n[0]!=0) max_n[0]=pow(sig_n[0],2)/bkg_n[0];
	//	if (bkg_n[0]!=0) max_n[0]=pow(sig_n[1],2)/(bkg_n[0]+bkg_n_tth[0]);
		start_n[1]=start_n[0]+precision;
///////////////
		bkg_sideband_n[1] = hist_B_sideband->Integral(hist_B_sideband->FindBin(start_n[0]),hist_B_sideband->GetNbinsX()+1);
		if (bkg_n[1]!=0) max_n[1]=pow(sig_n[1],2)/bkg_n[1];
		if (bkg_sideband_n[1]>minevents) {
			categories_scans0.push_back(start_n[0]);	
			significance_scans0.push_back(sqrt(max_n[1]));
		}
//////////////
		do {
			max_n[1]=0;
			sig_n[1] = hist_S->Integral(hist_S->FindBin(start_n[0]),hist_S->FindBin(start_n[1])-1);
			bkg_n[1] = hist_B->Integral(hist_B->FindBin(start_n[0]),hist_B->FindBin(start_n[1])-1);
			bkg_n_tth[1] = hist_B_cut_tth->Integral(hist_B_cut_tth->FindBin(start_n[0]),hist_B_cut_tth->FindBin(start_n[0])-1);
			bkg_sideband_n[1] = hist_B_sideband->Integral(hist_B_sideband->FindBin(start_n[0]),hist_B_sideband->FindBin(start_n[1])-1);
			if (bkg_n[1]!=0) max_n[1]=pow(sig_n[1],2)/bkg_n[1];
		//	if (bkg_n[1]!=0) max_n[1]=pow(sig_n[1],2)/(bkg_n[1]+bkg_n_tth[1]);
			start_n[2]=start_n[1]+precision;
			do{
				max_n[2]=0;
				if (NCAT<=2) {
					sig_n[2] = 0;
					bkg_n[2] = 1; 
					bkg_n_tth[2] = 1;
					bkg_sideband_n[2] = 1; 
				} else {
					sig_n[2] = hist_S->Integral(hist_S->FindBin(start_n[1]),hist_S->FindBin(start_n[2])-1);
					bkg_n[2] = hist_B->Integral(hist_B->FindBin(start_n[1]),hist_B->FindBin(start_n[2])-1);
					bkg_n_tth[2] = hist_B_cut_tth->Integral(hist_B_cut_tth->FindBin(start_n[1]),hist_B_cut_tth->FindBin(start_n[2])-1);
					bkg_sideband_n[2] = hist_B_sideband->Integral(hist_B_sideband->FindBin(start_n[1]),hist_B_sideband->FindBin(start_n[2])-1);
				}
				if (bkg_n[2]!=0) max_n[2]=pow(sig_n[2],2)/bkg_n[2];
		//		if (bkg_n[2]!=0) max_n[2]=pow(sig_n[2],2)/(bkg_n[2]+bkg_n_tth[2]);
				start_n[3]=start_n[2]+precision;
				do{
					max_n[3]=0;
					if (NCAT<=3) {
						sig_n[3] = 0;
						bkg_n[3] = 1; 
						bkg_n_tth[3] = 1;
						bkg_sideband_n[3] = 1; 
					} else {
						sig_n[3] = hist_S->Integral(hist_S->FindBin(start_n[2]),hist_S->FindBin(start_n[3])-1);
						bkg_n[3] = hist_B->Integral(hist_B->FindBin(start_n[2]),hist_B->FindBin(start_n[3])-1);
						bkg_n_tth[3] = hist_B_cut_tth->Integral(hist_B_cut_tth->FindBin(start_n[2]),hist_B_cut_tth->FindBin(start_n[3])-1);
						bkg_sideband_n[3] = hist_B_sideband->Integral(hist_B_sideband->FindBin(start_n[2]),hist_B_sideband->FindBin(start_n[3])-1);
					}
					if (bkg_n[3]!=0) max_n[3]=pow(sig_n[3],2)/bkg_n[3];
				//	if (bkg_n[3]!=0) max_n[3]=pow(sig_n[3],2)/(bkg_n[3]+bkg_n_tth[3]);
					max_n[4]=0;
               if (NCAT<=4) {
               	sig_n[4] = 0.;
                  bkg_n[4] = 1.;
						bkg_n_tth[4] = 1;
                  bkg_sideband_n[4] = 1.;
               } else {
						sig_n[4] = hist_S->Integral(hist_S->FindBin(start_n[3]),hist_S->GetNbinsX()+1);
						bkg_n[4] = hist_B->Integral(hist_B->FindBin(start_n[3]),hist_B->GetNbinsX()+1);
						bkg_n_tth[4] = hist_B_cut_tth->Integral(hist_B_cut_tth->FindBin(start_n[3]),hist_B_cut_tth->GetNbinsX()+1);
						bkg_sideband_n[4] = hist_B_sideband->Integral(hist_B_sideband->FindBin(start_n[3]),hist_B_sideband->GetNbinsX()+1);
               }
					if (bkg_n[4]!=0) max_n[4]=pow(sig_n[4],2)/bkg_n[4];
				//	if (bkg_n[4]!=0) max_n[4]=pow(sig_n[4],2)/(bkg_n[4]+bkg_n_tth[4]);

					double max_sum = 0;
					int minevt_cond = 0; //condition is false
					for (int index=1;index<NCAT;index++){ //start from 1 for tth
						max_sum+=max_n[index];
						minevt_cond_n[index] = (bkg_sideband_n[index]>minevents);
					}
					minevt_cond = std::accumulate(minevt_cond_n+1, minevt_cond_n + NCAT, 0); // +1 for tth
					if (((max_sum)>=max) && (minevt_cond==(NCAT-1))) { //NCAT-1 for tth
						max = max_sum;
						for (int index=0;index<NCAT;index++){
							borders[index+1] = start_n[index]; //first and last are START and END 
							max_final[index] = max_n[index]; 
							bkg_yields[index] = bkg_n[index]; 
							bkg_yields_sideband[index] = bkg_sideband_n[index]; 
							sig_yields[index] = sig_n[index];
							max_total = max_sum;
						} 
					}
					start_n[3]+=precision;
				} while (start_n[3]<=(END-(NCAT-4)*precision));
				start_n[2]+=precision;
			} while (start_n[2]<=(END-(NCAT-3)*precision));
			start_n[1]+=precision;
		} while (start_n[1]<=(END-(NCAT-2)*precision));
		start_n[0]+=precision;
	} while (start_n[0]<=(END-(NCAT-1)*precision));

	borders[NCAT] = END;

	
	ofstream out;
	out.open(s.Format("output_txt/%s/%s.txt",date.Data(),outname.Data()));
	out<<"subcategory : "<<subcategory<<endl;
	out<<"S2/B over all bins, sqrt : "<<max_all<<"  , "<<sqrt(max_all)<<endl;
	out<<endl;
	out<<"S**2/B total over the chosen categories : "<<max_total<<"  ,S/sqrt(B) =  "<<sqrt(max_total)<<endl;
	out<<endl;
	out<<"borders of categories : ";
	for (int index=0;index<NCAT+1;index++)
		out<<borders[index]<<"\t";
	out<<endl;
	out<<endl;
	out<<"S**2/B in each category : ";
	for (int index=0;index<NCAT;index++)
		out<<max_final[index]<<"\t";
	out<<endl;
	out<<endl;
	out<<"sqrt(S**2/B) in each category : ";
	for (int index=0;index<NCAT;index++)
		out<<sqrt(max_final[index])<<"\t";
	out<<endl;
	out<<endl;
	out<<"Mgg sidebands bkg yields in categories : ";
	for (int index=0;index<NCAT;index++)
		out<<bkg_yields_sideband[index]<<"\t";
	out<<endl;
	out<<"bkg yields in categories : ";
	for (int index=0;index<NCAT;index++)
		out<<bkg_yields[index]<<"\t";
	out<<endl;
	out<<"sig yields in categories : ";
	for (int index=0;index<NCAT;index++)
		out<<sig_yields[index]<<"\t";
	out.close();

  string line;
  ifstream outfile(s.Format("output_txt/%s/%s.txt",date.Data(),outname.Data()));
  if (outfile.is_open()){
    while ( getline (outfile,line) )
      cout << line << '\n';
    outfile.close();
  }



	float ymin=hist_S2->GetBinContent(hist_S2->FindFirstBinAbove(0.))*0.1;
	float ymax=hist_B2->GetMaximum()*1e02;

	TLine* lines[10];
	for (int index=0;index<NCAT-1;index++){
		lines[index] = new TLine(borders[index+1],ymin,borders[index+1],hist_B2->GetBinContent(hist_B2->FindBin(borders[index+1])));
		lines[index]->SetLineStyle(9);
		lines[index]->SetLineColor(1);
		lines[index]->SetLineWidth(2);
	}

	TCanvas *c1 = new TCanvas("Fit","",800,800);
	c1->SetLogy();
	c1->cd();
	TH1F *frame2 = new TH1F("frame2","",50,xmin,xmax);
	frame2->GetXaxis()->SetNdivisions(505);
//	frame2->GetYaxis()->SetRangeUser(80,150);
   frame2->SetStats(0);
	frame2->SetYTitle("Events");
	frame2->SetXTitle(s.Format("%s",what_to_opt.Data()));	
	frame2->SetMinimum(ymin);
	frame2->SetMaximum(ymax);
	frame2->Draw();

	hist_B2->Draw("HISTsame");
	hist_S2->Draw("HISTsame");
	hist_B_cut_tth->Draw("HISTsame");

	gPad->Update();
	pCMS1.Draw("same");
	pCMS2.Draw("same");
	pCMS12.Draw("same");
	pave22.Draw("same");
	pave33.Draw("same");
	leg->Draw("same");
	for (int index=0;index<NCAT-1;index++)
		lines[index]->Draw("same");
	gPad->RedrawAxis();
	c1->Print(s.Format("plots/%s/%s.png",date.Data(),outname.Data()));
	c1->Print(s.Format("plots/%s/%s.pdf",date.Data(),outname.Data()));



	double* cat_scan = &categories_scans0[0];
	double* sign_scan = &significance_scans0[0];
	int counter = significance_scans0.size();
	TGraph *gr =new TGraph(counter,cat_scan,sign_scan);
	ymin = *std::max_element(sign_scan,sign_scan+counter) * 0.5;
	ymax = *std::max_element(sign_scan,sign_scan+counter) * 1.1;
	int max_pos = std::distance(sign_scan, std::max_element(sign_scan,sign_scan+counter));

	TCanvas *c2 = new TCanvas("B","",800,800);
	c2->cd();
	TH1F *frame3 = new TH1F("frame3","",50,xmin,xmax);
	frame3->GetXaxis()->SetNdivisions(505);
   frame3->SetStats(0);
	frame3->SetYTitle("S/#sqrt{B_{#gamma#gamma}+B_{ttH}}");
//	frame3->SetYTitle("S/#sqrt{B}");
	frame3->SetXTitle(s.Format("%s",what_to_opt.Data()));	
	frame3->SetMinimum(ymin);
	frame3->SetMaximum(ymax);
	frame3->Draw();
	gr->Draw("Psame");
	gPad->Update();
	pCMS1.Draw("same");
	pCMS2.Draw("same");
	pCMS12.Draw("same");
	pave22.Draw("same");
	pave33.Draw("same");
	gPad->RedrawAxis();
	c2->Print(s.Format("plots/%s/significance_%s.png",date.Data(),outname.Data()));
	cout<<counter<<endl; 

return 0;

}


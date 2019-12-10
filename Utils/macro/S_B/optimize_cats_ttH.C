
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

using namespace std;

//g++ optimize_cats.C -g -o opt `root-config --cflags --glibs` -lMLP -lXMLIO

float calc_Z0(float S, float B)
{
  //return pow(S,2)/B;
  return 2*((S+B)*log(1+S/B)-S);
}


int main(int argc, char* argv[]){
	gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
	
	TString s; 
	TString date = "20191126";
	TString path=s.Format("/afs/cern.ch/work/n/nchernya/ETH/DiHiggs/optimization_files/%s/",date.Data());
   date = "20191210";	
//	const int NCAT=2; //for tth killer
	const int NCAT=4; //for MVA and MX
/*
	TString what_to_opt = "MVAOutputTransformed";
	double xmin = 0.;
	double xmax = 1.;
	Double_t precision=0.01;  //0.01 for MVA, 5 for MX
*/

	TString what_to_opt = "MX";
	double xmin = 250;
	double xmax = 800;
	Double_t precision=5;  //0.01 for MVA, 5 for MX


/*	TString what_to_opt = "ttHScore";
	double xmin = 0.;
	double xmax = 1.;
	Double_t precision=0.02;  //0.01 for MVA, 5 for MX
*/

	bool consider_ttH = false;
	bool consider_tt = false;
	TString Mgg_window = "*((Mgg>115)&&(Mgg<135))";
	if (consider_ttH)  Mgg_window = "*((Mgg>122)&&(Mgg<128))"; // this narrow window only for ttH
	TString Mgg_sideband = "*((Mgg<=115)||(Mgg>=135))";
	double tth_cut = 0.26;
	TString tth_cut_s;
	tth_cut_s.Form("*(ttHScore>%.3f)",tth_cut);
	TString selection_sig = "weight*lumi*eventTrainedOn*2*0.587*normalization/SumWeight";   ///0.587fb expected limit for sideband run II, divide by 3 if using mix of SM,3 and box. In addition multiply by *2 because we only optimize ont he same events we train (half of all) so we need to scale the cross section accordingly
	TString selection_bg = "weight*lumi*overlapSave*normalization/SumWeight";
	TString selection_diphoton = "*1.5"; //SF needed to match data normalization

	TString outstr = "test_bkg";
	TString subcategory = "";
   if (consider_ttH) outstr += "tth";
   if ((consider_ttH) && (consider_tt)) outstr += "tth_tt";
	double minevents = 45; //for bkg  # for MVA : 70 data in sidebands after tth killer -> 70/1.5 -> before tth killer *1.2 = 56,  because still need to be able to split in MX*

//	subcategory = "*((MVAOutputTransformed>0.7)&&(MVAOutputTransformed<1.))";
//	subcategory = "*((MVAOutputTransformed>0.54)&&(MVAOutputTransformed<.7))";
	subcategory = "*((MVAOutputTransformed>0.32)&&(MVAOutputTransformed<.54))";
	outstr += "_MVA2";
	minevents = 8; //for bkg  # for MVA :100,  because still need to be able to split in MX


	selection_sig += subcategory;
	selection_bg += subcategory;


	TString sel; 
	TString outname = s.Format("output_SB_%s_cat%d_minevents%.0f_%s",what_to_opt.Data(),NCAT,minevents,outstr.Data());


	TFile *file_s =  TFile::Open(path+"Total_runII_20191126.root");
	TTree *tree_sig = (TTree*)file_s->Get("reducedTree_sig");
	TH1F *hist_S = new TH1F("hist_S","hist_S",int((xmax-xmin)/precision),xmin,xmax);
   s.Form("%s>>hist_S",what_to_opt.Data());
   sel.Form("%s",(selection_sig+Mgg_window+tth_cut_s).Data());
	tree_sig->Draw(s,sel,"goff");

	TFile *file_bg =  TFile::Open(path+"Total_runII_20191126.root");
	TTree *tree_bg = (TTree*)file_bg->Get("reducedTree");
	TTree *tree_bg_ttH = (TTree*)file_bg->Get("reducedTree_bkg_ttH");
	TTree *tree_bg_TTGJets = (TTree*)file_bg->Get("reducedTree_bkg_TTGJets");
	TTree *tree_bg_TTTo2L2Nu = (TTree*)file_bg->Get("reducedTree_bkg_TTTo2L2Nu");
	TTree *tree_bg_TTGG_0Jets = (TTree*)file_bg->Get("reducedTree_bkg_TTGG_0Jets");

	TH1F *hist_B = new TH1F("hist_B","hist_B",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B",what_to_opt.Data());
   sel.Form("%s",(selection_bg+selection_diphoton+Mgg_window+tth_cut_s).Data());
	tree_bg->Draw(s,sel,"goff");

	TH1F *hist_B_ttH = new TH1F("hist_B_ttH","hist_B_ttH",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B_ttH",what_to_opt.Data());
   sel.Form("%s",(selection_bg+Mgg_window+tth_cut_s).Data());
	tree_bg_ttH->Draw(s,sel,"goff");
	TH1F *hist_B_TTGJets = new TH1F("hist_B_TTGJets","hist_B_TTGJets",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B_TTGJets",what_to_opt.Data());
   sel.Form("%s",(selection_bg+Mgg_window+tth_cut_s).Data());
	tree_bg_TTGJets->Draw(s,sel,"goff");
	TH1F *hist_B_TTTo2L2Nu = new TH1F("hist_B_TTTo2L2Nu","hist_B_TTTo2L2Nu",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B_TTTo2L2Nu",what_to_opt.Data());
   sel.Form("%s",(selection_bg+Mgg_window+tth_cut_s).Data());
	tree_bg_TTTo2L2Nu->Draw(s,sel,"goff");
	TH1F *hist_B_TTGG_0Jets = new TH1F("hist_B_TTGG_0Jets","hist_B_TTGG_0Jets",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B_TTGG_0Jets",what_to_opt.Data());
   sel.Form("%s",(selection_bg+Mgg_window+tth_cut_s).Data());
	tree_bg_TTGG_0Jets->Draw(s,sel,"goff");

	

	double END = hist_B->GetBinCenter(hist_B->FindLastBinAbove(0.))+hist_B->GetBinWidth(1)/2.; //right end of BDT distibution
	double START = hist_B->GetBinCenter(hist_B->FindFirstBinAbove(0.))-hist_B->GetBinWidth(1)/2.; //right end of BDT distibution
	cout<<"start = "<<START<<" , end = "<<END<<endl;


	hist_S->SetLineColor(kRed);
	hist_S->SetFillColor(kRed-7);
	hist_S->SetLineWidth(2);
	hist_B->SetLineColor(kBlue+1);
	hist_B->SetFillColor(kBlue-10);
	hist_B->SetLineWidth(2);
	hist_B_ttH->SetLineColor(kGreen+1);
	hist_B_ttH->SetLineWidth(2);
	hist_B_TTGJets->SetLineColor(kOrange+1);
	hist_B_TTGJets->SetLineWidth(2);
	hist_B_TTTo2L2Nu->SetLineColor(kMagenta+1);
	hist_B_TTTo2L2Nu->SetLineWidth(2);
	hist_B_TTGG_0Jets->SetLineColor(kViolet+1);
	hist_B_TTGG_0Jets->SetLineWidth(2);


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
	leg->AddEntry(hist_S,"Sig (exp. exclusion)","F");
	leg->AddEntry(hist_B,"BG","F");
   if (consider_ttH) {
		leg->AddEntry(hist_B_ttH,"ttH","L");
	}
   if (consider_tt) {
		leg->AddEntry(hist_B_TTGJets,"tt#gamma+jets","L");
		leg->AddEntry(hist_B_TTTo2L2Nu,"tt#rightarrow2l2#nu","L");
		leg->AddEntry(hist_B_TTGG_0Jets,"tt#gamma#gamma","L");
	}


	double bin=0.;
	double s1=0; double b1=0;
	int i=0;
	float max_all=0;	
		do	{
			s1=hist_S->GetBinContent(i+1);
			b1=hist_B->GetBinContent(i+1);
			if (consider_ttH) {
      	   b1+=hist_B_ttH->GetBinContent(i+1);
			}
			if (consider_tt) {
        		b1+=hist_B_TTGJets->GetBinContent(i+1);
        		b1+=hist_B_TTTo2L2Nu->GetBinContent(i+1);
         	b1+=hist_B_TTGG_0Jets->GetBinContent(i+1);
			}
			bin=(double) hist_S->GetBinCenter(i+1+1);
			//if ((b1)!=0) max_all += pow(s1,2)/(b1);
			if ((b1)!=0) max_all += calc_Z0(s1,b1);
			i++;
		} while (bin < END);


std::vector<std::vector<double>> categories_scans0_vec;
std::vector<std::vector<double>> categories_scans1_vec;
std::vector<std::vector<double>> categories_scans2_vec;
std::vector<std::vector<double>> categories_scans3_vec;
std::vector<std::vector<double>> categories_scans4_vec;
std::vector<std::vector<double>> significance_scans0_vec;
std::vector<std::vector<double>> significance_scans1_vec;
std::vector<std::vector<double>> significance_scans2_vec;
std::vector<std::vector<double>> significance_scans3_vec;
std::vector<std::vector<double>> significance_scans4_vec;

double max = 0;
double borders[10] = {};   // including START and END
borders[0] = START;
double sig_n[10] = {0,0,0,0,0,0,0,0,0,0};
double bkg_n[10] = {0,0,0,0,0,0,0,0,0,0};
double bkg_n_tth[10] = {0,0,0,0,0,0,0,0,0,0};
double bkg_sideband_n[10] = {0,0,0,0,0,0,0,0,0,0};
double max_n[10] = {0,0,0,0,0,0,0,0,0,0};
double significance = 0.;
double max_final[10] = {0,0,0,0,0,0,0,0,0,0};
double max_final_tth[10] = {0,0,0,0,0,0,0,0,0,0};
double max_total = 0;
double tth_cut_final = 0;
double tth_cut_idx_final = 0;
double start_n[10] = {0,0,0,0,0,0,0,0,0,0};
double bkg_yields[10] = {0,0,0,0,0,0,0,0,0,0};
double bkg_yields_sideband[10] = {0,0,0,0,0,0,0,0,0,0};
double sig_yields[10] = {0,0,0,0,0,0,0,0,0,0};
for (int index=0;index<NCAT;index++)
	start_n[index]=START+(index+1)*precision; 
int minevt_cond_n[10] = {};

double tth_max = 0.;
double tth_inc = 0.02;
if (consider_ttH) {
	tth_cut = 0.1;
	tth_max = 0.4;
	tth_inc = 0.02;
}
int tth_cut_idx = 0;
std::vector<double> categories_scans_tth;
std::vector<double> significance_scans_tth;
do {
	cout<<"Doing tth cut : "<<tth_cut<<endl;
	categories_scans_tth.push_back(tth_cut);

	for (int index=0;index<NCAT;index++)
		start_n[index]=START+(index+1)*precision; 

	std::vector<double> categories_scans0;
	std::vector<double> categories_scans1;
	std::vector<double> categories_scans2;
	std::vector<double> categories_scans3;
	std::vector<double> categories_scans4;
	std::vector<double> significance_scans0;
	std::vector<double> significance_scans1;
	std::vector<double> significance_scans2;
	std::vector<double> significance_scans3;
	std::vector<double> significance_scans4;
	

	tth_cut_s.Form("*(ttHScore>%.3f)",tth_cut);

	TH1F *hist_S_cut = new TH1F("hist_S_cut","hist_S_cut",int((xmax-xmin)/precision),xmin,xmax);
   s.Form("%s>>hist_S_cut",what_to_opt.Data());
   sel.Form("%s",(selection_sig+Mgg_window+tth_cut_s).Data());
	tree_sig->Draw(s,sel,"goff");

	TH1F *hist_B_cut = new TH1F("hist_B_cut","hist_B_cut",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B_cut",what_to_opt.Data());
   sel.Form("%s",(selection_bg+selection_diphoton+Mgg_window+tth_cut_s).Data());
	tree_bg->Draw(s,sel,"goff");
   s.Form("%s>>+hist_B_cut",what_to_opt.Data());
   sel.Form("%s",(selection_bg+Mgg_window+tth_cut_s).Data());
	if (consider_ttH) {
		tree_bg_ttH->Draw(s,sel,"goff");
	}
	if (consider_tt) {
		tree_bg_TTGJets->Draw(s,sel,"goff");
		tree_bg_TTTo2L2Nu->Draw(s,sel,"goff");
		tree_bg_TTGG_0Jets->Draw(s,sel,"goff");
	}

	TH1F *hist_B_cut_sideband = new TH1F("hist_B_cut_sideband","hist_B_cut_sideband",int((xmax-xmin)/precision),xmin,xmax); //200 bins
   s.Form("%s>>hist_B_cut_sideband",what_to_opt.Data());
   sel.Form("%s",(selection_bg+selection_diphoton+Mgg_sideband+tth_cut_s).Data());
	tree_bg->Draw(s,sel,"goff"); //only add diphoton to the sideband
   s.Form("%s>>+hist_B_cut_sideband",what_to_opt.Data());
   sel.Form("%s",(selection_bg+Mgg_sideband+tth_cut_s).Data());
	if (consider_ttH) {
		tree_bg_ttH->Draw(s,sel,"goff");
	}
	if (consider_tt) {
		tree_bg_TTGJets->Draw(s,sel,"goff");
		tree_bg_TTTo2L2Nu->Draw(s,sel,"goff");
		tree_bg_TTGG_0Jets->Draw(s,sel,"goff");
	}	

	do {
		max_n[0]=0;
		sig_n[0] = hist_S_cut->Integral(1,hist_S_cut->FindBin(start_n[0])-1);
		bkg_n[0] = hist_B_cut->Integral(1,hist_B_cut->FindBin(start_n[0])-1);
		bkg_sideband_n[0] = hist_B_cut_sideband->Integral(1,hist_B_cut_sideband->FindBin(start_n[0])-1);
		//if (bkg_n[0]!=0) max_n[0]=pow(sig_n[0],2)/(bkg_n[0]);
		if (bkg_n[0]!=0) max_n[0]=calc_Z0(sig_n[0],bkg_n[0]);
		start_n[1]=start_n[0]+precision;
		if (bkg_sideband_n[0]>minevents) {
			categories_scans0.push_back(start_n[0]);	
			significance_scans0.push_back(sqrt(max_n[1]));
		}
		do {
			max_n[1]=0;
			sig_n[1] = hist_S_cut->Integral(hist_S_cut->FindBin(start_n[0]),hist_S_cut->FindBin(start_n[1])-1);
			bkg_n[1] = hist_B_cut->Integral(hist_B_cut->FindBin(start_n[0]),hist_B_cut->FindBin(start_n[1])-1);
			bkg_sideband_n[1] = hist_B_cut_sideband->Integral(hist_B_cut_sideband->FindBin(start_n[0]),hist_B_cut_sideband->FindBin(start_n[1])-1);
		//	if (bkg_n[1]!=0) max_n[1]=pow(sig_n[1],2)/(bkg_n[1]);
			if (bkg_n[1]!=0) max_n[1]=calc_Z0(sig_n[1],bkg_n[1]);
			start_n[2]=start_n[1]+precision;
			if (bkg_sideband_n[1]>minevents) {
				categories_scans1.push_back(start_n[1]);	
				significance_scans1.push_back(sqrt(max_n[2]));
			}
			do{
				max_n[2]=0;
				if (NCAT<=2) {
					sig_n[2] = 0;
					bkg_n[2] = 1; 
					bkg_sideband_n[2] = 1; 
				} else {
					sig_n[2] = hist_S_cut->Integral(hist_S_cut->FindBin(start_n[1]),hist_S_cut->FindBin(start_n[2])-1);
					bkg_n[2] = hist_B_cut->Integral(hist_B_cut->FindBin(start_n[1]),hist_B_cut->FindBin(start_n[2])-1);
					bkg_sideband_n[2] = hist_B_cut_sideband->Integral(hist_B_cut_sideband->FindBin(start_n[1]),hist_B_cut_sideband->FindBin(start_n[2])-1);
				}
			//	if (bkg_n[2]!=0) max_n[2]=pow(sig_n[2],2)/(bkg_n[2]);
				if (bkg_n[2]!=0) max_n[2]=calc_Z0(sig_n[2],bkg_n[2]);
				start_n[3]=start_n[2]+precision;
				if (bkg_sideband_n[2]>minevents) {
					categories_scans2.push_back(start_n[2]);	
					significance_scans2.push_back(sqrt(max_n[3]));
				}
				do{
					max_n[3]=0;
					if (NCAT<=3) {
						sig_n[3] = 0;
						bkg_n[3] = 1; 
						bkg_sideband_n[3] = 1; 
					} else {
						sig_n[3] = hist_S_cut->Integral(hist_S_cut->FindBin(start_n[2]),hist_S_cut->FindBin(start_n[3])-1);
						bkg_n[3] = hist_B_cut->Integral(hist_B_cut->FindBin(start_n[2]),hist_B_cut->FindBin(start_n[3])-1);
						bkg_sideband_n[3] = hist_B_cut_sideband->Integral(hist_B_cut_sideband->FindBin(start_n[2]),hist_B_cut_sideband->FindBin(start_n[3])-1);
					}
					//if (bkg_n[3]!=0) max_n[3]=pow(sig_n[3],2)/(bkg_n[3]);
					if (bkg_n[3]!=0) max_n[3]=calc_Z0(sig_n[3],bkg_n[3]);
					if (bkg_sideband_n[3]>minevents) {
						categories_scans3.push_back(start_n[3]);	
						significance_scans3.push_back(sqrt(max_n[4]));
					}
					max_n[4]=0;
               if (NCAT<=4) {
               	sig_n[4] = 0.;
                  bkg_n[4] = 1.;
                  bkg_sideband_n[4] = 1.;
               } else {
						sig_n[4] = hist_S_cut->Integral(hist_S_cut->FindBin(start_n[3]),hist_S_cut->GetNbinsX()+1);
						bkg_n[4] = hist_B_cut->Integral(hist_B_cut->FindBin(start_n[3]),hist_B_cut->GetNbinsX()+1);
						bkg_sideband_n[4] = hist_B_cut_sideband->Integral(hist_B_cut_sideband->FindBin(start_n[3]),hist_B_cut_sideband->GetNbinsX()+1);
               }
				//	if (bkg_n[4]!=0) max_n[4]=pow(sig_n[4],2)/(bkg_n[4]);
					if (bkg_n[4]!=0) max_n[4]=calc_Z0(sig_n[4],bkg_n[4]);
					if (bkg_sideband_n[4]>minevents) {
						categories_scans4.push_back(start_n[4]);	
						significance_scans4.push_back(sqrt(max_n[4]));
					}

					double max_sum = 0;
					int minevt_cond = 0; //condition is false
					for (int index=0;index<NCAT;index++){
						max_sum+=max_n[index];
						minevt_cond_n[index] = (bkg_sideband_n[index]>minevents);
					}
					minevt_cond = std::accumulate(minevt_cond_n, minevt_cond_n + NCAT, 0);
					if (minevt_cond==NCAT) significance = sqrt(max_sum);
					if (((max_sum)>=max) && (minevt_cond==(NCAT))) {
						max = max_sum;
						max_total = max_sum;
						tth_cut_final = tth_cut;
						tth_cut_idx_final = tth_cut_idx;
						for (int index=0;index<NCAT;index++){
							borders[index+1] = start_n[index]; //first and last are START and END 
							max_final[index] = max_n[index]; 
							bkg_yields[index] = bkg_n[index]; 
							bkg_yields_sideband[index] = bkg_sideband_n[index]; 
							sig_yields[index] = sig_n[index];
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
	tth_cut+=0.02;
	tth_cut_idx+=1;
	std::cout<<"current max : "<<significance<<" , max so far : "<<sqrt(max_total)<<std::endl;

categories_scans0_vec.push_back(categories_scans0);
categories_scans1_vec.push_back(categories_scans1);
categories_scans2_vec.push_back(categories_scans2);
categories_scans3_vec.push_back(categories_scans3);
categories_scans4_vec.push_back(categories_scans4);
significance_scans0_vec.push_back(significance_scans0);
significance_scans1_vec.push_back(significance_scans1);
significance_scans2_vec.push_back(significance_scans2);
significance_scans3_vec.push_back(significance_scans3);
significance_scans4_vec.push_back(significance_scans4);
significance_scans_tth.push_back(significance);

} while (tth_cut<tth_max);

	borders[NCAT] = END;

	
	ofstream out;
	out<<endl;
	out.open(s.Format("output_txt/%s/%s.txt",date.Data(),outname.Data()));
	out<<"subcategory : "<<subcategory<<endl;
	out<<"S2/B over all bins, sqrt : "<<max_all<<"  , "<<sqrt(max_all)<<endl;
	out<<endl;
	out<<"sqrt(S**2/B) total over the chosen categories : "<<max_total<<"  ,S/sqrt(B) =  "<<sqrt(max_total)<<endl;
	out<<endl;
	out<<"borders of categories : ";
	for (int index=0;index<NCAT+1;index++)
		out<<borders[index]<<"\t";
	out<<endl;
	out<<endl;
	out<<"tth_cut : "<<tth_cut_final;
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


float ymin,ymax;
/////////////Plot ttH killer in each of the optimized categories////////////
TString what_to_plot = "ttHScore";
float plotmin = 0.;
float plotmax = 1.;
float plotprecision=0.02;
for (int index=0;index<NCAT;index++){
	subcategory.Form("*((%s>%.3f)&&(%s<%.3f))",what_to_opt.Data(),what_to_opt.Data(),borders[index],borders[index+1]);

	TH1F *hist_plot_S = new TH1F("hist_plot_S","hist_plot_S",int((plotmax-plotmin)/plotprecision),plotmin,plotmax);
   s.Form("%s>>hist_plot_S",what_to_plot.Data());
   sel.Form("%s",(selection_sig+subcategory+Mgg_window).Data());
	tree_sig->Draw(s,sel,"goff");

	TH1F *hist_plot_B = new TH1F("hist_plot_B","hist_plot_B",int((plotmax-plotmin)/plotprecision),plotmin,plotmax); //200 bins
   s.Form("%s>>hist_plot_B",what_to_plot.Data());
   sel.Form("%s",(selection_bg+subcategory+selection_diphoton+Mgg_window).Data());
	tree_bg->Draw(s,sel,"goff");

	TH1F *hist_plot_B_ttH = new TH1F("hist_plot_B_ttH","hist_plot_B_ttH",int((plotmax-plotmin)/plotprecision),plotmin,plotmax); //200 bins
   s.Form("%s>>hist_plot_B_ttH",what_to_plot.Data());
   sel.Form("%s",(selection_bg+subcategory+Mgg_window).Data());
	tree_bg_ttH->Draw(s,sel,"goff");
	TH1F *hist_plot_B_TTGJets = new TH1F("hist_plot_B_TTGJets","hist_plot_B_TTGJets",int((plotmax-plotmin)/plotprecision),plotmin,plotmax); //200 bins
   s.Form("%s>>hist_plot_B_TTGJets",what_to_plot.Data());
   sel.Form("%s",(selection_bg+subcategory+Mgg_window).Data());
	tree_bg_TTGJets->Draw(s,sel,"goff");
	TH1F *hist_plot_B_TTTo2L2Nu = new TH1F("hist_plot_B_TTTo2L2Nu","hist_plot_B_TTTo2L2Nu",int((plotmax-plotmin)/plotprecision),plotmin,plotmax); //200 bins
   s.Form("%s>>hist_plot_B_TTTo2L2Nu",what_to_plot.Data());
   sel.Form("%s",(selection_bg+subcategory+Mgg_window).Data());
	tree_bg_TTTo2L2Nu->Draw(s,sel,"goff");
	TH1F *hist_plot_B_TTGG_0Jets = new TH1F("hist_plot_B_TTGG_0Jets","hist_plot_B_TTGG_0Jets",int((plotmax-plotmin)/plotprecision),plotmin,plotmax); //200 bins
   s.Form("%s>>hist_plot_B_TTGG_0Jets",what_to_plot.Data());
   sel.Form("%s",(selection_bg+subcategory+Mgg_window).Data());
	tree_bg_TTGG_0Jets->Draw(s,sel,"goff");

	ymin=hist_plot_S->GetBinContent(hist_plot_S->FindFirstBinAbove(0.))*0.1;
	ymax=hist_plot_B->GetMaximum()*1e02;
	cout<<ymin<<"  "<<ymax<<endl;

	TCanvas *c10 = new TCanvas("c10","",800,800);
	c10->SetLogy();
	c10->cd();
	TH1F *frame20 = new TH1F("frame20","",50,plotmin,plotmax);
	frame20->GetXaxis()->SetNdivisions(505);
   frame20->SetStats(0);
	frame20->SetYTitle("Events");
	frame20->SetXTitle(s.Format("%s",what_to_plot.Data()));	
	frame20->SetMinimum(ymin);
	frame20->SetMaximum(ymax);
	frame20->Draw();


	hist_plot_S->SetLineColor(kRed);
	hist_plot_S->SetFillColor(kRed-7);
	hist_plot_S->SetLineWidth(2);
	hist_plot_B->SetLineColor(kBlue+1);
	hist_plot_B->SetFillColor(kBlue-10);
	hist_plot_B->SetLineWidth(2);
	hist_plot_B_ttH->SetLineColor(kGreen+1);
	hist_plot_B_ttH->SetLineWidth(2);
	hist_plot_B_TTGJets->SetLineColor(kOrange+1);
	hist_plot_B_TTGJets->SetLineWidth(2);
	hist_plot_B_TTTo2L2Nu->SetLineColor(kMagenta+1);
	hist_plot_B_TTTo2L2Nu->SetLineWidth(2);
	hist_plot_B_TTGG_0Jets->SetLineColor(kViolet+1);
	hist_plot_B_TTGG_0Jets->SetLineWidth(2);


	hist_plot_B->Draw("HISTsame");
	hist_plot_S->Draw("HISTsame");
   if (consider_ttH) {
		hist_plot_B_ttH->Draw("HISTsame");
	}
   if (consider_tt) {
		hist_plot_B_TTGJets->Draw("HISTsame");
		hist_plot_B_TTTo2L2Nu->Draw("HISTsame");
		hist_plot_B_TTGG_0Jets->Draw("HISTsame");
	}	
	gPad->Update();
	pCMS1.Draw("same");
	pCMS2.Draw("same");
	pCMS12.Draw("same");
	pave22.Draw("same");
	pave33.Draw("same");
	leg->Draw("same");
	gPad->RedrawAxis();
	sel.Form("output_plot%s_%scat%d_minevents%.0f_%s",what_to_plot.Data(),what_to_opt.Data(),index,minevents,outstr.Data());
	c10->Print(s.Format("plots/%s/%s.png",date.Data(),sel.Data()));
	c10->Print(s.Format("plots/%s/%s.pdf",date.Data(),sel.Data()));

}
/////////////////////////////////////////////////////////////////////////

	ymin=hist_S->GetBinContent(hist_S->FindFirstBinAbove(0.))*0.1;
	ymax=hist_B->GetMaximum()*1e02;

	TLine* lines[10];
	for (int index=0;index<NCAT-1;index++){
		lines[index] = new TLine(borders[index+1],ymin,borders[index+1],hist_B->GetBinContent(hist_B->FindBin(borders[index+1])));
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

	hist_B->Draw("HISTsame");
	hist_S->Draw("HISTsame");
   if (consider_ttH) {
		hist_B_ttH->Draw("HISTsame");
	}
   if (consider_tt) {
		hist_B_TTGJets->Draw("HISTsame");
		hist_B_TTTo2L2Nu->Draw("HISTsame");
		hist_B_TTGG_0Jets->Draw("HISTsame");
	}	

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

/////////////////////////////////Plot scan of significance vs point
std::vector<std::vector<double>> chosen_categories_scan;
std::vector<std::vector<double>> chosen_significance_scan;
chosen_categories_scan.push_back(categories_scans_tth);
chosen_categories_scan.push_back(categories_scans0_vec[tth_cut_idx_final]);
chosen_categories_scan.push_back(categories_scans1_vec[tth_cut_idx_final]);
chosen_categories_scan.push_back(categories_scans2_vec[tth_cut_idx_final]);
chosen_categories_scan.push_back(categories_scans3_vec[tth_cut_idx_final]);
chosen_categories_scan.push_back(categories_scans4_vec[tth_cut_idx_final]);
chosen_significance_scan.push_back(significance_scans_tth);
chosen_significance_scan.push_back(significance_scans0_vec[tth_cut_idx_final]);
chosen_significance_scan.push_back(significance_scans1_vec[tth_cut_idx_final]);
chosen_significance_scan.push_back(significance_scans2_vec[tth_cut_idx_final]);
chosen_significance_scan.push_back(significance_scans3_vec[tth_cut_idx_final]);
chosen_significance_scan.push_back(significance_scans4_vec[tth_cut_idx_final]);

//for (int index=0;index<NCAT+1;index++){ // first is ttH
for (int index=0;index<1;index++){ // first is ttH
   std::vector<double> cat_gr_vec = chosen_categories_scan[index];
   std::vector<double> sign_gr_vec = chosen_significance_scan[index];
	double* cat_scan = &cat_gr_vec[0];
	double* sign_scan = &sign_gr_vec[0];
	int counter = sign_gr_vec.size();
	TGraph *gr =new TGraph(counter,cat_scan,sign_scan);
	ymin = *std::max_element(sign_scan,sign_scan+counter) * 0.5;
	ymax = *std::max_element(sign_scan,sign_scan+counter) * 1.1;
	int max_pos = std::distance(sign_scan, std::max_element(sign_scan,sign_scan+counter));

	TCanvas *c2 = new TCanvas("B","",800,800);
	c2->cd();
	TH1F *frame3 = new TH1F("frame3","",50,xmin,xmax);
	frame3->GetXaxis()->SetNdivisions(505);
   frame3->SetStats(0);
//	frame3->SetYTitle("S/#sqrt{B_{#gamma#gamma}+B_{ttH}}");
	frame3->SetYTitle("S/#sqrt{B}");
	frame3->GetYaxis()->SetTitleOffset(1.32);
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
	c2->Print(s.Format("plots/%s/significance_CAT%d_%s.pdf",date.Data(),index,outname.Data()));
	c2->Print(s.Format("plots/%s/significance_CAT%d_%s.png",date.Data(),index,outname.Data()));
	cout<<counter<<endl; 
}

return 0;

}


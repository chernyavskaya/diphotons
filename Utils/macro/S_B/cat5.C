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

using namespace std;

//g++ cat4.C -g -o cat4 `root-config --cflags --glibs` -lMLP -lXMLIO

int main(int argc, char* argv[]){

	TString path="/afs/cern.ch/work/n/nchernya/ETH/DiHiggs/optimization_files/";

	gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
	double NCAT=5;
	double max1=0; double max2=0; double max3=0; double max4=0;double max5=0;
	double max1_final=0; double max2_final=0; double max3_final=0; double max4_final=0;double max5_final=0;
	double s1=0; double s2=0;double s3=0;  double s4=0;double s5=0;
	double b1=0; double b2=0;double b3=0;  double b4=0; double b5=0;
	double max = -10;
	double border1=0;
	double border2=0;
	double border3=0;
	double border4=0;
	double border5=0;
	

	
	TString ending = "";

//	TString what_to_opt = "MVAOutputTransformed";
//	double xmin = 0.;
//	double xmax = 1.;
//	Double_t precision=0.01;  //0.01 for MVA, 5 for MX

	TString what_to_opt = "MX";
	double xmin = 250;
	double xmax = 800;
	Double_t precision=5;  //0.01 for MVA, 5 for MX
	TString binning;
	binning.Form("%0.3f",precision);

	TString selection_sig = "weight*137.2/3*eventTrainedOn*0.587*((Mgg>115)&&(Mgg<135))";   ///0.587fb expected limit for full run II
	TString selection_bg = "weight*137.2*overlapSave*((Mgg>115)&&(Mgg<135))";
//	TString subcategory = "";
//	TString outstr = "";
//	double minevents = 100; //for bkg  # for MVA :100,  because still need to be able to split in MX
	TString subcategory = "*((MVAOutputTransformed>0.6)&&(MVAOutputTransformed<2))";
	TString outstr = "_MVA0";
	double minevents = 4; //for bkg  # for MVA :100,  because still need to be able to split in MX
	selection_sig += subcategory;
	selection_bg += subcategory;

	TFile *file_s =  TFile::Open(path+"Total_runII_sig_mix.root");
	TTree *tree_sig = (TTree*)file_s->Get("reducedTree_sig");
	TH1F *hist_S = new TH1F("hist_S","hist_S",200,xmin,xmax);
   TString s; s.Form("%s>>hist_S",what_to_opt.Data());
   TString sel; sel.Form("%s",selection_sig.Data());
	tree_sig->Draw(s,sel,"goff");

	TFile *file_bg =  TFile::Open(path+"Total_runII_sig_mix.root");
	TTree *tree_bg = (TTree*)file_bg->Get("reducedTree");
	TH1F *hist_B = new TH1F("hist_B","hist_B",200,xmin,xmax);
   s.Form("%s>>hist_B",what_to_opt.Data());
   sel.Form("%s",selection_bg.Data());
	tree_bg->Draw(s,sel,"goff");
	

	double END = hist_B->GetBinCenter(hist_B->FindLastBinAbove(0.)); //right end of BDT distibution
	double START = hist_B->GetBinCenter(hist_B->FindFirstBinAbove(0.)); //right end of BDT distibution
	cout<<"start = "<<START<<" , end = "<<END<<endl;
	double start1=START+precision;
	double start2=START+2*precision;
	double start3=START+3*precision;
	double start4=START+4*precision;


	hist_S->SetLineColor(kRed);
	hist_S->SetFillColor(kRed-7);
	hist_S->SetLineWidth(2);
	hist_B->SetLineColor(kBlue+1);
	hist_B->SetFillColor(kBlue-10);
	hist_B->SetLineWidth(2);
	TH1F *hist_B2 = (TH1F*)hist_B->Clone("b_new");
	hist_B2->Rebin(4);
	TH1F *hist_S2 = (TH1F*)hist_S->Clone("s_new");
	hist_S2->Rebin(4);
//	hist_S2->Scale();


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


		


	int num_of_bins = hist_S->GetNbinsX();

	int i=0; int j=0; int k=0; int l=0; int m=0; int p=0; int r=0;
	double bin=0.;

	float max_all=0;	
		do	{
			s1=hist_S->GetBinContent(i+1);
			b1=hist_B->GetBinContent(i+1);
			bin=(double) hist_S->GetBinCenter(i+1+1);
			if ((b1)!=0) max_all += pow(s1,2)/(b1);
			i++;
		} while (bin < END);


	cout<<max_all<<endl;
	cout<<sqrt(max_all)<<endl;
 


max=0;


	do {
		i=0;
		max1=0;
		s1=0;
		b1=0;
		do	{
			s1+=hist_S->GetBinContent(i+1);
			b1+=hist_B->GetBinContent(i+1);
			bin=(double) hist_S->GetBinCenter(i+1+1);
			i++;
		} while (bin < start1);
		if (b1!=0)max1=pow(s1,2)/(b1);
	//	cout<<max1<<endl;
		start2=start1+precision;
		do {
			j=i;
			max2=0;
			s2=0;
			b2=0;
			do	{
				s2+=hist_S->GetBinContent(j+1);
				b2+=hist_B->GetBinContent(j+1);
				bin=(double) hist_S->GetBinCenter(j+1+1);
				j++;
			} while (bin < start2);
			if ((b2)!=0)max2=pow(s2,2)/(b2);
			start3=start2+precision;
		//	cout<<max2<<endl;
			do {
				m=j;
				max3=0;
				s3=0;
				b3=0;
				do	{
					s3+=hist_S->GetBinContent(m+1);
					b3+=hist_B->GetBinContent(m+1);
					bin=(double) hist_S->GetBinCenter(m+1+1);
					m++;
				} while (bin < start3);
				if ((b3)!=0)max3=pow(s3,2)/(b3);
				start4=start3+precision;
				do{
					r=m;
					max4=0;
					s4=0;
					b4=0;
					do	{
						s4+=hist_S->GetBinContent(r+1);
						b4+=hist_B->GetBinContent(r+1);
						bin=(double) hist_S->GetBinCenter(r+1+1);
						r++;
					} while (bin < start4);
					if ((b4)!=0)	max4=pow(s4,2)/(b4);
				//	cout<<max4<<endl;
						l=r;
						max5=0;
						s5=0;
						b5=0.;
						do  {
							s5+=hist_S->GetBinContent(l+1);
							b5+=hist_B->GetBinContent(l+1);
							bin=(double) hist_S->GetBinCenter(l+1+1);
							l++;
						} while (bin < END);
						if ((b5)!=0 )max5=pow(s5,2)/(b5);
						if (((max1+max2+max3+max4+max5)>=max) && (b1>minevents) && (b2>minevents)  && (b3>minevents) && (b4>minevents) &(b5>minevents) ) {
							max = max1+max2+max3+max4+max5;
							border1=start1;
							border2=start2;
							border3=start3;
							border4=start4;
							max1_final=max1;
							max2_final=max2;
							max3_final=max3;
							max4_final=max4;
							max5_final=max5;
						}
					start4+=precision;
					} while (start4<=(END-(NCAT-4)*precision));
					start3+=precision;
				} while (start3<=(END-(NCAT-3)*precision));
				start2+=precision;
			} while (start2<=(END-(NCAT-2)*precision));
			start1+=precision;
		} while (start1<=(END-(NCAT-1)*precision));
	
	ofstream out;
	out.open(s.Format("output_txt/%s/output_SB_%s_cat4%s_minevents%.0f.txt",date.Data(),what_to_opt.Data(),outstr.Data(),minevents));
	out<<"subcategory : "<<subcategory<<endl;
	out<<"max all, sqrt : "<<max_all<<"  , "<<sqrt(max_all)<<endl;
	out<<"borders of categories : "<<START<<"   "<<border1<<"   "<<border2<<"   "<<border3<<"" << border4<<" , END = "<< END <<endl;
	out<<"S**2/B in each category : "<<max1_final<<"   "<<max2_final<<"   " << max3_final<<"  "<<max4_final<<"  "<<max5_final<< "    , max = "<<max<<endl;
	out<<"sqrt(S**2/B) in each category : "<<sqrt(max1_final)<<"   "<<sqrt(max2_final)<<"   " << sqrt(max3_final)<<"   " << sqrt(max4_final)<< "   "<<sqrt(max5_final)  <<"    , max = "<<sqrt(max)<<endl;
	cout<<"max all, sqrt : "<<max_all<<"  , "<<sqrt(max_all)<<endl;
	cout<<"borders of categories : "<<START<<"   "<<border1<<"   "<<border2<<"   "<<border3<<"  "<< border4<<" , END = "<< END <<endl;
	cout<<"S**2/B in each category : "<<max1_final<<"   "<<max2_final<<"   " << max3_final<<"  "<<max4_final<<"   "<<max5_final << "    , max = "<<max<<endl;
	cout<<"sqrt(S**2/B) in each category : "<<sqrt(max1_final)<<"   "<<sqrt(max2_final)<<"   " << sqrt(max3_final)<<"   " << sqrt(max4_final)<<"   "<<sqrt(max5_final) <<"    , max = "<<sqrt(max)<<endl;
	out.close();




	float ymin=hist_S2->GetBinContent(hist_S2->FindFirstBinAbove(0.))*0.1;
	float ymax=hist_B2->GetMaximum()*1e02;

	TLine line1(border1,ymin,border1,hist_B2->GetBinContent(hist_B2->FindBin(border1)));
	line1.SetLineStyle(9);
	line1.SetLineColor(1);
	line1.SetLineWidth(2);
	TLine line2(border2,ymin,border2,hist_B2->GetBinContent(hist_B2->FindBin(border2)));
	line2.SetLineStyle(9);
	line2.SetLineColor(1);
	line2.SetLineWidth(2);
	TLine line3(border3,ymin,border3,hist_B2->GetBinContent(hist_B2->FindBin(border3)));
	line3.SetLineStyle(9);
	line3.SetLineColor(1);
	line3.SetLineWidth(2);
	TLine line4(border4,ymin,border4,hist_B2->GetBinContent(hist_B2->FindBin(border4)));
	line4.SetLineStyle(9);
	line4.SetLineColor(1);
	line4.SetLineWidth(2);


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

	gPad->Update();
	pCMS1.Draw("same");
	pCMS2.Draw("same");
	pCMS12.Draw("same");
	pave22.Draw("same");
	pave33.Draw("same");
	leg->Draw("same");
	line1.Draw("same");
	line2.Draw("same");
	line3.Draw("same");
	line4.Draw("same");
	gPad->RedrawAxis();
	c1->Print(s.Format("plots/%s/output_SB_%s_cat4%s_minevents%.0f.png",date.Data(),what_to_opt.Data(),outstr.Data(),minevents));



return 0;

}


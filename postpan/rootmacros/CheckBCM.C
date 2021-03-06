//CheckBCM.C
// - Macro for WAC checking BCM signal from event and mul tree 
// author: Tao Ye
// date: 06-2019

#include "device_list.h"

void CheckBCM(){
  gStyle->SetOptStat(1);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.4);

  Int_t nbcm = vBCM.size();
  TTree *evt_tree = (TTree*)gROOT->FindObject("evt");
  TTree *mul_tree = (TTree*)gROOT->FindObject("mul");
  
  TCanvas *c1 = new TCanvas("c1","c1",2400,600);
  c1->Divide(4,1);
  
  TH1D *h_buff;
  TH2D *h2d_buff;
  TVirtualPad* pad_buff;
  TGraph* g_buff;
  for(int ibcm=0;ibcm<nbcm;ibcm++){
    const char* device_name = vBCM[ibcm];
    
    pad_buff = c1->cd(1);
    evt_tree->Draw(Form("%s:Entry$",device_name),"","l");
    g_buff = (TGraph*)pad_buff->FindObject("Graph");
    g_buff->SetName("GraphAll");
    evt_tree->Draw(Form("%s:Entry$",device_name),"ErrorFlag!=0","* same");
    g_buff = (TGraph*)pad_buff->FindObject("Graph");
    if(g_buff!=0)
      g_buff->SetMarkerColor(kRed);

    //===================================================

    pad_buff=c1->cd(2);
    evt_tree->Draw(Form("%s",device_name),"ErrorFlag==0","");
    h_buff=(TH1D*)pad_buff->FindObject("htemp");
    if(h_buff!=NULL)
      h_buff->SetName("evtTree");
    
    evt_tree->Draw(Form("%s",device_name),
		   Form("ErrorFlag==0 && %s.Device_Error_Code!=0",device_name),
		   "same");
    h_buff=(TH1D*)pad_buff->FindObject("htemp");
    if(h_buff!=0)
      h_buff->SetLineColor(kRed);

    //===================================================

    pad_buff=c1->cd(3);
    mul_tree->Draw(Form("asym_%s/ppm:pattern_number",
			device_name),
		   "ErrorFlag==0","COL");
    TH2F* h2d_buff = (TH2F*)pad_buff->FindObject("htemp");
    if (h2d_buff!=NULL)
      h2d_buff->Draw("candlex3");

    //===================================================

    pad_buff=c1->cd(4);
    TH1D *hAq = new TH1D("hAq","",100,-1500,1500);
    mul_tree->Draw(Form("asym_%s/ppm>>hAq",device_name),
		   "ErrorFlag==0");
    hAq->SetTitle(Form("asym_%s;ppm",device_name));
    pad_buff->Update();
    TPaveStats *st = (TPaveStats*)hAq->FindObject("stats");
    st->SetOptStat(111110);

    if(hAq!=0){
      mul_tree->Draw(Form("asym_%s/ppm",device_name),
		     Form("ErrorFlag==0 && asym_%s.Device_Error_Code!=0",
			  device_name),"same");
      h_buff = (TH1D*)pad_buff->FindObject("htemp");
      if(h_buff!=0)
	h_buff->SetLineColor(kRed);
    }
    
    plot_title = Form("run%s_%s.png",
		      run_seg.Data(),
		      device_name);

    TText *t1 = new TText(0.0,0.007,plot_title);
    t1->SetNDC();
    t1->SetTextSize(0.05);
    c1->cd();
    t1->Draw("same");
    c1->SaveAs(output_path+plot_title);
    c1->Clear("D");

    delete hAq;
  } // end of BCM loop 
  delete c1;
}

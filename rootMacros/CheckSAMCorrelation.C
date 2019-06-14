// CheckSAMCorrelation.C : 
//	ROOT script for checking SAM correlation
//
// author : Tao Ye <tao.ye@stonybrook.edu>
// 	 05-2019

#include "device_list.h"

void CheckSAMCorrelation(){
  
  TString treeName = "mul";
  TString draw_opts[] = {"COLZ","fit","scat"};
  Int_t nsam = vSAM.size();
  Int_t nbpm = vDitherBPM.size();
  Int_t nDV[2]={nsam,nsam};
  Int_t nIV[2]={nsam,nbpm};
  vector<const char* > vtag_dv ={"asym_sam","asym_sam"};
  vector<const char*> vtag_iv ={"asym_sam","diff_bpm" };

  vector<int> switch_key = {2,1};

  Int_t canvas_width = 600;
  TCanvas* c_this = new TCanvas("","",800,800);

  for(int iplot=0;iplot<2;iplot++){
    for(int iopt=0;iopt<3;iopt++){
      int ny = nDV[iplot];
      int nx = nIV[iplot];
      c_this->SetCanvasSize((nx+1)*canvas_width,ny*canvas_width);
      c_this->cd();
      PlotCorrelation(switch_key[iplot],treeName,draw_opts[iopt]);
      plot_title  = Form("run%d_%s_vs_%s-%s.png",
			 run_number,vtag_dv[iplot],vtag_iv[iplot],
			 draw_opts[iopt].Data());
      TText *label = new TText(0.0,0.005,plot_title);     
      label->SetTextFont(23);
      label->SetTextSize(70);
      label->SetNDC();
      c_this->cd();
      label->Draw("same");
      c_this->SaveAs(output_path+plot_title);
      c_this->Clear("D");
    } // end of draw_opts loop
    
  } // end of plots loop

}
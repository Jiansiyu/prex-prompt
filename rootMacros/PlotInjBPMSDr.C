// Injector BPMs Plot Macro 
// Source from Caryn Palatchi
// Adapted by Tao Ye for JAPAN 
// Last Update : 05-2019

#include "device_list.h"

void PlotInjBPMSDr(vector<const char*> &vBPM, 
		   Int_t IHWPstatus, TString ucut); // generic
void PlotInjBPMSDr(Int_t IHWPstatus, TString ucut); // interface to panguin
void PlotInjBPMSDr(); // interface to summary plots

void PlotInjBPMSDr(){
  TCanvas *c1 = new TCanvas("c1","c1",3600,2400);
  c1->cd();
  PlotInjBPMSDr(vInjBPM,1,"ErrorFlag==0");  
  plot_title = Form("run%s_injector_BPM_dr.png",run_seg.Data());
  c1->SaveAs(output_path+plot_title);
}

void PlotInjBPMSDr(Int_t IHWPstatus, TString ucut){
  PlotInjBPMSDr(vInjBPM,IHWPstatus,ucut);
}
void PlotInjBPMSDr(vector<const char*> &vBPM, 
		   Int_t IHWPstatus, TString ucut){

  //open first run file
  TTree* evt_tree = (TTree*)gROOT->FindObject("evt");
  TTree* pair_tree = (TTree*)gROOT->FindObject("pr");
  TTree* mul_tree = (TTree*)gROOT->FindObject("mul");

  Int_t nBPM = vBPM.size();
  const Int_t ndeti = nBPM;
  Double_t Dx[ndeti];
  Double_t Dy[ndeti];
  Double_t eDx[ndeti];
  Double_t eDy[ndeti];
  Double_t theta[ndeti];
  Double_t Dr[ndeti];
  Double_t eDr[ndeti];
  Double_t zero[ndeti];

  TPad *pad1= new TPad("pad1","pad1",0,0,1,1);
  pad1->Divide(1,2);
  pad1->Draw();

  TString objnam;
  TH1D* thishist;
  for(Int_t dd=0;dd<ndeti;dd++)
    {
      mul_tree->Draw(Form("diff_%sY/um>>histx%d",vBPM[dd],dd),ucut,"goff");
      objnam = Form("histx%d",dd);
      thishist = (TH1D*)gDirectory->FindObject(objnam);
      Dx[dd] = thishist->GetMean();
      eDx[dd]= thishist->GetMeanError();
      delete thishist;
      
      mul_tree->Draw(Form("diff_%sX/um>>histy%d",vBPM[dd],dd),ucut,"goff");
      objnam = Form("histy%d",dd);
      thishist = (TH1D*)gDirectory->FindObject(objnam);
      Dy[dd] = thishist->GetMean();
      eDy[dd]= thishist->GetMeanError();
      delete thishist;
      
      Dr[dd]=sqrt(pow(Dx[dd],2)+pow(Dy[dd],2));
      eDr[dd]=(0.5/sqrt(pow(Dx[dd],2)+pow(Dy[dd],2)))*sqrt(pow(2*Dx[dd]*eDx[dd],2)+pow(2*Dy[dd]*eDy[dd],2));
      theta[dd]=(180/3.14159)*atan(Dy[dd]/Dx[dd]);
      //      cout<<vBPM[dd]<<" Aq:"<<Aq[dd]<<"("<<rmsAq[dd]<<")+-"<<eAq[dd]<<"ppm Dx:"<<Dx[dd]<<"("<<rmsDx[dd]<<")+-"<<eDx[dd]<<"um Dy:"<<Dy[dd]<<"("<<rmsDy[dd]<<")+-"<<eDy[dd]<<"um "<<endl;
      // cout<<vBPM[dd]<<" Aq[ppm](RMS)+-error,Dx[nm](RMS)+-err,Dy[nm](RMS)+-err,Aelli(RMS*10^3)+-error,Dr[nm]+-err,theta[deg]: "<<Aq[dd]<<" "<<rmsAq[dd]<<" "<<eAq[dd]<<" "<<1e3*Dx[dd]<<" "<<1e3*rmsDx[dd]<<" "<<1e3*eDx[dd]<<" "<<1e3*Dy[dd]<<" "<<1e3*rmsDy[dd]<<" "<<1e3*eDy[dd]<<" "<<Aelli[dd]<<" "<<rmsAelli[dd]<<" "<<eAelli[dd]<<" "<<1e3*Dr[dd]<<" "<<1e3*eDr[dd]<<" "<<theta[dd]<<endl;
    }

 
  const Int_t numchains=ndeti;
  Double_t chain[numchains];
  Double_t chainerr[numchains];
  for (Int_t i=0;i<numchains;i++){
    chain[i]=i+1;
    chainerr[i]=0.0;
  };

  TGraphErrors *Drgraph;
  TGraphErrors *thetagraph;

  Drgraph = new TGraphErrors(numchains,chain,&(Dr[0]),chainerr,&(eDr[0]));
  thetagraph= new TGraphErrors(numchains,chain,&(theta[0]),chainerr,&(zero[0]));

  Double_t x1 = chain[0]-0.5;
  Double_t x2 = chain[numchains-1]+0.5;
 
  pad1->cd(1);

  Drgraph->GetHistogram()->GetXaxis()->Set(numchains,x1,x2);
  for(Int_t k=0;k<numchains;k++){
    Drgraph->GetHistogram()->GetXaxis()->SetBinLabel(k+1,vBPM[k]);
  }
  Drgraph->GetHistogram()->GetXaxis()->SetLabelSize(0.065);
  Drgraph->SetMarkerStyle(21);
  Drgraph->SetMarkerSize(3);
  Drgraph->SetMarkerColor(2);
  Drgraph->SetLineColor(2);
  Drgraph->Draw("AP");
  Drgraph->SetTitle(Form("IHWP=%d, %s ",IHWPstatus,  ucut.Data()));
  Drgraph->GetYaxis()->SetTitle("um, red=Dr=sqrt(Dx^2+Dy^2)");
  Drgraph->GetYaxis()->SetTitleOffset(0.0);
  Drgraph->GetYaxis()->SetTitleSize(0.05);
  Drgraph->SetMinimum(0.01);
  Drgraph->SetMaximum(2);
  pad1->cd(1)->SetGrid();

  pad1->cd(2);
  thetagraph->GetHistogram()->GetXaxis()->Set(numchains,x1,x2);
  for(Int_t k=0;k<numchains;k++){
    thetagraph->GetHistogram()->GetXaxis()->SetBinLabel(k+1,vBPM[k]);
  }
  thetagraph->GetHistogram()->GetXaxis()->SetLabelSize(0.065);
  thetagraph->SetMarkerStyle(21);
  thetagraph->SetMarkerSize(3);
  thetagraph->SetMarkerColor(2);
  thetagraph->SetLineColor(2);
  thetagraph->Draw("AP");
  thetagraph->SetTitle(Form("IHWP=%d, %s ",IHWPstatus,ucut.Data()));
  thetagraph->GetYaxis()->SetTitle("theta(deg) = atan(Dy/Dx)");
  thetagraph->GetYaxis()->SetTitleOffset(0.0);
  thetagraph->GetYaxis()->SetTitleSize(0.05);
  pad1->cd(2)->SetGrid();

}


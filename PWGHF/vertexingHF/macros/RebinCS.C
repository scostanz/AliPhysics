#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TStyle.h"
#endif


const Int_t maxPtBins=5;
Double_t coarsebins[maxPtBins+1]={1.,2.,4.,6.,8.,12.};
TH1D* MakeRatio(TH1D* hNum, TH1D* hDen);

void RebinCS() {

  TString filenameFine = "HFPtSpectrum_3SigPID_Pt400_all.root";
  TString filenameCoarse = "Cristina_HFPtSpectrum_D0_multInt_MB_final.root";
  Int_t mesCode=0;
  Int_t nPtBins=5;

  TFile* filF=new TFile(filenameFine.Data());
  TH1D* hFine=(TH1D*)filF->Get("histoSigmaCorr");
  TH1D* hReb=new TH1D("histoSigmaCorrRebinned","",nPtBins,coarsebins);

  hFine->SetStats(0);
  gStyle->SetOptStat(0);
  gStyle->SetTitleOffset(1.5,"Y");

  for(Int_t iB=1; iB<=hReb->GetNbinsX(); iB++){
    Double_t ptRebC=hReb->GetBinCenter(iB);
    Double_t ptRebL=hReb->GetBinLowEdge(iB);
    Double_t ptRebH=hReb->GetBinLowEdge(iB)+hReb->GetBinWidth(iB);
    Double_t sum=0;
    Double_t sume2=0;

    Double_t nRebs=0;
    for(Int_t iBf=3; iBf<=hFine->GetNbinsX(); iBf++){
      Double_t ptFineC=hFine->GetBinCenter(iBf);
      Double_t ptFineL=hFine->GetBinLowEdge(iBf);
      Double_t ptFineH=hFine->GetBinLowEdge(iBf)+hFine->GetBinWidth(iBf);
      Double_t bwFine=hFine->GetBinWidth(iBf);
      if(ptFineL>=ptRebL && ptFineH<=ptRebH){
	sum+=hFine->GetBinContent(iBf)*bwFine;
  cout<< " sum " << sum <<endl;
	sume2+=hFine->GetBinError(iBf)*hFine->GetBinError(iBf)*bwFine*bwFine;
	nRebs++;
	printf("Add bin %d (%.1f-%.1f) to coarse bin %d (%.1f-%.1f)\n",iBf,ptFineL,ptFineH,iB,ptRebL,ptRebH);
      }
    }
    sum/=hReb->GetBinWidth(iB);
    Double_t esum=TMath::Sqrt(sume2)/hReb->GetBinWidth(iB);
    hReb->SetBinContent(iB,sum);
    hReb->SetBinError(iB,esum);
  }

  TFile* filC=new TFile(filenameCoarse.Data());
  TH1D* hCoarse=(TH1D*)filC->Get("histoSigmaCorr");
  hCoarse->SetStats(0);
  hCoarse->SetLineColor(kBlue);
  hCoarse->SetMarkerColor(kBlue);
  hCoarse->SetMarkerStyle(20);
  hCoarse->SetName("standard");
  TH1D* hRatioRebinned=MakeRatio(hReb,hCoarse);
  hRatioRebinned->SetTitle("");
  hRatioRebinned->GetYaxis()->SetTitle("Ratio Xsec low_pt-rebinned/standard analysis");
  hRatioRebinned->GetYaxis()->SetTitleOffset(1.5);
  hRatioRebinned->SetMarkerStyle(20);
  hRatioRebinned->SetMarkerColor(kGreen);
  hRatioRebinned->SetLineColor(kGreen);
  hRatioRebinned->SetName("low_pt-rebinned/standard");
  hRatioRebinned->GetYaxis()->SetRangeUser(0.5,1.5);

  TCanvas * ccheck=new TCanvas("ccheck","",1200,800);
  ccheck->Divide(2,1);
  ccheck->cd(1);
  hReb->SetLineColor(kRed);
  hReb->SetMarkerColor(kRed);
  hReb->SetMarkerStyle(3);
  hReb->SetName("low-pt rebinned");
  hReb->Draw();
  hCoarse->Draw("same");
  gPad->BuildLegend();
  ccheck->cd(2);
  hRatioRebinned->Draw();
  gPad->BuildLegend();
  TLine *l = new TLine(1,1,12,1);
  l->SetLineStyle(2);
  l->Draw("same");

  TString outfilnam=filenameFine;
  outfilnam.ReplaceAll(".root","-Rebinned.root");
  TFile *filout=new TFile(outfilnam.Data(),"recreate");
  hReb->Write();
  hRatioRebinned->Write();
  filout->Close();
}


TH1D* MakeRatio(TH1D* hNum, TH1D* hDen){
  TH1D* hRat=0x0;
  if(hDen->GetNbinsX()<hNum->GetNbinsX()) hRat=(TH1D*)hDen->Clone(Form("hRatio%s",hNum->GetName()));
  else hRat=(TH1D*)hNum->Clone(Form("hRatio%s",hNum->GetName()));
  hRat->Reset("ICESM");
  hRat->SetMarkerStyle(hNum->GetMarkerStyle());
  hRat->SetMarkerColor(hNum->GetMarkerColor());
  hRat->SetLineColor(hNum->GetLineColor());
  for(Int_t iB=1; iB<=hRat->GetNbinsX(); iB++){
    Double_t ptRatC=hRat->GetBinCenter(iB);
    Double_t ptRatL=hRat->GetBinLowEdge(iB);
    Double_t ptRatH=hRat->GetBinLowEdge(iB)+hRat->GetBinWidth(iB);
    Int_t iBinNum=hNum->FindBin(ptRatC+0.0001);
    Double_t ptNumC=hNum->GetBinCenter(iBinNum);
    Double_t ptNumL=hNum->GetBinLowEdge(iBinNum);
    Double_t ptNumH=hNum->GetBinLowEdge(iBinNum)+hNum->GetBinWidth(iBinNum);
    Double_t yNum=-1;
    Double_t eRelNum=-1;
    if(TMath::Abs(ptNumC-ptRatC)<0.0001 && 
       TMath::Abs(ptNumL-ptRatL)<0.0001 &&
       TMath::Abs(ptNumH-ptRatH)<0.0001){
      yNum=hNum->GetBinContent(iBinNum);
      eRelNum=hNum->GetBinError(iBinNum)/yNum;
    }else{
      Int_t iBinNumL=hNum->FindBin(ptRatL+0.0001);
      Int_t iBinNumH=hNum->FindBin(ptRatH-0.0001);
      yNum=0;
      Double_t sum2=0;
      Double_t bw=0.;
      for(Int_t k=iBinNumL; k<=iBinNumH; k++){
	bw+=hNum->GetBinWidth(k);
	yNum+=hNum->GetBinContent(k)*hNum->GetBinWidth(k);
	sum2+=hNum->GetBinError(k)*hNum->GetBinWidth(k)*hNum->GetBinError(k)*hNum->GetBinWidth(k);
      }
      yNum/=bw;
      eRelNum=TMath::Sqrt(sum2)/bw;
      eRelNum/=yNum;
    }
    Int_t iBinDen=hDen->FindBin(ptRatC+0.0001);
    Double_t ptDenC=hDen->GetBinCenter(iBinDen);
    Double_t ptDenL=hDen->GetBinLowEdge(iBinDen);
    Double_t ptDenH=hDen->GetBinLowEdge(iBinDen)+hDen->GetBinWidth(iBinDen);
    Double_t yDen=-1;
    Double_t eRelDen=-1;
    if(TMath::Abs(ptDenC-ptRatC)<0.0001 && 
       TMath::Abs(ptDenL-ptRatL)<0.0001 &&
       TMath::Abs(ptDenH-ptRatH)<0.0001){
      yDen=hDen->GetBinContent(iBinDen);
      eRelDen=hDen->GetBinError(iBinDen)/yDen;
    }else{
      Int_t iBinDenL=hDen->FindBin(ptRatL+0.0001);
      Int_t iBinDenH=hDen->FindBin(ptRatH-0.0001);
      yDen=0;
      Double_t sum2=0;
      Double_t bw=0.;
      for(Int_t k=iBinDenL; k<=iBinDenH; k++){
	bw+=hDen->GetBinWidth(k);
	yDen+=hDen->GetBinContent(k)*hDen->GetBinWidth(k);
	sum2+=hDen->GetBinError(k)*hDen->GetBinWidth(k)*hDen->GetBinError(k)*hDen->GetBinWidth(k);
      }
      yDen/=bw;
      eRelDen=TMath::Sqrt(sum2)/bw;
      eRelDen/=yDen;
    }
    printf("Bin %d   Num=%f+-%f   Den=%f+-%f\n",iB,yNum,eRelNum*yNum,yDen,eRelDen*yDen);
    if(yNum>0 && yDen>0){
      Double_t ratio=yNum/yDen;
      Double_t eratio=0.0000001;
      hRat->SetBinContent(iB,ratio);
      cout<< " set bin error al rtio " << eratio << endl;
      hRat->SetBinError(iB,eratio);
    } 
  }

  return hRat;
}

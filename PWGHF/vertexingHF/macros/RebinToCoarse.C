#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TStyle.h"
#include "AliHFSystErr.h"
#endif


enum{kDzero,kLc};

const Int_t maxPtBins=8;
Double_t coarsebins[maxPtBins+1]={1.,2.,3.,4.,5.,6.,8.,12.,24.};
TH1D* MakeRatio(TH1D* hNum, TH1D* hDen);
TGraphAsymmErrors* MakeRatio(TGraphAsymmErrors* gNum, TGraphAsymmErrors* gDen);

void RebinToCoarse(Int_t meson=kDzero){

  AliHFSystErr* systUncCoarse=new AliHFSystErr();
  systUncCoarse->SetCollisionType(0);
  systUncCoarse->SetRunNumber(17);
  systUncCoarse->SetIs5TeVAnalysis(kTRUE);
  systUncCoarse->SetStandardBins(kTRUE);

  TString filenameFine;
  TString filenameCoarse;
  Int_t mesCode=0;
  Int_t nPtBins=8;
  if(meson==kDzero){
    filenameFine="HFPtSpectrum_D0fine_final_10012019.root";
    filenameCoarse="HFPtSpectrum_D0fine_final_10012019.root";
    systUncCoarse->Init(1);
  }else if(meson==kLc){
    filenameFine="CrossSections/HFPtSpectrumDs_Nb_d0cut_strongPIDpt6_LHC18a4a2_10Jan2019.root";
    filenameCoarse="";
    systUncCoarse->SetIs5TeVAnalysis(kFALSE);
    systUncCoarse->SetRunNumber(10);
    systUncCoarse->SetIsPass4Analysis(kFALSE);
    systUncCoarse->Init(4);
    nPtBins=4;
    coarsebins[0]=2.;
    coarsebins[1]=4.;
    coarsebins[2]=6.;
    coarsebins[3]=8.;
    coarsebins[4]=12.;
  }

  TFile* filF=new TFile(filenameFine.Data());
  TH1D* hFine=(TH1D*)filF->Get("histoSigmaCorr");
  TH1D* hRawYFine=(TH1D*)filF->Get("hRECpt");
  TH1D* hReb=new TH1D("histoSigmaCorrRebinned","",nPtBins,coarsebins);
  TGraphAsymmErrors* gSystFine=(TGraphAsymmErrors*)filF->Get("gSigmaCorr");
  TGraphAsymmErrors* gSystFcFine=(TGraphAsymmErrors*)filF->Get("gFcConservative");
  AliHFSystErr* systUncFine=(AliHFSystErr*)filF->Get("AliHFSystErr");
  TGraphAsymmErrors* gSystReb=new TGraphAsymmErrors(0);
  gSystReb->SetName("gSigmaCorr");
  TGraphAsymmErrors* gFc=new TGraphAsymmErrors(0);
  gFc->SetName("gFcConservative");
  TGraphAsymmErrors* gSigmaCorrConservative=new TGraphAsymmErrors(0);
  gSigmaCorrConservative->SetName("gSigmaCorrConservative");

  hFine->SetStats(0);
  gStyle->SetOptStat(0);
  gStyle->SetTitleOffset(1.5,"Y");
  TH1D* hRelStatCS=new TH1D("hRelStatCS"," ; p_{T} ; Relative stat. unc.",nPtBins,coarsebins);
  TH1D* hRelStatRY=new TH1D("hRelStatRY"," ; p_{T} ; Relative stat. unc.",nPtBins,coarsebins);  
  TH1D* hRelSystData=new TH1D("hRelSystData"," ; p_{T} ; Relative syst. unc.",nPtBins,coarsebins);
  TH1D* hRelSystDatadw=new TH1D("hRelSystDatadw"," ; p_{T} ; Relative syst. unc.",nPtBins,coarsebins);
  TH1D* hRelSystBup=new TH1D("hRelSystBup"," ; p_{T} ; Relative syst. unc.",nPtBins,coarsebins);
  TH1D* hRelSystBdw=new TH1D("hRelSystBdw"," ; p_{T} ; Relative syst. unc.",nPtBins,coarsebins);


  for(Int_t iB=1; iB<=hReb->GetNbinsX(); iB++){
    Double_t ptRebC=hReb->GetBinCenter(iB);
    Double_t ptRebL=hReb->GetBinLowEdge(iB);
    Double_t ptRebH=hReb->GetBinLowEdge(iB)+hReb->GetBinWidth(iB);
    Double_t sum=0;
    Double_t sume2=0;
    Double_t sumSystR=0;
    Double_t sumSystC=0;
    Double_t sumSystT=0;
    Double_t sumSystP=0;
    Double_t sumSystM=0;
    Double_t sumry=0;
    Double_t sumry2=0;
    Double_t nRebs=0;
    for(Int_t iBf=1; iBf<=hFine->GetNbinsX(); iBf++){
      Double_t ptFineC=hFine->GetBinCenter(iBf);
      Double_t ptFineL=hFine->GetBinLowEdge(iBf);
      Double_t ptFineH=hFine->GetBinLowEdge(iBf)+hFine->GetBinWidth(iBf);
      Double_t bwFine=hFine->GetBinWidth(iBf);
      if(ptFineL>=ptRebL && ptFineH<=ptRebH){
	sum+=hFine->GetBinContent(iBf)*bwFine;
  cout<< " sum " << sum <<endl;
	sume2+=hFine->GetBinError(iBf)*hFine->GetBinError(iBf)*bwFine*bwFine;
	sumry+=hRawYFine->GetBinContent(iBf)*bwFine;
	sumry2+=hRawYFine->GetBinError(iBf)*hRawYFine->GetBinError(iBf)*bwFine*bwFine;
	sumSystR+=hFine->GetBinContent(iBf)*bwFine*systUncFine->GetRawYieldErr(ptFineC);
	sumSystC+=hFine->GetBinContent(iBf)*bwFine*systUncFine->GetCutsEffErr(ptFineC);
	sumSystT+=hFine->GetBinContent(iBf)*bwFine*systUncFine->GetTrackingEffErr(ptFineC);
	sumSystP+=hFine->GetBinContent(iBf)*bwFine*systUncFine->GetPIDEffErr(ptFineC);
	sumSystM+=hFine->GetBinContent(iBf)*bwFine*systUncFine->GetMCPtShapeErr(ptFineC);
	nRebs++;
	printf("Add bin %d (%.1f-%.1f) to coarse bin %d (%.1f-%.1f)\n",iBf,ptFineL,ptFineH,iB,ptRebL,ptRebH);
      }
    }
    Double_t sumpr=0;
    Double_t sumpru=0;
    Double_t sumprd=0;
    Double_t sumw=0;
      for(Int_t iPt=0; iPt<gSystFcFine->GetN(); iPt++){
	Double_t ptFineC,fpr;
	gSystFcFine->GetPoint(iPt,ptFineC,fpr);
	if(ptFineC>0 && ptFineC>=ptRebL && ptFineC<=ptRebH){
	  Int_t theBin=hRawYFine->FindBin(ptFineC);
	  Double_t ry=hRawYFine->GetBinContent(theBin);
	  sumpr+=fpr*ry;
	  sumpru+=(fpr+gSystFcFine->GetErrorYhigh(iPt))*ry;
	  sumprd+=(fpr-gSystFcFine->GetErrorYlow(iPt))*ry;
	  sumw+=ry;
	  printf("  For fprompt add bin %d at %.2f \n",iPt,ptFineC);
	}
      }
    Double_t bSystUp=0;
    Double_t bSystDw=0;
    Double_t ept=0.5*hReb->GetBinWidth(iB);
    if(sumw>0){
      sumpr/=sumw;
      sumpru/=sumw;
      sumprd/=sumw;
      bSystUp=sumpru-sumpr;
      bSystDw=sumpr-sumprd;
      gFc->SetPoint(iB-1,ptRebC,sumpr);
      gFc->SetPointError(iB-1,ept,ept,bSystDw,bSystUp);
      printf("   fprompt = %f\n",sumpr);
    }
    sum/=hReb->GetBinWidth(iB);
    
    Double_t esum=TMath::Sqrt(sume2)/hReb->GetBinWidth(iB);
    sumry/=hReb->GetBinWidth(iB);
    Double_t esumry=TMath::Sqrt(sumry2)/hReb->GetBinWidth(iB);
    printf("  Relative error on sum CS = %.3f   on sum RY = %.3f\n",esum/sum,esumry/sumry);
    hReb->SetBinContent(iB,sum);
    cout<< " set bin error al hReb esum " << esum << endl;
    hReb->SetBinError(iB,esum);
    Double_t dataSystFromCoarseBefore=systUncCoarse->GetTotalSystErr(ptRebC);
    Double_t rawSystFromFine=sumSystR/hReb->GetBinWidth(iB)/sum;
    Double_t cutSystFromFine=sumSystC/hReb->GetBinWidth(iB)/sum;
    Double_t trkSystFromFine=sumSystT/hReb->GetBinWidth(iB)/sum;
    Double_t pidSystFromFine=sumSystP/hReb->GetBinWidth(iB)/sum;
    Double_t mcpSystFromFine=sumSystM/hReb->GetBinWidth(iB)/sum;
    Double_t rawSystFromCoarse=systUncCoarse->GetRawYieldErr(ptRebC);
    Double_t cutSystFromCoarse=systUncCoarse->GetCutsEffErr(ptRebC);
    Double_t trkSystFromCoarse=systUncCoarse->GetTrackingEffErr(ptRebC);
    Double_t pidSystFromCoarse=systUncCoarse->GetPIDEffErr(ptRebC);
    Double_t mcpSystFromCoarse=systUncCoarse->GetMCPtShapeErr(ptRebC);

    printf("  --> Syst from fine   = R:%f C:%f T:%f P:%f M:%f\n",rawSystFromFine,cutSystFromFine,trkSystFromFine,pidSystFromFine,mcpSystFromFine);
    printf("  --> Syst from coarse = R:%f C:%f T:%f P:%f M:%f\n",rawSystFromCoarse,cutSystFromCoarse,trkSystFromCoarse,pidSystFromCoarse,mcpSystFromCoarse);
    if(TMath::Abs(rawSystFromFine-rawSystFromCoarse)>0.00001){
      printf("    --> RESET RAW YIELD SYST UNC\n");
      systUncCoarse->ResetRawYieldErr(ptRebC,rawSystFromFine);
    }
    if(TMath::Abs(cutSystFromFine-cutSystFromCoarse)>0.00001){
      printf("    --> RESET CUT EFFIC SYST UNC\n");
      systUncCoarse->ResetCutEfficErr(ptRebC,cutSystFromFine);
    }
    if(TMath::Abs(pidSystFromFine-pidSystFromCoarse)>0.00001){
      printf("    --> RESET PID EFFIC SYST UNC\n");
      systUncCoarse->ResetPIDEfficErr(ptRebC,pidSystFromFine);
    }
    if(TMath::Abs(mcpSystFromFine-mcpSystFromCoarse)>0.00001){
      printf("    --> RESET MC PT SHAPE SYST UNC\n");
      systUncCoarse->ResetMCPtShapeErr(ptRebC,mcpSystFromFine);
    }
    if(TMath::Abs(trkSystFromFine-trkSystFromCoarse)>0.00001){
      printf("    --> RESET TRACK EFFIC SYST UNC\n");
      systUncCoarse->ResetTrackEfficErr(ptRebC,trkSystFromFine);
    }
    Double_t dataSystFromCoarseAfter=systUncCoarse->GetTotalSystErr(ptRebC);
    printf("  ---->  OLD=%f   NEW=%f\n",dataSystFromCoarseBefore,dataSystFromCoarseAfter);
      bSystUp=bSystUp/sumpr*sum;
      bSystDw=bSystDw/sumpr*sum;
    Double_t dataSyst=dataSystFromCoarseAfter*sum;
    Double_t totystU=TMath::Sqrt(dataSyst*dataSyst+bSystUp*bSystUp);
    Double_t totystD=TMath::Sqrt(dataSyst*dataSyst+bSystDw*bSystDw);
    if(sum>0){
      gSystReb->SetPoint(iB-1,ptRebC,sum);
      gSystReb->SetPointError(iB-1,ept,ept,totystD,totystU);
      gSigmaCorrConservative->SetPoint(iB-1,ptRebC,sum);
      gSigmaCorrConservative->SetPointError(iB-1,ept,ept,bSystDw,bSystUp);
      hRelStatCS->SetBinContent(iB,esum/sum);
      hRelStatRY->SetBinContent(iB,esumry/sumry);
      hRelSystData->SetBinContent(iB,dataSyst/sum);
      hRelSystDatadw->SetBinContent(iB,-dataSyst/sum);
      hRelSystBup->SetBinContent(iB,bSystUp/sum);
      hRelSystBdw->SetBinContent(iB,-bSystDw/sum);
    }
  }

  TCanvas* ccheck=new TCanvas("ccheck","",1200,800);
  ccheck->Divide(2,2);
  ccheck->cd(1);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.08);
  gPad->SetLogy();
  hFine->SetLineColor(kMagenta);
  hFine->SetMarkerColor(kMagenta);
  hFine->SetLineWidth(2);
  hFine->SetMarkerStyle(1);
  hReb->SetLineWidth(3);
  hReb->Draw();
  hReb->GetYaxis()->SetTitle("BR d#sigma/dp_{T} (pb/GeV)");
  hReb->GetXaxis()->SetTitle("p_{T}");
  hFine->Draw("same");
  TLegend* leg=new TLegend(0.4,0.7,0.89,0.89);
  leg->AddEntry(hFine,"Analysis in fine bins","L");
  leg->AddEntry(hReb,"Rebinned","L");
  leg->Draw();
  ccheck->cd(2);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.08);
  gFc->SetLineWidth(3);
  gFc->GetYaxis()->SetTitle("f_{prompt}");
  gFc->GetXaxis()->SetTitle("p_{T}");
  gFc->Draw("PZA");
    gSystFcFine->SetLineColor(kMagenta);
    gSystFcFine->SetMarkerColor(kMagenta);
    gSystFcFine->SetLineWidth(2);
    gSystFcFine->SetMarkerStyle(1);
    gSystFcFine->Draw("SAMEPZ");
  ccheck->cd(3);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.08);
  hRelStatCS->SetLineWidth(2);
  hRelStatCS->SetMinimum(0.8*hRelStatRY->GetMinimum());
  hRelStatCS->Draw();
  hRelStatRY->SetLineStyle(2);
  hRelStatRY->SetLineWidth(2);
  hRelStatRY->Draw("same");
  TLegend* legs=new TLegend(0.2,0.7,0.6,0.89);
  legs->AddEntry(hRelStatCS,"Sum of Xsec","L");
  legs->AddEntry(hRelStatRY,"Sum of Raw yields","L");
  legs->Draw();
  ccheck->cd(4);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.08);
  hRelSystData->SetMinimum(-0.15);
  hRelSystData->SetMaximum(0.15);
  hRelSystData->SetLineWidth(2);
  hRelSystData->SetLineStyle(7);
  hRelSystData->Draw();
  hRelSystDatadw->SetLineWidth(2);
  hRelSystDatadw->SetLineStyle(7);
  hRelSystDatadw->Draw("same");
  hRelSystBup->SetLineStyle(3);
  hRelSystBup->SetLineWidth(2);
  hRelSystBup->Draw("same");
  hRelSystBdw->SetLineStyle(3);
  hRelSystBdw->SetLineWidth(2);
  hRelSystBdw->Draw("same");

  hReb->SetName("histoSigmaCorr");
  TString outfilnam=filenameFine;
  outfilnam.ReplaceAll(".root","-Rebinned.root");
  systUncCoarse->SetNameTitle("AliHFSystErr","SystErrD0toKpi2017pp5TeVFineBinsRebinned");
  TFile *filout=new TFile(outfilnam.Data(),"recreate");
  hReb->Write();
  gSystReb->Write();
  gSigmaCorrConservative->Write();
  gFc->Write();
  systUncCoarse->Write();
  filout->Close();

  // Comparison rebinned and standard
  if(filenameCoarse=="") filenameCoarse=filenameFine.Data(); // for D_s
  TFile* filC=new TFile(filenameCoarse.Data());
  TH1D* hCoarse=(TH1D*)filC->Get("histoSigmaCorr");
  TGraphAsymmErrors* gSystCoarse=(TGraphAsymmErrors*)filC->Get("gSigmaCorr");
  TGraphAsymmErrors* gSystFcCoarse=(TGraphAsymmErrors*)filC->Get("gFcConservative");
  TGraphAsymmErrors* gSystbCoarse=(TGraphAsymmErrors*)filC->Get("gSigmaCorrConservative");
  hCoarse->SetStats(0);
  hCoarse->SetLineColor(kGreen+1);
  hCoarse->SetMarkerColor(kGreen+1);
  hCoarse->SetMarkerStyle(25);
  hCoarse->SetLineWidth(2);
    gSystFcCoarse->SetLineColor(kGreen+1);
    gSystFcCoarse->SetMarkerColor(kGreen+1);
    gSystFcCoarse->SetMarkerStyle(25);
    gSystFcCoarse->SetLineWidth(2);
    gSystbCoarse->SetLineColor(kGreen+1);
    gSystbCoarse->SetMarkerColor(kGreen+1);
    gSystbCoarse->SetMarkerStyle(25);
    gSystbCoarse->SetLineWidth(2);
  TH1D* hRatioRebinned=MakeRatio(hReb,hCoarse);
  hRatioRebinned->SetTitle("");
  hRatioRebinned->GetYaxis()->SetTitle("Ratio Xsec fine-rebinned/coarse binning");
  hRatioRebinned->GetYaxis()->SetTitleOffset(1.5);
  TGraphAsymmErrors* gRatioFc=0x0;
    gRatioFc=MakeRatio(gFc,gSystFcCoarse);
    gRatioFc->GetYaxis()->SetTitle("Ratio fprompt fine-rebinned/coarse binning");
    gRatioFc->GetXaxis()->SetTitle("p_{T}");
    gRatioFc->SetTitle("");
    gFc->SetTitle("");
  TH1D* hRelStatCoarse=(TH1D*)hCoarse->Clone("hRelStatCoarse");
  hRelStatCoarse->Reset("MICES");
  for(Int_t iB=1; iB<=hCoarse->GetNbinsX(); iB++){
    Double_t xsec=hCoarse->GetBinContent(iB);
    Double_t statxsec=hCoarse->GetBinError(iB);
    hRelStatCoarse->SetBinContent(iB,statxsec/xsec);
    hRelStatCoarse->SetBinError(iB,0.);
  }
  hRelStatCoarse->SetLineColor(kGreen+1);
  hRelStatCoarse->SetMarkerColor(kGreen+1);
  hRelStatCoarse->SetMarkerStyle(25);
  hRelStatCoarse->SetLineWidth(2);

  TH1D* hRelSystCoarseUp=(TH1D*)hCoarse->Clone("hRelSystCoarseUp");
  TH1D* hRelSystCoarseDw=(TH1D*)hCoarse->Clone("hRelSystCoarseDw");
  TH1D* hRelSystRebinUp=(TH1D*)hCoarse->Clone("hRelSystCoarseUp");
  TH1D* hRelSystRebinDw=(TH1D*)hCoarse->Clone("hRelSystCoarseDw");
  hRelSystCoarseUp->Reset("MICES");
  hRelSystCoarseDw->Reset("MICES");
  hRelSystRebinUp->Reset("MICES");
  hRelSystRebinDw->Reset("MICES");
  for(Int_t iPt=0; iPt<gSystCoarse->GetN(); iPt++){
    Double_t x,y;
    gSystCoarse->GetPoint(iPt,x,y);
    Int_t jBin=hRelSystCoarseUp->FindBin(x);
    Double_t eup=gSystCoarse->GetErrorYhigh(iPt)/y;
    Double_t edw=gSystCoarse->GetErrorYlow(iPt)/y;
    cout<< " setto error al coares " << eup << endl;
    hRelSystCoarseUp->SetBinContent(jBin,eup);
    hRelSystCoarseDw->SetBinContent(jBin,-edw);
  }
  for(Int_t iPt=0; iPt<gSystReb->GetN(); iPt++){
    Double_t x,y;
    gSystReb->GetPoint(iPt,x,y);
    Int_t jBin=hRelSystRebinUp->FindBin(x);
    Double_t eup=gSystReb->GetErrorYhigh(iPt)/y;
    Double_t edw=gSystReb->GetErrorYlow(iPt)/y;
    hRelSystRebinUp->SetBinContent(jBin,eup);
    hRelSystRebinDw->SetBinContent(jBin,-edw);
  }
  hRelSystCoarseUp->SetLineColor(kGreen+1);
  hRelSystCoarseUp->SetMarkerColor(kGreen+1);
  hRelSystCoarseUp->SetMarkerStyle(25);
  hRelSystCoarseUp->SetLineWidth(2);
  hRelSystCoarseDw->SetLineColor(kGreen+1);
  hRelSystCoarseDw->SetMarkerColor(kGreen+1);
  hRelSystCoarseDw->SetMarkerStyle(25);
  hRelSystCoarseDw->SetLineWidth(2);
  hRelSystRebinUp->SetLineWidth(4);
  hRelSystRebinDw->SetLineWidth(4);
  hRelSystRebinUp->SetLineColor(1);
  hRelSystRebinDw->SetLineColor(1);
  hRelSystRebinUp->SetTitle("");
  hRelSystRebinUp->GetYaxis()->SetTitle("Relative Syst. Unc.");
  hRelSystRebinUp->GetXaxis()->SetTitle("p_{T}");
  hRelSystRebinUp->GetYaxis()->SetTitleOffset(1.5);
  hRelSystRebinUp->SetMaximum(-1.2*hRelSystRebinDw->GetMinimum());
  hRelSystRebinUp->SetMinimum(1.2*hRelSystRebinDw->GetMinimum());

  TCanvas* ccomp=new TCanvas("ccomp","",1600,800);
  ccomp->Divide(3,2);
  ccomp->cd(1);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.08);
  gPad->SetLogy();
  hReb->Draw();
  hCoarse->Draw("same");
  TLegend* leg2=new TLegend(0.4,0.7,0.89,0.89);
  leg2->AddEntry(hCoarse,"Analysis in Std bins","L");
  leg2->AddEntry(hReb,"Analysis in fine bins, rebinned","L");
  leg2->Draw();
  ccomp->cd(4);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.08);
  hRatioRebinned->SetMinimum(0.98);
  hRatioRebinned->SetMaximum(1.02);
  hRatioRebinned->Draw("same");
  ccomp->cd(2);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.08);
    gFc->Draw("APZ");
    gSystFcCoarse->Draw("PZSAME");
  ccomp->cd(5);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.08);
  if(gRatioFc) gRatioFc->Draw("APZ");
  ccomp->cd(3);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.08);
  hRelStatCS->Draw();
  hRelStatCoarse->Draw("same");
  ccomp->cd(6);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.08);
  hRelSystRebinUp->Draw();
  hRelSystRebinDw->Draw("same");
  hRelSystCoarseDw->Draw("same");
  hRelSystCoarseUp->Draw("same");

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

TGraphAsymmErrors* MakeRatio(TGraphAsymmErrors* gNum, TGraphAsymmErrors* gDen){
  TGraphAsymmErrors* gRat=new TGraphAsymmErrors(0);
  Int_t jPoints=0;
  for(Int_t iPt=0; iPt<gNum->GetN(); iPt++){
    Double_t xn,yn;
    gNum->GetPoint(iPt,xn,yn);
    Double_t ynup=yn+gNum->GetErrorYhigh(iPt);
    Double_t yndw=yn+gNum->GetErrorYlow(iPt);
    for(Int_t iPt2=0; iPt2<gDen->GetN(); iPt2++){
     Double_t xd,yd;
     gDen->GetPoint(iPt2,xd,yd);
     if(TMath::Abs(xn-xd)<0.0001){
       if(yd>0){
	   Double_t r=yn/yd;
	   Double_t ydup=yd+gDen->GetErrorYhigh(iPt2);
	   Double_t yddw=yd+gDen->GetErrorYlow(iPt2);
	   Double_t r1=ynup/ydup;
	   Double_t r2=yndw/yddw;
	   Double_t erl=0;
	   if(r1<r || r2<r) erl=r-TMath::Min(r1,r2);
	   Double_t eru=0;
	   if(r1>r || r2>r) eru=TMath::Max(r1,r2)-r;
	   printf("r=%f  %f %f (From %f %f %f   %f %f %f\n",r,r1,r2,yn,ynup,yndw,yd,ydup,yddw);
	   gRat->SetPoint(jPoints,xn,r);
	   gRat->SetPointError(jPoints,gNum->GetErrorXlow(iPt),gNum->GetErrorXhigh(iPt),erl,eru);
	   ++jPoints;
       }
       break;
     }
    }
  }
  gRat->SetMarkerStyle(gNum->GetMarkerStyle());
  gRat->SetMarkerColor(gNum->GetMarkerColor());
  gRat->SetLineColor(gNum->GetLineColor());
  gRat->SetLineWidth(2);
  return gRat;
}

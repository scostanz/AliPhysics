#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TH1F.h>
#include <TF1.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TH1D.h>
#include <TFile.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TDatabasePDG.h>
#include "AliHFInvMassFitter.h"
#include "AliHFInvMassMultiTrialFit.h"
#endif


TString fileName="outputMassFits_FixedSigmaAll_Refl_010V0M_Pt400_SPDany_3SigPIDnSigmaTuned_FidY.root";
const Int_t nRebinSteps=4;
Int_t rebinStep[nRebinSteps]={3,4,5,6};
const Int_t nMinMassSteps=6;
Double_t minMassStep[nMinMassSteps]={1.68,1.70,1.72,1.74,1.76,1.78};
const Int_t nMaxMassSteps=6;
Double_t maxMassStep[nMinMassSteps]={2.04,2.02,2.00,1.98,1.96,1.94};

const Int_t nMinMassStepsSB=4;
Double_t minMassStepSB[nMinMassStepsSB]={1.72,1.74,1.76,1.78};
const Int_t nMaxMassStepsSB=4;
Double_t maxMassStepSB[nMinMassStepsSB]={2.02,2.00,1.98,1.96};

const Int_t nStepsBC=11;
Double_t nSigmasBC[nStepsBC]={2.0,2.5,3.0,3.5,4.0,
			      4.5,5.0,5.5,6.0,6.5,
			      7.0};

// const Int_t nRebinSteps=1;
// Int_t rebinStep[nRebinSteps]={5};
// const Int_t nMinMassSteps=1;
// Double_t minMassStep[nMinMassSteps]={1.70};
// const Int_t nMaxMassSteps=1;
// Double_t maxMassStep[nMinMassSteps]={2.06};

//Double_t sigmas[6]={0.0079,0.0084,0.0092,0.0101,0.0113,0.0129};

Double_t sigmas[20]={0.0076,0.0079,0.0082,0.0086,0.0090,
		     0.0094,0.0098,0.0103,0.0110,0.0118,
		     0.0125,0.0131,0.0138,0.0150,0.0150,
		     0.0150,0.0150,0.0150,0.0150,0.0150};


void RawYieldMultiTrials(Int_t iPtBin=0, TString bkg="ME", Int_t optOutRoot=1);

void DoAll(Int_t iPtBin=0){
  //  RawYieldMultiTrials(iPtBin,"Rot",1);
  //  RawYieldMultiTrials(iPtBin,"LS",2);
  RawYieldMultiTrials(iPtBin,"ME",1);
  //  RawYieldMultiTrials(iPtBin,"SB",2);
}
  

void RawYieldMultiTrials(Int_t iPtBin, TString bkg, Int_t optOutRoot){
 

  TString outFileBase=fileName.Data();
  outFileBase.ReplaceAll("outputMassFits","outputRawSyst");
  outFileBase.ReplaceAll(".root","");


  TFile* fil=new TFile(fileName.Data());
  TH1F* hSigmaMC=(TH1F*)fil->Get("hSigmaMC");
  TH1F* hGausSigmaRot=(TH1F*)fil->Get("hGausSigmaRot");
  printf("%f %f\n",hSigmaMC->GetBinContent(1),hGausSigmaRot->GetBinContent(1));
  if(TMath::Abs(hSigmaMC->GetBinContent(1)>0.001)){
    printf("Gaussian sigmas from root file: ");
    for(Int_t j=0; j<hSigmaMC->GetNbinsX(); j++){ 
      sigmas[j]=hSigmaMC->GetBinContent(j+1);
      printf(" %f",sigmas[j]);
    }
    printf("\n");
  }else{
    printf("Sigma values not found\n");
    return;
  }


  Double_t massD=TDatabasePDG::Instance()->GetParticle(421)->Mass();
  Double_t sigmaToFix=sigmas[iPtBin];

  AliHFInvMassMultiTrialFit* mt=new AliHFInvMassMultiTrialFit();
  mt->SetSuffixForHistoNames(bkg.Data());
  mt->SetMass(massD);
  mt->SetUpperMassToFix(1.868);
  mt->SetUseFixSigFixMeanDown(kFALSE);
  mt->SetUseFreeSigFixMeanDown(kFALSE);
  mt->SetSigmaGaussMC(sigmaToFix);
  mt->SetUseChi2Fit();
  mt->SetSigmaMCVariation(0.1);
  //  mt->SetUsePol3Background(0);

  TH1F* hReflTempl=(TH1F*)fil->Get(Form("hMCReflPtBin%d",iPtBin));
  TH1F* hSigTempl=(TH1F*)fil->Get(Form("hMCSigPtBin%d",iPtBin));

  TH1F* hRebin=(TH1F*)fil->Get("hRebin");
  Int_t defReb=TMath::Nint(hRebin->GetBinContent(iPtBin+1));
  Int_t delta=1;
  Int_t minReb=defReb;
  if(nRebinSteps>1 && nRebinSteps%2==1) minReb=defReb-delta*(nRebinSteps-1)/2;
  else if(nRebinSteps>2 && nRebinSteps%2==0) minReb=defReb-delta*(nRebinSteps-2)/2;
  if(defReb>5) minReb-=delta;
  printf("Default rebin = %d  variations =",defReb);
  for(Int_t jr=0; jr<nRebinSteps; jr++){
    rebinStep[jr]=minReb+delta*jr;
    printf(" %d",rebinStep[jr]);
  }
  printf("\n");

  TH1D* hToFit=0x0;
  if(bkg=="Rot" || bkg=="LS" || bkg=="ME"){
    hToFit=(TH1D*)fil->Get(Form("hMassSub%s_bin%d",bkg.Data(),iPtBin)); 
    mt->SetUseExpoBackground(kTRUE);
    mt->SetUseLinBackground(0);
    mt->SetUsePol2Background(kTRUE);
    mt->SetUsePol3Background(kTRUE);
    mt->SetUsePol4Background(kTRUE);
    mt->SetUsePol5Background(0);
    mt->ConfigureRebinSteps(nRebinSteps,rebinStep);
    mt->ConfigureLowLimFitSteps(nMinMassSteps,minMassStep);
    mt->ConfigureUpLimFitSteps(nMaxMassSteps,maxMassStep);
    if(hReflTempl && hSigTempl) mt->SetTemplatesForReflections(hReflTempl,hSigTempl);
  }else{
    hToFit=(TH1D*)fil->Get(Form("hMassPtBin%d",iPtBin));
    mt->SetUseExpoBackground(0);
    mt->SetUseLinBackground(0);
    mt->SetUsePol2Background(kTRUE);
    mt->SetUsePol3Background(kTRUE);
    mt->SetUsePol4Background(kTRUE);
    mt->SetUsePol5Background(kTRUE);
    mt->ConfigureRebinSteps(nRebinSteps,rebinStep);
    mt->ConfigureLowLimFitSteps(nMinMassStepsSB,minMassStepSB);
    mt->ConfigureUpLimFitSteps(nMaxMassStepsSB,maxMassStepSB);
    if(hReflTempl && hSigTempl) mt->SetTemplatesForReflections(hReflTempl,hSigTempl);
  }
  mt->SetDrawIndividualFits(kFALSE);

  //  TCanvas* c0=new TCanvas(Form("c0%s",bkg.Data()),Form("MassFit%s",bkg.Data()));
  Bool_t isOK=mt->DoMultiTrials(hToFit,0x0);

  //  TCanvas* cry=new TCanvas(Form("cry%s",bkg.Data()),Form("All%s",bkg.Data()),1200,800);
  if(isOK){  
    //    mt->DrawHistos(cry);
    if(optOutRoot>0){
      TString option="recreate";
      if(optOutRoot==2) option="update";
      TString outfilnam=Form("%s_PtBin%d.root",outFileBase.Data(),iPtBin);
      mt->SaveToRoot(outfilnam.Data(),option);
    }
  }
}

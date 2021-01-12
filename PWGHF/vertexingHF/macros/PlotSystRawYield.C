Double_t chi2Cut=2.;
Double_t sigCut=0.2; // relative difference from default value
Double_t meanCut=0.025;
Int_t defaultbkg=2;
Int_t defaultsig=1;
Int_t defaultmean=0;
Int_t defaultrebin=4;
Double_t defaultmin=1.68;
Double_t defaultmax=1.98;
Bool_t shiftMean=kTRUE;
Bool_t pdgMean=kFALSE;

TString suffix="3SigPID_Pt400_YFid_PileUpMV";
TString dir="/home/alidock/alice/D0_13TeV_lowpt/results/figures/all/templ/V0100range";

const Int_t nRebinSteps=4;
Int_t rebinStep[nRebinSteps]={3,4,5,6};
const Int_t nMinMassSteps=6;
Double_t minMassStep[nMinMassSteps]={1.68,1.70,1.72,1.74,1.76,1.78};
const Int_t nMaxMassSteps=6;
Double_t maxMassStep[nMinMassSteps]={2.10,2.07,2.05,2.02,2.00,1.98};

void SetBackgroundBinLabels(TH2F* h){
  h->GetXaxis()->SetBinLabel(1,"Expo");
  h->GetXaxis()->SetBinLabel(2,"Pol1");
  h->GetXaxis()->SetBinLabel(3,"Pol2");
  h->GetXaxis()->SetBinLabel(4,"Pol3");
  h->GetXaxis()->SetBinLabel(5,"Pol4");
  h->GetXaxis()->SetBinLabel(6,"Pol5");
}

void SetFitConfBinLabels(TH2F* h){
  h->GetXaxis()->SetBinLabel(1,"FreeSigma");
  h->GetXaxis()->SetBinLabel(2,"FixedSigma");
  h->GetXaxis()->SetBinLabel(3,"FixedSigmaUp");
  h->GetXaxis()->SetBinLabel(4,"FixedSigmaDown");
  h->GetXaxis()->SetBinLabel(5,"FixedMean");
  h->GetXaxis()->SetBinLabel(6,"FixedMeanSigma");
}

Bool_t IsCentralCase(Int_t bkgfunc, Int_t confsig, Int_t confmean){
  if(bkgfunc!=defaultbkg) return kFALSE;
  if(defaultsig==0 && confsig!=0) return kFALSE;
  if(defaultsig>0 && confsig!=1) return kFALSE;
  if(confmean!=defaultmean) return kFALSE;
  return kTRUE;
}

Bool_t UseForSyst(Int_t bkgfunc, Int_t confsig, Int_t confmean, Int_t rebin, Double_t minf, Double_t maxf, Bool_t isSB, Int_t iPtBin){

//   // Pb-Pb 2018 config
//   if(iPtBin>0){
//     if(confsig==0) return kFALSE;
//     if(!shiftMean && confmean>1) return kFALSE;
//     if(!pdgMean && confmean==1) return kFALSE;
//     if(confsig>1 && confmean>0) return kFALSE;
//     if(bkgfunc != defaultbkg && confmean==1) return kFALSE;
//     if(bkgfunc != defaultbkg && confmean==2) return kFALSE;
//     if(bkgfunc != defaultbkg && confsig!=1) return kFALSE;
//     if(defaultbkg==3 && (bkgfunc !=3 && bkgfunc !=4))  return kFALSE;
//     if(defaultbkg==2 && (bkgfunc !=3 && bkgfunc !=2))  return kFALSE;
//   }else{
//     if(confsig==0) return kFALSE;
//     if(confmean==0 || confmean==3) return kFALSE;
//     if(bkgfunc != defaultbkg && confsig!=1) return kFALSE;
//     if(defaultbkg==3 && (bkgfunc !=3 && bkgfunc !=4))  return kFALSE;
//     if(defaultbkg==2 && (bkgfunc !=3 && bkgfunc !=2))  return kFALSE;    
//   }
  // p-Pb 5 TeV config
  // if(isSB){
  //   if(bkgfunc!=4 && bkgfunc!=3) return kFALSE;
  // }else{
  //   if(bkgfunc!=2 && bkgfunc!=3) return kFALSE;
  // }
  // //  if(rebin!=defaultrebin) return kFALSE;
  // //  if(TMath::Abs(maxf-defaultmax)>0.02) return kFALSE;
  // // p-Pb preliminary config
  // if(confsig==0) return kFALSE;
  // if(confsig>1 && confmean==1) return kFALSE;
  // if(bkgfunc != defaultbkg && confmean==1) return kFALSE;
  // if(bkgfunc != defaultbkg && confsig!=1) return kFALSE;

  // pp 5 TeV config
  if(confsig==3) return kFALSE;
  if(confsig>=1 && confmean==1) return kFALSE;
  if(bkgfunc != defaultbkg && confmean==1) return kFALSE;
  if(iPtBin>=4 && iPtBin<7 && confsig==1) return kFALSE;
  
  return kTRUE;
}

void PlotSystRawYield(Int_t iPtBin=0, TString bkgTreat="Rot"){

  TString sigString="FreeSigma";
  if(defaultsig==1) sigString="FixedSigmaAll";
  if(defaultsig==2) sigString="FixedSigmaAll105";
  TFile* filraw=new TFile(Form("%s/outputMassFits_%s_Refl_%s.root",dir.Data(),sigString.Data(),suffix.Data()));
  TH1F* hRawYieldCentral=(TH1F*)filraw->Get(Form("hRawYield%s",bkgTreat.Data()));
  TH1F* hRebin=(TH1F*)filraw->Get("hRebin");
  if(hRebin) defaultrebin=hRebin->GetBinContent(iPtBin+1);
  TH1F* hBkgFunc=(TH1F*)filraw->Get("hBkgFitFunc");
  Bool_t isSB=kFALSE;
  if(bkgTreat=="SB"){
    hRawYieldCentral=(TH1F*)filraw->Get(Form("hRawYield%sfit",bkgTreat.Data()));
    hBkgFunc=(TH1F*)filraw->Get("hBkgFitFuncSB");
    if(iPtBin<2) defaultbkg=4;
    isSB=kTRUE;
  }
  TH1F* hGausSig=(TH1F*)filraw->Get(Form("hGausSigma%s",bkgTreat.Data()));
  Double_t gausSig=hGausSig->GetBinContent(iPtBin+1);
  Double_t centralYield=hRawYieldCentral->GetBinContent(iPtBin+1);
  if(hBkgFunc) defaultbkg=hBkgFunc->GetBinContent(iPtBin+1);
  if(defaultbkg>6) defaultbkg-=6;
  Double_t ptmin=hRawYieldCentral->GetXaxis()->GetBinLowEdge(iPtBin+1);
  Double_t ptmax=hRawYieldCentral->GetXaxis()->GetBinUpEdge(iPtBin+1);
  TString ptrange=Form("%.1f<p_{T}<%.1f GeV/c",ptmin,ptmax);
  printf("==== Central Case ====\n");
  printf(" Rebin = %d\n",defaultrebin);
  printf(" BkgFunc = %d\n",defaultbkg);
  printf(" GaussSigma = %f\n",gausSig);
  printf(" RawYield = %f\n",centralYield);

  Int_t delta=1;
  Int_t minReb=defaultrebin;
  if(nRebinSteps>1 && nRebinSteps%2==1) minReb=defaultrebin-delta*(nRebinSteps-1)/2;
  else if(nRebinSteps>2 && nRebinSteps%2==0) minReb=defaultrebin-delta*(nRebinSteps-2)/2;
  if(defaultrebin>5) minReb-=delta;
  for(Int_t jr=0; jr<nRebinSteps; jr++) rebinStep[jr]=minReb+delta*jr;

  TString nameFilSy=Form("%s/outputRawSyst_FixedSigmaAll_Refl_%s_PtBin%d.root",dir.Data(),suffix.Data(),iPtBin);
  TFile* filsy=new TFile(nameFilSy.Data());
  
  TNtuple * nt=(TNtuple*)filsy->Get(Form("ntuMultiTrial%s",bkgTreat.Data()));
  Float_t rebin,minfit,maxfit,bkgfunc,confsig,confmean,chi2,mean,emean,sigma,esigma,rawy,erawy;
  nt->SetBranchAddress("rebin",&rebin);
  nt->SetBranchAddress("minfit",&minfit);
  nt->SetBranchAddress("maxfit",&maxfit);
  nt->SetBranchAddress("bkgfunc",&bkgfunc);
  nt->SetBranchAddress("confsig",&confsig);
  nt->SetBranchAddress("confmean",&confmean);
  nt->SetBranchAddress("chi2",&chi2);
  nt->SetBranchAddress("mean",&mean);
  nt->SetBranchAddress("emean",&emean);
  nt->SetBranchAddress("sigma",&sigma);
  nt->SetBranchAddress("esigma",&esigma);
  nt->SetBranchAddress("rawy",&rawy);
  nt->SetBranchAddress("erawy",&erawy);

  Double_t axRebinLimits[nRebinSteps+1];
  axRebinLimits[0]=rebinStep[0]-0.5*(rebinStep[1]-rebinStep[0]);
  for(Int_t j=0; j<nRebinSteps-1; j++){
    axRebinLimits[j+1]=0.5*(rebinStep[j+1]+rebinStep[j]);
  }
  axRebinLimits[nRebinSteps]=rebinStep[nRebinSteps-1]+0.5*(rebinStep[nRebinSteps-1]-rebinStep[nRebinSteps-2]);

  Double_t axMinMassLimits[nMinMassSteps+1];
  axMinMassLimits[0]=minMassStep[0]-0.5*(minMassStep[1]-minMassStep[0]);
  for(Int_t j=0; j<nMinMassSteps-1; j++){
    axMinMassLimits[j+1]=0.5*(minMassStep[j+1]+minMassStep[j]);
  }
  axMinMassLimits[nMinMassSteps]=minMassStep[nMinMassSteps-1]+0.5*(minMassStep[nMinMassSteps-1]-minMassStep[nMinMassSteps-2]);

  Double_t axMaxMassLimits[nMaxMassSteps+1];
  axMaxMassLimits[0]=maxMassStep[nMaxMassSteps-1]-0.5*(maxMassStep[nMaxMassSteps-2]-maxMassStep[nMaxMassSteps-1]);
  for(Int_t j=0; j<nMinMassSteps-1; j++){
    axMaxMassLimits[j+1]=0.5*(maxMassStep[nMaxMassSteps-j-1]+maxMassStep[nMaxMassSteps-j-2]);
  }
  axMaxMassLimits[nMaxMassSteps]=maxMassStep[0]+0.5*(maxMassStep[0]-maxMassStep[1]);


  TH2F* hSigmaRebin=new TH2F("hSigmaRebin"," ; rebin ; Gaussian #sigma",nRebinSteps,axRebinLimits,100,0.005,0.015);
  TH2F* hYieldRebin=new TH2F("hYieldRebin"," ; rebin ; Ratio to central yield",nRebinSteps,axRebinLimits,100,0.75,1.25);
  TH2F* hSigmaMinMass=new TH2F("hSigmaMinMass"," ; min fit ; Gaussian #sigma",nMinMassSteps,axMinMassLimits,100,0.005,0.015);
  TH2F* hYieldMinMass=new TH2F("hYieldMinMass"," ; min fit ; Ratio to central yield",nMinMassSteps,axMinMassLimits,100,0.75,1.25);
  TH2F* hSigmaMaxMass=new TH2F("hSigmaMaxMass"," ; max fit ; Gaussian #sigma",nMaxMassSteps,axMaxMassLimits,100,0.005,0.015);
  TH2F* hYieldMaxMass=new TH2F("hYieldMaxMass"," ; max fit ; Ratio to central yield",nMaxMassSteps,axMaxMassLimits,100,0.75,1.25);
  TH2F* hSigmaBkgFunc=new TH2F("hSigmaBkgFunc"," ; Bkg Func ; Gaussian #sigma",6,-0.5,5.5,100,0.005,0.015);
  TH2F* hYieldBkgFunc=new TH2F("hYieldBkgFunc"," ; Bkg Func ; Ratio to central yield",6,-0.5,5.5,100,0.75,1.25);
  TH2F* hSigmaBkgFuncFix=new TH2F("hSigmaMinMassFix"," ; Bkg Func ; Gaussian #sigma",6,-0.5,5.5,100,0.005,0.015);
  TH2F* hYieldBkgFuncFix=new TH2F("hYieldMinMassFix"," ; Bkg Func ; Ratio to central yield",6,-0.5,5.5,100,0.75,1.25);
  TH2F* hSigmaFitConf=new TH2F("hSigmaFitConf"," ;  ; Gaussian #sigma",6,-0.5,5.5,100,0.005,0.015);
  TH2F* hYieldFitConf=new TH2F("hYieldFitConf"," ;  ; Ratio to central yield",6,-0.5,5.5,100,0.75,1.25);


  SetBackgroundBinLabels(hSigmaBkgFunc);
  SetBackgroundBinLabels(hYieldBkgFunc);
  SetBackgroundBinLabels(hSigmaBkgFuncFix);
  SetBackgroundBinLabels(hYieldBkgFuncFix);
  SetFitConfBinLabels(hSigmaFitConf);
  SetFitConfBinLabels(hYieldFitConf);

  hSigmaRebin->SetStats(0);
  hYieldRebin->SetStats(0);
  hSigmaMinMass->SetStats(0);
  hYieldMinMass->SetStats(0);
  hSigmaMaxMass->SetStats(0);
  hYieldMaxMass->SetStats(0);
  hSigmaBkgFunc->SetStats(0);
  hYieldBkgFunc->SetStats(0);
  hSigmaBkgFuncFix->SetStats(0);
  hYieldBkgFuncFix->SetStats(0);
  hSigmaFitConf->SetStats(0);
  hYieldFitConf->SetStats(0);

  Int_t totTrials=nt->GetEntries();
  Double_t minyd=9e9;
  Double_t maxyd=0;
  Int_t colorBC0=kGreen+2;
  Int_t colorBC1=kAzure-8;
  Double_t minYield=99999999.;
  Double_t maxYield=0.;
  const Int_t totConfCases=16;

  Int_t nEntForSyst=0;
  Int_t nEntForSystPerCase[totConfCases][6];
  for(Int_t j1=0; j1<totConfCases; j1++){
    for(Int_t j2=0; j2<6; j2++){
      nEntForSystPerCase[j1][j2]=0.;
    }
  }

  for(Int_t ie=0; ie<totTrials; ie++){
    nt->GetEvent(ie);
    if(chi2>chi2Cut) continue;
    Int_t jBkgFunc=TMath::Nint(bkgfunc);
    Int_t jConfSig=TMath::Nint(confsig);
    Int_t jConfMean=TMath::Nint(confmean);
    Int_t jRebin=TMath::Nint(rebin);
    Int_t confCase=jConfSig+jConfMean*4;
    Bool_t use=kFALSE;
    //    cout << "Trial " << ie << "\t" << jBkgFunc << "\t" << jConfSig << "\t" << jConfMean << "\t" << jRebin << "\t" << minfit << "\t" << maxfit << "\t" << isSB << "\t" << iPtBin << endl;
    if(UseForSyst(jBkgFunc,jConfSig,jConfMean,jRebin,minfit,maxfit,isSB,iPtBin)){
      nEntForSyst++;
      nEntForSystPerCase[confCase][jBkgFunc]+=1;
      use=kTRUE;
    }
    
    Double_t diffSig=TMath::Abs(sigma-gausSig)/gausSig;
    Double_t diffMean=TMath::Abs(mean-1.865);
    //    cout << "RAWY " << rawy<< endl;
    cout << "bkg " << jBkgFunc << "\t sig " << jConfSig << "\t mean " << jConfMean << endl;
    if(rawy>0.001 && erawy>(0.001*rawy) && erawy<(0.5*rawy) && chi2<chi2Cut && diffSig<sigCut && diffMean<meanCut){
      if(use){
	if(rawy<minyd) minyd=rawy;
	if(rawy>maxyd) maxyd=rawy;
      }
      if(IsCentralCase(jBkgFunc,jConfSig,jConfMean)){      
	hSigmaRebin->Fill(rebin,sigma);
	hYieldRebin->Fill(rebin,rawy/centralYield);
	hSigmaMinMass->Fill(minfit,sigma);
	hYieldMinMass->Fill(minfit,rawy/centralYield);
	hSigmaMaxMass->Fill(maxfit,sigma);
	hYieldMaxMass->Fill(maxfit,rawy/centralYield);
      }
      if(jConfMean==0 && jRebin==defaultrebin){
	if(jConfSig==0){
	  hSigmaBkgFunc->Fill(bkgfunc,sigma);
	  hYieldBkgFunc->Fill(bkgfunc,rawy/centralYield);
	}else if(jConfSig==2){
	  hSigmaBkgFuncFix->Fill(bkgfunc,sigma);
	  hYieldBkgFuncFix->Fill(bkgfunc,rawy/centralYield);
	}
      }
      if(jBkgFunc==defaultbkg && jRebin==defaultrebin){
	hSigmaFitConf->Fill(confCase,sigma);
	hYieldFitConf->Fill(confCase,rawy/centralYield);
      }
    }
  }

  cout<<nEntForSyst<<endl;
  for(Int_t j1=0; j1<totConfCases; j1++){
    for(Int_t j2=0; j2<6; j2++){
      cout<<nEntForSystPerCase[j1][j2]<<"   ";
    }
    cout<<endl;
  }



  TCanvas* c1=new TCanvas("c1","SystChecks",1500,800);
  c1->Divide(3,2);
  c1->cd(1);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  gPad->SetTickx();
  gPad->SetTicky();
  hSigmaMinMass->Draw("colz");
  c1->cd(2);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  gPad->SetTickx();
  gPad->SetTicky();
  hSigmaMaxMass->Draw("colz");
  c1->cd(3);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  gPad->SetTickx();
  gPad->SetTicky();
  hSigmaRebin->Draw("colz");
  c1->cd(4);  
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  gPad->SetTickx();
  gPad->SetTicky();
  hYieldMinMass->Draw("colz");
  TProfile* prof1=hYieldMinMass->ProfileX("prof1");
  prof1->SetMarkerStyle(20);
  prof1->Draw("same");
  c1->cd(5);  
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  gPad->SetTickx();
  gPad->SetTicky();
  hYieldMaxMass->Draw("colz");
  TProfile* prof2=hYieldMaxMass->ProfileX("prof2");
  prof2->SetMarkerStyle(20);
  prof2->Draw("same");
  c1->cd(6);  
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  gPad->SetTickx();
  gPad->SetTicky();
  hYieldRebin->Draw("colz");
  TProfile* prof3=hYieldRebin->ProfileX("prof3");
  prof3->SetMarkerStyle(20);
  prof3->Draw("same");
//   c1->SaveAs(Form("SystRawYield/RawSystChecks1_MultiTrial_%s_%s_PtBin%d.eps",bkgTreat.Data(),suffix.Data(),iPtBin)); 
//   c1->SaveAs(Form("SystRawYield/RawSystChecks1_MultiTrial_%s_%s_PtBin%d.png",bkgTreat.Data(),suffix.Data(),iPtBin)); 

  TCanvas* cc2=new TCanvas("cc2","SystChecks2",1500,800);
  cc2->Divide(3,2);
  cc2->cd(1);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  gPad->SetTickx();
  gPad->SetTicky();
  hSigmaBkgFunc->Draw("colz");
  cc2->cd(2);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  gPad->SetTickx();
  gPad->SetTicky();
  hSigmaBkgFuncFix->Draw("colz");
  cc2->cd(3);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  gPad->SetTickx();
  gPad->SetTicky();
  hSigmaFitConf->Draw("colz");
  cc2->cd(4);  
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  gPad->SetTickx();
  gPad->SetTicky();
  hYieldBkgFunc->Draw("colz");
  TProfile* prof4=hYieldBkgFunc->ProfileX("prof4");
  prof4->SetMarkerStyle(20);
  prof4->Draw("same");
  cc2->cd(5);  
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  gPad->SetTickx();
  gPad->SetTicky();
  hYieldBkgFuncFix->Draw("colz");
  TProfile* prof5=hYieldBkgFuncFix->ProfileX("prof5");
  prof5->SetMarkerStyle(20);
  prof5->Draw("same");
  cc2->cd(6);  
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  gPad->SetTickx();
  gPad->SetTicky();
  hYieldFitConf->Draw("colz");
  TProfile* prof6=hYieldFitConf->ProfileX("prof6");
  prof6->SetMarkerStyle(20);
  prof6->Draw("same");
//   cc2->SaveAs(Form("SystRawYield/RawSystChecks2_MultiTrial_%s_%s_PtBin%d.eps",bkgTreat.Data(),suffix.Data(),iPtBin));
//   cc2->SaveAs(Form("SystRawYield/RawSystChecks2_MultiTrial_%s_%s_PtBin%d.png",bkgTreat.Data(),suffix.Data(),iPtBin));

  TNtuple * ntBC=(TNtuple*)filsy->Get(Form("ntuBinCount%s",bkgTreat.Data()));
  Float_t rebinBC,minfitBC,maxfitBC,bkgfuncBC,confsigBC,confmeanBC,nsigBC,rawyBC0,erawyBC0,rawyBC1,erawyBC1,chi2BC;
  ntBC->SetBranchAddress("rebin",&rebinBC);
  ntBC->SetBranchAddress("minfit",&minfitBC);
  ntBC->SetBranchAddress("maxfit",&maxfitBC);
  ntBC->SetBranchAddress("bkgfunc",&bkgfuncBC);
  ntBC->SetBranchAddress("confsig",&confsigBC);
  ntBC->SetBranchAddress("confmean",&confmeanBC);
  ntBC->SetBranchAddress("chi2",&chi2BC);
  ntBC->SetBranchAddress("rawyBC0",&rawyBC0);
  ntBC->SetBranchAddress("erawyBC0",&erawyBC0);
  ntBC->SetBranchAddress("rawyBC1",&rawyBC1);
  ntBC->SetBranchAddress("erawyBC1",&erawyBC1);
  ntBC->SetBranchAddress("nSigmaBC",&nsigBC);

  Int_t totTrialsBC=ntBC->GetEntries();
  printf("Minimum yield = %f  Maximum =%f\n",minyd,maxyd);
  TH1F* hRawYieldAll=new TH1F("hRawYieldAll"," ; Trial # ; Raw Yield",totTrials+2*totTrialsBC,0.,totTrials+2*totTrialsBC);
  TH1F* hRawYieldAllBC0=new TH1F("hRawYieldAllBC0"," ; Trial # ; Raw Yield",totTrialsBC,nEntForSyst,nEntForSyst+totTrialsBC);
  TH1F* hRawYieldAllBC1=new TH1F("hRawYieldAllBC1"," ; Trial # ; Raw Yield",totTrialsBC,nEntForSyst,nEntForSyst+totTrialsBC);
  TH1F* hMeanAll=new TH1F("hMeanAll"," ; Trial # ; Gaussian mean",totTrials,0.,totTrials);
  TH1F* hSigmaAll=new TH1F("hSigmaAll"," ; Trial # ; Gaussian #sigma",totTrials,0,totTrials);
  TH1F* hChi2All=new TH1F("hChi2All"," ; Trial # ; #chi^{2}",totTrials,0.,totTrials);
  TH1F* hRawYieldDistAll=new TH1F("hRawYieldDistAll","  ; Raw Yield",200,TMath::Nint(minyd*0.85),TMath::Nint(maxyd*1.15));
  TH1F* hRawYieldDistAllBC0=new TH1F("hRawYieldDistAllBC0","  ; Raw Yield",200,TMath::Nint(minyd*0.85),TMath::Nint(maxyd*1.15));
  TH1F* hRawYieldDistAllBC1=new TH1F("hRawYieldDistAllBC1","  ; Raw Yield",200,TMath::Nint(minyd*0.85),TMath::Nint(maxyd*1.15));
  TH1F* hStatErrDistAll=new TH1F("hStatErrDistAll","  ; Stat Unc on Yield",200,TMath::Nint(minyd*0.85),TMath::Nint(maxyd*1.15));
  TH1F* hRelStatErrDistAll=new TH1F("hRelStatErrDistAll","  ; Rel Stat Unc on Yield",100,0.,1.);
  TH1F* hRawYieldDistSigFree=new TH1F("hRawYieldDistSigFree","  ; Raw Yield",200,TMath::Nint(minyd*0.85),TMath::Nint(maxyd*1.15));
  TH1F* hRawYieldDistSigFixed=new TH1F("hRawYieldDistSigFixed","  ; Raw Yield",200,TMath::Nint(minyd*0.85),TMath::Nint(maxyd*1.15));
				      
  Int_t filled[totConfCases][6];
  Int_t firstBin[totConfCases][6];
  TString textgs[4]={"Free#sigma","Fix#sigma","Fix#sigma+","Fix#sigma-"};
  TString textgm[4]={"Free#mu","Fix#mu","Fix#mu+","Fix#mu-"};
  TString ttt1[totConfCases];
  for(Int_t igs=0; igs<4; igs++){
    for(Int_t igm=0; igm<4; igm++){
      ttt1[igs+igm*4]=Form("#splitline{%s}{%s}",textgm[igm].Data(),textgs[igs].Data());
    }
  }
  TString ttt2[6]={"Expo","Lin","Pol2","Pol3","Pol4","Pol5"};
  TLatex **tlabels=new TLatex*[48];
  TLine **vlines=new TLine*[48];
  Int_t filledCases=0;
  Int_t sum=0;
  cout<<"-----"<<endl;
  for(Int_t j1=0; j1<totConfCases; j1++){
    for(Int_t j2=0; j2<6; j2++){
      filled[j1][j2]=0.;
      firstBin[j1][j2]=sum;
      sum+=nEntForSystPerCase[j1][j2];
      if(nEntForSystPerCase[j1][j2]>0){
	tlabels[filledCases]=new TLatex(firstBin[j1][j2]+0.02*nEntForSyst,10,Form("#splitline{%s}{%s}",ttt1[j1].Data(),ttt2[j2].Data()));
	tlabels[filledCases]->SetTextColor(kMagenta+2);
	tlabels[filledCases]->SetTextColor(kMagenta+2);
	vlines[filledCases]=new TLine(sum,0.,sum,50000.);
	vlines[filledCases]->SetLineColor(kMagenta+2);
	vlines[filledCases]->SetLineStyle(2);
 	++filledCases;
      }
    }
  }
  for(Int_t j1=0; j1<totConfCases; j1++){
    for(Int_t j2=0; j2<6; j2++){
      cout<<firstBin[j1][j2]<<"   ";
    }
    cout<<endl;
  }


  for(Int_t ie=0; ie<totTrials; ie++){
    nt->GetEvent(ie);
    if(chi2>chi2Cut) continue;
    Int_t jBkgFunc=TMath::Nint(bkgfunc);
    Int_t jConfSig=TMath::Nint(confsig);
    Int_t jConfMean=TMath::Nint(confmean);
    Int_t jRebin=TMath::Nint(rebin);
    Int_t confCase=jConfSig+jConfMean*4;
    Double_t diffSig=TMath::Abs(sigma-gausSig)/gausSig;
    Double_t diffMean=TMath::Abs(mean-1.865);
    if(rawy>0.001 && erawy>(0.001*rawy) && erawy<(0.5*rawy) && chi2<chi2Cut && diffSig<sigCut && diffMean<meanCut){
      if(defaultsig!=0){
	if(jBkgFunc==defaultbkg && jConfMean==0){
	  if(jConfSig==0) hRawYieldDistSigFree->Fill(rawy);
	  else if(jConfSig==defaultsig) hRawYieldDistSigFixed->Fill(rawy);
	}
      }
      if(UseForSyst(jBkgFunc,jConfSig,jConfMean,jRebin,minfit,maxfit,isSB,iPtBin)){
	Int_t theBin=firstBin[confCase][jBkgFunc]+filled[confCase][jBkgFunc]+1;
	filled[confCase][jBkgFunc]+=1;
	hRawYieldDistAll->Fill(rawy);
	hStatErrDistAll->Fill(erawy);
	hRelStatErrDistAll->Fill(erawy/rawy);
	hRawYieldAll->SetBinContent(theBin,rawy);
	hRawYieldAll->SetBinError(theBin,erawy);
	if(rawy<minYield) minYield=rawy;
	if(rawy>maxYield) maxYield=rawy;
	hSigmaAll->SetBinContent(theBin,sigma);
	hSigmaAll->SetBinError(theBin,esigma);
	hMeanAll->SetBinContent(theBin,mean);
	hMeanAll->SetBinError(theBin,emean);
	hChi2All->SetBinContent(theBin,chi2);
	hChi2All->SetBinError(theBin,0.000001);
      }
    }
  }

  Int_t binToFill=1;
  for(Int_t ie=0; ie<totTrialsBC; ie++){
    ntBC->GetEvent(ie);
    if(chi2BC>chi2Cut) continue;
     if(nsigBC<2.5 || nsigBC>4.6) continue;
    Int_t jBkgFunc=TMath::Nint(bkgfuncBC);
    Int_t jConfSig=TMath::Nint(confsigBC);
    Int_t jConfMean=TMath::Nint(confmeanBC);
    Int_t jRebin=TMath::Nint(rebinBC);
    if(IsCentralCase(jBkgFunc,jConfSig,jConfMean) && jRebin==defaultrebin){
      if(rawyBC0>0.001 && erawyBC0>(0.01*rawyBC0) && erawyBC0<(0.5*rawyBC0)){
	hRawYieldDistAllBC0->Fill(rawyBC0);
	hRawYieldDistAllBC1->Fill(rawyBC1);
	hRawYieldAllBC0->SetBinContent(binToFill,rawyBC0);
	hRawYieldAllBC0->SetBinError(binToFill,erawyBC0);
	hRawYieldAllBC1->SetBinContent(binToFill,rawyBC1);
	hRawYieldAllBC1->SetBinError(binToFill,erawyBC1);
	binToFill++;
      }
    }
  }

  hRawYieldAll->SetStats(0);
  hMeanAll->SetStats(0);
  hSigmaAll->SetStats(0);
  hChi2All->SetStats(0);
  hChi2All->SetMarkerStyle(7);
  hMeanAll->SetMinimum(1.85);
  hMeanAll->SetMaximum(1.88);
  hSigmaAll->SetMinimum(0.);
  if(hSigmaAll->GetMaximum()<0.018) hSigmaAll->SetMaximum(0.018);
  hRawYieldAllBC0->SetStats(0);
  hRawYieldAllBC1->SetStats(0);
  // hRawYieldAllBC->SetMarkerStyle(7);
  // hRawYieldAllBC->SetMarkerSize(0.1);
  hRawYieldAllBC0->SetMarkerColor(colorBC0);
  hRawYieldAllBC0->SetLineColor(colorBC0);
  hRawYieldDistAllBC0->SetFillColor(colorBC0);
  hRawYieldDistAllBC0->SetFillStyle(3001);
  hRawYieldDistAllBC0->SetLineColor(colorBC0);
  hRawYieldDistAllBC0->SetLineWidth(1);
  //  hRawYieldDistAllBC0->SetLineStyle(7);
  hRawYieldDistAllBC0->Scale(hRawYieldDistAll->GetEntries()/hRawYieldDistAllBC0->GetEntries());
  hRawYieldAllBC1->SetMarkerColor(colorBC1);
  hRawYieldDistAllBC1->SetFillColor(colorBC1);
  hRawYieldDistAllBC1->SetLineColor(colorBC1);
  hRawYieldDistAllBC1->SetFillStyle(3005);
  hRawYieldDistAllBC1->SetLineColor(colorBC1);
  hRawYieldDistAllBC1->SetLineWidth(2);
  //  hRawYieldDistAllBC1->SetLineStyle(2);
  hRawYieldDistAllBC1->Scale(hRawYieldDistAll->GetEntries()/hRawYieldDistAllBC1->GetEntries());
  hRawYieldDistAll->SetLineWidth(2);
  if(nEntForSyst>0){
    hRawYieldAll->GetXaxis()->SetRangeUser(0.,(Float_t)nEntForSyst);
    hSigmaAll->GetXaxis()->SetRangeUser(0.,(Float_t)nEntForSyst);
    hMeanAll->GetXaxis()->SetRangeUser(0.,(Float_t)nEntForSyst);
    hChi2All->GetXaxis()->SetRangeUser(0.,(Float_t)nEntForSyst);
  }

  TLine *lcenthor=new TLine(hRawYieldAll->GetXaxis()->GetXmin(),centralYield,hRawYieldAll->GetXaxis()->GetXmax(),centralYield);
  lcenthor->SetLineColor(2);
  lcenthor->SetLineWidth(2);
  TLine *lcentver=new TLine(centralYield,0.,centralYield,hRawYieldDistAll->GetMaximum());
  lcentver->SetLineColor(2);
  lcentver->SetLineWidth(2);

  hRawYieldDistSigFree->SetLineColor(kGreen+1);
  hRawYieldDistSigFree->SetFillColor(kGreen+1);
  hRawYieldDistSigFree->SetLineWidth(2);
  hRawYieldDistSigFree->SetFillStyle(3001);
  hRawYieldDistSigFixed->SetLineColor(kBlue+1);
  hRawYieldDistSigFixed->SetLineWidth(2);
  hRawYieldDistSigFixed->SetTitle(ptrange.Data());
  hRawYieldDistSigFree->SetTitle(ptrange.Data());
  if(defaultsig!=0){
    TCanvas* cff=new TCanvas("cff","FreeFixedSigma",1000,700);
    hRawYieldDistSigFixed->Draw();
    gPad->Update();
    TPaveStats* stfx=(TPaveStats*)hRawYieldDistSigFixed->GetListOfFunctions()->FindObject("stats");
    stfx->SetY1NDC(0.71);
    stfx->SetY2NDC(0.9);
    stfx->SetTextColor(hRawYieldDistSigFixed->GetLineColor());
    hRawYieldDistSigFree->Draw("sames");
    gPad->Update();
    TPaveStats* stfr=(TPaveStats*)hRawYieldDistSigFree->GetListOfFunctions()->FindObject("stats");
    stfr->SetY1NDC(0.51);
    stfr->SetY2NDC(0.7);
    stfr->SetTextColor(hRawYieldDistSigFree->GetLineColor());
    gPad->Modified();
    Double_t mfx=hRawYieldDistSigFixed->GetMean();
    TLatex* tmeanfx=new TLatex(0.55,0.45,Form("mean(Fixed #sigma)=%.1f",mfx));
    tmeanfx->SetNDC();
    tmeanfx->SetTextFont(43);
    tmeanfx->SetTextSize(22);
    tmeanfx->SetTextColor(hRawYieldDistSigFixed->GetLineColor());
    tmeanfx->Draw();
    Double_t rmsfx=hRawYieldDistSigFixed->GetRMS();
    TLatex* trmsfx=new TLatex(0.55,0.38,Form("rms(Fixed #sigma)=%.1f (%.1f%%)",rmsfx,rmsfx/mfx*100.));
    trmsfx->SetNDC();
    trmsfx->SetTextFont(43);
    trmsfx->SetTextSize(22);
    trmsfx->SetTextColor(hRawYieldDistSigFixed->GetLineColor());
    trmsfx->Draw();
    Double_t mfr=hRawYieldDistSigFree->GetMean();
    TLatex* tmeanfr=new TLatex(0.55,0.31,Form("mean(Free #sigma)=%.1f (shift=%.1f%%)",mfr,(mfr-mfx)/mfx*100.));
    tmeanfr->SetNDC();
    tmeanfr->SetTextFont(43);
    tmeanfr->SetTextSize(22);
    tmeanfr->SetTextColor(hRawYieldDistSigFree->GetLineColor());
    tmeanfr->Draw();
//     cff->SaveAs(Form("SystRawYield/RawSystChecks3_MultiTrial_%s_%s_PtBin%d.eps",bkgTreat.Data(),suffix.Data(),iPtBin)); 
//     cff->SaveAs(Form("SystRawYield/RawSystChecks3_MultiTrial_%s_%s_PtBin%d.png",bkgTreat.Data(),suffix.Data(),iPtBin)); 
  }
  
  TCanvas* call=new TCanvas("call","All",1400,800);
  call->Divide(3,2);
  call->cd(1);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  hSigmaAll->GetYaxis()->SetTitleOffset(1.7);
  hSigmaAll->Draw();
  call->cd(2);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  hMeanAll->GetYaxis()->SetTitleOffset(1.7);
  hMeanAll->Draw();
  call->cd(3);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  hChi2All->GetYaxis()->SetTitleOffset(1.7);
  hChi2All->Draw();
  call->cd(4);
  hRawYieldAll->SetTitle(ptrange.Data());
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  Double_t newmax=1.25*(hRawYieldAll->GetMaximum()+hRawYieldAll->GetBinError(1));
  hRawYieldAll->GetYaxis()->SetTitleOffset(1.7);
  hRawYieldAll->SetMinimum(0.);
  hRawYieldAll->SetMaximum(newmax);
  hRawYieldAll->Draw();
  hRawYieldAllBC0->Draw("same");
  hRawYieldAllBC1->Draw("same");
  for(Int_t j=0; j<filledCases; j++){
    vlines[j]->SetY2(newmax);
    vlines[j]->Draw("same"); 
    tlabels[j]->SetY(0.07*newmax);
    tlabels[j]->Draw();
  }
  lcenthor->Draw("same");
  call->cd(5);
  gPad->SetLeftMargin(0.14);
  gPad->SetRightMargin(0.06);
  hRawYieldDistAll->SetTitle(ptrange.Data());
  hRawYieldDistAll->Draw();
  //  hRawYieldDistAll->GetXaxis()->SetRangeUser(minYield*0.8,maxYield*1.2);
  hRawYieldDistAllBC0->Draw("histo,sames");
  hRawYieldDistAllBC1->Draw("histo,sames");
  hRawYieldDistAll->Draw("same");
  gPad->Update();
  TPaveStats* st=(TPaveStats*)hRawYieldDistAll->GetListOfFunctions()->FindObject("stats");
  st->SetY1NDC(0.71);
  st->SetY2NDC(0.9);
  TPaveStats* stb0=(TPaveStats*)hRawYieldDistAllBC0->GetListOfFunctions()->FindObject("stats");
  stb0->SetY1NDC(0.51);
  stb0->SetY2NDC(0.7);
  stb0->SetTextColor(hRawYieldDistAllBC0->GetLineColor());
  TPaveStats* stb1=(TPaveStats*)hRawYieldDistAllBC1->GetListOfFunctions()->FindObject("stats");
  stb1->SetY1NDC(0.31);
  stb1->SetY2NDC(0.5);
  stb1->SetTextColor(hRawYieldDistAllBC1->GetLineColor());
  lcentver->Draw("same");
  Double_t perc[3]={0.15,0.5,0.85}; // quantiles for +-1 sigma
  Double_t lim70[3];
  hRawYieldDistAll->GetQuantiles(3,lim70,perc);
  call->cd(6);
  gPad->SetLeftMargin(0.14);
  gPad->SetRightMargin(0.06);
  Double_t aver=hRawYieldDistAll->GetMean();
  TLatex* tcent=new TLatex(0.15,0.94,Form("Central=%.1f",centralYield));
  tcent->SetNDC();
  tcent->SetTextColor(2);
  tcent->Draw();
  TLatex* tmean=new TLatex(0.15,0.87,Form("mean=%.1f (shift=%.1f%%)",aver,(aver-centralYield)/centralYield*100.));
  tmean->SetNDC();
  tmean->Draw();
  TLatex* tmedian=new TLatex(0.15,0.8,Form("median=%.1f",lim70[1]));
  tmedian->SetNDC();
  tmedian->Draw();
  Double_t averBC0=hRawYieldDistAllBC0->GetMean();
  TLatex* tmeanBC0=new TLatex(0.15,0.73,Form("mean(BinCount0)=%.1f",averBC0));
  tmeanBC0->SetNDC();
  tmeanBC0->SetTextColor(hRawYieldDistAllBC0->GetLineColor());
  tmeanBC0->Draw();
  Double_t averBC1=hRawYieldDistAllBC1->GetMean();
  TLatex* tmeanBC1=new TLatex(0.15,0.67,Form("mean(BinCount1)=%.1f (shift=%.1f%%)",averBC1,(averBC1-centralYield)/centralYield*100.));
  tmeanBC1->SetNDC();
  tmeanBC1->SetTextColor(hRawYieldDistAllBC1->GetLineColor());
  tmeanBC1->Draw();
  Double_t val=hRawYieldDistAll->GetRMS();
  TLatex* thrms=new TLatex(0.15,0.56,Form("rms=%.1f  (%.1f%%)",val,val/aver*100.));
  thrms->SetNDC();
  thrms->Draw();
  TLatex* tmin=new TLatex(0.15,0.46,Form("min=%.1f      max=%.1f",minYield,maxYield));
  tmin->SetNDC();
  tmin->Draw();
  val=(maxYield-minYield)/sqrt(12);
  TLatex* trms=new TLatex(0.15,0.39,Form("(max-min)/sqrt(12)=%.1f  (%.1f%%)",val,val/aver*100.));
  trms->SetNDC();
  trms->Draw();
  val=(maxYield-aver)/sqrt(3);
  TLatex* tup=new TLatex(0.15,0.32,Form("(max-mean)/sqrt(3)=%.1f  (%.1f%%)",val,val/aver*100.));
  tup->SetNDC();
  tup->Draw();
  val=(aver-minYield)/sqrt(3);
  TLatex* tdw=new TLatex(0.15,0.25,Form("(mean-min)/sqrt(3)=%.1f  (%.1f%%)",val,val/aver*100.));
  tdw->SetNDC();
  tdw->Draw();
  TLatex* tl15=new TLatex(0.15,0.16,Form("15 percentile=%.1f",lim70[0]));
  tl15->SetNDC();
  tl15->Draw();
  TLatex* tl85=new TLatex(0.15,0.10,Form("85 percentile=%.1f",lim70[2]));
  tl85->SetNDC();
  tl85->Draw();
  val=(lim70[2]-lim70[0])/2.;
  TLatex* t1s=new TLatex(0.15,0.04,Form("70%% range =%.1f  (%.1f%%)",val,val/aver*100.));
  t1s->SetNDC();
  t1s->Draw();
//   call->SaveAs(Form("SystRawYield/MultiTrial_all_%s_%s_PtBin%d.eps",bkgTreat.Data(),suffix.Data(),iPtBin));
//   call->SaveAs(Form("SystRawYield/MultiTrial_all_%s_%s_PtBin%d.png",bkgTreat.Data(),suffix.Data(),iPtBin));




  TCanvas* c3p=new TCanvas("c3p","3Pad",1400,400);
  c3p->Divide(3,1);
  c3p->cd(1);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  hRawYieldAll->Draw();
  hRawYieldAllBC0->Draw("same");
  hRawYieldAllBC1->Draw("same");
  for(Int_t j=0; j<filledCases; j++){
    vlines[j]->SetY2(newmax);
    vlines[j]->Draw("same"); 
    tlabels[j]->SetY(0.07*newmax);
    tlabels[j]->Draw();
  }  
  lcenthor->Draw("same");
  c3p->cd(2);
  gPad->SetLeftMargin(0.14);
  gPad->SetRightMargin(0.06);
  hRawYieldDistAll->Draw();
  hRawYieldDistAllBC0->Draw("histo,sames");
  hRawYieldDistAllBC1->Draw("histo,sames");
  hRawYieldDistAll->Draw("same");
  lcentver->Draw("same");
  c3p->cd(3);
  gPad->SetLeftMargin(0.14);
  gPad->SetRightMargin(0.06);
  tcent->Draw();
  tmean->Draw();
  tmeanBC0->Draw();
  tmeanBC1->Draw();
  tmedian->Draw();
  thrms->Draw();
  tmin->Draw();
  trms->Draw();
  tup->Draw();
  tdw->Draw();
  tl15->Draw();
  tl85->Draw();
  t1s->Draw();
//   c3p->SaveAs(Form("SystRawYield/MultiTrial_3pad_%s_%s_PtBin%d.eps",bkgTreat.Data(),suffix.Data(),iPtBin));
//   c3p->SaveAs(Form("SystRawYield/MultiTrial_3pad_%s_%s_PtBin%d.png",bkgTreat.Data(),suffix.Data(),iPtBin));

  TCanvas* c2p=new TCanvas("c2p","2Pad",933,400);
  c2p->Divide(2,1);
  c2p->cd(1);
  gPad->SetLeftMargin(0.14);
  gPad->SetRightMargin(0.06);
  hRawYieldDistAll->Draw();
  hRawYieldDistAllBC0->Draw("histo,sames");
  hRawYieldDistAllBC1->Draw("histo,sames");
  hRawYieldDistAll->Draw("same");
  lcentver->Draw("same");
  c2p->cd(2);
  gPad->SetLeftMargin(0.14);
  gPad->SetRightMargin(0.06);
  tcent->Draw();
  tmean->Draw();
  tmeanBC0->Draw();
  tmeanBC1->Draw();
  tmedian->Draw();
  thrms->Draw();
  tmin->Draw();
  trms->Draw();
  tup->Draw();
  tdw->Draw();
  tl15->Draw();
  tl85->Draw();
  t1s->Draw();
//   c2p->SaveAs(Form("SystRawYield/MultiTrial_2pad_%s_%s_PtBin%d.eps",bkgTreat.Data(),suffix.Data(),iPtBin));
//   c2p->SaveAs(Form("SystRawYield/MultiTrial_2pad_%s_%s_PtBin%d.png",bkgTreat.Data(),suffix.Data(),iPtBin));
}

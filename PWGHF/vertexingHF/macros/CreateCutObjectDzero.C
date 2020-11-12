class AliRDHFCutsD0toKpi;


void CreateCutObjectDzero(TString fileout="D0toKpiNoTopolCuts_PP.root", 
			  Int_t pileupopt=1,
			  Int_t tpcopt=1,
			  Int_t spd=1,
			  Int_t its=2,
			  Int_t trigMask=AliVEvent::kINT7,
			  Int_t ycut=0,
			  Double_t minPt=0.3,
			  Int_t pidOption=2,
			  Bool_t corrNsigma=kFALSE,
			  Int_t minCentr=-1,
			  Int_t maxCentr=110){


  AliRDHFCutsD0toKpi *cutTight=new AliRDHFCutsD0toKpi("D0toKpiCutsStandard");

  // event selection and trigger setting
  cutTight->SetMinVtxContr(1);
  cutTight->SetMaxVtxZ(10.);
  cutTight->SetCutOnzVertexSPD(3);
  cutTight->SetTriggerMask(trigMask);
  cutTight->SetTriggerClass("");
  fileout.ReplaceAll(".root","_SPDvert3.root");
  if(minCentr>=0 && maxCentr>minCentr && maxCentr<=100){
    cutTight->SetUseCentrality(AliRDHFCuts::kCentV0M);
    cutTight->SetMinCentrality(minCentr);
    cutTight->SetMaxCentrality(maxCentr);
    fileout.ReplaceAll("PP",Form("PP%d-%dV0M",minCentr,maxCentr));
  }

  // PILE UP
  if(pileupopt==0){
    cutTight->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
    fileout.ReplaceAll(".root","_NoPileUp.root");
  }else if(pileupopt==1){
    cutTight->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent);
    fileout.ReplaceAll(".root","_PileUpMV.root");
  }else if(pileupopt==2){
    cutTight->SetOptPileup(AliRDHFCuts::kRejectPileupEvent);
    cutTight->ConfigureSPDPileupCuts(5,0.8);
    fileout.ReplaceAll(".root","_PileUpSPD5.root");
  }else if(pileupopt==3){
    cutTight->SetOptPileup(AliRDHFCuts::kRejectPileupEvent);
    cutTight->ConfigureSPDPileupCuts(5,0.8);
    cutTight->SetUseMultDepPileupCut(kTRUE);
    fileout.ReplaceAll(".root","_PileUpSPDMultDep.root");     
  }else if(pileupopt==4){
    cutTight->SetOptPileup(AliRDHFCuts::kRejectPileupEvent);
    cutTight->ConfigureSPDPileupCuts(3,0.8);
    fileout.ReplaceAll(".root","_PileUpSPD3.root");
  }else if(pileupopt==5){
    cutTight->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent);
    cutTight->SetMinContribPileupMV(3);
    fileout.ReplaceAll(".root","_PileUpMV3.root");
  }else{
    cutTight->SetOptPileup(AliRDHFCuts::kNoPileupSelection);
    fileout.ReplaceAll(".root","_NoPileUp.root");
  }

  // track cuts: most are redundant, already in filter bit 4
  fileout.ReplaceAll(".root",Form("_minPt%d.root",(Int_t)(minPt*1000.)));

  AliESDtrackCuts *esdcut = new AliESDtrackCuts("AliESDtrackCuts","default");
  esdcut->SetPtRange(minPt,1.e10);
  esdcut->SetEtaRange(-0.8,0.8);
  if(tpcopt==0){
    esdcut->SetMinNClustersTPC(70);
  }else{
    esdcut->SetMinNCrossedRowsTPC(70);
    esdcut->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
    fileout.ReplaceAll(".root","_CrossRow.root");
    if(tpcopt==2){
      esdcut->SetMinNClustersTPC(70);
      fileout.ReplaceAll("CrossRow","CrossRowAnd70clu");
    }
  }
  if(its>=1){
    if(spd==0){
      esdcut->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kOff);
      fileout.ReplaceAll(".root","_SPDoff.root");
    }else{
      esdcut->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);
      fileout.ReplaceAll(".root","_SPDany.root");
    }
    esdcut->SetRequireITSRefit(kTRUE);
    if(its==2){
      esdcut->SetMaxChi2PerClusterITS(36);
    }else{
      fileout.ReplaceAll(".root","_noITSchi2.root");      
    }
  }else{
    fileout.ReplaceAll(".root","_TPConly.root");    
  }
  esdcut->SetRequireTPCRefit(kTRUE);
  esdcut->SetRequireSigmaToVertex(kFALSE);
  esdcut->SetMaxChi2PerClusterTPC(4);
  esdcut->SetAcceptKinkDaughters(kFALSE);
  esdcut->SetMaxDCAToVertexXY(2.4);
  esdcut->SetMaxDCAToVertexZ(3.2);
  esdcut->SetDCAToVertex2D(kTRUE);
  cutTight->AddTrackCuts(esdcut);

  delete esdcut;

  // PID
  Double_t sigCutTPC=3.;
  Double_t sigCutTOF=3.;
  if(pidOption==0){
    sigCutTPC=10000.;
    sigCutTOF=10000.;
  }
  else if(pidOption==3){
    sigCutTPC=2.5;
    sigCutTOF=2.5;
  }
  else if(pidOption==4){
    sigCutTPC=2.;
    sigCutTOF=2.;
  }
  else if(pidOption==5){
    sigCutTPC=2.;
    sigCutTOF=3.;
  }
  else if(pidOption==6){
    sigCutTPC=3.;
    sigCutTOF=2.;
  }

  AliAODPidHF* pidObj=new AliAODPidHF();
  if(pidOption==1){
    Int_t mode=1;
    const Int_t nlims=2;
    Double_t plims[nlims]={0.6,0.8}; //TPC limits in momentum [GeV/c]
    Bool_t compat=kTRUE; //effective only for this mode
    Bool_t asym=kTRUE;
    Double_t sigmas[5]={2.,1.,0.,3.,0.}; //to be checked and to be modified with new implementation of setters by Rossella
    pidObj->SetAsym(asym);// if you want to use the asymmetric bands in TPC
    pidObj->SetMatch(mode);
    pidObj->SetPLimit(plims,nlims);
    pidObj->SetSigma(sigmas);
    pidObj->SetCompat(compat);
    pidObj->SetTPC(kTRUE);
    pidObj->SetTOF(kTRUE);
    pidObj->SetPCompatTOF(2.);
    pidObj->SetSigmaForTPCCompat(3.);
    pidObj->SetSigmaForTOFCompat(3.);  
    pidObj->SetOldPid(kFALSE);
  }else{
    pidObj->SetMatch(5);
    pidObj->SetTPCnSigmaRangeForPions(-sigCutTPC,sigCutTPC);
    pidObj->SetTPCnSigmaRangeForKaons(-sigCutTPC,sigCutTPC);
    pidObj->SetTPCnSigmaRangeForProtons(-sigCutTPC,sigCutTPC);
    pidObj->SetTOFnSigmaRangeForPions(-sigCutTOF,sigCutTOF);
    pidObj->SetTOFnSigmaRangeForKaons(-sigCutTOF,sigCutTOF);
    pidObj->SetTOFnSigmaRangeForProtons(-sigCutTOF,sigCutTOF);
    pidObj->SetOldPid(kFALSE);
  }

  if(pidOption==0){
    fileout.ReplaceAll(".root","_NoPID.root");
  }else if(pidOption==1){
    fileout.ReplaceAll(".root","_ConservPID.root");
  }else if(pidOption==2){
    fileout.ReplaceAll(".root","_3SigPID.root");
  }else if(pidOption==3){
    fileout.ReplaceAll(".root","_2dot5SigPID.root");
  }else if(pidOption==4){
    fileout.ReplaceAll(".root","_2SigPID.root");
  }else if(pidOption==5){
    fileout.ReplaceAll(".root","_2SigTPC3SigTOFPID.root");
  }else if(pidOption==6){
    fileout.ReplaceAll(".root","_3SigTPC2SigTOFPID.root");
  }
  if(corrNsigma){
    if(minCentr==0 && maxCentr==10){
      cutTight->EnableNsigmaDataDrivenCorrection(kTRUE, AliAODPidHF::kPbPb010);
      fileout.ReplaceAll("PID","PIDnSigmaTuned");
    }
    if(minCentr==30 && maxCentr==50){
      cutTight->EnableNsigmaDataDrivenCorrection(kTRUE, AliAODPidHF::kPbPb3050);
      fileout.ReplaceAll("PID","PIDnSigmaTuned");
    }
  }
  cutTight->SetPidHF(pidObj);
  
  // REMOVE FID ACC., set acc to 0.8
  if(ycut!=0){
    cutTight->SetMaxRapidityCandidate(0.8);
  }else{
    fileout.ReplaceAll(".root","_YFidAcc.root");
  }

  cutTight->PrintAll();
  
  TFile *fout=new TFile(fileout.Data(),"recreate");
  fout->cd();
  cutTight->Write("D0toKpiCutsStandard");
  fout->Close();
  
  

}

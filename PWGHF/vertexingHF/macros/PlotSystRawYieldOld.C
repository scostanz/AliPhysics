void PlotSystRawYieldOld(Int_t iPtBin=0, TString bkgTreat="Rot"){

  TString suffix="3SigPID_Pt400_YFid_PileUpMV";
  TString dir="/home/alidock/alice/D0_13TeV_lowpt/results/figures/all/templ/V0100range";

  TString filnam = Form("%s/outputRawSyst_FixedSigmaAll_Refl_%s_PtBin%d.root",dir.Data(),suffix.Data(),iPtBin);
  TFile* fil=new TFile(filnam.Data());

  const Int_t nBackFuncCases=6;
  const Int_t nConfigCases=6;
  Int_t colorBC0=kGreen+2;
  Int_t colorBC1=kAzure-8;
  Int_t minBCrange=1;
  Int_t maxBCrange=5;
  Int_t nBCranges=maxBCrange-minBCrange+1;
  TString confCase[nConfigCases]={"FixedS","FixedSp20","FixedSm20","FreeS","FixedMeanFreeS","FixedMeanFixedS"};
  TString bkgFunc[nBackFuncCases]={"Expo","Lin","Pol2","Pol3","Pol4","Pol5"};

  const Int_t totCases=nConfigCases*nBackFuncCases;

  // 0= not used; 1 = used for fit; 2= used also for bin count0, 3=use also bin count1, 4=use both binc
  Int_t mask[totCases]={0,0,0,0,0,0,   // fixed sigma (Expo, Lin, Pol2,Pol3,Pol4)
			0,0,0,0,0,0,   // fixed sigma upper
			0,0,0,0,0,0,   // fixed sigma lower
			4,1,1,0,0,0,   // free sigma, free mean
			0,0,0,0,0,0,   // free sigma, fixed mean
			0,0,0,0,0,0,   // fixed mean, fixed sigma
  };

  Double_t chi2Cut=2.;

  TH1F* histo[totCases];
  Int_t jh=0;
  cout << "nconfigcases " << nConfigCases << "\t nbackgfunccases " << nBackFuncCases << endl;
  for(Int_t iConf=0; iConf<nConfigCases; iConf++){
    for(Int_t iType=0; iType<nBackFuncCases; iType++){
      histo[jh++]=(TH1F*)fil->Get(Form("hRawYieldTrial%s%s%s",bkgFunc[iType].Data(),confCase[iConf].Data(),bkgTreat.Data()));
      if (!histo[jh]) cout << "Histo " << Form("hRawYieldTrial%s%s%s",bkgFunc[iType].Data(),confCase[iConf].Data(),bkgTreat.Data()) << " not found " << endl;
    }
  }

  Int_t totTrials=0;
  Int_t totTrialsBC0=0;
  Int_t totTrialsBC1=0;
  Int_t totHistos=0;
  Int_t first[totCases];
  Int_t last[totCases];
  Int_t firstBC0[totCases];
  Int_t lastBC0[totCases];
  Int_t firstBC1[totCases];
  Int_t lastBC1[totCases];
  Double_t minyd=9e9;
  Double_t maxyd=0;
  for(Int_t j=0; j<totCases; j++){ 
    if(mask[j]){
      if(histo[j]){
	first[j]=totTrials;
	totTrials+=histo[j]->GetNbinsX();
	last[j]=totTrials;
	Double_t thisMin=histo[j]->GetBinContent(histo[j]->GetMinimumBin());
	Double_t thisMax=histo[j]->GetBinContent(histo[j]->GetMaximumBin());
	if(thisMin<minyd) minyd=thisMin;
	if(thisMax>maxyd) maxyd=thisMax;
	++totHistos;
	if(mask[j]==2 || mask[j]==4){
	  TString hbcname=histo[j]->GetName();
	  hbcname.ReplaceAll("Trial","TrialBinC0");
	  TH2F* hbc2dt=(TH2F*)fil->Get(hbcname.Data());
	  Int_t bnx,bny,bnz;
	  hbc2dt->GetMinimumBin(bnx,bny,bnz);
	  thisMin=hbc2dt->GetBinContent(bnx,bny);
	  hbc2dt->GetMaximumBin(bnx,bny,bnz);
	  thisMax=hbc2dt->GetBinContent(bnx,bny);
	  if(thisMin<minyd) minyd=thisMin;
	  if(thisMax>maxyd) maxyd=thisMax;
	  firstBC0[j]=totTrialsBC0;
	  totTrialsBC0+=hbc2dt->GetNbinsX();
	  lastBC0[j]=totTrialsBC0;
	}
	if(mask[j]==3 || mask[j]==4){
	  TString hbcname=histo[j]->GetName();
	  hbcname.ReplaceAll("Trial","TrialBinC1");
	  TH2F* hbc2dt=(TH2F*)fil->Get(hbcname.Data());
	  Int_t bnx,bny,bnz;
	  hbc2dt->GetMinimumBin(bnx,bny,bnz);
	  thisMin=hbc2dt->GetBinContent(bnx,bny);
	  hbc2dt->GetMaximumBin(bnx,bny,bnz);
	  thisMax=hbc2dt->GetBinContent(bnx,bny);
	  if(thisMin<minyd) minyd=thisMin;
	  if(thisMax>maxyd) maxyd=thisMax;
	  firstBC1[j]=totTrialsBC1;
	  totTrialsBC1+=hbc2dt->GetNbinsX();
	  lastBC1[j]=totTrialsBC1;
	}
      }else{
	mask[j]=0;
      }
    }
  }

  TLine **vlines=new TLine*[totCases];
  TLatex **tlabels=new TLatex*[totCases+1];

  printf("Histos merged = %d    totTrials=%d\n",totHistos,totTrials);
  for(Int_t j=0; j<totCases; j++){
    if(mask[j]){
      printf("  %d) %s  -- %d \n",j,histo[j]->GetName(),first[j]);
      vlines[j]=new TLine(last[j],0.,last[j],50000.);
      vlines[j]->SetLineColor(kMagenta+2);
      vlines[j]->SetLineStyle(2);
      TString ttt=histo[j]->GetName();
      ttt.ReplaceAll("hRawYieldTrial","");
      if(ttt.Contains("FixedMean")) ttt="Fix #mu";
      if(ttt.Contains("FixedSp20")) ttt="#sigma+";
      if(ttt.Contains("FixedSm20")) ttt="#sigma-";
      if(ttt.Contains("FreeS")) ttt="Free #sigma";
      ttt.ReplaceAll("FixedS","");
      if(bkgTreat!="" && ttt.Contains(bkgTreat.Data())) ttt.ReplaceAll(bkgTreat.Data(),"");
      tlabels[j]=new TLatex(first[j]+0.02*totTrials,10,ttt.Data());
      tlabels[j]->SetTextColor(kMagenta+2);
      tlabels[j]->SetTextColor(kMagenta+2);
    }
  }
  tlabels[totCases]=new TLatex(totTrials+30,10,"BinCnt");
  tlabels[totCases]->SetTextColor(kMagenta+2);
  tlabels[totCases]->SetTextColor(kMagenta+2);

  TH1F* hRawYieldAll=new TH1F("hRawYieldAll"," ; Trial # ; Raw Yield",totTrials+totTrialsBC0+totTrialsBC1,0.,totTrials+totTrialsBC0+totTrialsBC1);
  TH1F* hRawYieldAllBC0=new TH1F("hRawYieldAllBC0"," ; Trial # ; Raw Yield",totTrialsBC0*nBCranges,totTrials,totTrials+totTrialsBC0);
  TH1F* hRawYieldAllBC1=new TH1F("hRawYieldAllBC1"," ; Trial # ; Raw Yield",totTrialsBC1*nBCranges,totTrials+totTrialsBC0,totTrials+totTrialsBC0+totTrialsBC1);
  TH1F* hMeanAll=new TH1F("hMeanAll"," ; Trial # ; Gaussian mean",totTrials,0.,totTrials);
  TH1F* hSigmaAll=new TH1F("hSigmaAll"," ; Trial # ; Gaussian #sigma",totTrials,0,totTrials);
  TH1F* hChi2All=new TH1F("hChi2All"," ; Trial # ; #chi^{2}",totTrials,0.,totTrials);
  TH1F* hRawYieldDistAll=new TH1F("hRawYieldDistAll","  ; Raw Yield",500,TMath::Nint(minyd*0.85),TMath::Nint(maxyd*1.15));
  TH1F* hRawYieldDistAllBC0=new TH1F("hRawYieldDistAllBC0","  ; Raw Yield",500,TMath::Nint(minyd*0.85),TMath::Nint(maxyd*1.15));
  TH1F* hRawYieldDistAllBC1=new TH1F("hRawYieldDistAllBC1","  ; Raw Yield",500,TMath::Nint(minyd*0.85),TMath::Nint(maxyd*1.15));
  TH1F* hStatErrDistAll=new TH1F("hStatErrDistAll","  ; Stat Unc on Yield",500,TMath::Nint(minyd*0.85),TMath::Nint(maxyd*1.15));
  TH1F* hRelStatErrDistAll=new TH1F("hRelStatErrDistAll","  ; Rel Stat Unc on Yield",100,0.,1.);

  Double_t minYield=999999.;
  Double_t maxYield=0.;
  Double_t sumy[4]={0.,0.,0.,0.};
  Double_t sumwei[4]={0.,0.,0.,0.};
  Double_t sumerr[4]={0.,0.,0.,0.};
  Double_t counts=0.;
  Double_t wei[4];
  Double_t maxFilled=-1;
  for(Int_t j=0; j<totCases; j++){
    if(mask[j]){
      TString hmeanname=histo[j]->GetName();
      hmeanname.ReplaceAll("RawYield","Mean");
      TH1F* hmeant=(TH1F*)fil->Get(hmeanname.Data());

      TString hsigmaname=histo[j]->GetName();
      hsigmaname.ReplaceAll("RawYield","Sigma");
      TH1F* hsigmat=(TH1F*)fil->Get(hsigmaname.Data());

      TString hchi2name=histo[j]->GetName();
      hchi2name.ReplaceAll("RawYield","Chi2");
      TH1F* hchi2t=(TH1F*)fil->Get(hchi2name.Data());
      
      TString hbcname=histo[j]->GetName();
      hbcname.ReplaceAll("Trial","TrialBinC0");
      TH2F* hbc2dt0=(TH2F*)fil->Get(hbcname.Data());
      hbcname.ReplaceAll("BinC0","BinC1");
      TH2F* hbc2dt1=(TH2F*)fil->Get(hbcname.Data());

      for(Int_t ib=1; ib<=histo[j]->GetNbinsX(); ib++){
	Double_t ry=histo[j]->GetBinContent(ib);
	Double_t ery=histo[j]->GetBinError(ib);

	Double_t pos=hmeant->GetBinContent(ib);
	Double_t epos=hmeant->GetBinError(ib);
	
	Double_t sig=hsigmat->GetBinContent(ib);
	Double_t esig=hsigmat->GetBinError(ib);
	
	Double_t chi2=hchi2t->GetBinContent(ib);

	if(ry>0.001 && ery>(0.01*ry) && ery<(0.5*ry) && chi2<chi2Cut){ 
	  hRawYieldDistAll->Fill(ry);
	  hStatErrDistAll->Fill(ery);
	  hRelStatErrDistAll->Fill(ery/ry);
	  hRawYieldAll->SetBinContent(first[j]+ib,ry);
	  hRawYieldAll->SetBinError(first[j]+ib,ery);
	  if(ry<minYield) minYield=ry;
	  if(ry>maxYield) maxYield=ry;
	  wei[0]=1.;
	  wei[1]=1./(ery*ery);
	  wei[2]=1./(ery*ery/(ry*ry));
	  wei[3]=1./(ery*ery/ry);
	  for(Int_t kw=0; kw<4; kw++){
	    sumy[kw]+=wei[kw]*ry;
	    sumerr[kw]+=wei[kw]*wei[kw]*ery*ery;
	    sumwei[kw]+=wei[kw];
	  }
	  counts+=1.;
	  hSigmaAll->SetBinContent(first[j]+ib,sig);
	  hSigmaAll->SetBinError(first[j]+ib,esig);
	  hMeanAll->SetBinContent(first[j]+ib,pos);
	  hMeanAll->SetBinError(first[j]+ib,epos);
	  hChi2All->SetBinContent(first[j]+ib,chi2);
	  hChi2All->SetBinError(first[j]+ib,0.000001);
	  if(mask[j]==2 || mask[j]==4){
	    for(Int_t iy=minBCrange; iy<=maxBCrange;iy++){
	      Double_t bc=hbc2dt0->GetBinContent(ib,iy);
	      Double_t ebc=hbc2dt0->GetBinError(ib,iy);
	      if(bc>0.001 && ebc<0.5*bc && bc<5.*ry){
		Int_t theBin=iy+(firstBC0[j]+ib-1)*nBCranges;
		hRawYieldAllBC0->SetBinContent(theBin,bc);
		hRawYieldAllBC0->SetBinError(theBin,ebc);
		hRawYieldDistAllBC0->Fill(bc);
		if(hRawYieldAllBC0->GetBinCenter(theBin)>maxFilled) maxFilled=hRawYieldAllBC0->GetBinCenter(theBin);
	      }
	    }
	  }
	  if(mask[j]==3 || mask[j]==4){
	    for(Int_t iy=minBCrange; iy<=maxBCrange;iy++){
	      Double_t bc=hbc2dt1->GetBinContent(ib,iy);
	      Double_t ebc=hbc2dt1->GetBinError(ib,iy);
	      if(bc>0.001 && ebc<0.5*bc && bc<5.*ry){
		Int_t theBin=iy+(firstBC1[j]+ib-1)*nBCranges;
		hRawYieldAllBC1->SetBinContent(theBin,bc);
		hRawYieldAllBC1->SetBinError(theBin,ebc);
		hRawYieldDistAllBC1->Fill(bc);
		if(hRawYieldAllBC1->GetBinCenter(theBin)>maxFilled) maxFilled=hRawYieldAllBC1->GetBinCenter(theBin);
	      }
	    }
	  }
	}
      }
    }
  }

  Double_t weiav[4]={0.,0.,0.,0.};
  Double_t eweiav[4]={0.,0.,0.,0.};
  for(Int_t kw=0; kw<4; kw++){
    if(sumwei[kw]>0.){
      weiav[kw]=sumy[kw]/sumwei[kw];
      eweiav[kw]=TMath::Sqrt(sumerr[kw])/sumwei[kw];
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
  hRawYieldDistAllBC0->SetLineColor(colorBC0);
  hRawYieldDistAllBC0->SetLineWidth(2);
  hRawYieldDistAllBC0->SetLineStyle(7);
  hRawYieldDistAllBC0->Scale(hRawYieldDistAll->GetEntries()/hRawYieldDistAllBC0->GetEntries());
  hRawYieldAllBC1->SetMarkerColor(colorBC1);
  hRawYieldAllBC1->SetLineColor(colorBC1);
  hRawYieldDistAllBC1->SetLineColor(colorBC1);
  hRawYieldDistAllBC1->SetLineWidth(2);
  hRawYieldDistAllBC1->SetLineStyle(2);
  hRawYieldDistAllBC1->Scale(hRawYieldDistAll->GetEntries()/hRawYieldDistAllBC1->GetEntries());
  hRawYieldDistAll->SetLineWidth(2);
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
  hRawYieldAll->SetTitle(Form("p_{T} bin %d",iPtBin));
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  Double_t newmax=1.25*(hRawYieldAll->GetMaximum()+hRawYieldAll->GetBinError(1));
  hRawYieldAll->GetYaxis()->SetTitleOffset(1.7);
  hRawYieldAll->SetMaximum(newmax);
  if(maxFilled>0) hRawYieldAll->GetXaxis()->SetRangeUser(0.,maxFilled);
  hRawYieldAll->Draw();
  hRawYieldAllBC0->Draw("same");
  hRawYieldAllBC1->Draw("same");
  TLatex* tweimean[4];
  for(Int_t kw=0; kw<4; kw++){
    tweimean[kw]=new TLatex(0.16,0.84-0.06*kw,Form("<Yield>_{wei%d} = %.1f #pm %.1f\n",kw,weiav[kw],eweiav[kw]*sqrt(counts)));
    tweimean[kw]->SetNDC();
    tweimean[kw]->SetTextColor(4);
    //    tweimean[kw]->Draw();
  }

  for(Int_t j=0; j<totCases; j++){
    if(mask[j]){
      vlines[j]->SetY2(newmax);
      vlines[j]->Draw("same"); 
      tlabels[j]->SetY(0.05*newmax);
      tlabels[j]->Draw();
    }
  }
  tlabels[totCases]->SetY(0.05*newmax);
  tlabels[totCases]->Draw();

  call->cd(5);
  gPad->SetLeftMargin(0.14);
  gPad->SetRightMargin(0.06);
  hRawYieldDistAll->SetTitle(Form("p_{T} bin %d",iPtBin));
  hRawYieldDistAll->Draw();
  //  hRawYieldDistAll->GetXaxis()->SetRangeUser(minYield*0.8,maxYield*1.2);
  hRawYieldDistAllBC0->Draw("sames");
  hRawYieldDistAllBC1->Draw("sames");
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
  Double_t perc[3]={0.15,0.5,0.85}; // quantiles for +-1 sigma
  Double_t lim70[3];
  hRawYieldDistAll->GetQuantiles(3,lim70,perc);
  call->cd(6);
  gPad->SetLeftMargin(0.14);
  gPad->SetRightMargin(0.06);
  Double_t aver=hRawYieldDistAll->GetMean();
  TLatex* tmean=new TLatex(0.15,0.93,Form("mean=%.1f",aver));
  tmean->SetNDC();
  tmean->Draw();
  TLatex* tmedian=new TLatex(0.15,0.86,Form("median=%.1f",lim70[1]));
  tmedian->SetNDC();
  tmedian->Draw();
  Double_t averBC0=hRawYieldDistAllBC0->GetMean();
  TLatex* tmeanBC0=new TLatex(0.15,0.79,Form("mean(BinCount0)=%.1f",averBC0));
  tmeanBC0->SetNDC();
  tmeanBC0->SetTextColor(hRawYieldDistAllBC0->GetLineColor());
  tmeanBC0->Draw();
  Double_t averBC1=hRawYieldDistAllBC1->GetMean();
  TLatex* tmeanBC1=new TLatex(0.15,0.72,Form("mean(BinCount1)=%.1f",averBC1));
  tmeanBC1->SetNDC();
  tmeanBC1->SetTextColor(hRawYieldDistAllBC1->GetLineColor());
  tmeanBC1->Draw();
  Double_t val=hRawYieldDistAll->GetRMS();
  TLatex* thrms=new TLatex(0.15,0.62,Form("rms=%.1f  (%.1f%%)",val,val/aver*100.));
  thrms->SetNDC();
  thrms->Draw();
  TLatex* tmin=new TLatex(0.15,0.52,Form("min=%.1f      max=%.1f",minYield,maxYield));
  tmin->SetNDC();
  tmin->Draw();
  val=(maxYield-minYield)/sqrt(12);
  TLatex* trms=new TLatex(0.15,0.45,Form("(max-min)/sqrt(12)=%.1f  (%.1f%%)",val,val/aver*100.));
  trms->SetNDC();
  trms->Draw();
  val=(maxYield-aver)/sqrt(3);
  TLatex* tup=new TLatex(0.15,0.39,Form("(max-mean)/sqrt(3)=%.1f  (%.1f%%)",val,val/aver*100.));
  tup->SetNDC();
  tup->Draw();
  val=(aver-minYield)/sqrt(3);
  TLatex* tdw=new TLatex(0.15,0.32,Form("(mean-min)/sqrt(3)=%.1f  (%.1f%%)",val,val/aver*100.));
  tdw->SetNDC();
  tdw->Draw();
  TLatex* tl15=new TLatex(0.15,0.22,Form("15 percentile=%.1f",lim70[0]));
  tl15->SetNDC();
  tl15->Draw();
  TLatex* tl85=new TLatex(0.15,0.13,Form("85 percentile=%.1f",lim70[2]));
  tl85->SetNDC();
  tl85->Draw();
  val=(lim70[2]-lim70[0])/2.;
  TLatex* t1s=new TLatex(0.15,0.06,Form("70%% range =%.1f  (%.1f%%)",val,val/aver*100.));
  t1s->SetNDC();
  t1s->Draw();
//   call->SaveAs(Form("MultiTrial.eps",iPtBin));
//   call->SaveAs(Form("MultiTrial.gif",iPtBin));
  for(Int_t kw=0; kw<4; kw++){
    printf("Weight %d: %.1f +- %.1f(stat) +- %.1f (syst)\n",kw,weiav[kw],eweiav[kw]*sqrt(counts),(maxYield-minYield)/sqrt(12));
  }




  TCanvas* c3p=new TCanvas("c3p","3Pad",1400,400);
  c3p->Divide(3,1);
  c3p->cd(1);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.06);
  hRawYieldAll->Draw();
  hRawYieldAllBC0->Draw("same");
  hRawYieldAllBC1->Draw("same");
  for(Int_t j=0; j<totCases; j++){
    if(mask[j]){
      vlines[j]->Draw("same");
      tlabels[j]->Draw();
    }
  }
  tlabels[totCases]->Draw();
  
  c3p->cd(2);
  gPad->SetLeftMargin(0.14);
  gPad->SetRightMargin(0.06);
  hRawYieldDistAll->Draw();
  hRawYieldDistAllBC0->Draw("sames");
  hRawYieldDistAllBC1->Draw("sames");
  c3p->cd(3);
  gPad->SetLeftMargin(0.14);
  gPad->SetRightMargin(0.06);
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
//   c3p->SaveAs(Form("MultiTrial_3pad.eps",iPtBin));
//   c3p->SaveAs(Form("MultiTrial_3pad.gif",iPtBin));

  TCanvas* c2p=new TCanvas("c2p","2Pad",933,400);
  c2p->Divide(2,1);
  c2p->cd(1);
  gPad->SetLeftMargin(0.14);
  gPad->SetRightMargin(0.06);
  hRawYieldDistAll->Draw();
  hRawYieldDistAllBC0->Draw("sames");
  hRawYieldDistAllBC1->Draw("sames");
  c2p->cd(2);
  gPad->SetLeftMargin(0.14);
  gPad->SetRightMargin(0.06);
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
//   c2p->SaveAs(Form("MultiTrial_2pad.eps",iPtBin));

//   TString outn = filnam.Data();
//   outn.Prepend(Form("%sComb",sysdir.Data()));
//   cout << outn.Data() << endl;
//   TFile *outf = new TFile(outn.Data(), "RECREATE");
//   call->Write();
//   outf->Write();
//   outf->Close();
//   TString callname = outn.Data();
//   callname.ReplaceAll(".root",".pdf");
//   call->Print(callname);
//   callname.ReplaceAll(".root",".png");
//   call->Print(callname);
}

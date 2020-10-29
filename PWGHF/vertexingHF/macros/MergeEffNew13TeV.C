TString datadir = "~/alice/D0_pp13TeV/ROOTfiles/data/lowpt/LHC";
TString effdir = "~/alice/D0_pp13TeV/results/efficiency/central_cuts/LHC";
//TString effdir = "~/alice/D0_pp13TeV/results/efficiency/PIDcuts/LHC";
TString year[5] = {"2016","2016","2016","2017","2018"};
TString subdir[5] = {"_deghjop","_k","_l","_cefhijklmor","_bdefghijklmnop"};
TString date = "7nov";
TString suffix = "3SigPID_Pt400_YFid_PileUpMV";

void MergeEffNew13TeV(){

  gStyle->SetOptTitle(0);
  
  const Int_t nper = 5;
  Double_t weight[nper];
  TH1F* hEvForNormData = new TH1F("hEvForNorm","",nper,-0.5,nper-0.5);

  Bool_t foundNorm=kFALSE;
  for(Int_t j=0; j<nper; j++){
    TString fname;
    if (j==0)
      fname = Form("%s%s_AOD208%s/%s/AnalysisResults_wo16otpc.root",datadir.Data(),year[j].Data(),subdir[j].Data(),date.Data());
    else 
      fname = Form("%s%s_AOD208%s/%s/AnalysisResults.root",datadir.Data(),year[j].Data(),subdir[j].Data(),date.Data());
    TFile* f=new TFile(fname.Data());
    TDirectoryFile* df=(TDirectoryFile*)f->Get(Form("PWG3_D2H_InvMassDzeroLowPt%s",suffix.Data()));
    foundNorm=kFALSE;
    
    if(!foundNorm){
      AliNormalizationCounter* cnt=(AliNormalizationCounter*)df->Get("NormalizationCounter");
      hEvForNormData->SetBinContent(j+1,cnt->GetNEventsForNorm());
      hEvForNormData->GetXaxis()->SetBinLabel(j+1,subdir[j].Data());
      weight[j]=hEvForNormData->GetBinContent(j+1);
      foundNorm=kTRUE;
    }
  }

//   printf("---- Events for norm in data ---\n");
//   for (Int_t j=0; j<nper; j++) 
//     printf("LHC%s%s: %7.3f M\n",year[j].Data(),subdir[j].Data(),hEvForNormData->GetBinContent(j+1)/1e6);
//   printf("TOT: %7.3f M\n",hEvForNormData->Integral()/1e6);


  TH1D** heff_pmpt = new TH1D*[nper];
  TH1D** heff_fddw = new TH1D*[nper];
  TH1D** hacceff_pmpt = new TH1D*[nper];
  TH1D** hacceff_fddw = new TH1D*[nper];
  TCanvas *ceff_pmpt = new TCanvas("PromptEff","PromptEff",800,600);
  TCanvas *ceff_fddw = new TCanvas("Feed-downEff","Feed-downEff",800,600);
  TCanvas *cacceff_pmpt = new TCanvas("PromptAccEff","PromptAccEff",800,600);
  TCanvas *cacceff_fddw = new TCanvas("Feed-downAccEff","Feed-downAccEff",800,600);
  TString hnamepmpt = "hEffPromptVsPtNoWeight";
  TString hnamefddw = "hEffFeeddwVsPtNoWeight";
  TString hnameacceffp = "hEffD";
  TString hnameaccefff = "hEffB";
  TH1D* hacc;
  for (Int_t j=0;j<nper; j++) {
    TFile *fileeff;
    if (j==0)
      fileeff = TFile::Open(Form("%s%s%s/test/outputEff_Prompt_%s_wo16otpc.root", effdir.Data(),year[j].Data(),subdir[j].Data(),suffix.Data()));
    else
      fileeff = TFile::Open(Form("%s%s%s/outputEff_Prompt_%s.root", effdir.Data(),year[j].Data(),subdir[j].Data(),suffix.Data()));
    heff_pmpt[j] = (TH1D*)fileeff->Get(hnamepmpt.Data());
    heff_fddw[j] = (TH1D*)fileeff->Get(hnamefddw.Data());
    hacceff_pmpt[j] = (TH1D*)fileeff->Get(hnameacceffp.Data());
    hacceff_fddw[j] = (TH1D*)fileeff->Get(hnameaccefff.Data());
    if (j==0) 
      hacc = (TH1D*) fileeff->Get("hAccToy");

    switch (j) {
    case 0: 
      heff_pmpt[j]->SetMarkerStyle(20); heff_pmpt[j]->SetMarkerColor(kBlack);
      heff_pmpt[j]->SetLineColor(kBlack); 
      heff_fddw[j]->SetMarkerStyle(20); heff_fddw[j]->SetMarkerColor(kBlack);
      heff_fddw[j]->SetLineColor(kBlack); 
      hacceff_pmpt[j]->SetMarkerStyle(20); hacceff_pmpt[j]->SetMarkerColor(kBlack);
      hacceff_pmpt[j]->SetLineColor(kBlack); 
      hacceff_fddw[j]->SetMarkerStyle(20); hacceff_fddw[j]->SetMarkerColor(kBlack);
      hacceff_fddw[j]->SetLineColor(kBlack); 
      break;
    case 1:
      heff_pmpt[j]->SetMarkerStyle(21); heff_pmpt[j]->SetMarkerColor(kRed);
      heff_pmpt[j]->SetLineColor(kRed); 
      heff_fddw[j]->SetMarkerStyle(21); heff_fddw[j]->SetMarkerColor(kRed);
      heff_fddw[j]->SetLineColor(kRed); 
      hacceff_pmpt[j]->SetMarkerStyle(21); hacceff_pmpt[j]->SetMarkerColor(kRed);
      hacceff_pmpt[j]->SetLineColor(kRed); 
      hacceff_fddw[j]->SetMarkerStyle(21); hacceff_fddw[j]->SetMarkerColor(kRed);
      hacceff_fddw[j]->SetLineColor(kRed); 
      break;
    case 2:
      heff_pmpt[j]->SetMarkerStyle(23); heff_pmpt[j]->SetMarkerColor(kBlue);
      heff_pmpt[j]->SetLineColor(kBlue); 
      heff_fddw[j]->SetMarkerStyle(23); heff_fddw[j]->SetMarkerColor(kBlue);
      heff_fddw[j]->SetLineColor(kBlue); 
      hacceff_pmpt[j]->SetMarkerStyle(23); hacceff_pmpt[j]->SetMarkerColor(kBlue);
      hacceff_pmpt[j]->SetLineColor(kBlue); 
      hacceff_fddw[j]->SetMarkerStyle(23); hacceff_fddw[j]->SetMarkerColor(kBlue);
      hacceff_fddw[j]->SetLineColor(kBlue); 
      break;
    case 3:
      heff_pmpt[j]->SetMarkerStyle(26); heff_pmpt[j]->SetMarkerColor(kGreen);
      heff_pmpt[j]->SetLineColor(kGreen);
      heff_fddw[j]->SetMarkerStyle(26); heff_fddw[j]->SetMarkerColor(kGreen);
      heff_fddw[j]->SetLineColor(kGreen); 
      hacceff_pmpt[j]->SetMarkerStyle(26); hacceff_pmpt[j]->SetMarkerColor(kGreen);
      hacceff_pmpt[j]->SetLineColor(kGreen);
      hacceff_fddw[j]->SetMarkerStyle(26); hacceff_fddw[j]->SetMarkerColor(kGreen);
      hacceff_fddw[j]->SetLineColor(kGreen); 
      break;
    case 4:
      heff_pmpt[j]->SetMarkerStyle(33); heff_pmpt[j]->SetMarkerColor(kMagenta);
      heff_pmpt[j]->SetLineColor(kMagenta);
      heff_fddw[j]->SetMarkerStyle(33); heff_fddw[j]->SetMarkerColor(kMagenta);
      heff_fddw[j]->SetLineColor(kMagenta); 
      hacceff_pmpt[j]->SetMarkerStyle(33); hacceff_pmpt[j]->SetMarkerColor(kMagenta);
      hacceff_pmpt[j]->SetLineColor(kMagenta);
      hacceff_fddw[j]->SetMarkerStyle(33); hacceff_fddw[j]->SetMarkerColor(kMagenta);
      hacceff_fddw[j]->SetLineColor(kMagenta); 
      break;
    }

    // efficiency plots
    ceff_pmpt->cd();
    heff_pmpt[j]->SetStats(0);
    heff_pmpt[j]->GetYaxis()->SetRangeUser(0.,1.);
    heff_pmpt[j]->GetYaxis()->SetTitle("Efficiency");
    heff_pmpt[j]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    heff_pmpt[j]->Draw("same");
    heff_pmpt[j]->SetTitle(Form("LHC%s%s",year[j].Data(),subdir[j].Data()));
    ceff_fddw->cd();
    heff_fddw[j]->SetStats(0);
    heff_fddw[j]->GetYaxis()->SetRangeUser(0.,1.);
    heff_fddw[j]->GetYaxis()->SetTitle("Efficiency");
    heff_fddw[j]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    heff_fddw[j]->Draw("same");
    heff_fddw[j]->SetTitle(Form("LHC%s%s",year[j].Data(),subdir[j].Data()));
    // acceptance x efficiency plots
    cacceff_pmpt->cd();
    hacceff_pmpt[j]->SetStats(0);
    hacceff_pmpt[j]->GetYaxis()->SetRangeUser(0.,1.);
    hacceff_pmpt[j]->GetYaxis()->SetTitle("Acceptance x Efficiency");
    hacceff_pmpt[j]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hacceff_pmpt[j]->Draw("same");
    hacceff_pmpt[j]->SetTitle(Form("LHC%s%s",year[j].Data(),subdir[j].Data()));
    cacceff_fddw->cd();
    hacceff_fddw[j]->SetStats(0);
    hacceff_fddw[j]->GetYaxis()->SetRangeUser(0.,1.);
    hacceff_fddw[j]->GetYaxis()->SetTitle("Acceptance x Efficiency");
    hacceff_fddw[j]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hacceff_fddw[j]->Draw("same");
    hacceff_fddw[j]->SetTitle(Form("LHC%s%s",year[j].Data(),subdir[j].Data()));
  }


  TH1D* heffall_pmpt = (TH1D*)heff_pmpt[0]->Clone("heffall_pmpt");
  heffall_pmpt->Reset();
  TH1D* heffall_fddw = (TH1D*)heff_fddw[0]->Clone("heffall_fddw");
  heffall_fddw->Reset();
  TH1D* hacceffall_pmpt = (TH1D*)hacceff_pmpt[0]->Clone("hacceffall_pmpt");
  hacceffall_pmpt->Reset();
  TH1D* hacceffall_fddw = (TH1D*)hacceff_fddw[0]->Clone("hacceffall_fddw");
  hacceffall_fddw->Reset();

  Double_t effPer_pmpt[nper], effPer_fddw[nper];
  Double_t acceffPer_pmpt[nper], acceffPer_fddw[nper];
  Double_t sump=0, sumpw=0, sumf=0, sumfw=0;
  Double_t sum2p=0, sum2pw=0, sum2f=0, sum2fw=0;
  for(Int_t ib=1; ib<=heff_pmpt[0]->GetNbinsX(); ib++){
    sump=0;     sumpw=0;
    sumf=0;     sumfw=0;
    sum2p=0;    sum2pw=0;
    sum2f=0;    sum2fw=0;
    for(Int_t j=0; j<nper; j++){ 
      // EFFICIENCY
      // prompt
      effPer_pmpt[j]=heff_pmpt[j]->GetBinContent(ib);
      sump+=effPer_pmpt[j]*weight[j];
      sumpw+=weight[j];
      // feed-down
      effPer_fddw[j]=heff_fddw[j]->GetBinContent(ib);
      sumf+=effPer_fddw[j]*weight[j];
      sumfw+=weight[j];
      // ACCEPTANCE X EFFICIENCY
      // prompt
      acceffPer_pmpt[j]=hacceff_pmpt[j]->GetBinContent(ib);
      sum2p+=acceffPer_pmpt[j]*weight[j];
      sum2pw+=weight[j];
      // feed-down
      acceffPer_fddw[j]=hacceff_fddw[j]->GetBinContent(ib);
      sum2f+=acceffPer_fddw[j]*weight[j];
      sum2fw+=weight[j];
    }
    // prompt eff
    heffall_pmpt->SetBinContent(ib,sump/sumpw);
    Double_t err_pmpt = TMath::Sqrt(1./sumpw);
    heffall_pmpt->SetBinError(ib,err_pmpt);
    // feed-down eff
    heffall_fddw->SetBinContent(ib,sumf/sumfw);
    Double_t err_fddw = TMath::Sqrt(1./sumfw);
    heffall_fddw->SetBinError(ib,err_fddw);
    // prompt acc x eff
    hacceffall_pmpt->SetBinContent(ib,sum2p/sum2pw);
    Double_t err2_pmpt = TMath::Sqrt(1./sum2pw);
    hacceffall_pmpt->SetBinError(ib,err2_pmpt);
    // feed-down acc x eff
    hacceffall_fddw->SetBinContent(ib,sum2f/sum2fw);
    Double_t err2_fddw = TMath::Sqrt(1./sum2fw);
    hacceffall_fddw->SetBinError(ib,err2_fddw);
  }
  // efficiency
  ceff_pmpt->cd();
  heffall_pmpt->SetMarkerStyle(3);
  heffall_pmpt->SetMarkerColor(kCyan);
  heffall_pmpt->SetLineColor(kCyan);
  heffall_pmpt->SetStats(0);
  heffall_pmpt->SetTitle("Weighted mean");
  heffall_pmpt->GetYaxis()->SetRangeUser(0.,1.);
  heffall_pmpt->Draw("same");
  ceff_fddw->cd();
  heffall_fddw->SetMarkerStyle(3);
  heffall_fddw->SetMarkerColor(kCyan);
  heffall_fddw->SetLineColor(kCyan);
  heffall_fddw->SetStats(0);
  heffall_fddw->SetTitle("Weighted mean");
  heffall_fddw->GetYaxis()->SetRangeUser(0.,1.);
  heffall_fddw->GetYaxis()->SetTitle("Efficiency");
  heffall_fddw->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  heffall_fddw->Draw("same");
  ceff_pmpt->BuildLegend();
  ceff_fddw->BuildLegend();
  // acceptance x efficiency
  cacceff_pmpt->cd();
  hacceffall_pmpt->SetMarkerStyle(3);
  hacceffall_pmpt->SetMarkerColor(kCyan);
  hacceffall_pmpt->SetLineColor(kCyan);
  hacceffall_pmpt->SetStats(0);
  hacceffall_pmpt->SetTitle("Weighted mean");
  hacceffall_pmpt->GetYaxis()->SetRangeUser(0.,1.);
  hacceffall_pmpt->Draw("same");
  cacceff_fddw->cd();
  hacceffall_fddw->SetMarkerStyle(3);
  hacceffall_fddw->SetMarkerColor(kCyan);
  hacceffall_fddw->SetLineColor(kCyan);
  hacceffall_fddw->SetStats(0);
  hacceffall_fddw->SetTitle("Weighted mean");
  hacceffall_fddw->GetYaxis()->SetRangeUser(0.,1.);
  hacceffall_fddw->GetYaxis()->SetTitle("Efficiency");
  hacceffall_fddw->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hacceffall_fddw->Draw("same");
  cacceff_pmpt->BuildLegend();
  cacceff_fddw->BuildLegend();

  // efficiency
  TCanvas *cratiop = new TCanvas("effratio_pmpt","effratio_pmpt",800,600);
  TCanvas *cratiof = new TCanvas("effratio_fddw","effratio_fddw",800,600);
  TH1D *hrpmpt[nper], *hrfddw[nper];

  for (int j=0;j<nper;j++) {
    hrpmpt[j] = (TH1D*) heff_pmpt[j]->Clone();
    hrpmpt[j]->Divide(heff_pmpt[j], heffall_pmpt,1,1,"B");
    cratiop->cd();
    hrpmpt[j]->GetYaxis()->SetTitle("Efficiency ratio");
    hrpmpt[j]->GetYaxis()->SetRangeUser(0.8,1.2);
    hrpmpt[j]->SetTitle(Form("LHC%s%s/weighted mean",year[j].Data(),subdir[j].Data()));
    hrpmpt[j]->Draw("same");
    hrfddw[j] = (TH1D*) heff_fddw[j]->Clone();
    hrfddw[j]->Divide(heff_fddw[j], heffall_fddw,1,1,"B");
    cratiof->cd();
    hrfddw[j]->GetYaxis()->SetTitle("Efficiency ratio");
    hrfddw[j]->GetYaxis()->SetRangeUser(0.8,1.2);
    hrfddw[j]->SetTitle(Form("LHC%s%s/weighted mean",year[j].Data(),subdir[j].Data()));
    hrfddw[j]->Draw("same");
  }
  TLine *ll = new TLine(0., 1., 12., 1.);
  ll->SetLineStyle(2);
  ll->SetLineColor(kBlack);
  cratiop->cd();
  cratiop->BuildLegend();
  ll->Draw("same");
  cratiof->cd();
  cratiof->BuildLegend();
  ll->Draw("same");

  TCanvas *cratiopf = new TCanvas("ratio_pmpt-fddw","ratio_pmpt-fddw",800,600);
  TH1D* hratio = (TH1D*) heffall_pmpt->Clone();
  hratio->Divide(heffall_pmpt,heffall_fddw,1,1,"B");
  hratio->SetTitle("prompt/feed-down");
  hratio->SetMarkerColor(kBlack);
  hratio->SetLineColor(kBlack);
  hratio->GetYaxis()->SetTitle("Efficiency ratio");
  hratio->GetYaxis()->SetTitleOffset(1.4);
  hratio->GetYaxis()->SetRangeUser(0.98,1.02);
  hratio->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  cratiopf->cd();
  hratio->Draw();
  cratiopf->BuildLegend();
  ll->Draw("same");

  // output file
  TFile *coutput = new TFile(Form("~/alice/D0_pp13TeV/results/efficiency/central_cuts/all/outputEff_Weighted_%s_wo16otpc.root",suffix.Data()), "RECREATE");
  //  TFile *coutput = new TFile(Form("~/alice/D0_pp13TeV/results/efficiency/PIDcuts/all/outputEff_Weighted_%s.root",suffix.Data()), "RECREATE");
  heffall_pmpt->SetName(hnamepmpt.Data());
  heffall_fddw->SetName(hnamefddw.Data());
  heffall_pmpt->Write();
  heffall_fddw->Write();
  hratio->SetName("PromptToFeeddown");
  hratio->Write();
  ceff_pmpt->Write();
  ceff_fddw->Write();
  cratiop->Write();
  cratiof->Write();
  cratiopf->Write();
  hacceffall_pmpt->SetName(hnameacceffp.Data());
  hacceffall_fddw->SetName(hnameaccefff.Data());
  hacceffall_pmpt->Write();
  hacceffall_fddw->Write();
  cacceff_pmpt->Write();
  cacceff_fddw->Write();
  hacc->Write();
  coutput->Write();
  coutput->Close();

}

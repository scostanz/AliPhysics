TString datadir = "~/alice/D0_13TeV_lowpt/ROOTfiles/data/lowpt/LHC";
TString effdir = "~/alice/D0_13TeV_lowpt/results/efficiency/central_cuts/";
TString year[6] = {"2016","2016","2016","2017","2018","all"};
TString subdir[6] = {"_deghjop","_k","_l","_cefhijklmor","_bdefghijklmnop","/noweights"};
TString date = "23nov";
TString suffix = "3SigPID_Pt400_YFid_PileUpMV";

void ratio_eff(){

  gStyle->SetOptTitle(0);
  
  const Int_t nper = 6;
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
      if (j!=nper-1)
	fileeff	= TFile::Open(Form("%sLHC%s%s/outputEff_Prompt_%s.root", effdir.Data(),year[j].Data(),subdir[j].Data(),suffix.Data()));
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
    case 5:
      heff_pmpt[j]->SetMarkerStyle(3); heff_pmpt[j]->SetMarkerColor(kCyan);
      heff_pmpt[j]->SetLineColor(kCyan);
      heff_fddw[j]->SetMarkerStyle(3); heff_fddw[j]->SetMarkerColor(kCyan);
      heff_fddw[j]->SetLineColor(kCyan); 
      hacceff_pmpt[j]->SetMarkerStyle(3); hacceff_pmpt[j]->SetMarkerColor(kCyan);
      hacceff_pmpt[j]->SetLineColor(kCyan);
      hacceff_fddw[j]->SetMarkerStyle(3); hacceff_fddw[j]->SetMarkerColor(kCyan);
      hacceff_fddw[j]->SetLineColor(kCyan); 
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

  ceff_pmpt->BuildLegend();
  ceff_fddw->BuildLegend();
  cacceff_pmpt->BuildLegend();
  cacceff_fddw->BuildLegend();

  TH1D* heffall_pmpt = (TH1D*)heff_pmpt[nper-1]->Clone("heffall_pmpt");
  TH1D* heffall_fddw = (TH1D*)heff_fddw[nper-1]->Clone("heffall_fddw");
  TH1D* hacceffall_pmpt = (TH1D*)hacceff_pmpt[nper-1]->Clone("hacceffall_pmpt");
  TH1D* hacceffall_fddw = (TH1D*)hacceff_fddw[nper-1]->Clone("hacceffall_fddw");

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
    hrpmpt[j]->SetTitle(Form("LHC%s%s/all",year[j].Data(),subdir[j].Data()));
    hrpmpt[j]->Draw("same");
    hrfddw[j] = (TH1D*) heff_fddw[j]->Clone();
    hrfddw[j]->Divide(heff_fddw[j], heffall_fddw,1,1,"B");
    cratiof->cd();
    hrfddw[j]->GetYaxis()->SetTitle("Efficiency ratio");
    hrfddw[j]->GetYaxis()->SetRangeUser(0.8,1.2);
    hrfddw[j]->SetTitle(Form("LHC%s%s/all",year[j].Data(),subdir[j].Data()));
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
  TFile *coutput = new TFile(Form("~/alice/D0_13TeV_lowpt/results/efficiency/central_cuts/all/outputEff_all_%s.root",suffix.Data()), "RECREATE");
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

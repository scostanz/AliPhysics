TString datadir = "~/alice/D0_pp13TeV/ROOTfiles/data/lowpt/LHC";
TString effdir = "~/alice/D0_pp13TeV/results/efficiency/PID_cuts/";
TString year[3] = {"2016","2017","2018"};
TString subdir[6] = {"","_cefhijklmor","_bdefghijklmnop"};
TString date = "23nov";
TString suffix = "NoPID_Pt400_YFid_PileUpMV";

void ratio_eff_pid2(){

  gStyle->SetOptTitle(0);
  
  const Int_t nper = 3;
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
    fileeff	= TFile::Open(Form("%sLHC%s%s/outputEff_Prompt_%s.root", effdir.Data(),year[j].Data(),subdir[j].Data(),suffix.Data()));
    heff_pmpt[j] = (TH1D*)fileeff->Get(hnamepmpt.Data());
    heff_fddw[j] = (TH1D*)fileeff->Get(hnamefddw.Data());
    hacceff_pmpt[j] = (TH1D*)fileeff->Get(hnameacceffp.Data());
    hacceff_fddw[j] = (TH1D*)fileeff->Get(hnameaccefff.Data());
    if (j==0) 
      hacc = (TH1D*) fileeff->Get("hAccToy");

    switch (j) {
    case 0: 
      heff_pmpt[j]->SetMarkerStyle(20); heff_pmpt[j]->SetMarkerColor(kBlue);
      heff_pmpt[j]->SetLineColor(kBlue); 
      heff_fddw[j]->SetMarkerStyle(20); heff_fddw[j]->SetMarkerColor(kBlue);
      heff_fddw[j]->SetLineColor(kBlue); 
      hacceff_pmpt[j]->SetMarkerStyle(20); hacceff_pmpt[j]->SetMarkerColor(kBlue);
      hacceff_pmpt[j]->SetLineColor(kBlue); 
      hacceff_fddw[j]->SetMarkerStyle(20); hacceff_fddw[j]->SetMarkerColor(kBlue);
      hacceff_fddw[j]->SetLineColor(kBlue); 
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
      heff_pmpt[j]->SetMarkerStyle(23); heff_pmpt[j]->SetMarkerColor(kGreen);
      heff_pmpt[j]->SetLineColor(kGreen); 
      heff_fddw[j]->SetMarkerStyle(23); heff_fddw[j]->SetMarkerColor(kGreen);
      heff_fddw[j]->SetLineColor(kGreen); 
      hacceff_pmpt[j]->SetMarkerStyle(23); hacceff_pmpt[j]->SetMarkerColor(kGreen);
      hacceff_pmpt[j]->SetLineColor(kGreen); 
      hacceff_fddw[j]->SetMarkerStyle(23); hacceff_fddw[j]->SetMarkerColor(kGreen);
      hacceff_fddw[j]->SetLineColor(kGreen); 
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

  // output file
  TFile *coutput = new TFile(Form("~/alice/D0_pp13TeV/results/efficiency/PID_cuts/comparison/outputEff_%s.root",suffix.Data()), "RECREATE");
  heffall_pmpt->SetName(hnamepmpt.Data());
  heffall_fddw->SetName(hnamefddw.Data());
  heffall_pmpt->Write();
  heffall_fddw->Write();
  ceff_pmpt->Write();
  ceff_fddw->Write();
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

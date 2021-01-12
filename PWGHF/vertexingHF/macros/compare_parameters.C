#include <TH1D.h>
#include <TDatabasePDG.h>

void compare_parameters() {
  
  gStyle->SetOptTitle(0);

  TString fname = "outputMassFits_FixedSigmaAll_Refl_3SigPID_Pt400_YFid_PileUpMV.root";
  //  TString fname = "outputMassFits_FreeSigma_Refl_3SigPID_Pt400_YFid_PileUpMV.root";
  TString inputdir = "~/alice/D0_13TeV_lowpt/results/figures/all/templ/V0100range/";

  TString hsuffix[4] = {"Rot","LS","ME","SB"};

  TCanvas *cmean   = new TCanvas("cmean",   "cmean",   800, 600);
  TCanvas *csigma  = new TCanvas("csigma",  "csigma",  800, 600);
  TCanvas *csignif = new TCanvas("csignif", "csignof", 800, 600);
  TCanvas *csoverb = new TCanvas("csoverb", "csoverb", 800, 600);

  Double_t massD = TDatabasePDG::Instance()->GetParticle(421)->Mass(); ;

  TFile *inFile = TFile::Open(Form("%s%s",inputdir.Data(),fname.Data()));
  if (!inFile) {
    cout << "-E- Input file " << Form("%s%s",inputdir.Data(),fname.Data())<< " not found" << endl;
    exit(0);
  }
    
  TH1D *hmean[4], *hsigma[4], *hsignif[4], *hsoverb[4];

  for (Int_t i=0; i<4; i++) {
    hmean[i] = (TH1D*) inFile->Get(Form("hGausMean%s",hsuffix[i].Data()));
    if (!hmean[i]) { cout << "-E- Histogram " << Form("hGausMean%s",hsuffix[i].Data()) << " not found" << endl; exit(0); }
    hsigma[i] = (TH1D*) inFile->Get(Form("hGausSigma%s",hsuffix[i].Data()));
    if (!hsigma[i]) { cout << "-E- Histogram " << Form("hGausSigma%s",hsuffix[i].Data()) << " not found" << endl; exit(0); }
    hsignif[i] = (TH1D*) inFile->Get(Form("hSignif%s",hsuffix[i].Data()));
    if (!hsignif[i]) { cout << "-E- Histogram " << Form("hSignif%s",hsuffix[i].Data()) << " not found" << endl; exit(0); }
    hsoverb[i] = (TH1D*) inFile->Get(Form("hSoverB%s",hsuffix[i].Data()));
    if (!hsoverb[i]) { cout << "-E- Histogram " << Form("hSoverB%s",hsuffix[i].Data()) << " not found" << endl; exit(0); }

    switch(i) {
    case 0:  
      hmean[i]->SetLineColor(kBlack);      hmean[i]->SetMarkerColor(kBlack);     hmean[i]->SetMarkerStyle(21); 
      hsigma[i]->SetLineColor(kBlack);     hsigma[i]->SetMarkerColor(kBlack);    hsigma[i]->SetMarkerStyle(21);
      hsignif[i]->SetLineColor(kBlack);    hsignif[i]->SetMarkerColor(kBlack);   hsignif[i]->SetMarkerStyle(21);
      hsoverb[i]->SetLineColor(kBlack);    hsoverb[i]->SetMarkerColor(kBlack);   hsoverb[i]->SetMarkerStyle(21);
      break;
    case 1:  
      hmean[i]->SetLineColor(kGreen);      hmean[i]->SetMarkerColor(kGreen);     hmean[i]->SetMarkerStyle(22); 
      hsigma[i]->SetLineColor(kGreen);     hsigma[i]->SetMarkerColor(kGreen);    hsigma[i]->SetMarkerStyle(22);
      hsignif[i]->SetLineColor(kGreen);    hsignif[i]->SetMarkerColor(kGreen);   hsignif[i]->SetMarkerStyle(22);
      hsoverb[i]->SetLineColor(kGreen);    hsoverb[i]->SetMarkerColor(kGreen);   hsoverb[i]->SetMarkerStyle(22);
      break;
    case 2:
      hmean[i]->SetLineColor(kBlue);      hmean[i]->SetMarkerColor(kBlue);     hmean[i]->SetMarkerStyle(25); 
      hsigma[i]->SetLineColor(kBlue);     hsigma[i]->SetMarkerColor(kBlue);    hsigma[i]->SetMarkerStyle(25);
      hsignif[i]->SetLineColor(kBlue);    hsignif[i]->SetMarkerColor(kBlue);   hsignif[i]->SetMarkerStyle(25);
      hsoverb[i]->SetLineColor(kBlue);    hsoverb[i]->SetMarkerColor(kBlue);   hsoverb[i]->SetMarkerStyle(25);
      break;
    case 3: 
      hmean[i]->SetLineColor(kPink);      hmean[i]->SetMarkerColor(kPink);     hmean[i]->SetMarkerStyle(27); 
      hsigma[i]->SetLineColor(kPink);     hsigma[i]->SetMarkerColor(kPink);    hsigma[i]->SetMarkerStyle(27);
      hsignif[i]->SetLineColor(kPink);    hsignif[i]->SetMarkerColor(kPink);   hsignif[i]->SetMarkerStyle(27);
      hsoverb[i]->SetLineColor(kPink);    hsoverb[i]->SetMarkerColor(kPink);   hsoverb[i]->SetMarkerStyle(27);
      break;
    }
    // gaussian mean
    cmean->cd();
    hmean[i]->GetYaxis()->SetRangeUser(1.86,1.88);
    hmean[i]->Draw("PESAME");
    hmean[i]->SetTitle("");
    hmean[i]->SetStats(0);
    hmean[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hmean[i]->GetYaxis()->SetTitle("Gaussian mean (GeV/c^{2})");
    hmean[i]->GetYaxis()->SetTitleOffset(1.4);
    hmean[i]->SetName(Form("GausMean%s",hsuffix[i].Data()));
    hmean[i]->SetTitle(Form("GausMean%s",hsuffix[i].Data()));
    // gaussian sigma
    csigma->cd();
    hsigma[i]->Draw("PESAME");
    hsigma[i]->SetTitle("");
    hsigma[i]->SetStats(0);
    hsigma[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hsigma[i]->GetYaxis()->SetTitle("Gaussian sigma (GeV/c^{2})");
    hsigma[i]->GetYaxis()->SetTitleOffset(1.4);
    hsigma[i]->SetName(Form("GausSigma%s",hsuffix[i].Data()));
    hsigma[i]->SetTitle(Form("GausSigma%s",hsuffix[i].Data()));
    // significance
    csignif->cd();
    hsignif[i]->GetYaxis()->SetRangeUser(0,50);
    hsignif[i]->Draw("PESAME");
    hsignif[i]->SetTitle("");
    hsignif[i]->SetStats(0);
    hsignif[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hsignif[i]->GetYaxis()->SetTitle("Significance");
    hsignif[i]->SetName(Form("Signif%s",hsuffix[i].Data()));
    hsignif[i]->SetTitle(Form("Signif%s",hsuffix[i].Data()));
    // 
    csoverb->cd();
    hsoverb[i]->GetYaxis()->SetRangeUser(0,0.04);
    hsoverb[i]->Draw("PESAME");
    hsoverb[i]->SetTitle("");
    hsoverb[i]->SetStats(0);
    hsoverb[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hsoverb[i]->GetYaxis()->SetTitle("S/B");
    hsoverb[i]->GetYaxis()->SetTitleOffset(1.4);
    hsoverb[i]->SetName(Form("Soverb%s",hsuffix[i].Data()));
    hsoverb[i]->SetTitle(Form("Soverb%s",hsuffix[i].Data()));
  }
  cmean->BuildLegend();
  TLine *ll = new TLine(0,massD,12,massD);
  ll->SetLineStyle(2);
  ll->SetLineColor(kBlack);
  cmean->cd();
  ll->Draw("same");
  csigma->BuildLegend();
  csignif->BuildLegend();
  csoverb->BuildLegend();

  TCanvas *call = new TCanvas("call","call", 1200,800);
  call->Divide(2,2);
  call->cd(1); cmean->DrawClonePad();
  call->cd(2); csigma->DrawClonePad();
  call->cd(3); csignif->DrawClonePad();
  call->cd(4); csoverb->DrawClonePad();
  call->Draw();
			      
  TFile *fout = new TFile(Form("%sCompare_IMparameters.root",inputdir.Data()), "RECREATE");
  cmean->Write();
  csigma->Write();
  csignif->Write();
  csoverb->Write();
  fout->Write();
  fout->Close();
//   cmean->Print(Form("%sComparison_mean.png",inputdir.Data()));
//   csigma->Print(Form("%sComparison_sigma.png",inputdir.Data()));
//   csignif->Print(Form("%sComparison_signif.png",inputdir.Data()));
//   csoverb->Print(Form("%sComparison_soverb.png",inputdir.Data()));
}

#include <TH1D.h>
#include <TDatabasePDG.h>

void compare_parameters_periods() {
  
  gStyle->SetOptTitle(0);

  const Int_t nMax = 4;

  //  TString fname = "outputMassFits_FreeSigma_Refl_3SigPID_Pt400_YFid_PileUpMV.root";
  TString fname = "outputMassFits_FixedSigmaAll_Refl_3SigPID_Pt400_YFid_PileUpMV.root";
  TString inputdir = "~/alice/D0_13TeV_lowpt/results/figures/";
  TString dataset[nMax] = {"LHC2016","LHC2017","LHC2018","all"};
  TString legend[nMax] = {"2016","2017","2018","merged datasets"};
//   TString dataset[nMax] = {"LHC2016","LHC2017","LHC2018"};
//   TString legend[nMax] = {"2016 fixed #sigma","2017 fixed #sigma","2018 fixed #sigma"};

  TCanvas *cmean   = new TCanvas("cmean",   "cmean",   800, 600);
  TCanvas *csigma  = new TCanvas("csigma",  "csigma",  800, 600);
  TCanvas *csignif = new TCanvas("csignif", "csignof", 800, 600);
  TCanvas *csoverb = new TCanvas("csoverb", "csoverb", 800, 600);

  Double_t massD = TDatabasePDG::Instance()->GetParticle(421)->Mass(); ;

  TFile *inFile[nMax];
    
  TH1D *hmean[nMax], *hsigma[nMax], *hsignif[nMax], *hsoverb[nMax];

  for (Int_t i=0; i<nMax; i++) {
    inFile[i] = TFile::Open(Form("%s%s/templ/%s",inputdir.Data(),dataset[i].Data(), fname.Data()));
    if (!inFile[i]) {
      cout << "-E- Input file " << Form("%s%s/templ/%s",inputdir.Data(),dataset[i].Data(),fname.Data())<< " not found" << endl;
      exit(0);
    }
    hmean[i] = (TH1D*) inFile[i]->Get("hGausMeanRot");
    if (!hmean[i]) { cout << "-E- Histogram " << "hGausMeanRot" << " not found" << endl; exit(0); }
    hmean[i]->SetName(legend[i].Data()); hmean[i]->SetTitle(legend[i].Data());
    hsigma[i] = (TH1D*) inFile[i]->Get("hGausSigmaRot");
    if (!hsigma[i]) { cout << "-E- Histogram " << "hGausSigmaRot" << " not found" << endl; exit(0); }
    hsigma[i]->SetName(legend[i].Data()); hsigma[i]->SetTitle(legend[i].Data());
    hsignif[i] = (TH1D*) inFile[i]->Get("hSignifRot");
    if (!hsignif[i]) { cout << "-E- Histogram " << "hSignifRot" << " not found" << endl; exit(0); }
    hsignif[i]->SetName(legend[i].Data()); hsignif[i]->SetTitle(legend[i].Data());
    hsoverb[i] = (TH1D*) inFile[i]->Get("hSoverBRot");
    if (!hsoverb[i]) { cout << "-E- Histogram " << "hSoverBRot" << " not found" << endl; exit(0); }
    hsoverb[i]->SetName(legend[i].Data()); hsoverb[i]->SetTitle(legend[i].Data());

    switch(i) {
    case 0:  
      hmean[i]->SetLineColor(kRed);      hmean[i]->SetMarkerColor(kRed);     hmean[i]->SetMarkerStyle(20); 
      hsigma[i]->SetLineColor(kRed);     hsigma[i]->SetMarkerColor(kRed);    hsigma[i]->SetMarkerStyle(20);
      hsignif[i]->SetLineColor(kRed);    hsignif[i]->SetMarkerColor(kRed);   hsignif[i]->SetMarkerStyle(20);
      hsoverb[i]->SetLineColor(kRed);    hsoverb[i]->SetMarkerColor(kRed);   hsoverb[i]->SetMarkerStyle(20);
      break;
    case 1:  
      hmean[i]->SetLineColor(kBlue);      hmean[i]->SetMarkerColor(kBlue);     hmean[i]->SetMarkerStyle(21); 
      hsigma[i]->SetLineColor(kBlue);     hsigma[i]->SetMarkerColor(kBlue);    hsigma[i]->SetMarkerStyle(21);
      hsignif[i]->SetLineColor(kBlue);    hsignif[i]->SetMarkerColor(kBlue);   hsignif[i]->SetMarkerStyle(21);
      hsoverb[i]->SetLineColor(kBlue);    hsoverb[i]->SetMarkerColor(kBlue);   hsoverb[i]->SetMarkerStyle(21);
      break;
    case 2:
      hmean[i]->SetLineColor(kGreen);      hmean[i]->SetMarkerColor(kGreen);     hmean[i]->SetMarkerStyle(22); 
      hsigma[i]->SetLineColor(kGreen);     hsigma[i]->SetMarkerColor(kGreen);    hsigma[i]->SetMarkerStyle(22);
      hsignif[i]->SetLineColor(kGreen);    hsignif[i]->SetMarkerColor(kGreen);   hsignif[i]->SetMarkerStyle(22);
      hsoverb[i]->SetLineColor(kGreen);    hsoverb[i]->SetMarkerColor(kGreen);   hsoverb[i]->SetMarkerStyle(22);
      break;
    case 3:
      hmean[i]->SetLineColor(kBlack);      hmean[i]->SetMarkerColor(kBlack);     hmean[i]->SetMarkerStyle(3); 
      hsigma[i]->SetLineColor(kBlack);     hsigma[i]->SetMarkerColor(kBlack);    hsigma[i]->SetMarkerStyle(3);
      hsignif[i]->SetLineColor(kBlack);    hsignif[i]->SetMarkerColor(kBlack);   hsignif[i]->SetMarkerStyle(3);
      hsoverb[i]->SetLineColor(kBlack);    hsoverb[i]->SetMarkerColor(kBlack);   hsoverb[i]->SetMarkerStyle(3);
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
    // gaussian sigma
    csigma->cd();
    hsigma[i]->Draw("PESAME");
    hsigma[i]->SetTitle("");
    hsigma[i]->SetStats(0);
    hsigma[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hsigma[i]->GetYaxis()->SetTitle("Gaussian sigma (GeV/c^{2})");
    hsigma[i]->GetYaxis()->SetTitleOffset(1.4);
    hsigma[i]->GetYaxis()->SetRangeUser(0,0.03);
    // significance
    csignif->cd();
    hsignif[i]->GetYaxis()->SetRangeUser(0,50);
    hsignif[i]->Draw("PESAME");
    hsignif[i]->SetTitle("");
    hsignif[i]->SetStats(0);
    hsignif[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hsignif[i]->GetYaxis()->SetTitle("Significance");
    // 
    csoverb->cd();
    hsoverb[i]->GetYaxis()->SetRangeUser(0,0.04);
    hsoverb[i]->Draw("PESAME");
    hsoverb[i]->SetTitle("");
    hsoverb[i]->SetStats(0);
    hsoverb[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hsoverb[i]->GetYaxis()->SetTitle("S/B");
    hsoverb[i]->GetYaxis()->SetTitleOffset(1.4);
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

  TFile *fout = new TFile(Form("%sCompare_IMparameters_fixedSigma.root",inputdir.Data()), "RECREATE");
  cmean->Write();
  csigma->Write();
  csignif->Write();
  csoverb->Write();
  fout->Write();
  fout->Close();
  cmean->Print(Form("%sComparison_mean_fixedSigma.png",inputdir.Data()));
  csigma->Print(Form("%sComparison_sigma_fixedSigma.png",inputdir.Data()));
  csignif->Print(Form("%sComparison_signif_fixedSigma.png",inputdir.Data()));
  csoverb->Print(Form("%sComparison_soverb_fixedSigma.png",inputdir.Data()));
}

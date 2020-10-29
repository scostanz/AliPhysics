TString year[3] = {"2016","2017","2018"};
TString csdir = "~/alice/D0_pp13TeV/results/cs/sys_PID/HFPtSpectrum_";
TString suffix = "NoPID_Pt400";

void ratio_cs_pid2(){

  gStyle->SetOptTitle(0);
  
  const Int_t nper = 3;
  TH1D** hcs = new TH1D*[nper];
  TCanvas *ccs = new TCanvas("CompareCS","CompareCS",800,600);
  TString hname = "histoSigmaCorr";
  Double_t max = 0.;
  for (Int_t j=0;j<nper; j++) {
    TFile *infile;
    infile = TFile::Open(Form("%s%s_%s.root", csdir.Data(),suffix.Data(),year[j].Data()));
    hcs[j] = (TH1D*)infile->Get(hname.Data());
    if (hcs[j]->GetMaximum()> max) max = hcs[j]->GetMaximum();
    switch (j) {
    case 0: 
      hcs[j]->SetMarkerStyle(20); hcs[j]->SetMarkerColor(kBlue); hcs[j]->SetLineColor(kBlue); break;
    case 1:
      hcs[j]->SetMarkerStyle(21); hcs[j]->SetMarkerColor(kRed); hcs[j]->SetLineColor(kRed); break;
    case 2:
      hcs[j]->SetMarkerStyle(23); hcs[j]->SetMarkerColor(kGreen); hcs[j]->SetLineColor(kGreen); break;
    }

    // efficiency plots
    ccs->cd();
    hcs[j]->SetStats(0);
    hcs[j]->GetYaxis()->SetRangeUser(0, max+5e6);
    hcs[j]->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV/c^{2})");
    hcs[j]->GetYaxis()->SetTitleOffset(1.2);
    hcs[j]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hcs[j]->Draw("same");
    hcs[j]->SetTitle(Form("LHC%s",year[j].Data()));
  }

  ccs->BuildLegend();

  // output file
  TFile *coutput = new TFile(Form("~/alice/D0_pp13TeV/results/cs/sys_PID/comparison/outputCS_%s.root",suffix.Data()), "RECREATE");
  ccs->Write();
  coutput->Write();
  coutput->Close();

}

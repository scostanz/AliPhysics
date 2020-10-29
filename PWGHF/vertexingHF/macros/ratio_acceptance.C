#include <TH1D.h>

void ratio_acceptance() {

  TString hName = "hpteffCFMCAccMCLimAcc";

  TString prefix, suffix, sysdir, meson, model;
  sysdir = "~/alice/D0_pp13TeV/results/acceptance/";
  prefix = "Acceptance_Toy_D0Kpi_yfidPtDep_etaDau09_ptDau100_";
  meson = "D0";
  model = "PYTHIA";//"FONLL";
  suffix = "13ptshapeReb.root";
  
  TString cuts[2] = {"prompt","feeddown"};

  TCanvas *c = new TCanvas(hName, hName, 800, 600);
  
  TString hNameR = Form("%sratio",hName.Data());

  TString fileName[2];  
  Double_t max[2] = {0.};
  Int_t cset[2];
  TH1D* h[2], *hr;

  for (Int_t i=0; i<2; i++) {
    fileName[i].Form("%s%s%s%s%s%s",sysdir.Data(), prefix.Data(), cuts[i].Data(), meson.Data(), model.Data(), suffix.Data());
    cout << fileName[i].Data() << endl;

    TFile *inFile = TFile::Open(fileName[i].Data());
    if (!inFile) {
      cout << "-E- Input file " << fileName[i].Data() << " not found" << endl;
      exit(0);
    }
    
    h[i] = (TH1D*) inFile->Get(hName.Data());
    if (!h[i]) {
      cout << "-E- Histogram " << hName.Data() << " not found" << endl;
      exit(0);
    }

    h[i]->SetMarkerStyle(20+i);
    h[i]->SetLineWidth(1);
    switch(i) {
    case 0: h[i]->SetLineColor(kBlue);      h[i]->SetMarkerColor(kBlue);      break;
    case 1: h[i]->SetLineColor(kRed);       h[i]->SetMarkerColor(kRed);       break;
    }

    c->cd();
    h[i]->SetName(cuts[i].Data());
    h[i]->GetYaxis()->SetRangeUser(0.5,1.8);
    h[i]->GetXaxis()->SetRangeUser(0., 12.);
    h[i]->Draw("PESAME");
    h[i]->SetTitle("");
    h[i]->SetStats(0);
    h[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    h[i]->GetYaxis()->SetTitle("acceptance");
    c->SetLogy();
  }
  TLegend *leg = new TLegend(0.6, 0.25, 0.8, 0.35);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(h[0], "prompt", "lp");
  leg->AddEntry(h[1], "feeddown", "lp");
  leg->Draw("same");
  //  c->BuildLegend();
  c->Update();   c->Modified();

  TCanvas *cratio = new TCanvas(hNameR, hNameR, 800, 600);
  TLegend *lratio = new TLegend(0.2, 0.72, 0.5, 0.82);
  lratio->SetFillColor(0);
  lratio->SetBorderSize(0);

  TH1D *href = (TH1D*) h[1]->Clone();
  hr = (TH1D*)h[0]->Clone();
  hr->Divide(h[0],href,1,1,"B");

  hr->SetMarkerStyle(20);
  //  hr->SetMarkerSize(0.8);
  hr->SetLineColor(kBlue);      
  hr->SetMarkerColor(kBlue);  
  cratio->cd();
  hr->GetYaxis()->SetRangeUser(0.97,1.03);
  hr->GetXaxis()->SetRangeUser(0., 12.);
  hr->Draw("PESAME");
  hr->SetTitle("");
  hr->SetStats(0);
  hr->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hr->GetYaxis()->SetTitle("acceptance ratio");

  lratio->AddEntry(hr, "prompt/feeddown", "lp");
  lratio->Draw("same");

  TLine *ll = new TLine(0., 1., 12., 1.);
  ll->SetLineStyle(2);
  ll->SetLineWidth(2);
  ll->SetLineColor(kBlack);
  ll->Draw("same");
  cratio->Update();   cratio->Modified();

  TString outname = Form("%sCompareAcc_%s.root",sysdir.Data(),model.Data());
  TFile *fout = new TFile(outname.Data(), "RECREATE");
  c->Write();
  cratio->Write();
  fout->Write();
  fout->Close();
}

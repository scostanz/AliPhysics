#include <TH1D.h>

void ratio_cs() {

  gStyle->SetOptTitle(0);

  // yield
//   TString hName = "histoYieldCorr";
//   TString title = "Corrected yield";
  // cs
  TString hName = "histoSigmaCorr";
  TString title = "d#sigma/dp_{T} (pb/GeV/c^2)";

  const Int_t nMax = 5;

  TString fname = "HFPtSpectrum_";
  // reflections
  TString inputdir = "~/alice/D0_pp13TeV/results/cs/sys_reflections/";
  TString dataset[nMax] = {"2gaus","templx05","templx2","norefl","central"};
  TString legend[nMax] = {"2gaus fit", "template/2","template x2","no reflections", "template"};
//   //PID
//   TString inputdir = "~/alice/D0_pp13TeV/results/cs/sys_PID/";
//   TString dataset[nMax] = {"23PID_pt400","2PID_pt400","32PID_pt400","central"};
//   TString legend[nMax] = {"2#sigma TPC 3#sigma TOF, p_{T} 400 MeV/c","2#sigma TPC 2#sigma TOF, p_{T} 400 MeV/c","3#sigma TPC 2#sigma TOF, p_{T} 400 MeV/c","central cuts"};

  TCanvas *c = new TCanvas(hName, hName, 800, 600);

  // reflections
  TString hNameR = Form("%sratioToTemplate",hName.Data());
//   // PID
//   TString hNameR = Form("%sratioToCentral",hName.Data());

  TString fileName[nMax];  
  Double_t max[nMax] = {0.};
  Int_t cset[nMax];
  TH1D* h[nMax], *hr[nMax];

  for (Int_t i=0; i<nMax; i++) {
    fileName[i].Form("%s%s%s.root",inputdir.Data(), fname.Data(), dataset[i].Data());
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
    switch(i) {
    case 0:  h[i]->SetLineColor(kBlue);      h[i]->SetMarkerColor(kBlue);      break;
    case 1:  h[i]->SetLineColor(kRed);       h[i]->SetMarkerColor(kRed);       break;
    case 2:  h[i]->SetLineColor(kGreen);     h[i]->SetMarkerColor(kGreen);     break;
    case 3:  h[i]->SetLineColor(kMagenta);   h[i]->SetMarkerColor(kMagenta);   break;
    case 4:  h[i]->SetLineColor(kBlack);     h[i]->SetMarkerColor(kBlack);  h[i]->SetMarkerStyle(3);        break;
      //    case 3:  h[i]->SetLineColor(kBlack);     h[i]->SetMarkerColor(kBlack);     h[i]->SetMarkerStyle(3);        break;
    }
    h[i]->SetLineWidth(1);
    c->cd();
    h[i]->GetXaxis()->SetRangeUser(0., 12.);
    h[i]->Draw("PESAME");
    h[i]->SetTitle("");
    h[i]->SetStats(0);
    h[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    h[i]->GetYaxis()->SetTitle(title.Data());
    h[i]->SetName(legend[i]);
    h[i]->SetTitle(legend[i]);
  }
  c->BuildLegend();

  TCanvas *cratio = new TCanvas(hNameR, hNameR, 800, 600);
  TH1D *href = (TH1D*) h[nMax-1]->Clone();
  for (Int_t i=0; i<nMax-1; i++) {
    hr[i] = (TH1D*)h[i]->Clone();
    hr[i]->Divide(h[i],href,1,1,"B");
    hr[i]->SetMarkerStyle(20+i);
    hr[i]->SetLineWidth(1);
    switch(i) {
    case 0:  hr[i]->SetLineColor(kBlue);      hr[i]->SetMarkerColor(kBlue);      break;
    case 1:  hr[i]->SetLineColor(kRed);       hr[i]->SetMarkerColor(kRed);       break;
    case 2:  hr[i]->SetLineColor(kGreen);     hr[i]->SetMarkerColor(kGreen);     break;
    case 3:  hr[i]->SetLineColor(kMagenta);   hr[i]->SetMarkerColor(kMagenta);   break;
    case 4:  hr[i]->SetLineColor(kBlack);     hr[i]->SetMarkerColor(kBlack);     hr[i]->SetMarkerStyle(3);             break;
//    case 3:  hr[i]->SetLineColor(kBlack);     hr[i]->SetMarkerColor(kBlack);     hr[i]->SetMarkerStyle(3);             break;
    }

    cratio->cd();
    //hr[i]->GetYaxis()->SetRangeUser(0.9,1.1);
    hr[i]->GetYaxis()->SetRangeUser(0.7,1.2);
    hr[i]->GetXaxis()->SetRangeUser(0., 12.);
    hr[i]->Draw("PESAME");
    hr[i]->SetTitle("");
    hr[i]->SetStats(0);
    hr[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hr[i]->GetYaxis()->SetTitle(Form("%s ratio",title.Data()));
    // reflections
    hr[i]->SetName(Form("%s/template",legend[i].Data()));
    hr[i]->SetTitle(Form("%s/template",legend[i].Data()));
    // PID
//     hr[i]->SetName(Form("%s/central",legend[i].Data()));
//     hr[i]->SetTitle(Form("%s/central",legend[i].Data()));
  }
  cratio->BuildLegend();

  TLine *ll = new TLine(0., 1., 12., 1.);
  ll->SetLineStyle(2);
  ll->SetLineColor(kBlack);
  ll->Draw("same");

  // reflections
  TFile *fout = new TFile(Form("%sCompare_%s_refl.root",inputdir.Data(),hName.Data()), "RECREATE");
  // PID
  //TFile *fout = new TFile(Form("%sCompare_%s_PID.root",inputdir.Data(),hName.Data()), "RECREATE");
  c->Write();
  cratio->Write();
  fout->Write();
  fout->Close();
}

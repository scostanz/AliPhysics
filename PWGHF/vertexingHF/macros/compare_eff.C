#include <TH1D.h>

enum EPFd{kPrompt,kFeeddw};
enum EEff{kEff,kAccEff};

void compare_eff() {

  gStyle->SetOptTitle(0);

  TString hName = "hEffPromptVsPtNoWeight";
  TString title = "Eff";

  const Int_t nMax = 9;

  TString fname = "outputEff_Prompt_3SigPID_Pt400_YFid_PileUpMV.root";
  TString inputdir = "~/alice/D0_13TeV_lowpt/results/efficiency/central_cuts/LHC2016_deghjop/test/";
  TString dataset[nMax] = {"16d","16e","16g","16h_negB","16h_posB","16j","16o_hadronPID","16o_incompleteTPC","16p"};

  TCanvas *c = new TCanvas(hName, hName, 800, 600);

  TString fileName[nMax];  
  Double_t max[nMax] = {0.};
  Int_t cset[nMax];
  TH1D* h[nMax], *hr[nMax];

  for (Int_t i=0; i<nMax; i++) {
    fileName[i].Form("%s%s/%s",inputdir.Data(), dataset[i].Data(), fname.Data());
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
    case 3:  h[i]->SetLineColor(kBlack);     h[i]->SetMarkerColor(kBlack);     break;
    case 4:  h[i]->SetLineColor(kCyan);      h[i]->SetMarkerColor(kCyan);      break;
    case 5:  h[i]->SetLineColor(kMagenta);   h[i]->SetMarkerColor(kMagenta);   break;
    case 6:  h[i]->SetLineColor(kYellow);    h[i]->SetMarkerColor(kYellow);    break;
    case 7:  h[i]->SetLineColor(kViolet+7);  h[i]->SetMarkerColor(kViolet+7);  break;
    case 8:  h[i]->SetLineColor(kBlack);     h[i]->SetMarkerColor(kBlack);     break;
    }
    h[i]->SetLineWidth(1);
    c->cd();
    h[i]->GetYaxis()->SetRangeUser(0.2,1.);
    h[i]->GetXaxis()->SetRangeUser(0., 12.);
    h[i]->Draw("PESAME");
    h[i]->SetTitle("");
    h[i]->SetStats(0);
    h[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    h[i]->GetYaxis()->SetTitle(title.Data());
    h[i]->SetName(dataset[i].Data());
    h[i]->SetTitle(dataset[i].Data());
  }
  c->BuildLegend();

//   TCanvas *cratio = new TCanvas(hNameR, hNameR, 800, 600);
//   TH1D *href = (TH1D*) h[nMax-1]->Clone();
//   for (Int_t i=0; i<nMax-1; i++) {
//     hr[i] = (TH1D*)h[i]->Clone();
//     hr[i]->Divide(h[i],href,1,1,"B");
//     hr[i]->SetMarkerStyle(20+i);
//     hr[i]->SetLineWidth(1);
//     switch(i) {
//     case 0:  hr[i]->SetLineColor(kBlue);      hr[i]->SetMarkerColor(kBlue);      break;
//     case 1:  hr[i]->SetLineColor(kRed);       hr[i]->SetMarkerColor(kRed);       break;
//     case 2:  hr[i]->SetLineColor(kGreen);     hr[i]->SetMarkerColor(kGreen);     break;
//     case 3:  hr[i]->SetLineColor(kBlack);     hr[i]->SetMarkerColor(kBlack);     hr[i]->SetMarkerStyle(3);             break;
//     }

//     cratio->cd();
//     hr[i]->GetYaxis()->SetRangeUser(0.8,1.2);
//     hr[i]->GetXaxis()->SetRangeUser(0., 12.);
//     hr[i]->Draw("PESAME");
//     hr[i]->SetTitle("");
//     hr[i]->SetStats(0);
//     hr[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
//     hr[i]->GetYaxis()->SetTitle(Form("%s ratio",title.Data()));
//     hr[i]->SetName(Form("%s/ central cuts",legend[i].Data()));
//     hr[i]->SetTitle(Form("%s/ central cuts",legend[i].Data()));
//   }
//   cratio->BuildLegend();

  TLine *ll = new TLine(0., 1., 12., 1.);
  ll->SetLineStyle(2);
  ll->SetLineColor(kBlack);
  ll->Draw("same");

  TFile *fout = new TFile(Form("%sCompare_%s_%s.root",inputdir.Data(),title.Data(),hName.Data()), "RECREATE");
  c->Write();
  //  cratio->Write();
  fout->Write();
  fout->Close();
}

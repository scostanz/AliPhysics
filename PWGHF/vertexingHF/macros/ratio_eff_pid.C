#include <TH1D.h>

enum EPFd{kPrompt,kFeeddw};
enum EEff{kEff,kAccEff};

void ratio_eff_pid() {

  gStyle->SetOptTitle(0);

  //  Int_t ieff = kEff;
  Int_t ieff = kAccEff;
  Int_t iprompt = kPrompt;
  //  Int_t iprompt = kFeeddw;

  TString hName, title;

  if (ieff==0) {
    if (iprompt==0) 
      hName = "hEffPromptVsPtNoWeight";
    else 
      hName = "hEffFeeddwVsPtNoWeight";
    title = "Eff";
  }
  else {
    if (iprompt==0)
      hName = "hEffD";
    else 
      hName = "hEffB";
    title = "AccXEff";
  }

  const Int_t nMax = 5;

  TString fname = "outputEff_Prompt_";
  TString inputdir = "~/alice/D0_13TeV_lowpt/results/efficiency/pidcuts/";
//   TString dataset[nMax] = {"23SigPID_Pt400_YFid_PileUpMV","2SigPID_Pt400_YFid_PileUpMV","32SigPID_Pt400_YFid_PileUpMV","3SigPID_Pt400_YFid_PileUpMV"};
//   TString legend[nMax] = {"2#sigma TPC 3#sigma TOF, p_{T} 400 MeV/c","2#sigma TPC 2#sigma TOF, p_{T} 400 MeV/c","3#sigma TPC 2#sigma TOF, p_{T} 400 MeV/c","3#sigma PID, p_{T} 400 MeV/c"};
//  TString legend[nMax] = {"s23","s2", "s32","s3"};

  TString dataset[nMax] = {"3SigPID_Pt300_YFid_PileUpMV","3SigPID_Pt300_YFid_PileUpMV_SPDoff","3SigPID_Pt400_YFid_PileUpMV_SPDoff","NoPID_Pt400_YFid_PileUpMV","3SigPID_Pt400_YFid_PileUpMV"};
  TString legend[nMax] = {"3#sigma PID, p_{T} 300 MeV/c","3#sigma PID, p_{T} 300 MeV/c, SPDoff","3#sigma PID, p_{T} 400 MeV/c, SPDoff","No PID, p_{T} 400 MeV/c", "3#sigma PID, p_{T} 400 MeV/c"};
  //  TString legend[nMax] = {"s3MeV300","s3MeV300SPDoff","s3MeV400SPDoff","NoPID400", "s3MeV400"};

  TCanvas *c = new TCanvas(hName, hName, 800, 600);

  TString hNameR = Form("%sratioToStandard",hName.Data());

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
    case 3:  h[i]->SetLineColor(kCyan);      h[i]->SetMarkerColor(kCyan);      break;
    case 4:  h[i]->SetLineColor(kBlack);     h[i]->SetMarkerColor(kBlack);     
             h[i]->SetMarkerStyle(3);        break;
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
    h[i]->SetName(legend[i]);
    h[i]->SetTitle(legend[i]);
    c->SetLogy();

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
    case 3:  hr[i]->SetLineColor(kCyan);      hr[i]->SetMarkerColor(kCyan);      break;
    case 4:  hr[i]->SetLineColor(kBlack);     hr[i]->SetMarkerColor(kBlack);     hr[i]->SetMarkerStyle(3);             break;
    }

    cratio->cd();
    hr[i]->GetYaxis()->SetRangeUser(0.8,1.2);
    hr[i]->GetXaxis()->SetRangeUser(0., 12.);
    hr[i]->Draw("PESAME");
    hr[i]->SetTitle("");
    hr[i]->SetStats(0);
    hr[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hr[i]->GetYaxis()->SetTitle(Form("%s ratio",title.Data()));
    hr[i]->SetName(Form("%s/ standard cuts",legend[i].Data()));
    hr[i]->SetTitle(Form("%s/ standard cuts",legend[i].Data()));
  }
  cratio->BuildLegend();

  TLine *ll = new TLine(0., 1., 12., 1.);
  ll->SetLineStyle(2);
  ll->SetLineColor(kBlack);
  ll->Draw("same");

  TFile *fout = new TFile(Form("%sCompare_set2_%s.root",inputdir.Data(),title.Data()), "RECREATE");
  c->Write();
  cratio->Write();
  fout->Write();
  fout->Close();
}

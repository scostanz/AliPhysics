#include <TH1D.h>

void compare_yields_periods_pid() {

  gStyle->SetOptTitle(0);

  // yield
  TString hName = "hRawYieldRot";
  TString title = "Raw Yield / events";

  const Int_t nMax = 3;

  TString fname = "outputMassFits_FixedSigmaAll_Refl_";
  TString pidcut = "NoPID_Pt400";
  TString suffix = "_YFid_PileUpMV.root";

  TString inputdir = "~/alice/D0_pp13TeV/results/figures/";
//   TString dataset[nMax] = {"LHC2016_deghjop","LHC2016_kl","LHC2017","LHC2018","all"};
//   TString legend[nMax] = {"LHC2016_deghjop","LHC2016_kl","LHC2017","LHC2018","all datasets"};
  TString dataset[nMax] = {"LHC2016","LHC2017","LHC2018"};
  TString legend[nMax] = {"LHC2016","LHC2017","LHC2018"};
  TCanvas *c = new TCanvas(hName, hName, 800, 600);

  TString hNameR = Form("%sratioToTotal",hName.Data());
  TString fileName[nMax];  
  Double_t nevents = 0;
  TH1D* h[nMax], *hr[nMax];

  for (Int_t i=0; i<nMax; i++) {
    fileName[i].Form("%s%s/templ/%s/%s%s%s",inputdir.Data(), dataset[i].Data(), pidcut.Data(),fname.Data(), pidcut.Data(), suffix.Data());
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
    TH1F* hNorm=(TH1F*)inFile->Get("hEvForNorm");
    if(hNorm){
      nevents=hNorm->GetBinContent(1);
    }else{
      printf("Histogram with number of events for norm not found in raw yiled file\n");
      printf("  nevents = %.0f will be used\n",nevents);
    }
    h[i]->Scale(1./nevents);
    // printouts
    cout << "Set " << dataset[i].Data() << endl;
    for (int kk=1; kk<15; kk++)
      cout << kk << "\t" << h[i]->GetBinContent(kk) << endl;

    h[i]->SetMarkerStyle(20+i);
    switch(i) {
    case 0:  h[i]->SetLineColor(kBlue);      h[i]->SetMarkerColor(kBlue);      break;
    case 1:  h[i]->SetLineColor(kRed);       h[i]->SetMarkerColor(kRed);       break;
    case 2:  h[i]->SetLineColor(kGreen);     h[i]->SetMarkerColor(kGreen);     break;
    case 3:  h[i]->SetLineColor(kBlack);     h[i]->SetMarkerColor(kBlack);  h[i]->SetMarkerStyle(3);        break;
    }
    h[i]->SetLineWidth(1);
    c->cd();
    h[i]->GetXaxis()->SetRangeUser(0., 12.);
    //    h[i]->GetYaxis()->SetRangeUser(0., 45e6);
    h[i]->Draw("PESAME");
    h[i]->SetTitle("");
    h[i]->SetStats(0);
    h[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    h[i]->GetYaxis()->SetTitle(title.Data());
    h[i]->SetName(legend[i]);
    h[i]->SetTitle(legend[i]);
  }
  c->BuildLegend();


  TFile *fout = new TFile(Form("~/alice/D0_pp13TeV/results/cs/noeffweights/PID_cuts/Compare_yieldsXevt_periods_%s_SPDoff.root",pidcut.Data()), "RECREATE");
  c->Write();
  fout->Write();
  fout->Close();

}

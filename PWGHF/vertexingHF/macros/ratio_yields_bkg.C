#include <TH1D.h>

void ratio_yields_bkg() {

  gStyle->SetOptTitle(0);

  // yield
  TString hName = "hRawYield";
  TString title = "Raw Yield / events";

  const Int_t nMax = 4;

  TString fname = "outputMassFits_FixedSigmaAll_Refl_3SigPID_Pt400_YFid_PileUpMV.root";

  TString inputdir = "~/alice/D0_pp13TeV/results/figures/all/templ/V0100range/";
  TString dataset[nMax] = {"LHC2016","LHC2017","LHC2018","all"};
  TString legend[nMax] = {"LHC2016","LHC2017","LHC2018","all datasets"};
  TCanvas *c = new TCanvas(hName, hName, 800, 600);

  TString hNameR = Form("%sratioToRot",hName.Data());
  Double_t nevents = 0;
  TH1D* h[nMax], *hr[nMax];
 
  TString fileName = Form("%s%s",inputdir.Data(), , fname.Data());
  cout << fileName.Data() << endl;
  
  TFile *inFile = TFile::Open(fileName.Data());
  if (!inFile) {
    cout << "-E- Input file " << fileName.Data() << " not found" << endl;
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
    }

    cratio->cd();
    hr[i]->GetYaxis()->SetRangeUser(0,2);
    hr[i]->GetXaxis()->SetRangeUser(0., 12.);
    hr[i]->Draw("PESAME");
    hr[i]->SetTitle("");
    hr[i]->SetStats(0);
    hr[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hr[i]->GetYaxis()->SetTitle(Form("%s ratio",title.Data()));
    hr[i]->SetName(Form("%s/all datasets",legend[i].Data()));
    hr[i]->SetTitle(Form("%s/all datasets",legend[i].Data()));
  }
  cratio->BuildLegend();

  TLine *ll = new TLine(0., 1., 12., 1.);
  ll->SetLineStyle(2);
  ll->SetLineColor(kBlack);
  ll->Draw("same");

  TFile *fout = new TFile("~/alice/D0_pp13TeV/results/cs/PID_cuts/Compare_yieldsXevt_periods.root", "RECREATE");
  c->Write();
  cratio->Write();
  fout->Write();
  fout->Close();

}

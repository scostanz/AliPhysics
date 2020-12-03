#include <TH1D.h>

void ratio_templates() {

  gStyle->SetOptTitle(0);

  TString dir = "~/alice/D0_13TeV_lowpt/results/figures/";
  TString year = "all";
  TString subdir[5] = {"norefl","2gaus","templX2","templHalf","templ/V0100range"};
  TString legend[5] = {"no reflections", "2gaus fit", "2*template", "0.5*template", "template"};
  TString fName = "outputMassFits_FixedSigmaAll_Refl_3SigPID_Pt400_YFid_PileUpMV.root";
  TString fileName[5];
  TString hName = "hRawYieldRot";

  TString hNameR= Form("%s_ratio",hName.Data());

  TCanvas *cc = new TCanvas("overlap","overlap", 800, 600);
  TCanvas *cr = new TCanvas("cratio","cratio", 800, 600);

  TH1D *h[5], *hr[5];

  cc->cd();
  for (Int_t i=0; i<5; i++) {
    if (i==0)
      fileName[i] = Form("%s%s/%s/outputMassFits_FixedSigmaAll_3SigPID_Pt400_YFid_PileUpMV.root",dir.Data(),year.Data(),subdir[i].Data());
    else 
      fileName[i] = Form("%s%s/%s/%s",dir.Data(),year.Data(),subdir[i].Data(),fName.Data());

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
    h[i]->SetName(Form("%s_%s",hName.Data(),subdir[i].Data()));
    h[i]->SetTitle(legend[i].Data());
    h[i]->GetYaxis()->SetTitleOffset(1.2);
    switch (i) {
    case 0: h[i]->SetMarkerStyle(21); h[i]->SetMarkerColor(kBlue);  h[i]->SetLineColor(kBlue); break;
    case 1: h[i]->SetMarkerStyle(22); h[i]->SetMarkerColor(kRed);  h[i]->SetLineColor(kRed);break;
    case 2: h[i]->SetMarkerStyle(20); h[i]->SetMarkerColor(kGreen);  h[i]->SetLineColor(kGreen); break;
    case 3: h[i]->SetMarkerStyle(29); h[i]->SetMarkerColor(kMagenta);  h[i]->SetLineColor(kMagenta); break;
    case 4: h[i]->SetMarkerStyle(3);
    }
    h[i]->Draw("same");
  }
  gPad->BuildLegend();
  
  cr->cd();
  TLine *ll = new TLine(0., 1., 12., 1.);
  ll->SetLineStyle(2);
  ll->SetLineColor(kBlack);
  for (int i=0;i<4; i++) {
    hr[i] = (TH1D*) h[4]->Clone();
    hr[i]->Divide(h[i], h[4],1,1,"B");
    hr[i]->GetYaxis()->SetTitle("Ratio to templ");
    hr[i]->GetYaxis()->SetTitleOffset(1.2);
    hr[i]->GetYaxis()->SetRangeUser(0.9,1.1);
    hr[i]->SetTitle(legend[i].Data());
    switch (i) {
    case 0: hr[i]->SetMarkerStyle(21); hr[i]->SetMarkerColor(kBlue);    hr[i]->SetLineColor(kBlue); break;
    case 1: hr[i]->SetMarkerStyle(22); hr[i]->SetMarkerColor(kRed);     hr[i]->SetLineColor(kRed);break;
    case 2: hr[i]->SetMarkerStyle(20); hr[i]->SetMarkerColor(kGreen);   hr[i]->SetLineColor(kGreen); break;
    case 3: hr[i]->SetMarkerStyle(29); hr[i]->SetMarkerColor(kMagenta); hr[i]->SetLineColor(kMagenta); break;
    }
    hr[i]->Draw("same");
  }
  gPad->BuildLegend();
  ll->Draw("same");
  
  TString outname = Form("%s%s/CompareYields-templ_v_2gaus.root",dir.Data(),year.Data());
  TFile *fout = new TFile(outname.Data(), "RECREATE");
  cc->Write();
  cr->Write();
  fout->Write();
  fout->Close();
}

#include <TH1D.h>

void ratio_templates() {

  gStyle->SetOptTitle(0);

  TString dir = "~/alice/D0_pp13TeV/results/figures/";
  TString year = "all";
  TString subdir[2] = {"2gaus","templ"};
  TString fName = "outputMassFits_FixedSigmaAll_Refl_3SigPID_Pt400_YFid_PileUpMV.root";
  //  TString fName[2] = {"outputMassFits_FixedSigmaAll_Refl_3SigPID_Pt400_YFid_PileUpMV.root","outputMassFits_FixedSigmaAll_3SigPID_Pt400_YFid_PileUpMV.root"};
  TString fileName[2];
  TString hName = "hRawYield";
  TString hSuffix[4] = {"Rot", "LS", "ME", "SB"};

  TString hNameR[4];
  for (int i=0; i<4; i++) hNameR[i] = Form("%s%s_ratio",hName.Data(), hSuffix[i].Data());  

  TCanvas *cc = new TCanvas("overlap","overlap", 800, 600);
  cc->Divide(2,2);
  TCanvas *cr = new TCanvas("cratio","cratio", 800, 600);
  cr->Divide(2,2);

  TH1D *h[4][2], *hr[4];

  for (Int_t i=0; i<2; i++) {
    fileName[i] = Form("%s%s/%s/%s",dir.Data(),year.Data(),subdir[i].Data(),fName.Data());
    //    fileName[i] = Form("%s%s/%s/%s",dir.Data(),year.Data(),subdir[i].Data(),fName[i].Data());

    TFile *inFile = TFile::Open(fileName[i].Data());
    if (!inFile) {
      cout << "-E- Input file " << fileName[i].Data() << " not found" << endl;
      exit(0);
    }
    
    for (int j=0; j<4; j++) {
      h[j][i] = (TH1D*) inFile->Get(Form("%s%s",hName.Data(), hSuffix[j].Data()));
      if (!h[j][i]) {
	cout << "-E- Histogram " << Form("%s%s",hName.Data(), hSuffix[j].Data()) << " not found" << endl;
	exit(0);
      }
      h[j][i]->SetName(Form("%s%s_%s",hName.Data(), hSuffix[j].Data(),subdir[i].Data()));
      //      h[j][i]->SetTitle(hSuffix[j].Data());
      h[j][i]->SetTitle(subdir[i].Data());
      if (i==0) {
	switch (j) {
	case 0: h[j][i]->SetMarkerStyle(21); break;
	case 1: h[j][i]->SetMarkerStyle(22); break;
	case 2: h[j][i]->SetMarkerStyle(20); break;
	case 3: h[j][i]->SetMarkerStyle(29); break;
	}
      }
      else {
	switch (j) {
	case 0: h[j][i]->SetMarkerStyle(25); break;
	case 1: h[j][i]->SetMarkerStyle(26); break;
	case 2: h[j][i]->SetMarkerStyle(24); break;
	case 3: h[j][i]->SetMarkerStyle(30); break;
	}
      }
      cc->cd(j+1);
      h[j][i]->Draw("same");
      //      gPad->BuildLegend();
    }
  }
  cc->cd(1); gPad->BuildLegend();
  cc->cd(2); gPad->BuildLegend();
  cc->cd(3); gPad->BuildLegend();
  cc->cd(4); gPad->BuildLegend();

  TLine *ll = new TLine(0., 1., 12., 1.);
  ll->SetLineStyle(2);
  ll->SetLineColor(kBlack);
  for (int j=0;j<4;j++) {
    hr[j] = (TH1D*) h[j][1]->Clone();
    hr[j]->Divide(h[j][1], h[j][0],1,1,"B");
    cr->cd(j+1);
    hr[j]->GetYaxis()->SetTitle("Raw yield ratio 2gaus/templ");
    hr[j]->SetTitle(hSuffix[j].Data());
    hr[j]->Draw();
    ll->Draw("same");
  }
  cr->Update();   cr->Modified();

  TString outname = Form("%s%s/CompareYields-templ_v_2gaus.root",dir.Data(),year.Data());
  TFile *fout = new TFile(outname.Data(), "RECREATE");
  cc->Write();
  cr->Write();
  fout->Write();
  fout->Close();
}

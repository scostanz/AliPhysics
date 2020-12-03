void SaveStats();

TString inputdir = "~/alice/D0_13TeV_lowpt/ROOTfiles/MC/lowpt/";
TString dir[5] = {"LHC17h8a","LHC18f4a_16k","LHC18f4a_16l","LHC18l4a","LHC18l4b"};
TString date = "pidcuts";

void Merge_MC(){

  TString dirtomerge="";

  TFile * fi=new TFile(Form("%s%s/%s/AnalysisResults.root",inputdir.Data(),dir[0].Data(),date.Data()));
  cout << "Opening file: " << Form("%s%s/%s/AnalysisResults.root",inputdir.Data(),dir[0].Data(),date.Data()) << endl;
  cout << fi->GetNkeys() << endl;
  TList* lk = fi->GetListOfKeys();
  for(Int_t k=0; k<fi->GetNkeys(); k++){
    TObject* o=(TObject*)lk->At(k);
    TString name=o->GetName();
    if(name.Contains("PWG3_D2H_InvMassDzeroLowPt")) {
      printf("%s  --> will be merged\n",name.Data());
      dirtomerge.Append(Form("%s ",name.Data()));
    }
    else
      printf("%s  --> will be skipped\n",name.Data());
  }

  printf("\nTHESE ARE THE TDIRECTORY FILES THAT WILL BE MERGED:\n");
  Int_t mode = TFileMerger::kAll | TFileMerger::kIncremental | TFileMerger::kOnlyListed;
  
  TFileMerger mga;
  mga.AddObjectNames(dirtomerge.Data());
  mga.OutputFile(Form("%sAnalysisResults_%s.root",inputdir.Data(),date.Data()));
  for (int i=0; i<5;i++) {
    cout << "Adding file " << Form("%s%s/%s/AnalysisResults.root",inputdir.Data(),dir[i].Data(),date.Data()) << endl;
    mga.AddFile(Form("%s%s/%s/AnalysisResults.root",inputdir.Data(),dir[i].Data(),date.Data()));
  }
  mga.PartialMerge(mode);

  SaveStats();

}

void SaveStats(){
  TString filnam1=Form("%s%s/%s/AnalysisResults.root",inputdir.Data(),dir[0].Data(),date.Data());
  TString filnam2=Form("%s%s/%s/AnalysisResults.root",inputdir.Data(),dir[1].Data(),date.Data());
  TString filnam3=Form("%s%s/%s/AnalysisResults.root",inputdir.Data(),dir[2].Data(),date.Data());
  TString filnam4=Form("%s%s/%s/AnalysisResults.root",inputdir.Data(),dir[3].Data(),date.Data());
  TString filnam5=Form("%s%s/%s/AnalysisResults.root",inputdir.Data(),dir[4].Data(),date.Data());
  TFile* fil1=new TFile(filnam1.Data());
  TString dirName="";
  TString listName="";
  TList* lk=fil1->GetListOfKeys();
  for(Int_t k=0; k<fil1->GetNkeys(); k++){
    TKey* o=(TKey*)lk->At(k);
    TString name=o->GetName();
    TString cname=o->GetClassName();
    if(cname == "TDirectoryFile" && name.Contains("PWG3_D2H_InvMassDzeroLowPt")){
      dirName=name.Data();
      TDirectoryFile* df=(TDirectoryFile*)fil1->Get(dirName.Data());
      listName=dirName.Data();
      listName.ReplaceAll("PWG3_D2H_InvMassDzeroLowPt","coutputDzero");
      TList* lst=(TList*)df->Get(listName.Data());
      if(lst) break;
    }
  }
  printf("%s  %s\n",dirName.Data(),listName.Data());

  TDirectoryFile* dir1=(TDirectoryFile*)fil1->Get(dirName.Data());
  TList* lis1=(TList*)dir1->Get(listName.Data());
  TH1F* h1=(TH1F*)lis1->FindObject("hNEvents");
  h1->SetName(Form("hNEvents_%s",dir[0].Data()));
  
  TFile* fil2=new TFile(filnam2.Data());
  TDirectoryFile* dir2=(TDirectoryFile*)fil2->Get(dirName.Data());
  TList* lis2=(TList*)dir2->Get(listName.Data());
  TH1F* h2=(TH1F*)lis2->FindObject("hNEvents");
  h2->SetName(Form("hNEvents_%s",dir[1].Data()));

  TFile* fil3=new TFile(filnam3.Data());
  TDirectoryFile* dir3=(TDirectoryFile*)fil3->Get(dirName.Data());
  TList* lis3=(TList*)dir3->Get(listName.Data());
  TH1F* h3=(TH1F*)lis3->FindObject("hNEvents");
  h3->SetName(Form("hNEvents_%s",dir[2].Data()));

  TFile* fil4=new TFile(filnam4.Data());
  TDirectoryFile* dir4=(TDirectoryFile*)fil4->Get(dirName.Data());
  TList* lis4=(TList*)dir4->Get(listName.Data());
  TH1F* h4=(TH1F*)lis4->FindObject("hNEvents");
  h4->SetName(Form("hNEvents_%s",dir[3].Data()));

  TFile* fil5=new TFile(filnam5.Data());
  TDirectoryFile* dir5=(TDirectoryFile*)fil5->Get(dirName.Data());
  TList* lis5=(TList*)dir5->Get(listName.Data());
  TH1F* h5=(TH1F*)lis5->FindObject("hNEvents");
  h5->SetName(Form("hNEvents_%s",dir[4].Data()));

  Double_t tot1=h1->GetBinContent(1)+h2->GetBinContent(1)+h3->GetBinContent(1)+h4->GetBinContent(1)+h5->GetBinContent(1);
  Double_t tot2=h1->GetBinContent(2)+h2->GetBinContent(2)+h3->GetBinContent(2)+h4->GetBinContent(2)+h5->GetBinContent(2);

  printf("Statistics in trains - runs: %s - %s - %s - %s -%s, date: %s\n",dir[0].Data(), dir[1].Data(), dir[2].Data(), dir[3].Data(),dir[4].Data(),date.Data());
  printf("Nevents\t\t read (M)\t selected (M)\n");
  printf("%s\t%8.3f      %8.3f\n",dir[0].Data(),h1->GetBinContent(1)/1.e6,h1->GetBinContent(2)/1.e6);
  printf("%s\t%8.3f      %8.3f\n",dir[1].Data(),h2->GetBinContent(1)/1.e6,h2->GetBinContent(2)/1.e6);
  printf("%s\t%8.3f      %8.3f\n",dir[2].Data(),h3->GetBinContent(1)/1.e6,h3->GetBinContent(2)/1.e6);
  printf("%s\t%8.3f      %8.3f\n",dir[3].Data(),h4->GetBinContent(1)/1.e6,h4->GetBinContent(2)/1.e6);
  printf("%s\t%8.3f      %8.3f\n",dir[4].Data(),h5->GetBinContent(1)/1.e6,h5->GetBinContent(2)/1.e6);
  printf("Percentages\t read (M)\t selected (M)\n");
  printf("%s\t  %5.2f%%        %5.2f%%\n",dir[0].Data(),h1->GetBinContent(1)/tot1*100,h1->GetBinContent(2)/tot2*100);
  printf("%s\t  %5.2f%%        %5.2f%%\n",dir[1].Data(),h2->GetBinContent(1)/tot1*100,h2->GetBinContent(2)/tot2*100);
  printf("%s\t  %5.2f%%        %5.2f%%\n",dir[2].Data(),h3->GetBinContent(1)/tot1*100,h3->GetBinContent(2)/tot2*100);
  printf("%s\t  %5.2f%%        %5.2f%%\n",dir[3].Data(),h4->GetBinContent(1)/tot1*100,h4->GetBinContent(2)/tot2*100);
  printf("%s\t  %5.2f%%        %5.2f%%\n",dir[4].Data(),h5->GetBinContent(1)/tot1*100,h5->GetBinContent(2)/tot2*100);

  FILE* ftxtout = fopen("Statistics.txt","w");
  fprintf(ftxtout,"Statistics in trains - runs: %s - %s - %s - %s, date: %s\n",dir[0].Data(), dir[1].Data(), dir[2].Data(), dir[3].Data(),date.Data());
  fprintf(ftxtout,"\t\t Nevents\t read (M)\t selected (M)\n");
  fprintf(ftxtout,"%s\t\t%8.3f      %8.3f\n",dir[0].Data(),h1->GetBinContent(1)/1.e6,h1->GetBinContent(2)/1.e6);
  fprintf(ftxtout,"%s\t\t%8.3f      %8.3f\n",dir[1].Data(),h2->GetBinContent(1)/1.e6,h2->GetBinContent(2)/1.e6);
  fprintf(ftxtout,"%s\t%8.3f      %8.3f\n",dir[2].Data(),h3->GetBinContent(1)/1.e6,h3->GetBinContent(2)/1.e6);
  fprintf(ftxtout,"%s\t%8.3f      %8.3f\n",dir[3].Data(),h4->GetBinContent(1)/1.e6,h4->GetBinContent(2)/1.e6);
  fprintf(ftxtout,"%s\t%8.3f      %8.3f\n",dir[4].Data(),h5->GetBinContent(1)/1.e6,h5->GetBinContent(2)/1.e6);
  fprintf(ftxtout,"\t\tPercentages\t read (M)\t selected (M)\n");
  fprintf(ftxtout,"%s\t\t  %5.2f%%        %5.2f%%\n",dir[0].Data(),h1->GetBinContent(1)/tot1*100,h1->GetBinContent(2)/tot2*100);
  fprintf(ftxtout,"%s\t\t  %5.2f%%        %5.2f%%\n",dir[1].Data(),h2->GetBinContent(1)/tot1*100,h2->GetBinContent(2)/tot2*100);
  fprintf(ftxtout,"%s\t  %5.2f%%        %5.2f%%\n",dir[2].Data(),h3->GetBinContent(1)/tot1*100,h3->GetBinContent(2)/tot2*100);
  fprintf(ftxtout,"%s\t  %5.2f%%        %5.2f%%\n",dir[3].Data(),h4->GetBinContent(1)/tot1*100,h4->GetBinContent(2)/tot2*100);
  fprintf(ftxtout,"%s\t  %5.2f%%        %5.2f%%\n",dir[4].Data(),h5->GetBinContent(1)/tot1*100,h5->GetBinContent(2)/tot2*100);

  TFile* outStat=new TFile(Form("%sStatsDataSets_%s-%s-%s-%s-%s_%s.root",inputdir.Data(),dir[0].Data(), dir[1].Data(), dir[2].Data(), dir[3].Data(),dir[4].Data(),date.Data()),"recreate");
  h1->Write();
  h2->Write();
  h3->Write();
  h4->Write();
  h5->Write();
  outStat->Close();
}

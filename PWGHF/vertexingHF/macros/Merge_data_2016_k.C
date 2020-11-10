void SaveStats();

TString inputdir = "~/alice/D0_13TeV_lowpt/ROOTfiles/data/lowpt/LHC2016_k";
//TString inputdir = "~/alice/D0_13TeV_lowpt/ROOTfiles/data/lowpt/LHC2016_l";
TString suffix[2]={"runlist1","runlist2"};

void Merge_data_2016_k(){
  
  TString dirtomerge="";

  TFile * fi=new TFile(Form("%s/AnalysisResults.root-%s",inputdir.Data(),suffix[0].Data()));
  cout << "Opening file: " << Form("%s/AnalysisResults.root-%s",inputdir.Data(),suffix[0].Data()) << endl;
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
  mga.OutputFile(Form("%s/AnalysisResults.root",inputdir.Data()));
  for (int i=0; i<2;i++) {
    cout << "Adding file " << Form("%s/AnalysisResults.root-%s",inputdir.Data(),suffix[i].Data()) << endl;
    mga.AddFile(Form("%s/AnalysisResults.root-%s",inputdir.Data(),suffix[i].Data()));
  }
  mga.PartialMerge(mode);

  SaveStats();

}

void SaveStats(){
  TString filnam1=Form("%s/AnalysisResults.root-%s",inputdir.Data(),suffix[0].Data());
  TString filnam2=Form("%s/AnalysisResults.root-%s",inputdir.Data(),suffix[1].Data());
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
  //  printf("%s  %s\n",dirName.Data(),listName.Data());

  TDirectoryFile* dir1=(TDirectoryFile*)fil1->Get(dirName.Data());
  TList* lis1=(TList*)dir1->Get(listName.Data());
  TH1F* h1=(TH1F*)lis1->FindObject("hNEvents");
  h1->SetName(Form("hNEvents_%s",suffix[0].Data()));
  
  TFile* fil2=new TFile(filnam2.Data());
  TDirectoryFile* dir2=(TDirectoryFile*)fil2->Get(dirName.Data());
  TList* lis2=(TList*)dir2->Get(listName.Data());
  TH1F* h2=(TH1F*)lis2->FindObject("hNEvents");
  h2->SetName(Form("hNEvents_%s",suffix[1].Data()));

  Double_t tot1=h1->GetBinContent(1)+h2->GetBinContent(1);
  Double_t tot2=h1->GetBinContent(2)+h2->GetBinContent(2);

  printf("Statistics in trains - runs: %s - %s: \n",suffix[0].Data(), suffix[1].Data());
  printf("Nevents\t\t read (M)\t selected (M)\n");
  printf("%s\t\t%8.3f      %8.3f\n",suffix[0].Data(),h1->GetBinContent(1)/1.e6,h1->GetBinContent(2)/1.e6);
  printf("%s\t\t%8.3f      %8.3f\n",suffix[1].Data(),h2->GetBinContent(1)/1.e6,h2->GetBinContent(2)/1.e6);
  printf("Percentages\t read (M)\t selected (M)\n");
  printf("%s\t\t  %5.2f%%        %5.2f%%\n",suffix[0].Data(),h1->GetBinContent(1)/tot1*100,h1->GetBinContent(2)/tot2*100);
  printf("%s\t\t  %5.2f%%        %5.2f%%\n",suffix[1].Data(),h2->GetBinContent(1)/tot1*100,h2->GetBinContent(2)/tot2*100);

  TFile* outStat=new TFile(Form("%s/StatsDataSets_%s-%s.root",inputdir.Data(),suffix[0].Data(), suffix[1].Data()),"recreate");
  h1->Write();
  h2->Write();
  outStat->Close();
 
  FILE* ftxtout = fopen("Statistics.txt","w");
  fprintf(ftxtout,"Statistics in trains - runs: %s - %s: \n",suffix[0].Data(), suffix[1].Data());
  fprintf(ftxtout,"\t\t Nevents\t read (M)\t selected (M)\n");
  fprintf(ftxtout,"%s\t\t%8.3f      %8.3f\n",suffix[0].Data(),h1->GetBinContent(1)/1.e6,h1->GetBinContent(2)/1.e6);
  fprintf(ftxtout,"%s\t\t%8.3f      %8.3f\n",suffix[1].Data(),h2->GetBinContent(1)/1.e6,h2->GetBinContent(2)/1.e6);
  fprintf(ftxtout,"\t\tPercentages\t read (M)\t selected (M)\n");
  fprintf(ftxtout,"%s\t\t  %5.2f%%        %5.2f%%\n",suffix[0].Data(),h1->GetBinContent(1)/tot1*100,h1->GetBinContent(2)/tot2*100);
  fprintf(ftxtout,"%s\t\t  %5.2f%%        %5.2f%%\n",suffix[1].Data(),h2->GetBinContent(1)/tot1*100,h2->GetBinContent(2)/tot2*100);


}

void SaveStats(Int_t ntq, Int_t ntr);

void Merge18qr(){
  Int_t ntq=1596;
  Int_t ntr=1597;
  
  TString dirtomerge="";
  TFile * fi=new TFile(Form("AnalysisResults18q_train%d.root",ntq));
  cout<<fi->GetNkeys()<<endl;
  TList* lk=fi->GetListOfKeys();
  for(Int_t k=0; k<fi->GetNkeys(); k++){
    TObject* o=(TObject*)lk->At(k);
    TString name=o->GetName();
    if(name.Contains("PWG3_D2H_InvMassDzeroLowPt")){
      printf("%s  --> will be merged\n",name.Data());
      dirtomerge.Append(Form("%s ",name.Data()));
    }else{
      printf("%s  --> will be skipped\n",name.Data());
    }
  }

  printf("\nTHESE ARE THE TDIRECTORY FILES THAT WILL BE MERGED:\n");
  printf("%s\n",dirtomerge.Data());
  Int_t mode = TFileMerger::kAll | TFileMerger::kIncremental | TFileMerger::kOnlyListed;
  
  TFileMerger mga;
  mga.AddObjectNames(dirtomerge.Data());
  mga.OutputFile(Form("AnalysisResults18qr_trains%d-%d.root",ntq,ntr));
  mga.AddFile(Form("AnalysisResults18q_train%d.root",ntq));
  mga.AddFile(Form("AnalysisResults18r_train%d.root",ntr));
  mga.PartialMerge(mode);

  TFileMerger mgb;
  mgb.AddObjectNames(dirtomerge.Data());
  mgb.OutputFile(Form("AnalysisResults18qr_with7runs_trains%d-%d.root",ntq,ntr));
  mgb.AddFile(Form("AnalysisResults18q_train%d.root",ntq));
  mgb.AddFile(Form("AnalysisResults18r_train%d.root",ntr));
  mgb.AddFile(Form("AnalysisResults18r_7badPIDruns_train%d.root",ntr));
  mgb.PartialMerge(mode);

  SaveStats(ntq,ntr);

}

void SaveStats(Int_t ntq, Int_t ntr){
  TString filnam1=Form("AnalysisResults18q_train%d.root",ntq);
  TString filnam2=Form("AnalysisResults18r_train%d.root",ntr);
  TString filnam3=Form("AnalysisResults18r_7badPIDruns_train%d.root",ntr);
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
  h1->SetName("hNEvents18q");
  
  TFile* fil2=new TFile(filnam2.Data());
  TDirectoryFile* dir2=(TDirectoryFile*)fil2->Get(dirName.Data());
  TList* lis2=(TList*)dir2->Get(listName.Data());
  TH1F* h2=(TH1F*)lis2->FindObject("hNEvents");
  h2->SetName("hNEvents18r");

  TFile* fil3=new TFile(filnam3.Data());
  TDirectoryFile* dir3=(TDirectoryFile*)fil3->Get(dirName.Data());
  TList* lis3=(TList*)dir3->Get(listName.Data());
  TH1F* h3=(TH1F*)lis3->FindObject("hNEvents");
  h3->SetName("hNEvents18r7r");

  Double_t tot1=h1->GetBinContent(1)+h2->GetBinContent(1)+h3->GetBinContent(1);
  Double_t tot2=h1->GetBinContent(2)+h2->GetBinContent(2)+h3->GetBinContent(2);

  printf("Statistics in trains %d-%d\n",ntq,ntr);
  printf("      Nevents    read (M)     selected (M)\n");
  printf("LHC18q          %8.3f      %8.3f\n",h1->GetBinContent(1)/1.e6,h1->GetBinContent(2)/1.e6);
  printf("LHC18r          %8.3f      %8.3f\n",h2->GetBinContent(1)/1.e6,h2->GetBinContent(2)/1.e6);
  printf("LHC18r (7 runs) %8.3f      %8.3f\n",h3->GetBinContent(1)/1.e6,h3->GetBinContent(2)/1.e6);
  printf("   Percentages   read (M)     selected (M)\n");
  printf("LHC18q          %5.2f%%        %5.2f%%\n",h1->GetBinContent(1)/tot1*100,h1->GetBinContent(2)/tot2*100);
  printf("LHC18r          %5.2f%%        %5.2f%%\n",h2->GetBinContent(1)/tot1*100,h2->GetBinContent(2)/tot2*100);
  printf("LHC18r (7 runs) %5.2f%%        %5.2f%%\n",h3->GetBinContent(1)/tot1*100,h3->GetBinContent(2)/tot2*100);

  FILE* ftxtout = fopen(Form("StatisticsTrain%d-%d.txt",ntq,ntr),"w");
  fprintf(ftxtout,"Statistics in trains %d-%d\n",ntq,ntr);
  fprintf(ftxtout,"      Nevents    read (M)     selected (M)\n");
  fprintf(ftxtout,"LHC18q          %8.3f      %8.3f\n",h1->GetBinContent(1)/1.e6,h1->GetBinContent(2)/1.e6);
  fprintf(ftxtout,"LHC18r          %8.3f      %8.3f\n",h2->GetBinContent(1)/1.e6,h2->GetBinContent(2)/1.e6);
  fprintf(ftxtout,"LHC18r (7 runs) %8.3f      %8.3f\n",h3->GetBinContent(1)/1.e6,h3->GetBinContent(2)/1.e6);
  fprintf(ftxtout,"   Percentages   read (M)     selected (M)\n");
  fprintf(ftxtout,"LHC18q          %5.2f%%        %5.2f%%\n",h1->GetBinContent(1)/tot1*100,h1->GetBinContent(2)/tot2*100);
  fprintf(ftxtout,"LHC18r          %5.2f%%        %5.2f%%\n",h2->GetBinContent(1)/tot1*100,h2->GetBinContent(2)/tot2*100);
  fprintf(ftxtout,"LHC18r (7 runs) %5.2f%%        %5.2f%%\n",h3->GetBinContent(1)/tot1*100,h3->GetBinContent(2)/tot2*100);
  
  TFile* outStat=new TFile(Form("StatsTrain%d-%d.root",ntq,ntr),"recreate");
  h1->Write();
  h2->Write();
  h3->Write();
  outStat->Close();
 
}

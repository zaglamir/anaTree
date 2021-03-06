void sumTrigTypeHist(const Char_t *inputFile="processedRuns2.list", TString outputFile="trigTypeHists.root", TString title="Run 15 pp 200 GeV")
{
  TH1::AddDirectory(kFALSE); // option needed to allow opening many files

  TH1F* h[3];
  TH1F* trigOut[3];
  int counter = 0;
  ifstream fin(inputFile);
  TString fname;
  if(fin.is_open())
  {
    while(fin >> fname)
    {
      cout << "Opening: " << fname << endl;
      TFile* f = TFile::Open(fname,"READ");
      for(int i=0; i<3; i++)
      {
        h[i] = (TH1F*)f->Get(Form("hTrigType_%i",i));
        if(!counter)
          trigOut[i] = h[i];
        else
          trigOut[i] -> Add(h[i]);
      }
      counter++;
      delete f;
    }
  }
  else
  {
    cout << "FileList not found!" << endl;
  }


  TCanvas* c1 = new TCanvas("c1","c1",0,0,700,700);
  TString legName[3] = {"MuDST","PicoDST","After Vz Cuts"};
  Int_t colors[3] = {kBlack, kRed, kAzure+1};
  TLegend* leg = new TLegend(0.5,0.6,0.87,0.87);
  for(int i=0; i<3; i++)
  {
    trigOut[i]->SetLineColor(colors[i]);
    trigOut[i]->SetFillColor(colors[i]);
    trigOut[i]->SetStats(kFALSE);
    trigOut[i]->SetLineWidth(2);
    trigOut[i]->GetXaxis()->SetRange(0,12);
    leg->AddEntry(trigOut[i],legName[i],"f");
    if(i==0)
    {
      TString binLabel[11] = {"","","BHT0","BHT1","BHT2","MB","","MB*BHTX","BHT0*BHT1","BHT0*BHT2","BHT1*BHT2"};
      for(int l=1; l<11;l++){
        trigOut[i]->GetXaxis()->SetBinLabel(l,binLabel[l]);
      }
    }
    trigOut[i]->Draw((i==0)?"hist":"hist same");
    trigOut[i]->SetTitle(title);
  }
  TPaveText* lbl = new TPaveText(0.5,.4,0.87,0.59,"NB NDC");
  lbl->SetFillColorAlpha(kWhite,1);
  int events[3];
  for(int bin=2;bin<5;bin++)
  {
    events[bin-2] = trigOut[2]->GetBinContent(bin);
    lbl->AddText(Form("BHT%i: %.2fM",bin-2,(float)events[bin-2]/1.e6));
  }
  lbl->Draw("same");
  leg->Draw("same");
  c1->Update();
  TString pdfName = outputFile;
  pdfName.ReplaceAll(".root",".pdf");
  c1->SaveAs(pdfName);
  
}

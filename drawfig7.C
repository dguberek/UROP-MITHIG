void drawfig7()
{
  //open the file with all the histograms
  TFile *_file0 = TFile::Open("all.root");
  const int ncentbins = 4;
  int centmins[] = {0,20,60,100};
  int centmaxs[] = {20,60,100,200};
  TCanvas * canvases[ncentbins];

  //declare the histograms
  TH1D * signalJetEphoE[ncentbins];
  TH1D * mix_signalJetEphoE[ncentbins];
  TH1D * sidebandJetEphoE[ncentbins];
  TH1D * mix_sidebandJetEphoE[ncentbins];

  for (int icent = 0; icent < 4; icent++) {
    canvases[icent] = new TCanvas(Form("canvas_%d_%d",centmins[icent],centmaxs[icent]));

    //get the histograms from the file
    signalJetEphoE[icent] = (TH1D*) _file0->Get(Form("signalJetEphoE_cent%d_%d",centmins[icent],centmaxs[icent]));
    mix_signalJetEphoE[icent] = (TH1D*) _file0->Get(Form("mix_signalJetEphoE_cent%d_%d",centmins[icent],centmaxs[icent]));
    sidebandJetEphoE[icent] = (TH1D*) _file0->Get(Form("sidebandJetEphoE_cent%d_%d",centmins[icent],centmaxs[icent]));
    mix_sidebandJetEphoE[icent] = (TH1D*) _file0->Get(Form("mix_sidebandJetEphoE_cent%d_%d",centmins[icent],centmaxs[icent]));

    mix_signalJetEphoE[icent]->Scale(-1); // make this negative, so when added it's subtracted
    signalJetEphoE[icent]->Add(mix_signalJetEphoE[icent]);

    signalJetEphoE[icent]->Draw();
  }
}

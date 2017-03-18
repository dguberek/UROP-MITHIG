float getpurity(float phoetmin, float hibinmin, bool ispp)
{
  float allpuritypbpb[] = {0.725758, 0.720249, 0.753094, 0.703853, 0.730487, 0.756007, 0.741809, 0.737945, 0.725995, 0.786819, 0.704426, 0.743147, 0.775786, 0.827101, 0.715906, 0.710863, 0.739059, 0.687001, 0.719965, 0.743805, 0.720358, 0.724734, 0.719121, 0.758637, 0.699659, 0.73539, 0.76691, 0.731031, 0.730948, 0.717619, 0.786722, 0.695959, 0.733827, 0.771255, 0.836782, 0.749695, 0.739283, 0.788322, 0.711966, 0.763817, 0.786493, 0.816522, 0.733207, 0.719114, 0.785248, 0.705784, 0.734519, 0.769831, 0.84767, 0.772594, 0.766181, 0.802428, 0.720997, 0.80606, 0.822834, 0.774406};
  float allpuritypp[] = {0.823368, 0.823368, 0.823368, 0.823368, 0.823368, 0.823368, 0.823368, 0.846154, 0.846154, 0.846154, 0.846154, 0.846154, 0.846154, 0.846154, 0.820975, 0.820975, 0.820975, 0.820975, 0.820975, 0.820975, 0.820975, 0.830048, 0.830048, 0.830048, 0.830048, 0.830048, 0.830048, 0.830048, 0.846293, 0.846293, 0.846293, 0.846293, 0.846293, 0.846293, 0.846293, 0.859037, 0.859037, 0.859037, 0.859037, 0.859037, 0.859037, 0.859037, 0.863744, 0.863744, 0.863744, 0.863744, 0.863744, 0.863744, 0.863744, 0.857244, 0.857244, 0.857244, 0.857244, 0.857244, 0.857244, 0.857244};
  
    int row = -1;
    int col = -1;
    if(phoetmin==40)  row = 0;
    if(phoetmin==60)  row = 1;
    if(phoetmin==100) row = 7;
    if(hibinmin==0)   col = 3;
    if(hibinmin==20)  col = 4;
    if(hibinmin==60)  col = 5;
    if(hibinmin==100) col = 6;
    if(row>-1 && col > -1 && ispp) return allpuritypp[row*7+col];
    if(row>-1 && col > -1 && !ispp) return allpuritypbpb[row*7+col];
    return 1; //no purity applied
}

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
    
    mix_sidebandJetEphoE[icent]->Scale(-1); // make this negative, so when added it's subtracted
    sidebandJetEphoE[icent]->Add(mix_sidebandJetEphoE[icent]);

    //purities
        if (icent==0)    {
            purities[icent] = getpurity(60.0, centmins[icent], false);
        }
        if (icent==1)    {
            purities[icent] = getpurity(60.0, centmins[icent], false);
        }
        if (icent==2)    {
            purities[icent] = getpurity(60.0, centmins[icent], false);
        }
        if (icent==3)    {
            purities[icent] = getpurity(60.0, centmins[icent], false);
        }
    
    signalJetEphoE[icent]->Scale(1/purities[icent]);
    sidebandJetEphoE[icent]->Scale(-1*(1-purities[icent])/purities[icent]);
    signalJetEphoE[icent]->Add(sidebandJetEphoE[icent]);
    
    signalJetEphoE[icent]->Draw();
  }
}

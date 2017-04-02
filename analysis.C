//this code creates the histogram for the lowest centrality bin (hiBin<20) and "signal" photons
#define analysis_cxx
#include "analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void analysis::Loop(int centmin, int centmax)
{
    //   In a ROOT session, you can do:
    //      root> .L analysis.C
    //      root> analysis t
    //      root> t.GetEntry(12); // Fill t data members with entry number 12
    //      root> t.Show();       // Show values of entry 12
    //      root> t.Show(16);     // Read and show values of entry 16
    //      root> t.Loop();       // Loop on all entries
    //
    
    //     This is the loop skeleton where:
    //    jentry is the global entry number in the chain
    //    ientry is the entry number in the current Tree
    //  Note that the argument to GetEntry must be:
    //    jentry for TChain::GetEntry
    //    ientry for TTree::GetEntry and TBranch::GetEntry
    //
    //       To read only selected branches, Insert statements like:
    // METHOD1:
    //    fChain->SetBranchStatus("*",0);  // disable all branches
    //    fChain->SetBranchStatus("branchname",1);  // activate branchname
    // METHOD2: replace line
    //    fChain->GetEntry(jentry);       //read all branches
    //by  b_branchname->GetEntry(ientry); //read only this branch
    
    if (fChain == 0) return;
    using namespace std;
    
    TFile * outputfile = new TFile(Form("outfilename1_cent%d_%d.root",centmin,centmax),"recreate");
    TH1D * signalJetEphoE = new TH1D(Form("signalJetEphoE_cent%d_%d",centmin,centmax),"JetPt/PhoE title;yaxis title",16,0,2);
    TH1D * mix_signalJetEphoE = new TH1D(Form("mix_signalJetEphoE_cent%d_%d",centmin,centmax),"JetPt/PhoE title;yaxis title",16,0,2);
    TH1D * sidebandJetEphoE = new TH1D(Form("sidebandJetEphoE_cent%d_%d",centmin,centmax),"JetPt/PhoE title;yaxis title",16,0,2);
    TH1D * mix_sidebandJetEphoE = new TH1D(Form("mix_sidebandJetEphoE_cent%d_%d",centmin,centmax),"JetPt/PhoE title;yaxis title",16,0,2);
    //TH1D * rhoR = new TH1D("rhoR","title;xaxis title;yaxis title",20,0,1);
    
    Long64_t nentries = fChain->GetEntriesFast();
    
    int sig_nphotons = 0;
    int side_nphotons = 0;
    bool signal;
    bool sideband;
    
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        //if (jentry > 10000) break; //to check earlier
        Long64_t ientry = LoadTree(jentry);
        if (jentry%10000==0) cout<< jentry << " out of " << nentries << endl ;
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        if (phoNoise[0]==0) continue;
        if (phoEtCorrected[0] < 60 ) continue;
        signal = (phoSigmaIEtaIEta_2012[0]<0.010);
        sideband = (phoSigmaIEtaIEta_2012[0]>0.011 && phoSigmaIEtaIEta_2012[0]<0.017);
        if(!signal && !sideband) continue;                                                  //make sure this is doing what I want it to do
        
        if (signal) {
            if(hiBin>=centmax || hiBin<centmin) continue;           //put this statement before incrementing sig_nphotons !!!
            sig_nphotons++;
            
            for(int ijet = 0 ; ijet < njet ; ijet++) {
                if(jetpt[ijet]<30) continue;
                if(jeteta[ijet]>1.6) continue;
                if(acos(cos(phoPhi[0]-jetphi[ijet]))<(7.0*3.14/8.0)) continue;
                signalJetEphoE->Fill(jetpt[ijet]/phoEtCorrected[0]);
            }
            
            for(int ijet = 0 ; ijet < njet ; ijet++) {
                if(jetpt_mix[ijet]<30) continue;
                if(jeteta_mix[ijet]>1.6) continue;
                if(acos(cos(phoPhi[0]-jetphi_mix[0]))<(7.0*3.14/8.0)) continue;
                mix_signalJetEphoE->Fill(jetpt_mix[ijet]/phoEtCorrected[0]);
            }
        }
        
        if (sideband) {
            if(hiBin>=centmax || hiBin<centmin) continue;               // put this before increminting  !!!
            side_nphotons++;
            
            for(int ijet = 0 ; ijet < njet ; ijet++) {
                if(jetpt[ijet]<30) continue;
                if(jeteta[ijet]>1.6) continue;
                if(acos(cos(phoPhi[0]-jetphi[ijet]))<(7.0*3.14/8.0)) continue;
                sidebandJetEphoE->Fill(jetpt[ijet]/phoEtCorrected[0]);
            }
            
            for(int ijet = 0 ; ijet < njet ; ijet++) {
                if(jetpt_mix[ijet]<30) continue;
                if(jeteta_mix[ijet]>1.6) continue;
                if(acos(cos(phoPhi[0]-jetphi_mix[ijet]))<(7.0*3.14/8.0)) continue;
                mix_sidebandJetEphoE->Fill(jetpt_mix[ijet]/phoEtCorrected[0]);
            }
        }
        //for(int iTrk = 0; iTrk < nTrk; iTrk++) {
        //double dr;
        //double dPhi = acos(cos(jetphi[ijet]-trkPhi[iTrk]));
        //double dEta =(jeteta[ijet]-trkEta[iTrk]);
        //dr = sqrt((dPhi*dPhi)+(dEta*dEta));
        //rhoR->Fill(dr,trkPt[iTrk]/jetpt[ijet]); // Fill(what you plot, how you weight)
        
        //}
        
    }
    
    signalJetEphoE->Scale(1.0/(sig_nphotons*(2.0/16.0)));
    mix_signalJetEphoE->Scale(1.0/(sig_nphotons*(2.0/16.0)));
    sidebandJetEphoE->Scale(1.0/(side_nphotons*(2.0/16.0)));
    mix_sidebandJetEphoE->Scale(1.0/(side_nphotons*(2.0/16.0)));
    outputfile->Write();
    outputfile->Close();
    
    //how is the outputfile filled with all the generated data?
}


int main(int argc, char *argv[])
{
    analysis * ana = new analysis();
    ana->Loop(std::atoi(argv[1]),std::atoi(argv[2]));
    return 0;
}

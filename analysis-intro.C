//work towards fig 7 http://cds.cern.ch/record/2217884/files/HIN-16-002-pas.pdf, fig 3 https://arxiv.org/pdf/1310.0878.pdf

#define analysis_cxx
#include "analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void analysis::Loop()
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
    TFile * outputfile = new TFile("outfilename1.root","recreate");
    TH1D * jetEphoE = new TH1D("jetEphoE","title;xaxis title;yaxis title",16,0,2);
    TH1D * rhoR = new TH1D("rhoR","title;xaxis title;yaxis title",20,0,1);
    
    Long64_t nentries = fChain->GetEntriesFast();
    
    int nphotons = 0;
    
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        //if (jentry > 10000) break; to check earlier
        if (jentry > 10000) break;
        Long64_t ientry = LoadTree(jentry);
        if (jentry%10000==0) cout<< jentry << " out of " << nentries << endl ;
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        if( phoEtCorrected[jentry] < 60 ) continue;
        nphotons++;
        if(hiBin>20) continue;
    
        for(int ijet = 0 ; ijet < njet ; ijet++) {
            if(jetpt[ijet]<30) continue;
            if(jeteta[ijet]>1.6) continue;
            if(acos(cos(phoPhi[ijet]-jetphi[ijet]))<(7.0*3.14/8.0)) continue;
            jetEphoE->Fill(jetpt[ijet]/phoEtCorrected[ijet]);
            
            for(int iTrk = 0; iTrk < nTrk; iTrk++) {
                double dr;
                double dPhi = acos(cos(jetphi[ijet]-trkPhi[iTrk]));
                double dEta =(jeteta[ijet]-trkEta[iTrk]);
                dr = sqrt((dPhi*dPhi)+(dEta*dEta));
                rhoR->Fill(dr,trkPt[iTrk]/jetpt[ijet]); // Fill(what you plot, how you weight)
                
            }
        }
    }
    
    jetEphoE->Scale(1.0/(nphotons*(2.0/16.0)));
    outputfile->Write();
    outputfile->Close();
}


int main(int argc, char *argv[])
{
    analysis * ana = new analysis();
    ana->Loop();
    return 0;
}

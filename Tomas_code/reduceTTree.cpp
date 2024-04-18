#include <TString.h>
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include <vector>
#include "iostream"
#include "fstream"
#include "MyClass.C"
using namespace std;

void reduceTTree()
{
    // Creamos un objeto TChain para leer los rootfiles
    TChain *ch = new TChain("Delphes", "");
    ch->Add("../tag_1_delphes_events.root");
    // ch->Add("../rootfiles/run_02.root");
    // ch->Add("../rootfiles/run_03.root");
    // ch->Add("../rootfiles/run_04.root");
    TTree *Delphes = (TTree*)ch;
    Delphes->MakeClass("MyClass"); // Crea la clase de lectura de la TTree
    MyClass t(Delphes); // Evalúa la clase de lectura de la TTree
    Long64_t nentries = t.fChain->GetEntries(); // Obtiene el número de entradas en la TTree 
    
    TFile* outputFile = new TFile("reduceTTree.root", "RECREATE");
    TTree *newtree = new TTree("newtree","Reduced Tree");

    // Using vectors to handle multiple entries per event
    vector<Float_t> Jet_PT, Jet_Eta, Jet_Phi, Jet_Mass;
    UInt_t Jet_Size; // Assuming you still want to track the size
    vector<Float_t> Particle_PT, Particle_Eta, Particle_Phi, Particle_Mass;

    // Create branches for vectors
    newtree->Branch("Jet_PT", &Jet_PT);
    newtree->Branch("Jet_Eta", &Jet_Eta);
    newtree->Branch("Jet_Phi", &Jet_Phi);
    newtree->Branch("Jet_Mass", &Jet_Mass);
    newtree->Branch("Jet_Size", &Jet_Size, "Jet_Size/i");
    newtree->Branch("Particle_PT", &Particle_PT);
    newtree->Branch("Particle_Eta", &Particle_Eta);
    newtree->Branch("Particle_Phi", &Particle_Phi);
    newtree->Branch("Particle_Mass", &Particle_Mass);

    cout << " Entries : " << nentries << endl;
    
    Int_t nTen = nentries / 10; // Para imprimir el porcentaje de avance
    Int_t k = 0;
    Int_t nbytes = 0, nb = 0;

    vector<TLorentzVector> jets; // Vector para almacenar los jets
    vector<TLorentzVector> particles; // Vector para almacenar las particles

    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {   
        // Clear vectors for this entry
        Jet_PT.clear();
        Jet_Eta.clear();
        Jet_Phi.clear();
        Jet_Mass.clear();
        Particle_PT.clear();
        Particle_Eta.clear();
        Particle_Phi.clear();
        Particle_Mass.clear();
        
        Long64_t ientry = t.LoadTree(jentry); 
        if (ientry < 0)                        // Para imprimir el porcentaje de avance
            break;
        nb = t.fChain->GetEntry(jentry);
        nbytes += nb;

        if (jentry % nTen == 0)
            cout << 10 * (jentry / nTen) << "%-" << flush;
        if (jentry == nentries - 1)
            cout << "100%"<< endl;

        if (t.Jet_size == 0) continue;

        for (Int_t i = 0; i < t.Jet_size; i++) {
        Jet_PT.push_back(t.Jet_PT[i]);
        Jet_Eta.push_back(t.Jet_Eta[i]);
        Jet_Phi.push_back(t.Jet_Phi[i]);
        Jet_Mass.push_back(t.Jet_Mass[i]);
        }

        for (Int_t i = 0; i < t.Particle_size; i++) {
        Particle_PT.push_back(t.Particle_PT[i]);
        Particle_Eta.push_back(t.Particle_Eta[i]);
        Particle_Phi.push_back(t.Particle_Phi[i]);
        Particle_Mass.push_back(t.Particle_Mass[i]);
    }
    /*
        // Escribe en el archivo CSV de los jets
        for (Int_t i = 0; i < t.Jet_size; i++)
        {
            TLorentzVector jetVector;
            jetVector.SetPtEtaPhiM(t.Jet_PT[i], t.Jet_Eta[i], t.Jet_Phi[i], t.Jet_Mass[i]);
            jets.push_back(jetVector);
        }
        
        // Escribe en el archivo CSV de las partículas
        for (Int_t i = 0; i < t.Particle_size; i++)
        {
            // Filling particles vector
            TLorentzVector particleVector;
            particleVector.SetPtEtaPhiM(t.Particle_PT[i], t.Particle_Eta[i], t.Particle_Phi[i], t.Particle_Mass[i]); // Assuming mass is negligible
            particles.push_back(particleVector);
        }
        */
        newtree->Fill();

    }
    

/*
    vector<vector<Double_t>> deltaRValues(jets.size());

    for (size_t i = 0; i < jets.size(); ++i) {
        //deltaRValues[i].reserve(particles.size()); // Para optimizar la asignación de memoria

        for (size_t j = 0; j < particles.size(); ++j) {
            Double_t deltaR = jets[i].DeltaR(particles[j]);
            if (deltaR < 0.4){
            deltaRValues[i].push_back(deltaR); // Aquí debería ser seguro hacer push_back
        }
        }
    }
*/
    cout << "El archivo se escribió correctamente" << endl;

    outputFile->Write("",TObject::kOverwrite);

    cout << "Fin del programa" << endl;
}
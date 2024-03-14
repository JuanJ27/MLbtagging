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

void TTreeToCsv()
{
    // Creamos un objeto TChain para leer los rootfiles
    TChain *ch = new TChain("Delphes", "");
    ch->Add("../rootfiles/run_01.root");
    // ch->Add("../rootfiles/run_02.root");
    // ch->Add("../rootfiles/run_03.root");
    // ch->Add("../rootfiles/run_04.root");
    TTree *Delphes = (TTree*)ch;
    Delphes->MakeClass("MyClass"); // Crea la clase de lectura de la TTree
    MyClass t(Delphes); // Evalúa la clase de lectura de la TTree
    Long64_t nentries = 1000;//t.fChain->GetEntries(); // Obtiene el número de entradas en la TTree 

    cout << " Entries : " << nentries << endl;
    
    // Abre los archivos de salida
    ofstream salida_jet("jet_out.csv");
    ofstream salida_particle("particle_out.csv");

    // Escribe el encabezado de los archivos CSV
    salida_jet <<"Evento"<<","<<"Jet.Size"<<","<< "Jet.PT" <<","<< "Jet.Eta"<<","<< "Jet.Phi" << endl;
    salida_particle <<"Evento"<<","<<"Particle.Size"<<","<< "Particle.PT" <<","<< "Particle.Eta"<<","<< "Particle.Phi" << endl;

    Int_t nTen = nentries / 10; // Para imprimir el porcentaje de avance
    Int_t k = 0;
    Int_t nbytes = 0, nb = 0;

    vector<TLorentzVector> jets; // Vector para almacenar los jets
    vector<TLorentzVector> particles; // Vector para almacenar las particles

    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        Long64_t ientry = t.LoadTree(jentry); 
        if (ientry < 0)                        // Para imprimir el porcentaje de avance
            break;
        nb = t.fChain->GetEntry(jentry);
        nbytes += nb;

        if (jentry % nTen == 0)
            cout << 10 * (jentry / nTen) << "%-" << flush;
        if (jentry == nentries - 1)
            cout << "100%"<< endl;

        // Escribe en el archivo CSV de los jets
        for (Int_t i = 0; i < t.Jet_size; i++)
        {
            TLorentzVector jetVector;
            jetVector.SetPtEtaPhiM(t.Jet_PT[i], t.Jet_Eta[i], t.Jet_Phi[i], 0.0);
            jets.push_back(jetVector);

            salida_jet <<ientry<<","<< t.Jet_size <<","<< t.Jet_PT[i] <<","<< t.Jet_Eta[i] <<","<< t.Jet_Phi[i] << endl; // Para cada jet escribe en el archivo CSV
        }
        
        // Escribe en el archivo CSV de las partículas
        for (Int_t i = 0; i < t.Particle_size; i++)
        {
            // Filling particles vector
            TLorentzVector particleVector;
            particleVector.SetPtEtaPhiM(t.Particle_PT[i], t.Particle_Eta[i], t.Particle_Phi[i], 0); // Assuming mass is negligible
            particles.push_back(particleVector);
        //    salida_particle <<ientry<<","<< t.Particle_size <<","<< t.Particle_PT[i] <<","<< t.Particle_Eta[i] <<","<< t.Particle_Phi[i] << endl; // Para cada partícula escribe en el archivo CSV
        }
    }

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

    cout << "El archivo se escribió correctamente" << endl;

    // Cierra el archivo CSV
    salida_jet.close();
    salida_particle.close();

    cout << "Fin del programa" << endl;
}
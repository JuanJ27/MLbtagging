int Read_Root_File() {
  Float_t jetSize, jetPt, jetEta, jetPhi;
  Float_t particleSize, particlePt, particleEta, particlePhi;

  TFile f ("rootfiles/tag_1_delphes_events.root", "READ");
  TTree *t = (TTree*) f.Get ("Delphes");
  
  t->SetBranchAddress("Jet_size", &jetSize);
  t->SetBranchAddress("Jet.PT", &jetPt);
  t->SetBranchAddress("Jet.Eta", &jetEta);
  t->SetBranchAddress("Jet.Phi", &jetPhi);
  t->SetBranchAddress("Particle_size", &particleSize);
  t->SetBranchAddress("Particle.PT", &particlePt);
  t->SetBranchAddress("Particle.Eta", &particleEta);
  t->SetBranchAddress("Particle.Phi", &particlePhi);

  cout << "* This tree has " << t->GetEntries() << " entries.\n\n";

  Long64_t nentries = t->GetEntries();
  for (int ievt=0; ievt<nentries; ievt++) 
  {
    //t->GetEntry (ievt);
    Long64_t ientry = t->LoadTree(ievt);
    if (ientry < 0) break;
    t->GetEntry(ievt);
    
    cout << "* Event " << ievt << " has " << t.jetSize << " jets:\n";
    
    }
  return 0;
}
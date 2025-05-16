void PlotCode(){
    TFile *myFile = TFile::Open("OO_JPsi_dimu.root");
    myFile->ls();
    TDirectory *dir = (TDirectory*)myFile->Get("diMuAna");
    dir->ls();
    TTree *tree = (TTree*)dir->Get("ParticleTree");

    TH2F *h2 = new TH2F("h2", "gen_mass vs gen_pT;gen_mass;gen_pT", 100, 0, 2, 100, 0, 2);
    std::cout << "Total Entries: " << tree->GetEntries() << std::endl;


    for (int ievent = 0; ievent < 1000; ievent++) {
    // Declare a pointer to hold the branch data
    std::vector<int> *gen_pdgId = nullptr;
    std::vector<float> *gen_mass = nullptr;
    std::vector<float> *gen_pT = nullptr;
    unsigned int EventNb = 0;

    // Set the branch address to point to the gen_pdgId branch (only do this once)
    if (ievent == 0) {
        tree->SetBranchAddress("gen_pdgId", &gen_pdgId);
        tree->SetBranchAddress("gen_mass", &gen_mass);
        tree->SetBranchAddress("gen_pT", &gen_pT);
        tree->SetBranchAddress("EventNb", &EventNb);
    }

    // Get the entry corresponding to the loop index
    tree->GetEntry(ievent);

    // Check the size of gen_pdgId to avoid out-of-range errors
    // if (gen_pdgId->empty()) {
    //     std::cout << "Entry " << ievent << " has no gen_pdgId values." << std::endl;
    //     continue;
    // }

    // Loop over elements in the gen_pdgId vector
    for (size_t i = 0; i < gen_pdgId->size(); ++i) {
        // std::cout << "Entry " << ievent << ", gen_mass[" << i << "] = " << gen_mass->at(i) << std::endl;
        // std::cout << "Entry " << ievent << ", gen_pT[" << i << "] = " << gen_pT->at(i) << std::endl;
        // std::cout << "Entry " << ievent << ", EventNb = " << EventNb << std::endl;
        std::cout << "Entry " << ievent << ", gen_pT[" << i << "] = " << gen_pdgId->at(i) << std::endl;
        h2->Fill(gen_mass->at(i), gen_pT->at(i), EventNb);
    }
    }
    //TCanvas *canvas = new TCanvas("canvas", "gen_mass vs gen_pT", 800, 600);
    //h2->Draw("COLZ");
}

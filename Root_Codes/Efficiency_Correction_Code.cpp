#include <vector>
#include <TMath.h>

// Function to check if a point is inside the excluded region
bool isInsideExcludedRegion(float eta, float pT) {
    // Define the vertices of the excluded region (spanning both positive and negative eta)
    std::vector<std::pair<float, float>> excludedRegion = {
        {1.0, 3.3}, {1.3, 2.1}, {1.3, 1.53}, {1.7, 1.0}, {2.4, 1.0}, // Positive eta
        {-2.4, 1.0}, {-1.7, 1.0}, {-1.3, 1.53}, {-1.3, 2.1}, {-1.0, 3.3}, // Negative eta
        {1.0, 3.3} // Closing the loop
    };

    // Helper function to check if a point is inside a polygon using the winding number method
    auto isInsidePolygon = [](float x, float y, const std::vector<std::pair<float, float>>& polygon) {
        int windingNumber = 0;
        size_t n = polygon.size();
        for (size_t i = 0; i < n; ++i) {
            float x1 = polygon[i].first;
            float y1 = polygon[i].second;
            float x2 = polygon[(i + 1) % n].first;
            float y2 = polygon[(i + 1) % n].second;

            if (y1 <= y) {
                if (y2 > y && (x2 - x1) * (y - y1) - (x - x1) * (y2 - y1) > 0)
                    ++windingNumber;
            } else {
                if (y2 <= y && (x2 - x1) * (y - y1) - (x - x1) * (y2 - y1) < 0)
                    --windingNumber;
            }
        }
        return windingNumber != 0; // Inside if winding number is non-zero
    };

    // Check if the point is inside the excluded region
    return isInsidePolygon(eta, pT, excludedRegion);
}

void PlotCode() {
    TFile *myFile = TFile::Open("~/Desktop/UPC/merged.root");
    if (!myFile || myFile->IsZombie()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    myFile->cd("diMuAna");
    TDirectory *dir = gDirectory;
    dir->ls();

    TTree *tree = (TTree*)dir->Get("ParticleTree");
    if (!tree) {
        std::cerr << "Error: ParticleTree not found!" << std::endl;
        return;
    }

    const int nBins = 10;
    double binEdges[nBins + 1] = {-2.4, -2.2, -1.9, -1.6, -1.2, 0, 1.2, 1.6, 1.9, 2.2, 2.4};
    double totbinEdges[13] = {-10, -2.4, -2.2, -1.9, -1.6, -1.2, 0, 1.2, 1.6, 1.9, 2.2, 2.4, 10};

    // Set up the histograms
    TH2F *h0 = new TH2F("h0", "p_{T} vs. #eta;#eta;p_{T} (GeV)", 100, -2.5, 2.5, 100, 1, 4);
    TH1F* h1 = new TH1F("h1", "Candidate Rapidity", nBins, binEdges);
    TH1F* h2 = new TH1F("h2", "Accepted Generate Rapidity", nBins, binEdges);
    TH1F* h3 = new TH1F("h3", "Total Rapidity", 100, -10, 10);
    TH1F* h4 = new TH1F("h4", "Ntrig", nBins, binEdges);
    TH1F* h5 = new TH1F("h5", "Nreco/Nacc", nBins, binEdges);
    TH1F* h6 = new TH1F("h6", "Ntrig/Nreco", nBins, binEdges);
    TH1F* h7 = new TH1F("h7", "Ncand", nBins, binEdges);
    TH1F* h8 = new TH1F("h8", "Ncand / Ngen", nBins, binEdges);
    TH1F* h9 = new TH1F("h9", "Ntrig / Ngen", nBins, binEdges);

    // Pointers to the branches (vectors)
    std::vector<int>   *cand_pdgId = nullptr;
    std::vector<int>   *gen_pdgId = nullptr;
    std::vector<float> *cand_mass  = nullptr;
    std::vector<float> *cand_eta  = nullptr;
    std::vector<float> *cand_y     = nullptr;
    std::vector<float> *cand_pT     = nullptr;
    std::vector<float> *gen_y      = nullptr;
    std::vector<float> *gen_p      = nullptr;
    std::vector<float> *gen_pT     = nullptr;
    std::vector<float> *gen_mass   = nullptr;
    std::vector<float> *gen_phi    = nullptr;
    std::vector<float> *gen_eta    = nullptr;
    std::vector<bool> *passHLT     = nullptr;
    std::vector<bool> *muon_softID = nullptr;

    double kineacc = 0;

    tree->SetBranchAddress("cand_pdgId", &cand_pdgId);
    tree->SetBranchAddress("gen_pdgId",  &gen_pdgId);
    tree->SetBranchAddress("cand_mass",  &cand_mass);
    tree->SetBranchAddress("cand_eta",   &cand_eta);
    tree->SetBranchAddress("cand_y",     &cand_y);
    tree->SetBranchAddress("cand_pT",  &cand_pT);
    tree->SetBranchAddress("gen_y",      &gen_y);
    tree->SetBranchAddress("gen_p",      &gen_p);
    tree->SetBranchAddress("gen_pT",     &gen_pT);
    tree->SetBranchAddress("gen_mass",   &gen_mass);
    tree->SetBranchAddress("gen_phi",    &gen_phi);
    tree->SetBranchAddress("gen_eta",    &gen_eta);
    tree->SetBranchAddress("passHLT",    &passHLT);
    tree->SetBranchAddress("muon_softID",&muon_softID);

    for (int ievent = 0; ievent < tree->GetEntries(); ievent++) {
        tree -> GetEntry(ievent);
        //--------------------------------------------------------------------------------
        //SoftMuon Selection Part

        double softmuon = 0;
        for (int i = 0; i < muon_softID->size(); ++i) {
            if (muon_softID->at(i)){
                softmuon += 1;
            }
        }
        
        //--------------------------------------------------------------------------------
        //This part Constructs the Generation
        TLorentzVector vec0;
        vec0.SetPtEtaPhiM(gen_pT->at(0), gen_eta->at(0), gen_phi->at(0), gen_mass->at(0));
        TLorentzVector vec1;
        vec1.SetPtEtaPhiM(gen_pT->at(1), gen_eta->at(1), gen_phi->at(1), gen_mass->at(1));
        TLorentzVector comb;
        comb = vec0 + vec1;

        double combmass = comb.M();
        double combrap = comb.Rapidity();

        //--------------------------------------------------------------------------------
        //This part constructs kinematic muon selection
        double kinemuon = 0;
        for (size_t i = 0; i < gen_eta->size(); ++i) {
            float eta = gen_eta->at(i);
            float pT = gen_pT->at(i);

            // Fill the histogram only if the point is outside the excluded region
            if (!isInsideExcludedRegion(eta, pT)) {
                //h0->Fill(eta, pT);
                kinemuon += 1;
            }
        }
        // Construct h2, the kinematic accepted generation event
        if (kinemuon == 2 && combmass >= 2.9 && combmass <= 3.3){
                h2->Fill(combrap);
                kineacc += 1;
            }
        
        //--------------------------------------------------------------------------------
        //This part selects the candidate events (Reconstruction Events)
        //First lets loop through all the particles, in this case there are two muons/antimuon and one J/Psi
        double ckinemuon = 0;
        for (size_t i = 0; i < cand_pdgId->size(); ++i) {
            if (cand_pdgId->at(i) == 13 || cand_pdgId->at(i) == -13){
                float eta = cand_eta->at(i);
                float pT = cand_pT->at(i);
        //Check if the particle is in the range of kinematic acceptance
                if (!isInsideExcludedRegion(eta, pT)) {
                    ckinemuon += 1;
                    h0->Fill(eta, pT);
                } 
            } 
        }

        for (size_t i = 0; i < cand_pdgId->size(); ++i) {
        //Check if the particle is J/Psi (pdgId 443)
            if (cand_pdgId->at(i) == 443 && ckinemuon == 2 && cand_mass->at(i) >= 2.9 && cand_mass->at(i) <= 3.3 && softmuon >= 1){
                h7->Fill(cand_y->at(i));
                if (passHLT->at(17) && passHLT->at(18)) {
                h4->Fill(cand_y->at(i));
                }
            }
        }
    }

    //--------------------------------------------------------------------------------
    //This part calculates the construction efficiency
    for (int iBin = 1; iBin <= h7->GetNbinsX(); ++iBin) {
        double binContent7 = h7->GetBinContent(iBin);
        double binContent2 = h2->GetBinContent(iBin);
        double Efficiency = binContent7 / binContent2;
        if (binContent2 == 0){
            h8->SetBinContent(iBin, 0);}
            else {
                h8->SetBinContent(iBin, Efficiency);
            }
        double binLowEdge = h2->GetBinLowEdge(iBin);
        double binUpEdge  = h2->GetBinLowEdge(iBin + 1);   
        // std::cout << "Bin " << iBin 
        //           << " (range [" << binLowEdge << ", " << binUpEdge << ")): "
        //           << Efficiency << std::endl;     
    }

    std::cout << "Kinematic Accepted Rate:" << kineacc / 947000 << std::endl;

    //--------------------------------------------------------------------------------
    //The part discussed the trigger Efficiency
     for (int iBin = 1; iBin <= h4->GetNbinsX(); ++iBin) {
        double binContent4 = h4->GetBinContent(iBin);
        double binContent7 = h7->GetBinContent(iBin);
        double Efficiency = binContent4 / binContent7;
        if (binContent7 == 0){
            h6->SetBinContent(iBin, 0);
            } else {
                 h6->SetBinContent(iBin, Efficiency);
            }
        double binLowEdge = h7->GetBinLowEdge(iBin);
        double binUpEdge  = h7->GetBinLowEdge(iBin + 1);   
        // std::cout << "Ncand/Ngen:" << std::endl;
        // std::cout << "Bin " << iBin 
        //           << " (range [" << binLowEdge << ", " << binUpEdge << ")): "
        //           << Efficiency << std::endl;     
    }

//Calculate the total efficiency, by dividing the two histograms h4 and h7
    for (int iBin = 1; iBin <= h4->GetNbinsX(); ++iBin) {
        double binContent4 = h4->GetBinContent(iBin);
        double binContent2 = h2->GetBinContent(iBin);
        double Efficiency = binContent4 / binContent2;
        if (binContent2 == 0){
            h9->SetBinContent(iBin, 0);}
            else {
                h9->SetBinContent(iBin, Efficiency);
            }
        double binLowEdge = h2->GetBinLowEdge(iBin);
        double binUpEdge  = h2->GetBinLowEdge(iBin + 1);   
        std::cout << "Ntrig/Ngen:" << std::endl;
        std::cout << "Bin " << iBin 
                  << " (range [" << binLowEdge << ", " << binUpEdge << ")): "
                  << Efficiency << std::endl;     
    }

    double trig_eff = h4->Integral() / h7->Integral();
    std::cout << "Trigger Effciency = " << trig_eff << std::endl;

    TCanvas *c1 = new TCanvas("c1", "Canvas 1", 800, 600);
    h4->Draw();

    TCanvas *c2 = new TCanvas("c2", "Canvas 2", 800, 600);
    h7->Draw();

    TCanvas *c3 = new TCanvas("c3", "Canvas 3", 800, 600);
    h8->Draw();

    TCanvas *c4 = new TCanvas("c4", "Canvas 4", 800, 600);
    h9->Draw();

    TCanvas *c5 = new TCanvas("c5", "Canvas 5", 800, 600);
    h6->Draw();

    // Draw the excluded region (red lines)
    // TPolyLine *excludedRegion = new TPolyLine(11);
    // excludedRegion->SetPoint(0, 1.0, 3.3);
    // excludedRegion->SetPoint(1, 1.3, 2.1);
    // excludedRegion->SetPoint(2, 1.3, 1.53);
    // excludedRegion->SetPoint(3, 1.7, 1.0);
    // excludedRegion->SetPoint(4, 2.4, 1.0);
    // excludedRegion->SetPoint(5, -2.4, 1.0);
    // excludedRegion->SetPoint(6, -1.7, 1.0);
    // excludedRegion->SetPoint(7, -1.3, 1.53);
    // excludedRegion->SetPoint(8, -1.3, 2.1);
    // excludedRegion->SetPoint(9, -1.0, 3.3);
    // excludedRegion->SetPoint(10, 1.0, 3.3); // Close the polygon
    // excludedRegion->SetLineColor(kRed);
    // excludedRegion->SetLineWidth(2);
    // excludedRegion->Draw("L");
}

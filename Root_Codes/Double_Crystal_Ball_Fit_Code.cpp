#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "RooDataHist.h"
#include "RooCBShape.h"
#include "RooCrystalBall.h"
#include "RooExtendPdf.h"
#include "RooAddPdf.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include <vector>
#include <iostream>
#include <TMath.h>
using namespace RooFit;

// Function to check if a point is inside the excluded region
bool isInsideExcludedRegion(float eta, float pT) {
    std::vector<std::pair<float, float>> excludedRegion = {
        {1.0, 3.3}, {1.3, 2.1}, {1.3, 1.53}, {1.7, 1.0}, {2.4, 1.0}, 
        {-2.4, 1.0}, {-1.7, 1.0}, {-1.3, 1.53}, {-1.3, 2.1}, {-1.0, 3.3},
        {1.0, 3.3} 
    };

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
        return windingNumber != 0;
    };

    return isInsidePolygon(eta, pT, excludedRegion);
}

void PlotCode(){
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

    // Create histograms for three rapidity regions
    TH1F* h_16_19 = new TH1F("h_16_19", "Mass Distribution (1.6 < y < 1.9)", 30, 2.7, 4.2);
    TH1F* h_19_21 = new TH1F("h_19_21", "Mass Distribution (1.9 < y < 2.1)", 30, 2.7, 4.2);
    TH1F* h_21_24 = new TH1F("h_21_24", "Mass Distribution (2.1 < y < 2.4)", 30, 2.7, 4.2);

    // Variables
    std::vector<int>   *cand_pdgId = nullptr;
    std::vector<int>   *gen_pdgId = nullptr;
    std::vector<float> *cand_mass  = nullptr;
    std::vector<float> *cand_eta   = nullptr;
    std::vector<float> *cand_y     = nullptr;
    std::vector<float> *cand_pT    = nullptr;
    std::vector<float> *gen_y      = nullptr;
    std::vector<float> *gen_p      = nullptr;
    std::vector<float> *gen_pT     = nullptr;
    std::vector<float> *gen_mass   = nullptr;
    std::vector<float> *gen_phi    = nullptr;
    std::vector<float> *gen_eta    = nullptr;
    std::vector<bool>  *passHLT    = nullptr;
    std::vector<bool>  *muon_softID= nullptr;

    // Branches
    tree->SetBranchAddress("cand_pdgId", &cand_pdgId);
    tree->SetBranchAddress("gen_pdgId",  &gen_pdgId);
    tree->SetBranchAddress("cand_mass",  &cand_mass);
    tree->SetBranchAddress("cand_eta",   &cand_eta);
    tree->SetBranchAddress("cand_y",     &cand_y);
    tree->SetBranchAddress("cand_pT",    &cand_pT);
    tree->SetBranchAddress("gen_y",      &gen_y);
    tree->SetBranchAddress("gen_p",      &gen_p);
    tree->SetBranchAddress("gen_pT",     &gen_pT);
    tree->SetBranchAddress("gen_mass",   &gen_mass);
    tree->SetBranchAddress("gen_phi",    &gen_phi);
    tree->SetBranchAddress("gen_eta",    &gen_eta);
    tree->SetBranchAddress("passHLT",    &passHLT);
    tree->SetBranchAddress("muon_softID",&muon_softID);

    for (int ievent = 0; ievent < tree->GetEntries(); ievent++) {

        //--------------------------------------------------------------------------------
        //SoftMuon Selection Part

        double softmuon = 0;
        for (int i = 0; i < muon_softID->size(); ++i) {
            if (muon_softID->at(i)){
                softmuon += 1;
            }
        }

        //Lorentz combine the daughters

        tree->GetEntry(ievent);

        TLorentzVector vec0, vec1, comb;
        vec0.SetPtEtaPhiM(gen_pT->at(0), gen_eta->at(0), gen_phi->at(0), gen_mass->at(0));
        vec1.SetPtEtaPhiM(gen_pT->at(1), gen_eta->at(1), gen_phi->at(1), gen_mass->at(1));
        comb = vec0 + vec1;

        double kinemuon = 0;
        for (size_t i = 0; i < gen_eta->size(); ++i) {
            float eta = gen_eta->at(i);
            float pT = gen_pT->at(i);
            if (!isInsideExcludedRegion(eta, pT)) {
                kinemuon += 1;
            }
        }

        for (size_t j = 0; j < cand_pdgId->size(); ++j) {
            if (cand_pdgId->at(j) == 443 && kinemuon == 2 && passHLT->at(17) && passHLT->at(18) && softmuon == 0) {
                float yval = comb.Rapidity();  // Use combined dimuon rapidity
                float mass = cand_mass->at(j);
                if (yval > 1.6 && yval <= 1.9) h_16_19->Fill(mass);
                else if (yval > 1.9 && yval <= 2.1) h_19_21->Fill(mass);
                else if (yval > 2.1 && yval <= 2.4) h_21_24->Fill(mass);
            }
        }
    }

    // Utility lambda to fit and draw
    auto FitAndDraw = [](TH1F* hist, const char* label, int canvasID) {
        RooRealVar x("m(J/#psi)", "m[GeV]", 2.7, 4.2);
        RooDataHist dh("dh", "dh", x, RooFit::Import(*hist));
        RooPlot* frame = x.frame();
        dh.plotOn(frame, RooFit::Name("hist"));

        // Define Double Crystal Ball
        RooRealVar mean("mean","mean", 3.09, 3.08, 3.1);
        RooRealVar sigmaL("sigmaL","sigmaL", 0.03, 0.01, 0.09);
        RooRealVar sigmaR("sigmaR","sigmaR", 0.03, 0.01, 0.09);
        RooRealVar alphaL("alphaL", "alphaL", 1.5, 0.5, 3.0);
        RooRealVar nL("nL", "nL", 2, 1, 10);
        RooRealVar alphaR("alphaR", "alphaR", 1.5, 0.5, 3.0);
        RooRealVar nR("nR", "nR", 2, 1, 10);

        RooCrystalBall dcb("dcb", "Double CB", x, mean, sigmaL, sigmaR, alphaL, nL, alphaR, nR);
        RooRealVar nsig("nsig", "nsig", 500, 0, 1e6);
        RooExtendPdf model("model", "model", dcb, nsig);

        RooFitResult* result = model.fitTo(dh, RooFit::Save(true), RooFit::PrintLevel(-1));
        model.plotOn(frame, RooFit::LineColor(kRed), RooFit::Name("fit"));

        double chi2 = frame->chiSquare("fit", "hist");

        std::cout << "=========== Fit for region: " << label << " ===========" << std::endl;
        result->Print("v");
        std::cout << "Chi^2 = " << chi2 << std::endl;
        std::cout << "========================================================" << std::endl;

        TCanvas* c = new TCanvas(Form("canvas%d", canvasID), label, 800, 600);
        frame->Draw();
    };

    int bin_low_16_19 = h_16_19->FindBin(2.9);
    int bin_high_16_19 = h_16_19->FindBin(3.2);
    std::cout << "[1.6 < y < 1.9] Integral [2.9, 3.2] = "
          << h_16_19->Integral(bin_low_16_19, bin_high_16_19) << std::endl;

    int bin_low_19_21 = h_19_21->FindBin(2.9);
    int bin_high_19_21 = h_19_21->FindBin(3.2);
    std::cout << "[1.9 < y < 2.1] Integral [2.9, 3.2] = "
        << h_19_21->Integral(bin_low_19_21, bin_high_19_21) << std::endl;

    int bin_low_21_24 = h_21_24->FindBin(2.9);
    int bin_high_21_24 = h_21_24->FindBin(3.2);
    std::cout << "[2.1 < y < 2.4] Integral [2.9, 3.2] = "
        << h_21_24->Integral(bin_low_21_24, bin_high_21_24) << std::endl;


    FitAndDraw(h_16_19, "1.6 < y < 1.9", 1);
    FitAndDraw(h_19_21, "1.9 < y < 2.1", 2);
    FitAndDraw(h_21_24, "2.1 < y < 2.4", 3);
}

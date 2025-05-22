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

    struct CBParams {
        double mean, sigma, alpha, n, nsig;
    };

    CBParams FitCrystalBall(TH1F* hist, const char* label, RooRealVar& x) {
        RooDataHist dh(Form("dh_fit_%s", label), "", x, Import(*hist));

        RooRealVar mean("mean", "mean", 3.09, 3.08, 3.1);
        RooRealVar sigma("sigma", "sigma", 0.03, 0.01, 0.09);
        RooRealVar alpha("alpha", "alpha", 1.5, 0.5, 3.0);
        RooRealVar n("n", "n", 2, 1, 10);
        RooCBShape cb("cb", "", x, mean, sigma, alpha, n);
        RooRealVar nsig("nsig", "", hist->Integral(), 0, 1e6);
        RooExtendPdf model("model", "", cb, nsig);

        RooFitResult* fitResult = model.fitTo(dh, Save(true), PrintLevel(-1));

        std::cout << "=========== Fit Results for " << label << " ===========" << std::endl;
        fitResult->Print("v");
        std::cout << "========================================================" << std::endl;

        return {mean.getVal(), sigma.getVal(), alpha.getVal(), n.getVal(), nsig.getVal()};
    }

    void DrawCrystalBallFit(TH1F* hist, const char* label, const CBParams& p, RooRealVar& x, RooPlot* frame, Color_t color) {
        RooDataHist dh(Form("dh_draw_%s", label), "", x, Import(*hist));

        RooRealVar mean(Form("mean_draw_%s", label), "", p.mean);
        RooRealVar sigma(Form("sigma_draw_%s", label), "", p.sigma);
        RooRealVar alpha(Form("alpha_draw_%s", label), "", p.alpha);
        RooRealVar n(Form("n_draw_%s", label), "", p.n);
        RooCBShape cb(Form("cb_draw_%s", label), "", x, mean, sigma, alpha, n);
        RooRealVar nsig(Form("nsig_draw_%s", label), "", p.nsig);
        RooExtendPdf model(Form("model_draw_%s", label), "", cb, nsig);

        dh.plotOn(frame, MarkerColor(color), LineColor(color), Name(Form("hist_%s", label)));
        model.plotOn(frame, LineColor(color), Name(Form("fit_%s", label)));
    }

void PlotCode(){
    TFile *File1 = TFile::Open("~/Desktop/UPC/Trees/diMu_ana_mc_0520.root");
    TFile *File2 = TFile::Open("~/Desktop/UPC/Trees/diMu_ana_mc_inc_0516.root");
    TFile *File3 = TFile::Open("~/Desktop/UPC/Trees/diEE_ana_mc_0516.root");
    TFile *File4 = TFile::Open("~/Desktop/UPC/Trees/diEE_ana_mc_inc_0516.root");

    // Check for file open errors
    if (!File1 || File1->IsZombie()) { std::cerr << "Error opening File1\n"; return; }
    if (!File2 || File2->IsZombie()) { std::cerr << "Error opening File2\n"; return; }
    if (!File3 || File3->IsZombie()) { std::cerr << "Error opening File3\n"; return; }
    if (!File4 || File4->IsZombie()) { std::cerr << "Error opening File4\n"; return; }

    // Navigate to the "diMuAna" or "diEEAna" directory in each file
    File1->cd("diMuAna");
    TTree* tree_mu_coh = (TTree*)gDirectory->Get("ParticleTree");

    File2->cd("diMuAna");
    TTree* tree_mu_inc = (TTree*)gDirectory->Get("ParticleTree");

    File3->cd("diMuAna");  // It's still under "diMuAna" despite being electrons (likely reused analyzer)
    TTree* tree_ee_coh = (TTree*)gDirectory->Get("ParticleTree");

    File4->cd("diMuAna");  // Same assumption
    TTree* tree_ee_inc = (TTree*)gDirectory->Get("ParticleTree");

    // Create histograms for the rapidity bins
    TH1F* CohMu_0_4 = new TH1F("CohMu_0_4", "Mass Distribution (0 < y < 4)", 20, 2.0, 3.4);
    TH1F* CohMu_4_8 = new TH1F("CohMu_4_8", "Mass Distribution (4 < y < 8)", 20, 2.0, 3.4);
    TH1F* CohMu_8_12 = new TH1F("CohMu_8_12", "Mass Distribution (8 < y < 1.2)", 20, 2.0, 3.4);
    TH1F* CohMu_12_16 = new TH1F("CohMu_12_16", "Mass Distribution (1.2 < y < 1.6)", 20, 2.0, 3.4);
    TH1F* CohMu_16_19 = new TH1F("CohMu_16_19", "Mass Distribution (1.6 < y < 1.9)", 20, 2.0, 3.4);
    TH1F* CohMu_19_24 = new TH1F("CohMu_19_24", "Mass Distribution (1.9 < y < 2.4)", 20, 2.0, 3.4);

    TH1F* InCohMu_0_4 = new TH1F("InCohMu_0_4", "Mass Distribution (0 < y < 4)", 20, 2.0, 3.4);
    TH1F* InCohMu_4_8 = new TH1F("InCohMu_4_8", "Mass Distribution (4 < y < 8)", 20, 2.0, 3.4);
    TH1F* InCohMu_8_12 = new TH1F("InCohMu_8_12", "Mass Distribution (8 < y < 1.2)", 20, 2.0, 3.4);
    TH1F* InCohMu_12_16 = new TH1F("InCohMu_12_16", "Mass Distribution (1.2 < y < 1.6)", 20, 2.0, 3.4);
    TH1F* InCohMu_16_19 = new TH1F("InCohMu_16_19", "Mass Distribution (1.6 < y < 1.9)", 20, 2.0, 3.4);
    TH1F* InCohMu_19_24 = new TH1F("InCohMu_19_24", "Mass Distribution (1.9 < y < 2.4)", 20, 2.0, 3.4);

    TH1F* CohE_0_4 = new TH1F("CohE_0_4", "Mass Distribution (0 < y < 4)", 20, 2.0, 3.4);
    TH1F* CohE_4_8 = new TH1F("CohE_4_8", "Mass Distribution (4 < y < 8)", 20, 2.0, 3.4);
    TH1F* CohE_8_12 = new TH1F("CohE_8_12", "Mass Distribution (8 < y < 1.2)", 20, 2.0, 3.4);
    TH1F* CohE_12_16 = new TH1F("CohE_12_16", "Mass Distribution (1.2 < y < 1.6)", 20, 2.0, 3.4);
    TH1F* CohE_16_19 = new TH1F("CohE_16_19", "Mass Distribution (1.6 < y < 1.9)", 20, 2.0, 3.4);
    TH1F* CohE_19_24 = new TH1F("CohE_19_24", "Mass Distribution (1.9 < y < 2.4)", 20, 2.0, 3.4);

    TH1F* InCohE_0_4 = new TH1F("InCohE_0_4", "Mass Distribution (0 < y < 4)", 20, 2.0, 3.4);
    TH1F* InCohE_4_8 = new TH1F("InCohE_4_8", "Mass Distribution (4 < y < 8)", 20, 2.0, 3.4);
    TH1F* InCohE_8_12 = new TH1F("InCohE_8_12", "Mass Distribution (8 < y < 1.2)", 20, 2.0, 3.4);
    TH1F* InCohE_12_16 = new TH1F("InCohE_12_16", "Mass Distribution (1.2 < y < 1.6)", 20, 2.0, 3.4);
    TH1F* InCohE_16_19 = new TH1F("InCohE_16_19", "Mass Distribution (1.6 < y < 1.9)", 20, 2.0, 3.4);
    TH1F* InCohE_19_24 = new TH1F("InCohE_19_24", "Mass Distribution (1.9 < y < 2.4)", 20, 2.0, 3.4);

    std::map<std::string, TH1F*> histMap = {
        {"CohMu_0_4", CohMu_0_4}, {"CohMu_4_8", CohMu_4_8}, {"CohMu_8_12", CohMu_8_12},
        {"CohMu_12_16", CohMu_12_16}, {"CohMu_16_19", CohMu_16_19}, {"CohMu_19_24", CohMu_19_24},

        {"InCohMu_0_4", InCohMu_0_4}, {"InCohMu_4_8", InCohMu_4_8}, {"InCohMu_8_12", InCohMu_8_12},
        {"InCohMu_12_16", InCohMu_12_16}, {"InCohMu_16_19", InCohMu_16_19}, {"InCohMu_19_24", InCohMu_19_24},

        {"CohE_0_4", CohE_0_4}, {"CohE_4_8", CohE_4_8}, {"CohE_8_12", CohE_8_12},
        {"CohE_12_16", CohE_12_16}, {"CohE_16_19", CohE_16_19}, {"CohE_19_24", CohE_19_24},

        {"InCohE_0_4", InCohE_0_4}, {"InCohE_4_8", InCohE_4_8}, {"InCohE_8_12", InCohE_8_12},
        {"InCohE_12_16", InCohE_12_16}, {"InCohE_16_19", InCohE_16_19}, {"InCohE_19_24", InCohE_19_24} };

    std::vector<std::pair<TTree*, std::string>> treeList = {
    {tree_mu_coh,  "CohMu"},
    {tree_mu_inc,  "InCohMu"},
    {tree_ee_coh,  "CohE"},
    {tree_ee_inc,  "InCohE"}
};
    std::vector<int>   *cand_pdgId = nullptr;
    std::vector<float> *cand_mass  = nullptr;
    std::vector<float> *cand_eta   = nullptr;
    std::vector<float> *cand_y     = nullptr;
    std::vector<float> *cand_pT    = nullptr;
    std::vector<float> *cand_phi    = nullptr;
    std::vector<bool>  *passHLT    = nullptr;
    std::vector<bool>  *muon_softID = nullptr;

    const double me = 0.000511;

    // Loop over trees
    for (auto& entry : treeList) {
        TTree* tree = entry.first;
        std::string prefix = entry.second;

        // Set branches
        tree->SetBranchAddress("cand_pdgId", &cand_pdgId);
        tree->SetBranchAddress("cand_mass",  &cand_mass);
        tree->SetBranchAddress("cand_eta",   &cand_eta);
        tree->SetBranchAddress("cand_y",     &cand_y);
        tree->SetBranchAddress("cand_pT",    &cand_pT);
        tree->SetBranchAddress("cand_phi",    &cand_phi);
        tree->SetBranchAddress("passHLT",    &passHLT);
        tree->SetBranchAddress("muon_softID", &muon_softID);

        for (int ievent = 0; ievent < tree->GetEntries(); ++ievent) {
            tree->GetEntry(ievent);

            float yval = -999, mass = -999;

            // ---------- Muon case: J/ψ is stored directly ----------
            if (prefix.find("Mu") != std::string::npos) {
                for (size_t j = 0; j < cand_pdgId->size(); ++j) {
                    if (cand_pdgId->at(j) == 443 &&
                        passHLT->size() > 18 && passHLT->at(17) && passHLT->at(18)) {

                        yval = cand_y->at(j);
                        mass = cand_mass->at(j);
                    }
                }
            }
            // ---------- Electron case: reconstruct from daughters ----------
            else {
                if (cand_pT->size() > 2 && cand_phi->size() > 2 && cand_eta->size() > 2 && passHLT->size() > 18 && passHLT->at(17) && passHLT->at(18)) {

                    TLorentzVector v1, v2, comb;
                    v1.SetPtEtaPhiM(cand_pT->at(1), cand_eta->at(1), cand_phi->at(1), me);  // phi is unused here
                    v2.SetPtEtaPhiM(cand_pT->at(2), cand_eta->at(2), cand_phi->at(2), me);
                    comb = v1 + v2;

                    yval = comb.Rapidity();
                    mass = comb.M();
                }
            }

            // ---------- Fill appropriate histogram ----------
            if (yval < 0) continue;

            std::string hname;
            if (yval > 0 && yval <= 0.4)      hname = prefix + "_0_4";
            else if (yval > 0.4 && yval <= 0.8) hname = prefix + "_4_8";
            else if (yval > 0.8 && yval <= 1.2) hname = prefix + "_8_12";
            else if (yval > 1.2 && yval <= 1.6) hname = prefix + "_12_16";
            else if (yval > 1.6 && yval <= 1.9) hname = prefix + "_16_19";
            else if (yval > 1.9 && yval <= 2.4) hname = prefix + "_19_24"; 

            if (!hname.empty() && histMap.count(hname)) {
                histMap[hname]->Fill(mass);
            } else if (!hname.empty()) {
                std::cerr << "Warning: Histogram not found for name " << hname << std::endl;
            }
        }
    }

        RooRealVar x("mJpsi", "m(J/#psi) [GeV]", 2.0, 3.6);
        RooPlot* frame = x.frame(Title("Combined Fit"));

        TH1F* h1 = histMap["CohMu_0_4"];
        TH1F* h2 = histMap["InCohMu_0_4"];
        TH1F* h3 = histMap["CohE_0_4"];
        TH1F* h4 = histMap["InCohE_0_4"];

        TH1F* hTotal = (TH1F*)h1->Clone("hTotal_0_4");
        hTotal->Add(h2);
        hTotal->Add(h3);
        hTotal->Add(h4);

        CBParams p1 = FitCrystalBall(h1, "CohMu_4_8", x);
        CBParams p2 = FitCrystalBall(h2, "InCohMu_4_8", x);
        CBParams p3 = FitCrystalBall(h3, "CohE_4_8", x);
        CBParams p4 = FitCrystalBall(h4, "InCohE_4_8", x);
        CBParams pTot = FitCrystalBall(hTotal, "Total_4_8", x);

        DrawCrystalBallFit(h1, "CohMu_0_4", p1, x, frame, kBlue);
        DrawCrystalBallFit(h2, "InCohMu_0_4", p2, x, frame, kRed);
        DrawCrystalBallFit(h3, "CohE_0_4", p3, x, frame, kGreen + 2);
        DrawCrystalBallFit(h4, "InCohE_0_4", p4, x, frame, kMagenta);
        DrawCrystalBallFit(hTotal, "Total_0_4", pTot, x, frame, kBlack);

        TCanvas* c = new TCanvas("c_all", "Combined Fit", 800, 600);
        frame->Draw();
}

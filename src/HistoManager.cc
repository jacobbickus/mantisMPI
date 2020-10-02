#include "HistoManager.hh"
#include "G4AutoDelete.hh"
#include "g4root.hh"
#include "G4RootAnalysisManager.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal HistoManager* the_analysis = 0;

//extern G4String gOutName;
extern G4bool output;

HistoManager* HistoManager::GetAnalysis()
{
  if(!the_analysis)
  {
    the_analysis = new HistoManager();
    G4AutoDelete::Register(the_analysis);
  }
  return the_analysis;
}
HistoManager::HistoManager(): fUseNtuple(true), fMergeNtuple(true), fchopper_hist(0)
{}

HistoManager::~HistoManager()
{}

void HistoManager::Book()
{
  if(output)
  {
    G4cout << "HistoManger::Book start, fUseNtuple: " << fUseNtuple << G4endl;
    G4AnalysisManager* manager = G4AnalysisManager::Instance();
    manager->SetVerboseLevel(4);

    // Create Histogram for Chopper Data
    fchopper_hist = manager->CreateH1("ChopperData","Chopper Data", 100, 0., 6*MeV);

    if(fUseNtuple)
    {
      // Create Ntuple for Interogation Object
      manager->CreateNtuple("IntObj","IntObj");
      manager->CreateNtupleDColumn("E_incident");
      manager->CreateNtupleIColumn("isNRF");
      manager->FinishNtuple();

      // Create Ntuple for incident water data
      manager->CreateNtuple("IncidentWater","IncidentWater");
      manager->CreateNtupleDColumn("E_incident");
      manager->CreateNtupleIColumn("isNRF");
      manager->FinishNtuple();

    // Create Ntuple for water data
      manager->CreateNtuple("WaterData","WaterData");

      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleDColumn("x");
      manager->CreateNtupleDColumn("y");
      manager->CreateNtupleDColumn("z");
      manager->CreateNtupleDColumn("Theta");
      manager->CreateNtupleDColumn("Time");
      manager->FinishNtuple();

      // Create Ntuple for Incident Detector Data

      manager->CreateNtuple("IncidentDet","IncidentDet");

      manager->CreateNtupleDColumn("E_incident");
      manager->CreateNtupleDColumn("x");
      manager->CreateNtupleDColumn("y");
      manager->CreateNtupleDColumn("z");
      manager->FinishNtuple();

      // Create Nutple for Energy if detected
      manager->CreateNtuple("Detected","Detected");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleDColumn("x");
      manager->CreateNtupleDColumn("y");
      manager->CreateNtupleDColumn("z");
      manager->FinishNtuple();

      // Create Ntuple for Reactions within detector
      manager->CreateNtuple("DetProcess","DetProcess");

      manager->CreateNtupleSColumn("Process");
      manager->FinishNtuple();
    }

      G4cout << "HistoManager::Book Finished" << G4endl;

  }
  else
  {
    std::cout << "Visualization...no histogramming." << std::endl;
  }

}

void HistoManager::OpenFile(const G4String& fname)
{
  G4AnalysisManager* manager = G4AnalysisManager::Instance();
  std::cout << "File Name to open " << fname << std::endl;
  manager->OpenFile(fname.c_str());
  std::cout << "File Opened" << std::endl;

}

void HistoManager::Save()
{
  G4AnalysisManager* manager = G4AnalysisManager::Instance();
  manager->Write();
}

void HistoManager::Close(G4bool reset)
{
  G4AnalysisManager *manager = G4AnalysisManager::Instance();
  manager->CloseFile(reset);
}

void HistoManager::FillChopper(G4double E_incident)
{
  G4AnalysisManager*mgr = G4AnalysisManager::Instance();
  mgr->FillH1(fchopper_hist, E_incident);
}

void HistoManager::FillIntObj(G4double E_incident, G4int isNRF)
{
  G4AnalysisManager*mgr = G4AnalysisManager::Instance();
  mgr->FillNtupleDColumn(0,0,E_incident);
  mgr->FillNtupleIColumn(0,1,isNRF);
  mgr->AddNtupleRow(0);
}

void HistoManager::FillIncidentWater(G4double E_incident, G4int isNRF)
{
  G4AnalysisManager*mgr = G4AnalysisManager::Instance();
  mgr->FillNtupleDColumn(2,0,E_incident);
  mgr->FillNtupleIColumn(2,1,isNRF);
  mgr->AddNtupleRow(2);
}

void HistoManager::FillWater(G4double Energy, G4double x, G4double y, G4double z, G4double theta, G4double thetime)
{
  G4AnalysisManager*mgr = G4AnalysisManager::Instance();
  mgr->FillNtupleDColumn(3,0,Energy);
  mgr->FillNtupleDColumn(3,1,x);
  mgr->FillNtupleDColumn(3,2,y);
  mgr->FillNtupleDColumn(3,3,z);
  mgr->FillNtupleDColumn(3,4,theta);
  mgr->FillNtupleDColumn(3,5,thetime);
  mgr->AddNtupleRow(3);
}

void HistoManager::FillIncDet(G4double E_incident, G4double x, G4double y, G4double z)
{
  G4AnalysisManager*mgr = G4AnalysisManager::Instance();
  mgr->FillNtupleDColumn(4,0,E_incident);
  mgr->FillNtupleDColumn(4,1,x);
  mgr->FillNtupleDColumn(4,2,y);
  mgr->FillNtupleDColumn(4,3,z);
  mgr->AddNtupleRow(4);

}

void HistoManager::FillDetected(G4double Energy, G4double x, G4double y, G4double z)
{
  G4AnalysisManager*mgr = G4AnalysisManager::Instance();
  mgr->FillNtupleDColumn(5,0,Energy);
  mgr->FillNtupleDColumn(5,1,x);
  mgr->FillNtupleDColumn(5,2,y);
  mgr->FillNtupleDColumn(5,3,z);
  mgr->AddNtupleRow(5);
}

void HistoManager::FillDetProcess(G4String Process)
{
  G4AnalysisManager*mgr = G4AnalysisManager::Instance();
  mgr->FillNtupleSColumn(6,0,Process);
  mgr->AddNtupleRow(6);
}

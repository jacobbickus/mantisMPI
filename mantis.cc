// Always include
#include "G4RunManager.hh"

#include "G4MPImanager.hh"
#include "G4MPIsession.hh"
#include "G4MPIextraWorker.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "physicsList.hh"
#include "ActionInitialization.hh"
#include "G4UserRunAction.hh"
#include "RunAction.hh"
// Typcially include
#include "time.h"
#include "Randomize.hh"
#include <iostream>
#include "G4Types.hh"
#include "G4UIExecutive.hh"

// For G4cout and G4cerr handling
#include "MySession.hh"
#include "G4ios.hh"
#include "G4UIsession.hh"

// declare global variables
G4long seed;
G4String root_output_name;
G4String gOutName;
G4bool output;

int main(int argc,char **argv)
{
  // Defaults
  G4int start_time = time(0);
  G4bool use_xsec_tables = true;
  G4bool use_xsec_integration = true;
  G4bool force_isotropic = false;
  G4bool addNRF = true;
  G4bool useNtuple = true;
  G4bool mergeNtuple = true;
  G4String macro = "mantis.in";
  seed = 1;
  output = true;
  root_output_name = "test";

  G4MPImanager* g4MPI = new G4MPImanager(argc,argv,1);
  g4MPI->SetVerbose(4);
  G4MPIsession* session = g4MPI->GetMPIsession();

  std::string RootOutputFile = (std::string)root_output_name;
  if(RootOutputFile.find(".root")<RootOutputFile.length()) {
          gOutName=(std::string)RootOutputFile.substr(0, RootOutputFile.find(".root"));
  }
  else gOutName=(std::string)root_output_name;

  G4UImanager* UI = G4UImanager::GetUIpointer();
  //MySession* LoggedSession = new MySession;

  //UI->SetCoutDestination(LoggedSession);

        // choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine);
  CLHEP::HepRandom::setTheSeed(seed);

  G4RunManager *runManager = new G4RunManager;
  runManager->SetUserInitialization(new DetectorConstruction);
  runManager->SetUserInitialization(new physicsList(addNRF, use_xsec_tables, use_xsec_integration, force_isotropic));
  runManager->SetUserInitialization(new ActionInitialization(useNtuple, mergeNtuple));

  runManager->Initialize();
  if ( g4MPI->IsExtraWorker() ) {
    G4cout << "Set extra worker" << G4endl;
    G4UserRunAction* runAction
      = const_cast<G4UserRunAction*>(runManager->GetUserRunAction());
    g4MPI->SetExtraWorker(new G4MPIextraWorker(runAction));
  }

  session->SessionStart();

  G4int stop_time = time(0);
  G4cout << "The MC took:\t\t" << stop_time - start_time << "s" <<G4endl;

  //delete LoggedSession;
  delete g4MPI;
  delete runManager;

  return 0;
}

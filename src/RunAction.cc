
#include "G4MPImanager.hh"
#include "HistoManager.hh"
#include "RunAction.hh"
#include "g4root.hh"
#include "RunMerger.hh"
#include "G4MPIhistoMerger.hh"
#include "G4MPIntupleMerger.hh" // must be kept in .cc file
#include "MyRun.hh"

extern G4bool output;
RunAction::RunAction(G4bool useNtuple, G4bool mergeNtuple)
        : G4UserRunAction(), fTimer(nullptr), fMPIntupleMerger(nullptr)
{
  fTimer = new G4Timer;
  //std::cout << "RunAction::RunAction Prior to calling fMPIntupleMerger" << std::endl;
  if(mergeNtuple && G4MPImanager::GetManager()->GetTotalSize() >=2)
  {
    G4int nofReducedNtupleFiles = 0;
    G4bool rowWise = true;
    fMPIntupleMerger = new G4MPIntupleMerger(nofReducedNtupleFiles, rowWise);
  }
  //std::cout << "RunAction::RunAction Prior to calling HistoManager" << std::endl;
  HistoManager* manager = HistoManager::GetAnalysis();
  manager->SetUseNtuple(useNtuple);
  manager->SetMergeNtuple(mergeNtuple);
  manager->Book();
}

RunAction::~RunAction()
{
  delete fTimer;
  delete fMPIntupleMerger;
}

G4Run* RunAction::GenerateRun()
{
  return new MyRun;
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4int rank = G4MPImanager::GetManager()->GetRank();
  HistoManager* manager = HistoManager::GetAnalysis();
  //G4int rank = G4MPImanager::GetManager()->GetRank();
  std::ostringstream fname;
  fname<<"test-rank"<<rank;
  std::cout << "RunAction::BeginOfRunAction:: prior to openfile" << std::endl;
  manager->OpenFile(fname.str());
  std::cout << "File " << fname.str() << " OPENED" << std::endl;

  fTimer->Start();
}

void RunAction::EndOfRunAction(const G4Run* arun)
{
  std::cout << "RunAction::EndOfRunAction prior to grabbing rank" << std::endl;
  const G4int rank = G4MPImanager::GetManager()->GetRank();
  if(! G4MPImanager::GetManager()->IsExtraWorker())
  {
    // this is where I merge Run objects
    // This is where I merge histograms
    // Save histograms before MPI merging for rank #0
    if(rank == 0)
    {
      HistoManager* manager = HistoManager::GetAnalysis();
      manager->Save();
      manager->Close(false); // close without resetting histograms
    }
    // Merge of g4analysis objects
    G4cout << "Go to merge histograms " << G4endl;
    G4MPIhistoMerger hm(G4AnalysisManager::Instance());
    hm.SetVerbosity(4);
    hm.Merge();
    G4cout << "Done merging histograms" << G4endl;
  }
  // Save g4analysis objects to a file
  // This is done after MPI-merging of histograms
  // one can save all ranks or just rank0
  if(true)
  {
    HistoManager* manager = HistoManager::GetAnalysis();
    if(rank == 0)
    {
      manager->OpenFile("test-merged");
    }
    manager->Save();
    manager->Close();
  }

  fTimer->Stop();
  G4cout << "End EndOfRunAction for master thread in rank: " << rank << G4endl;

}

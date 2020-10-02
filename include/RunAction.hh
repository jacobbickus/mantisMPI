#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4MPImanager.hh"
#include "HistoManager.hh"
#include "MyRun.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Timer.hh"

class G4MPIntupleMerger;
class G4Timer;

class RunAction : public G4UserRunAction
{
  public:
    RunAction(G4bool useNtuple, G4bool mergeNtuple);
    virtual ~RunAction();
    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

  private:
    G4Timer* fTimer;
    G4MPIntupleMerger* fMPIntupleMerger;
};


#endif

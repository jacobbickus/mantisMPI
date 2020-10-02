#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "EventAction.hh"


class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(G4bool, G4bool);
    virtual ~ActionInitialization();
    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    G4bool fUseNtuple;
    G4bool fMergeNtuple;
};

#endif

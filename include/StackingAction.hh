#ifndef StackingAction_h
#define StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4Neutron.hh"
//#include "EventAction.hh"

class StackingAction : public G4UserStackingAction
{
public:
StackingAction();
virtual ~StackingAction();

public:
virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
private:
  //EventAction* event;
};

#endif

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"
#include "G4OpBoundaryProcess.hh"
#include "RunAction.hh"
//#include "EventAction.hh"
#include "StackingAction.hh"
#include "StepMessenger.hh"
#include "eventInformation.hh"

#include "G4SteppingManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"

class StepMessenger;

class SteppingAction : public G4UserSteppingAction
{
public:
SteppingAction();
virtual ~SteppingAction();

// method from the base class
virtual void UserSteppingAction(const G4Step*);

public: // for now set to public until I can figure out how to work with protected

void SetChopperDataFlag(G4int val)
{
        drawChopperDataFlag = val;
}
void SetWaterDataFlag(G4int val){
        drawWaterFlag = val;
};
void SetIncidentDataFlag(G4int val){
        drawIncFlag = val;
};
void SetDetDataFlag(G4int val){
        drawDetFlag = val;
};
void SetIntObjDataFlag(G4int val){
        drawIntObjDataFlag = val;
};
void SetIncWatDataFlag(G4int val){
        drawWaterIncDataFlag = val;
};


private:
G4double weight;
int EventGeneratorParticle;
float LowEnergyCutoff;
G4ThreeVector p;
G4ThreeVector X;
G4ThreeVector Xdet, Xdetected;
G4ThreeVector incX;
//EventAction* event;
G4OpBoundaryProcessStatus fExpectedNextStatus;
G4String procCount;
G4int drawChopperDataFlag;
G4int drawIntObjDataFlag;
G4int drawWaterFlag;
G4int drawIncFlag;
G4int drawDetFlag;
G4int drawWaterIncDataFlag;
G4double det_energy;
StepMessenger* stepM;

};

#endif

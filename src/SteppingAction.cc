#include "SteppingAction.hh"
#include "HistoManager.hh"

SteppingAction::SteppingAction()
        : G4UserSteppingAction(), drawChopperDataFlag(1), drawIntObjDataFlag(1),
        drawWaterFlag(0), drawIncFlag(0), drawDetFlag(1), drawWaterIncDataFlag(0), stepM(NULL)
{
        stepM = new StepMessenger(this);
        fExpectedNextStatus = Undefined;
        //event = evt;
}

SteppingAction::~SteppingAction()
{
        delete stepM;
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

        // Run Logical Checks
        if(aStep->GetPostStepPoint()==NULL) {
                return; // at the end of the world
        }
        else if(aStep->GetPostStepPoint()->GetPhysicalVolume()==NULL) {
                return;
        }

        G4Track* theTrack = aStep->GetTrack();

        // Run Cuts
        if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0, 10, "Collimator") == 0)
        {
                // kill photons in collimator
                theTrack->SetTrackStatus(fStopAndKill);
                //event->AddStatusKilled();
        }
        else if(theTrack->GetPosition().z()/(cm) < -10.*cm)
        {
                // Kill photons that go in behind beam origin
                theTrack->SetTrackStatus(fStopAndKill);
                //event->AddStatusKilled();
        }

// *********************** Checks and Cuts Complete ************************ //

        G4int isNRF = 0;
        eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
        weight = info->GetWeight();
        G4String particleName = aStep->GetTrack()->GetDynamicParticle()
                                ->GetParticleDefinition()->GetParticleName();
        HistoManager* manager = HistoManager::GetAnalysis();

        if(theTrack->GetCreatorProcess() !=0)
        {
                G4String CPName = theTrack->GetCreatorProcess()->GetProcessName();
                if(CPName == "NRF")
                {
                        isNRF = 1;
                }
        }

        // Testing Brem Beam Analysis
        if(drawChopperDataFlag)
        {
                if(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName().compare(0, 7,"Chopper") == 0
                   && aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0, 7, "Chopper") != 0
                   && theTrack->GetParticleDefinition() == G4Gamma::Definition())
                {
                        G4double energy_chopper = theTrack->GetKineticEnergy()/(MeV);
                        manager->FillChopper(energy_chopper);
                }
        }
        // Testing NRF Analysis
        // inside Interogation Object for first time
        if(drawIntObjDataFlag)
        {
                if(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName().compare(0, 14,"IntObjPhysical") != 0
                   && aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0, 14, "IntObjPhysical") == 0)
                {
                        G4double energy_IntObj = theTrack->GetKineticEnergy()/(MeV);
                        manager->FillIntObj(energy_IntObj, isNRF);
                }
        }

        // Water Analysis
        // first time in detector determine incident water energies
        if(drawWaterIncDataFlag)
        {
                if(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName().compare(0, 5,"Water") == 0
                   && aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0, 5, "Water") != 0)
                {
                        G4double energy_inc_water = theTrack->GetKineticEnergy()/(MeV);
                        manager->FillIncidentWater(energy_inc_water,isNRF);
                }
        }

        // Here I am inside the water
        if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0,5,"Water")==0) {

                // only care about secondaries that occur in water volume
                const std::vector<const G4Track*>* secondaries = aStep->GetSecondaryInCurrentStep();

                if(secondaries->size()>0) {
                        for(unsigned int i=0; i<secondaries->size(); ++i) {
                                if(secondaries->at(i)->GetParentID()>0) {
                                        if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
                                        {
                                              if(drawWaterFlag)
                                              {
                                                X= secondaries->at(i)->GetPosition(); //take the position from the post step position
                                                p = secondaries->at(i)->GetMomentum();
                                                manager->FillWater(weight*secondaries->at(i)->GetKineticEnergy()/(MeV),
                                                X.x()/(cm), X.y()/(cm), X.z()/(cm),
                                                asin(sqrt(pow(p.x(),2)+pow(p.y(),2))/p.mag()),
                                                secondaries->at(i)->GetGlobalTime());

                                              }

                                        }
                                }
                        }
                } // end of optical photons if statement

        } // end of if loop while inside water

        // PMT Analysis

        G4StepPoint* endPoint   = aStep->GetPostStepPoint();
        G4StepPoint* startPoint = aStep->GetPreStepPoint();

        if(endPoint->GetStepStatus() == fGeomBoundary) {

                const G4DynamicParticle* theParticle = theTrack->GetDynamicParticle();

                G4ThreeVector oldMomentumDir = theParticle->GetMomentumDirection();

                G4ThreeVector m0 = startPoint->GetMomentumDirection(); // don't use these yet?
                G4ThreeVector m1 = endPoint->GetMomentumDirection();

                G4OpBoundaryProcessStatus theStatus = Undefined;
                G4ProcessManager* OpManager =
                        G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
                G4int MAXofPostStepLoops =
                        OpManager->GetPostStepProcessVector()->entries();
                G4ProcessVector* postStepDoItVector =
                        OpManager->GetPostStepProcessVector(typeDoIt);

                if(endPoint->GetPhysicalVolume()->GetName().compare(0,2,"PC")==0 && startPoint->GetPhysicalVolume()->GetName().compare(0,2,"PC")!=0) { // first time in photocathode
                        if(drawIncFlag)
                        {
                                Xdet = endPoint->GetPosition();
                                manager->FillIncDet(weight*theParticle->GetKineticEnergy()/(MeV),
                                Xdet.x()/(cm), Xdet.y()/(cm), Xdet.z()/(cm));
                        }


                        for (G4int i=0; i<MAXofPostStepLoops; ++i) {
                                G4VProcess* currentProcess = (*postStepDoItVector)[i];

                                G4OpBoundaryProcess* opProc = dynamic_cast<G4OpBoundaryProcess*>(currentProcess);

                                if(opProc) {
                                        theStatus = opProc->GetStatus();

                                        if(theStatus == Transmission)
                                        {
                                                procCount = "Trans";
                                        }
                                        else if(theStatus == FresnelRefraction) {
                                                procCount = "Refr";
                                        }
                                        else if (theStatus == TotalInternalReflection) {
                                                procCount = "Int_Refl";
                                        }
                                        else if (theStatus == LambertianReflection) {
                                                procCount = "Lamb";
                                        }
                                        else if (theStatus == LobeReflection) {
                                                procCount = "Lobe";
                                        }
                                        else if (theStatus == SpikeReflection) {
                                                procCount = "Spike";
                                        }
                                        else if (theStatus == BackScattering)
                                        {
                                                procCount = "BackS";
                                        }
                                        else if (theStatus == Absorption) {
                                                procCount = "Abs";
                                        }
                                        else if (theStatus == Detection) {
                                                //event->AddDetected();
                                                procCount = "Det";
                                                det_energy = theParticle->GetKineticEnergy()/(MeV);
                                                G4StepPoint* Xdetected_point = aStep->GetPostStepPoint();
                                                Xdetected = Xdetected_point->GetPosition();
                                                manager->FillDetected(det_energy*weight,Xdetected.x()/(cm),
                                                Xdetected.y()/(cm), Xdetected.z()/(cm));

                                        }
                                        else if (theStatus == NotAtBoundary) {
                                                procCount = "NotAtBoundary";
                                        }
                                        else if (theStatus == SameMaterial) {
                                                procCount = "SameMaterial";
                                        }
                                        else if (theStatus == StepTooSmall) {
                                                procCount = "SteptooSmall";
                                        }
                                        else if (theStatus == NoRINDEX) {
                                                procCount = "NoRINDEX";
                                        }
                                        else {
                                                G4cout << "theStatus: " << theStatus
                                                       << " was none of the above." << G4endl;
                                                procCount = "noStatus";
                                        }
                                } // for if opProc
                                if(drawDetFlag)
                                {
                                        manager->FillDetProcess(procCount);
                                }

                        } // for for loop


                } // for if statement if first time in photocathode

        } // for if at boundary
          //G4cout << "Stepping Fine" << G4endl;

} // end of user steepping action function

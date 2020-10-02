#ifndef HistoManager_h
#define HistoManager_h 1

#include <tools/histo/h1d>
#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)


class HistoManager
{
public:
~HistoManager();
static HistoManager* GetAnalysis();

void Book();
void EndOfRun();
void OpenFile(const G4String& fname);
void Save();
void Close(G4bool reset = true);

void SetUseNtuple(G4bool useNtuple)
{
  //G4cout << "Set useNtuple: " << useNtuple << G4endl;
  fUseNtuple = useNtuple;
}
void SetMergeNtuple(G4bool mergeNtuple)
{
  //G4cout << "Set mergeNtuple: " << mergeNtuple << G4endl;
  fMergeNtuple = mergeNtuple;
}
void FillChopper(G4double);
void FillIntObj(G4double, G4int);
void FillIncidentWater(G4double, G4int);
void FillWater(G4double, G4double, G4double, G4double, G4double, G4double);
void FillIncDet(G4double, G4double, G4double, G4double);
void FillDetected(G4double, G4double, G4double, G4double);
void FillDetProcess(G4String);

private:
  HistoManager();
  DISALLOW_COPY_AND_ASSIGN(HistoManager);
  G4bool fUseNtuple;
  G4bool fMergeNtuple;
  G4int fchopper_hist;

};

#endif

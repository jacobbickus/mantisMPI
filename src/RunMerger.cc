#include "RunMerger.hh"
#include "MyRun.hh"

void RunMerger::Pack()
{
  InputUserData(const_cast<int*>(&(fMyRun->fDummyCounter)), MPI::INT,1);
}

G4Run* RunMerger::UnPack()
{
  MyRun* aDummyRun = new MyRun;
  OutputUserData(&(aDummyRun->fDummyCounter),MPI::INT,1);
  return aDummyRun;
}

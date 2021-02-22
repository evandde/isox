#include "RunAction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction()
    : G4UserRunAction()
{
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run *aRun)
{
    G4RunManager::GetRunManager()->SetPrintProgress(static_cast<G4int>(aRun->GetNumberOfEventToBeProcessed() * .1));
}

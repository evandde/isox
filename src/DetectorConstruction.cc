#include "DetectorConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSEnergyDeposit.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    // materials
    G4NistManager *nist = G4NistManager::Instance();
    auto matAir = nist->FindOrBuildMaterial("G4_AIR");
    auto matAl = nist->FindOrBuildMaterial("G4_Al");
    auto matGe = nist->FindOrBuildMaterial("G4_Ge");
    auto matNaI = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");

    // vis
    auto visWhiteWire = new G4VisAttributes(G4Color::White());
    visWhiteWire->SetForceWireframe();
    auto visCyanSol = new G4VisAttributes(G4Color::Cyan());
    visCyanSol->SetForceSolid();
    auto colorGrayAlpha5 = G4Color::Gray();
    colorGrayAlpha5.SetAlpha(0.5);
    auto visGrayAlpha5Sol = new G4VisAttributes(colorGrayAlpha5);
    visGrayAlpha5Sol->SetForceSolid();

    // World
    auto worldSize = 1. * m;
    auto solWorld = new G4Box("World", .5 * worldSize, .5 * worldSize, .5 * worldSize);
    auto lvWorld = new G4LogicalVolume(solWorld, matAir, "World");
    lvWorld->SetVisAttributes(visWhiteWire);
    auto pvWorld = new G4PVPlacement(0, G4ThreeVector(), lvWorld, "World", nullptr, false, 0);

    // HPGe main detector
    auto mainDetHeight = 5. * cm;
    auto mainDetDiam = 5. * cm;
    auto solMainDet = new G4Tubs("MainDet", 0., .5 * mainDetDiam, .5 * mainDetHeight, 0., 360. * deg);
    auto lvMainDet = new G4LogicalVolume(solMainDet, matGe, "MainDet");
    lvMainDet->SetVisAttributes(visCyanSol);
    auto pvMainDet = new G4PVPlacement(0, G4ThreeVector(- solMainDet->GetOuterRadius(), 0., 0.), lvMainDet, "MainDet", lvWorld, false, 0);

    // NaI(Tl) sub detector
    auto subDetX0 = 2. * solMainDet->GetOuterRadius();
    auto subDetX1 = 5. * cm;
    auto subDetY0 = 2. * solMainDet->GetOuterRadius();
    auto subDetY1 = 5. * cm;
    auto subDetY2 = 5. * cm;
    auto subDetZ0 = 2. * solMainDet->GetZHalfLength();
    auto subDetZ1 = 5. * cm;
    auto solSubDet1 = new G4Box("SubDet1", .5 * (subDetX0 + subDetX1), .5 * subDetY1, .5 * (subDetZ0 + subDetZ1));
    auto lvSubDet1 = new G4LogicalVolume(solSubDet1, matNaI, "SubDet1");
    lvSubDet1->SetVisAttributes(visGrayAlpha5Sol);
    new G4PVPlacement(0,
                      pvMainDet->GetObjectTranslation() + G4ThreeVector(solMainDet->GetOuterRadius() - solSubDet1->GetXHalfLength(),
                                                                        solMainDet->GetOuterRadius() + solSubDet1->GetYHalfLength(),
                                                                        solMainDet->GetZHalfLength() - solSubDet1->GetZHalfLength()),
                      lvSubDet1, "SubDet1", lvWorld, false, 1);
    auto solSubDet2 = new G4Box("SubDet2", .5 * subDetX1, .5 * subDetY0, .5 * (subDetZ0 + subDetZ1));
    auto lvSubDet2 = new G4LogicalVolume(solSubDet2, matNaI, "SubDet2");
    lvSubDet2->SetVisAttributes(visGrayAlpha5Sol);
    new G4PVPlacement(0,
                      pvMainDet->GetObjectTranslation() + G4ThreeVector(- solMainDet->GetOuterRadius() - solSubDet2->GetXHalfLength(),
                                                                        0.,
                                                                        solMainDet->GetZHalfLength() - solSubDet2->GetZHalfLength()),
                      lvSubDet2, "SubDet2", lvWorld, false, 2);
    auto solSubDet3 = new G4Box("SubDet3", .5 * subDetX0, .5 * subDetY0, .5 * subDetZ1);
    auto lvSubDet3 = new G4LogicalVolume(solSubDet3, matNaI, "SubDet3");
    lvSubDet3->SetVisAttributes(visGrayAlpha5Sol);
    new G4PVPlacement(0,
                      pvMainDet->GetObjectTranslation() + G4ThreeVector(0.,
                                                                        0.,
                                                                        - solMainDet->GetZHalfLength() - solSubDet3->GetZHalfLength()),
                      lvSubDet3, "SubDet3", lvWorld, false, 3);    
    auto solSubDet4 = new G4Box("SubDet4", .5 * (subDetX0 + subDetX1), .5 * subDetY2, .5 * (subDetZ0 + subDetZ1));
    auto lvSubDet4 = new G4LogicalVolume(solSubDet4, matNaI, "SubDet4");
    lvSubDet4->SetVisAttributes(visGrayAlpha5Sol);
    new G4PVPlacement(0,
                      pvMainDet->GetObjectTranslation() + G4ThreeVector(solMainDet->GetOuterRadius() - solSubDet4->GetXHalfLength(),
                                                                        - solMainDet->GetOuterRadius() - solSubDet4->GetYHalfLength(),
                                                                        solMainDet->GetZHalfLength() - solSubDet4->GetZHalfLength()),
                      lvSubDet4, "SubDet4", lvWorld, false, 4);

    return pvWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    auto mfdDetector = new G4MultiFunctionalDetector("Detector");
    G4SDManager::GetSDMpointer()->AddNewDetector(mfdDetector);
    auto psEDep = new G4PSEnergyDeposit("EDep");
    mfdDetector->RegisterPrimitive(psEDep);
    SetSensitiveDetector("MainDet", mfdDetector);
}

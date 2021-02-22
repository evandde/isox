#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

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

    auto visBlueSol = new G4VisAttributes(G4Color::Blue());
    visBlueSol->SetForceSolid();

    auto visCyanSol = new G4VisAttributes(G4Color::Cyan());
    visCyanSol->SetForceSolid();

    auto colorPurpleAlpha5 = G4Color(.3, 0., 1., .5);
    auto visPurpleAlpha5Sol = new G4VisAttributes(colorPurpleAlpha5);
    visPurpleAlpha5Sol->SetForceSolid();

    auto colorYellowAlpha5 = G4Color::Yellow();
    colorYellowAlpha5.SetAlpha(0.5);
    auto visYellowAlpha5Sol = new G4VisAttributes(colorYellowAlpha5);
    visYellowAlpha5Sol->SetForceSolid();

    auto colorCyanAlpha5 = G4Color::Cyan();
    colorCyanAlpha5.SetAlpha(0.5);
    auto visCyanAlpha5Sol = new G4VisAttributes(colorCyanAlpha5);
    visCyanAlpha5Sol->SetForceSolid();

    auto colorMagentaAlpha5 = G4Color::Magenta();
    colorMagentaAlpha5.SetAlpha(0.5);
    auto visMagnetaAlpha5Sol = new G4VisAttributes(colorMagentaAlpha5);
    visMagnetaAlpha5Sol->SetForceSolid();

    // World
    auto worldSize = 1. * m;
    auto solWorld = new G4Box("World", .5 * worldSize, .5 * worldSize, .5 * worldSize);
    auto lvWorld = new G4LogicalVolume(solWorld, matAir, "World");
    lvWorld->SetVisAttributes(visWhiteWire);
    auto pvWorld = new G4PVPlacement(0, G4ThreeVector(), lvWorld, "World", nullptr, false, 0);

    // HPGe main detector
    auto mainDetHeight = 13.5 * cm;
    auto mainDetDiam = 7.62 * cm;
    auto solMainDet = new G4Tubs("MainDet", 0., .5 * mainDetDiam, .5 * mainDetHeight, 0., 360. * deg);
    auto lvMainDet = new G4LogicalVolume(solMainDet, matGe, "MainDet");
    lvMainDet->SetVisAttributes(visBlueSol);
    auto pvMainDet = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), lvMainDet, "MainDet", lvWorld, false, 0);
    // HPGe main detector cooler line
    auto mainDetCoolerLineHeight = 5. * cm;
    auto mainDetCoolerLineDiam = 2.54 * cm;
    auto solMainDetCoolerLine = new G4Tubs("MainDetCoolerLine", 0., .5 * mainDetCoolerLineDiam, .5 * mainDetCoolerLineHeight, 0., 360. * deg);
    auto lvMainDetCoolerLine = new G4LogicalVolume(solMainDetCoolerLine, matAl, "MainDetCoolerLine");
    lvMainDetCoolerLine->SetVisAttributes(visCyanSol);
    new G4PVPlacement(0,
                      pvMainDet->GetObjectTranslation() + G4ThreeVector(0., 0., -solMainDet->GetZHalfLength() - solMainDetCoolerLine->GetZHalfLength()),
                      lvMainDetCoolerLine, "MainDetCoolerLine", lvWorld, false, 999);

    // NaI(Tl) sub detector
    auto subDetX1 = 14. * cm;
    auto subDetX2 = 7.8 * cm;
    auto subDetY1 = 10. * cm;
    auto subDetY2 = 7.8 * cm;
    auto subDetY3 = 10. * cm;
    auto subDetZ0 = 23.5 * cm;
    auto subDetZ1 = 10. * cm;
    auto solSubDet1 = new G4Box("SubDet1", .5 * subDetX2, .5 * subDetY2, .5 * subDetZ1);
    auto lvSubDet1 = new G4LogicalVolume(solSubDet1, matNaI, "SubDet1");
    lvSubDet1->SetVisAttributes(visYellowAlpha5Sol);
    auto pvSubDet1 = new G4PVPlacement(0,
                                       pvMainDet->GetObjectTranslation() + G4ThreeVector(0.,
                                                                                         0.,
                                                                                         solMainDet->GetZHalfLength() + solSubDet1->GetZHalfLength()),
                                       lvSubDet1, "SubDet1", lvWorld, false, 1);
    auto solSubDet2 = new G4Box("SubDet2", .5 * subDetX2, .5 * subDetY1, .5 * subDetZ0);
    auto lvSubDet2 = new G4LogicalVolume(solSubDet2, matNaI, "SubDet2");
    lvSubDet2->SetVisAttributes(visMagnetaAlpha5Sol);
    new G4PVPlacement(0,
                      pvSubDet1->GetObjectTranslation() + G4ThreeVector(0.,
                                                                        -solSubDet1->GetYHalfLength() - solSubDet2->GetYHalfLength(),
                                                                        solSubDet1->GetZHalfLength() - solSubDet2->GetZHalfLength()),
                      lvSubDet2, "SubDet2", lvWorld, false, 2);
    auto solSubDet3 = new G4Box("SubDet3", .5 * subDetX2, .5 * subDetY3, .5 * subDetZ0);
    auto lvSubDet3 = new G4LogicalVolume(solSubDet3, matNaI, "SubDet3");
    lvSubDet3->SetVisAttributes(visCyanAlpha5Sol);
    new G4PVPlacement(0,
                      pvSubDet1->GetObjectTranslation() + G4ThreeVector(0.,
                                                                        solSubDet1->GetYHalfLength() + solSubDet3->GetYHalfLength(),
                                                                        solSubDet1->GetZHalfLength() - solSubDet3->GetZHalfLength()),
                      lvSubDet3, "SubDet3", lvWorld, false, 3);
    auto solSubDet4 = new G4Box("SubDet4", .5 * subDetX1, .5 * (subDetY1 + subDetY2 + subDetY3), .5 * subDetZ0);
    auto lvSubDet4 = new G4LogicalVolume(solSubDet4, matNaI, "SubDet4");
    lvSubDet4->SetVisAttributes(visPurpleAlpha5Sol);
    new G4PVPlacement(0,
                      pvSubDet1->GetObjectTranslation() + G4ThreeVector(-solSubDet1->GetXHalfLength() - solSubDet4->GetXHalfLength(),
                                                                        0.,
                                                                        solSubDet1->GetZHalfLength() - solSubDet4->GetZHalfLength()),
                      lvSubDet4, "SubDet4", lvWorld, false, 4);

    return pvWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    auto sd = new SensitiveDetector("Detector");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    SetSensitiveDetector("MainDet", sd);
}

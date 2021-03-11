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
    auto matVaccum = nist->FindOrBuildMaterial("G4_Galactic");
    auto matAir = nist->FindOrBuildMaterial("G4_AIR");
    auto matAl = nist->FindOrBuildMaterial("G4_Al");
    auto matCu = nist->FindOrBuildMaterial("G4_Cu");
    auto matGe = nist->FindOrBuildMaterial("G4_Ge");
    auto matNaI = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");

    // vis
    auto visWhiteWire = new G4VisAttributes(G4Color::White());
    visWhiteWire->SetForceWireframe();

    auto visBlueSol = new G4VisAttributes(G4Color::Blue());
    visBlueSol->SetForceSolid();

    auto visCyanSol = new G4VisAttributes(G4Color::Cyan());
    visCyanSol->SetForceSolid();

    auto colorGrayAlpha3 = G4Color(.5, .5, .5, .3);
    auto visGrayAlpha3Sol = new G4VisAttributes(colorGrayAlpha3);
    visGrayAlpha3Sol->SetForceSolid();

    auto colorGrayAlpha7 = G4Color(.9, .9, .9, .7);
    auto visGrayAlpha7Sol = new G4VisAttributes(colorGrayAlpha7);
    visGrayAlpha7Sol->SetForceSolid();

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
    // User arbitrarily defined (From MCNP6 input file)
    auto isoxDetChamberDiameter = 7.6 * cm;
    auto isoxDetChamberHeight = 13.35 * cm;
    auto isoxCrystalHolderVaccumHeight = 1.77 * cm;
    auto isoxTeflonInsulatorDiameter = 1.6 * cm;
    auto isoxTeflonInsulatorHeight = 2.3 * cm;
    auto isoxCrystalHolderRearDiameter = 2.8 * cm;
    auto isoxCrystalHolderRearHeight = 2.98 * cm;
    auto isoxCrystalHolderFront2RingDistance = 1.45 * cm;
    auto isoxCrystalHolderRing2RingDistance = 1.16 * cm;
    // Figure Details (datasheet)
    auto isoxDetChamberHPGeDistance = 5. * mm;
    auto isoxCrystalHolderRingHeight = 8.6 * mm;
    auto isoxCrystalHolderRingThk = 2.7 * mm;
    auto isoxCrystalHolderRadialThk = .76 * mm;
    auto isoxCrystalHolderAxialThk = 3.2 * mm;
    // Ge Crystal Details (datasheet)
    auto isoxHPGeDeadlayerOuterThk = .42 * mm; // Side
    auto isoxHPGeDeadlayerInnerThk = 3e-3 * mm;
    auto isoxHPGeDeadlayerWindowThk = .42 * mm; // Front
    auto isoxHPGeDiameter = 62. * mm;
    auto isoxHPGeHeight = 57.5 * mm;
    auto isoxCoreHoleDiameter = 8. * mm;
    auto isoxCoreHoleDepth = 47. * mm;
    // Detector Chamber Material Details (datasheet)
    auto isoxDetChamberCryostatWindowMat = matAl; // Front
    auto isoxDetChamberCryostatWindowThk = 1.5 * mm;
    auto isoxDetChamberEndcapMat = matAl; // Radial
    auto isoxDetChamberEndcapThk = 1.5 * mm;
    auto isoxCrystalHolderMat = matAl;
    // Geometry - Detector chamber
    auto solIsoxDetChamber = new G4Tubs("IsoxDetChamber", 0., .5 * isoxDetChamberDiameter, .5 * isoxDetChamberHeight, 0., 360. * deg);
    auto lvIsoxDetChamber = new G4LogicalVolume(solIsoxDetChamber, matVaccum, "IsoxDetChamber");
    lvIsoxDetChamber->SetVisAttributes(visYellowAlpha5Sol);
    auto pvIsoxDetChamber = new G4PVPlacement(0, G4ThreeVector(), lvIsoxDetChamber, "IsoxDetChamber", lvWorld, false, 0);
    auto solIsoxDetChamberCryostatWindow = new G4Tubs("IsoxDetChamberCryostatWindow", 0., .5 * isoxDetChamberDiameter - isoxDetChamberEndcapThk, .5 * isoxDetChamberCryostatWindowThk, 0., 360. * deg);
    auto lvIsoxDetChamberCryostatWindow = new G4LogicalVolume(solIsoxDetChamberCryostatWindow, isoxDetChamberCryostatWindowMat, "IsoxDetChamberCryostatWindow");
    lvIsoxDetChamberCryostatWindow->SetVisAttributes(visGrayAlpha3Sol);
    new G4PVPlacement(0, G4ThreeVector(0., 0., solIsoxDetChamber->GetZHalfLength() - solIsoxDetChamberCryostatWindow->GetZHalfLength()), lvIsoxDetChamberCryostatWindow, "IsoxDetChamberCryostatWindow", lvIsoxDetChamber, false, 0);
    auto solIsoxDetChamberEndcap = new G4Tubs("IsoxDetChamberEndcap", .5 * isoxDetChamberDiameter - isoxDetChamberEndcapThk, .5 * isoxDetChamberDiameter, .5 * isoxDetChamberHeight, 0., 360. * deg);
    auto lvIsoxDetChamberEndcap = new G4LogicalVolume(solIsoxDetChamberEndcap, isoxDetChamberEndcapMat, "IsoxDetChamberEndcap");
    lvIsoxDetChamberEndcap->SetVisAttributes(visGrayAlpha3Sol);
    new G4PVPlacement(0, G4ThreeVector(), lvIsoxDetChamberEndcap, "IsoxDetChamberEndcap", lvIsoxDetChamber, false, 0);
    // Geometry - CrystalHolder Front part
    auto solIsoxCrystalHolderSide = new G4Tubs("IsoxCrystalHolderSide", 0., .5 * isoxHPGeDiameter + isoxCrystalHolderRadialThk, .5 * (isoxHPGeHeight + isoxCrystalHolderVaccumHeight), 0., 360. * deg);
    auto lvIsoxCrystalHolderSide = new G4LogicalVolume(solIsoxCrystalHolderSide, isoxCrystalHolderMat, "IsoxCrystalHolderSide");
    lvIsoxCrystalHolderSide->SetVisAttributes(visGrayAlpha3Sol);
    auto pvIsoxCrystalHolderSide = new G4PVPlacement(0, G4ThreeVector(0., 0., solIsoxDetChamber->GetZHalfLength() - isoxDetChamberHPGeDistance - solIsoxCrystalHolderSide->GetZHalfLength()), lvIsoxCrystalHolderSide, "IsoxCrystalHolderSide", lvIsoxDetChamber, false, 0);
    auto solIsoxHPGe = new G4Tubs("IsoxHPGe", 0., .5 * isoxHPGeDiameter, .5 * isoxHPGeHeight, 0., 360. * deg);
    auto lvIsoxHPGe = new G4LogicalVolume(solIsoxHPGe, matGe, "IsoxHPGe");
    lvIsoxHPGe->SetVisAttributes(visCyanAlpha5Sol);
    new G4PVPlacement(0, G4ThreeVector(0., 0., solIsoxCrystalHolderSide->GetZHalfLength() - solIsoxHPGe->GetZHalfLength()), lvIsoxHPGe, "IsoxHPGe", lvIsoxCrystalHolderSide, false, 0);
    auto solIsoxHPGeDeadlayerOuter = new G4Tubs("IsoxHPGeDeadlayerOuter", .5 * isoxHPGeDiameter - isoxHPGeDeadlayerOuterThk, .5 * isoxHPGeDiameter, .5 * isoxHPGeHeight, 0., 360. * deg);
    auto lvIsoxHPGeDeadlayerOuter = new G4LogicalVolume(solIsoxHPGeDeadlayerOuter, matGe, "IsoxHPGeDeadlayerOuter");
    lvIsoxHPGeDeadlayerOuter->SetVisAttributes(G4VisAttributes::Invisible);
    new G4PVPlacement(0, G4ThreeVector(), lvIsoxHPGeDeadlayerOuter, "IsoxHPGeDeadlayerOuter", lvIsoxHPGe, false, 0);
    auto solIsoxHPGeDeadlayerInner = new G4Tubs("IsoxHPGeDeadlayerInner", .5 * isoxCoreHoleDiameter, .5 * isoxCoreHoleDiameter + isoxHPGeDeadlayerInnerThk, .5 * isoxCoreHoleDepth, 0., 360. * deg);
    auto lvIsoxHPGeDeadlayerInner = new G4LogicalVolume(solIsoxHPGeDeadlayerInner, matGe, "IsoxHPGeDeadlayerInner");
    lvIsoxHPGeDeadlayerInner->SetVisAttributes(G4VisAttributes::Invisible);
    new G4PVPlacement(0, G4ThreeVector(0., 0., -solIsoxHPGe->GetZHalfLength() + solIsoxHPGeDeadlayerInner->GetZHalfLength()), lvIsoxHPGeDeadlayerInner, "IsoxHPGeDeadlayerInner", lvIsoxHPGe, false, 0);
    auto solIsoxHPGeDeadlayerWindow = new G4Tubs("IsoxHPGeDeadlayerWindow", 0., .5 * isoxHPGeDiameter - isoxHPGeDeadlayerOuterThk, .5 * isoxHPGeDeadlayerWindowThk, 0., 360. * deg);
    auto lvIsoxHPGeDeadlayerWindow = new G4LogicalVolume(solIsoxHPGeDeadlayerWindow, matGe, "IsoxHPGeDeadlayerWindow");
    lvIsoxHPGeDeadlayerWindow->SetVisAttributes(G4VisAttributes::Invisible);
    new G4PVPlacement(0, G4ThreeVector(0., 0., solIsoxHPGe->GetZHalfLength() - solIsoxHPGeDeadlayerWindow->GetZHalfLength()), lvIsoxHPGeDeadlayerWindow, "IsoxHPGeDeadlayerWindow", lvIsoxHPGe, false, 0);
    auto solIsoxCopperContact1 = new G4Tubs("IsoxCopperContact1", 0., .5 * isoxCoreHoleDiameter, .5 * isoxCoreHoleDepth, 0., 360. * deg);
    auto lvIsoxCopperContact1 = new G4LogicalVolume(solIsoxCopperContact1, matCu, "IsoxCopperContact1");
    lvIsoxCopperContact1->SetVisAttributes(visMagnetaAlpha5Sol);
    new G4PVPlacement(0, G4ThreeVector(0., 0., -solIsoxHPGe->GetZHalfLength() + solIsoxCopperContact1->GetZHalfLength()), lvIsoxCopperContact1, "IsoxCopperContact1", lvIsoxHPGe, false, 0);
    auto solIsoxCopperContact2 = new G4Tubs("IsoxCopperContact2", 0., .5 * isoxCoreHoleDiameter, .5 * isoxCrystalHolderVaccumHeight, 0., 360. * deg);
    auto lvIsoxCopperContact2 = new G4LogicalVolume(solIsoxCopperContact2, matCu, "IsoxCopperContact2");
    lvIsoxCopperContact2->SetVisAttributes(visMagnetaAlpha5Sol);
    new G4PVPlacement(0, G4ThreeVector(0., 0., -solIsoxCrystalHolderSide->GetZHalfLength() + solIsoxCopperContact2->GetZHalfLength()), lvIsoxCopperContact2, "IsoxCopperContact2", lvIsoxCrystalHolderSide, false, 0);
    auto solIsoxCrystalHolderVaccum = new G4Tubs("IsoxCrystalHolderVaccum", .5 * isoxCoreHoleDiameter, .5 * isoxHPGeDiameter, .5 * isoxCrystalHolderVaccumHeight, 0., 360. * deg);
    auto lvIsoxCrystalHolderVaccum = new G4LogicalVolume(solIsoxCrystalHolderVaccum, matVaccum, "IsoxCrystalHolderVaccum");
    lvIsoxCrystalHolderVaccum->SetVisAttributes(G4VisAttributes::Invisible);
    new G4PVPlacement(0, G4ThreeVector(0., 0., -solIsoxCrystalHolderSide->GetZHalfLength() + solIsoxCrystalHolderVaccum->GetZHalfLength()), lvIsoxCrystalHolderVaccum, "IsoxCrystalHolderVaccum", lvIsoxCrystalHolderSide, false, 0);
    // Geometry - CrystalHolder Rear part
    auto solIsoxCrystalHolderRear1 = new G4Tubs("IsoxCrystalHolderRear1", .5 * isoxTeflonInsulatorDiameter, .5 * isoxHPGeDiameter + isoxCrystalHolderRadialThk, .5 * isoxCrystalHolderAxialThk, 0., 360. * deg);
    auto lvIsoxCrystalHolderRear1 = new G4LogicalVolume(solIsoxCrystalHolderRear1, isoxCrystalHolderMat, "IsoxCrystalHolderRear1");
    lvIsoxCrystalHolderRear1->SetVisAttributes(visGrayAlpha3Sol);
    auto pvIsoxCrystalHolderRear1 = new G4PVPlacement(0, pvIsoxCrystalHolderSide->GetObjectTranslation() + G4ThreeVector(0., 0., -solIsoxCrystalHolderSide->GetZHalfLength() - solIsoxCrystalHolderRear1->GetZHalfLength()), lvIsoxCrystalHolderRear1, "IsoxCrystalHolderRear1", lvIsoxDetChamber, false, 0);
    auto solIsoxCrystalHolderRear2 = new G4Tubs("IsoxCrystalHolderRear2", .5 * isoxTeflonInsulatorDiameter, .5 * isoxCrystalHolderRearDiameter, .5 * (isoxTeflonInsulatorHeight - isoxCrystalHolderAxialThk), 0., 360. * deg);
    auto lvIsoxCrystalHolderRear2 = new G4LogicalVolume(solIsoxCrystalHolderRear2, isoxCrystalHolderMat, "IsoxCrystalHolderRear2");
    lvIsoxCrystalHolderRear2->SetVisAttributes(visGrayAlpha3Sol);
    auto pvIsoxCrystalHolderRear2 = new G4PVPlacement(0, pvIsoxCrystalHolderRear1->GetObjectTranslation() + G4ThreeVector(0., 0., -solIsoxCrystalHolderRear1->GetZHalfLength() - solIsoxCrystalHolderRear2->GetZHalfLength()), lvIsoxCrystalHolderRear2, "IsoxCrystalHolderRear2", lvIsoxDetChamber, false, 0);
    auto solIsoxCrystalHolderRear3 = new G4Tubs("IsoxCrystalHolderRear3", 0., .5 * isoxCrystalHolderRearDiameter, .5 * (isoxCrystalHolderRearHeight + isoxCrystalHolderAxialThk - isoxTeflonInsulatorHeight), 0., 360. * deg);
    auto lvIsoxCrystalHolderRear3 = new G4LogicalVolume(solIsoxCrystalHolderRear3, isoxCrystalHolderMat, "IsoxCrystalHolderRear3");
    lvIsoxCrystalHolderRear3->SetVisAttributes(visGrayAlpha3Sol);
    new G4PVPlacement(0, pvIsoxCrystalHolderRear2->GetObjectTranslation() + G4ThreeVector(0., 0., -solIsoxCrystalHolderRear2->GetZHalfLength() - solIsoxCrystalHolderRear3->GetZHalfLength()), lvIsoxCrystalHolderRear3, "IsoxCrystalHolderRear3", lvIsoxDetChamber, false, 0);
    auto solIsoxTeflonInsulator = new G4Tubs("IsoxTeflonInsulator", 0., .5 * isoxTeflonInsulatorDiameter, .5 * isoxTeflonInsulatorHeight, 0., 360. * deg);
    auto lvIsoxTeflonInsulator = new G4LogicalVolume(solIsoxTeflonInsulator, matAir, "IsoxTeflonInsulator");
    lvIsoxTeflonInsulator->SetVisAttributes(visGrayAlpha7Sol);
    new G4PVPlacement(0, pvIsoxCrystalHolderSide->GetObjectTranslation() + G4ThreeVector(0., 0., -solIsoxCrystalHolderSide->GetZHalfLength() - solIsoxTeflonInsulator->GetZHalfLength()), lvIsoxTeflonInsulator, "IsoxTeflonInsulator", lvIsoxDetChamber, false, 0);
    auto solIsoxCopperContact3 = new G4Tubs("IsoxCopperContact3", 0., .5 * isoxCoreHoleDiameter, .5 * isoxTeflonInsulatorHeight, 0., 360. * deg);
    auto lvIsoxCopperContact3 = new G4LogicalVolume(solIsoxCopperContact3, matCu, "IsoxCopperContact3");
    lvIsoxCopperContact3->SetVisAttributes(visMagnetaAlpha5Sol);
    new G4PVPlacement(0, G4ThreeVector(), lvIsoxCopperContact3, "IsoxCopperContact3", lvIsoxTeflonInsulator, false, 0);
    // Geometry - CrystalHolder Ring part
    auto solIsoxCrystalHolderRing = new G4Tubs("IsoxCrystalHolderRing", .5 * isoxHPGeDiameter + isoxCrystalHolderRadialThk, .5 * isoxHPGeDiameter + isoxCrystalHolderRadialThk + isoxCrystalHolderRingThk, .5*isoxCrystalHolderRingHeight, 0., 360.*deg);
    auto lvIsoxCrystalHolderRing = new G4LogicalVolume(solIsoxCrystalHolderRing, isoxCrystalHolderMat, "IsoxCrystalHolderRing");
    lvIsoxCrystalHolderRing->SetVisAttributes(visGrayAlpha3Sol);
    auto pvIsoxCrystalHolderRing1 = new G4PVPlacement(0, pvIsoxCrystalHolderSide->GetObjectTranslation() + G4ThreeVector(0., 0., solIsoxCrystalHolderSide->GetZHalfLength() - isoxCrystalHolderFront2RingDistance - solIsoxCrystalHolderRing->GetZHalfLength()), lvIsoxCrystalHolderRing, "IsoxCrystalHolderRing1", lvIsoxDetChamber, false, 0);
    new G4PVPlacement(0, pvIsoxCrystalHolderRing1->GetObjectTranslation() + G4ThreeVector(0., 0., -solIsoxCrystalHolderRing->GetZHalfLength() - isoxCrystalHolderRing2RingDistance), lvIsoxCrystalHolderRing, "IsoxCrystalHolderRing2", lvIsoxDetChamber, false, 0);

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
                                       pvIsoxDetChamber->GetObjectTranslation() + G4ThreeVector(0.,
                                                                                                0.,
                                                                                                solIsoxDetChamber->GetZHalfLength() + solSubDet1->GetZHalfLength()),
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
    SetSensitiveDetector("IsoxHPGe", sd);
}

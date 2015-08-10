// File:	G10XDetectorConstruction.hh
// Date:	01-Jul-2014 GM Dubna


#ifndef G10XDetectorConstruction_h
#define G10XDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"

#include <vector>

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4GenericMessenger;


class G10XDetectorConstruction : public G4VUserDetectorConstruction {

public:

    G10XDetectorConstruction();
    virtual ~G10XDetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();

    void SetArmAngle(G4double val);
    G4double GetArmAngle() { return fArmAngle; }
    
    void ConstructMaterials();
    
private:

    void DefineCommands();

    G4GenericMessenger* fMessenger;
    
    G4LogicalVolume* fHodoscope1Logical;
    G4LogicalVolume* fHodoscope2Logical;
    G4LogicalVolume* fWirePlane1Logical;
    G4LogicalVolume* fWirePlane2Logical;
    G4LogicalVolume* fCellLogical;
    G4LogicalVolume* fMagneticLogical;
    
    std::vector<G4VisAttributes*> fVisAttributes;
    
    G4double fArmAngle;
    G4RotationMatrix* fArmRotation;
    G4VPhysicalVolume* fSecondArmPhys;

	} ;

#endif

// e-o-f

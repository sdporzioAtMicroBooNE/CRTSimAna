#ifndef AttenuationStudy_module
#define AttenuationStudy_module

// c++ includes
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>

// root includes
#include "TInterpreter.h"
#include "TNtuple.h"
#include "TROOT.h"
#include "TFile.h"

// framework includes
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Optional/TFileDirectory.h"
#include "fhiclcpp/ParameterSet.h"

// art includes
#include "canvas/Utilities/InputTag.h"
#include "canvas/Persistency/Common/FindMany.h"

// larsoft object includes
#include "nusimdata/SimulationBase/MCParticle.h"
#include "larcore/Geometry/Geometry.h"
#include "larcore/Geometry/AuxDetGeometry.h"
#include "larcore/CoreUtils/ServiceUtil.h" // lar::providerFrom<>()
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"
#include "lardataobj/Simulation/AuxDetSimChannel.h"


// CRT includes
#include "uboone/CRT/CRTProducts/CRTSimData.hh"

// Analyzer class
class AttenuationStudy : public art::EDAnalyzer
{
public:
  explicit AttenuationStudy(fhicl::ParameterSet const & pset);
  virtual ~AttenuationStudy();
  void analyze(art::Event const & evt);
  void beginJob();
  void endJob();
private:
  // Declare fhiclcpp variables
  bool fVerbose;
  std::string fCrtLabel;
  double fADC_Threshold;

  // Declare services
  geo::GeometryCore const* fGeometry; // Pointer to the Geometry service
  detinfo::DetectorProperties const* fDetectorProperties; // Pointer to the Detector Properties

  // Declare trees
  TTree *tTree;
  TTree *metaTree;

  // Declare analysis variables
  Int_t run, subrun, event;
  std::vector<int> tChannel;
  std::vector<double> tADC, tEnergyDeposit, tReadoutDistance1, tReadoutDistance2;
  std::vector<double> tHitX, tHitY, tHitZ, tHitLocX, tHitLocY, tHitLocZ, tHitStartT, tHitEndT, tReadX, tReadY, tReadZ;

  // Declare analysis functions
  void ClearData();
}; // End class AttenuationStudy

AttenuationStudy::AttenuationStudy(fhicl::ParameterSet const & pset) :
    EDAnalyzer(pset),
    fVerbose(pset.get<bool>("Verbose")),
    fCrtLabel(pset.get<std::string>("CrtLabel")),
    fADC_Threshold(pset.get<double>("ADC_Threshold"))
{} // END constructor AttenuationStudy

AttenuationStudy::~AttenuationStudy()
{} // END destructor AttenuationStudy

void AttenuationStudy::beginJob()
{
  // Declare tree variables
  art::ServiceHandle< art::TFileService > tfs;

  metaTree = tfs->make<TTree>("Metadata","");
  metaTree->Branch("verbose",&fVerbose);
  metaTree->Branch("ADC_Threshold", &fADC_Threshold);
  metaTree->Fill();

  tTree = tfs->make<TTree>("Data","");
  tTree->Branch("run",&run,"run/I");
  tTree->Branch("subrun",&subrun,"subrun/I");
  tTree->Branch("event",&event,"event/I");
  tTree->Branch("Channel",&tChannel);
  tTree->Branch("ADC",&tADC);
  tTree->Branch("EnergyDeposit",&tEnergyDeposit);
  tTree->Branch("ReadoutDistance1",&tReadoutDistance1);
  tTree->Branch("ReadoutDistance2",&tReadoutDistance2);
  tTree->Branch("HitX",&tHitX);
  tTree->Branch("HitY",&tHitY);
  tTree->Branch("HitZ",&tHitZ);
  tTree->Branch("HitLocX",&tHitLocX);
  tTree->Branch("HitLocY",&tHitLocY);
  tTree->Branch("HitLocZ",&tHitLocZ);
  tTree->Branch("HitStartT",&tHitStartT);
  tTree->Branch("HitEndT",&tHitEndT);
  tTree->Branch("ReadoutX",&tReadX);
  tTree->Branch("ReadoutY",&tReadY);
  tTree->Branch("ReadoutZ",&tReadZ);

  fGeometry = lar::providerFrom<geo::Geometry>();
  fDetectorProperties = lar::providerFrom<detinfo::DetectorPropertiesService>();
} // END function beginJob

void AttenuationStudy::endJob()
{
} // END function endJob

void AttenuationStudy::ClearData()
{
  run = -1;
  subrun = -1;
  event = -1;
  tChannel.clear();
  tADC.clear();
  tEnergyDeposit.clear();
  tReadoutDistance1.clear();
  tReadoutDistance2.clear();
  tHitX.clear();
  tHitY.clear();
  tHitZ.clear();
  tHitLocX.clear();
  tHitLocY.clear();
  tHitLocZ.clear();
  tReadX.clear();
  tReadY.clear();
  tReadZ.clear();
  tHitStartT.clear();
  tHitEndT.clear();

} // END function ClearData

void AttenuationStudy::analyze(art::Event const & evt)
{
  // Clear data
  ClearData();

  // Get run information
  run = evt.id().run();
  subrun = evt.id().subRun();
  event = evt.id().event();
  if (fVerbose) {printf("||INFORMATION FOR EVENT %i [RUN %i, SUBRUN %i]||\n", event, run, subrun);}

  // Prepare handle and associations
  art::InputTag crtTag {fCrtLabel};
  const auto& crtHandle = evt.getValidHandle< std::vector<crt::CRTSimData> >(crtTag);
  art::FindMany<sim::AuxDetSimChannel> crt_aux_ass(crtHandle,evt,crtTag);

  // Deal with all geo crap
  art::ServiceHandle<geo::AuxDetGeometry> geoService;
  const geo::AuxDetGeometry* geometry = &*geoService;
  const geo::AuxDetGeometryCore* geoServiceProvider = geometry->GetProviderPtr();

  for(std::vector<int>::size_type i=0; i!=crtHandle->size(); i++)
  {
    // Get crt hit
    crt::CRTSimData crt = crtHandle->at(i);
    // Process only real hits that are above pedestal
    if (crt.ADC()>fADC_Threshold)
    {
      // Prepare useful variables
      double x = -1.;
      double y = -1.;
      double z = -1.;
      double tStart = -1.;
      double tEnd = -1;
      double trueDeposit = -1;
      double svHitPosLocal[3];

      // Get the TrackID for track responsible for crt hit
      uint32_t crtTrackID = crt.TrackID();
      // Get the channel of the AuxiliaryDetector associated with the crt hit
      std::vector<sim::AuxDetSimChannel const*> auxDet_v;
      crt_aux_ass.get(i,auxDet_v);
      sim::AuxDetSimChannel const* auxDet = auxDet_v.at(0);
      const geo::AuxDetGeo& adGeo = geoServiceProvider->AuxDet(auxDet->AuxDetID());
      const geo::AuxDetSensitiveGeo& adsGeo = adGeo.SensitiveVolume(auxDet->AuxDetSensitiveID());
      // Get a vector of IDE that went through that AuxiliaryDetector
      std::vector<sim::AuxDetIDE> ides = auxDet->AuxDetIDEs();
      // Loop through all the ides and find the one which has the same trackID
      // and is thus respondible for the crt hit
      int idePerCrtHit = 0;
      for(auto ide : ides)
      {
        if ((int)crtTrackID == (int)ide.trackID)
        {
          x = (ide.entryX + ide.exitX)/2.;
          y = (ide.entryY + ide.exitY)/2.;
          z = (ide.entryZ + ide.exitZ)/2.;
          tStart = ide.entryT;
          tEnd = ide.exitT;
          trueDeposit = ide.energyDeposited;
          idePerCrtHit++;
        }
      }
      // There should be a 1-to-1 ide-crtHit correlation.
      // If there's more, that means something is wrong in the way the code has been set up
      if (idePerCrtHit > 1)
      {
        printf("Oh no... A crt hit in channel %i has %i ides.\n", crt.Channel(), idePerCrtHit);
      }

      // Determine distance from readout
      double world[3] = {x, y, z};
      adsGeo.WorldToLocal(world, svHitPosLocal);
      double distToReadout = abs(-adsGeo.HalfHeight() - svHitPosLocal[1]);
      double dist2 = adsGeo.DistanceToPoint(svHitPosLocal);

      // Push data to vectors
      tChannel.push_back(crt.Channel());
      tADC.push_back(crt.ADC());
      tEnergyDeposit.push_back(trueDeposit);
      tReadoutDistance1.push_back(distToReadout);
      tReadoutDistance2.push_back(dist2);
      tHitX.push_back(x);
      tHitY.push_back(y);
      tHitZ.push_back(z);
      tHitLocX.push_back(svHitPosLocal[0]);
      tHitLocY.push_back(svHitPosLocal[1]);
      tHitLocZ.push_back(svHitPosLocal[2]);
      tHitStartT.push_back(tStart);
      tHitEndT.push_back(tEnd);
      tReadX.push_back(adsGeo.HalfWidth1());
      tReadY.push_back(adsGeo.HalfHeight());
      tReadZ.push_back(adsGeo.HalfWidth2());

      printf("Channel: %i, ADC: %i\n", crt.Channel(),crt.ADC());
      printf("Coordinates: [%.2f, %.2f, %.2f, %.2f], Energy deposit: %.2e\n", x,y,z,tStart,trueDeposit);
      printf("Distance to readout1: %.2f\n", distToReadout);
      printf("Distance to readout2: %.2f\n\n", dist2);
    }
  }


  // Fill tree and finish event loop
  tTree->Fill();
  if (fVerbose) {printf("---------------------------------------------\n\n");}
  return;
} // END function analyze

// Name that will be used by the .fcl to invoke the module
DEFINE_ART_MODULE(AttenuationStudy)

#endif // END def AttenuationStudy_module

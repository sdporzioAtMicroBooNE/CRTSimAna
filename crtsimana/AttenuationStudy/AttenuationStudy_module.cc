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

  // Declare services
  geo::GeometryCore const* fGeometry; // Pointer to the Geometry service
  detinfo::DetectorProperties const* fDetectorProperties; // Pointer to the Detector Properties

  // Declare trees
  TTree *tTree;
  TTree *metaTree;

  // Declare analysis variables
  Int_t run, subrun, event;

  // Declare analysis functions
  void ClearData();
}; // End class AttenuationStudy

AttenuationStudy::AttenuationStudy(fhicl::ParameterSet const & pset) :
    EDAnalyzer(pset),
    fVerbose(pset.get<bool>("Verbose"))
{} // END constructor AttenuationStudy

AttenuationStudy::~AttenuationStudy()
{} // END destructor AttenuationStudy

void AttenuationStudy::beginJob()
{
  // Declare tree variables
  art::ServiceHandle< art::TFileService > tfs;

  metaTree = tfs->make<TTree>("Metadata","");
  metaTree->Branch("verbose",&fVerbose);
  metaTree->Fill();

  tTree = tfs->make<TTree>("Data","");
  tTree->Branch("run",&run,"run/I");
  tTree->Branch("subrun",&subrun,"subrun/I");
  tTree->Branch("event",&event,"event/I");

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
} // END function ClearData

void AttenuationStudy::analyze(art::Event const & evt)
{
  // Get run information
  run = evt.id().run();
  subrun = evt.id().subRun();
  event = evt.id().event();
  if (fVerbose) {printf("||INFORMATION FOR EVENT %i [RUN %i, SUBRUN %i]||\n", event, run, subrun);}

  // Check if there's MCParts in the event
  art::InputTag mcTag {"largeant"};
  auto truthHandle = evt.getValidHandle<std::vector<simb::MCParticle>>(mcTag);
  // Print out information
  int nParts = 0;
  for (auto truth : *truthHandle)
  {
    // printf("|_PDG %i | Energy %.2f\n", truth.PdgCode(), truth.E());
    nParts++;
  }

  // Check if any AuxDetSimChannel has a deposit in it
  art::InputTag adscTag {"largeant"};
  auto adscHandle = evt.getValidHandle<std::vector<sim::AuxDetSimChannel>>(adscTag);
  // Print out information
  int nEnergyDeposits = 0;
  int nIDE = 0;
  for (auto adsc : *adscHandle)
  {
    for (auto ide : adsc.AuxDetIDEs())
    {
      if (ide.energyDeposited!=0)
      {
      	printf("|_Track %i deposited energy %.10f\n", ide.trackID, ide.energyDeposited);
	      nEnergyDeposits++;
      }
      nIDE++;
     }
  }

  // Check if there's any CrtHit (which is not null)
  art::InputTag crtTag {"crtdetsim"};
  auto crtHandle = evt.getValidHandle<std::vector<crt::CRTSimData>>(crtTag);
  // Print out information
  int nCrtHits = 0;
  for (auto crt : *crtHandle)
  {
    if (crt.ADC()!=0)
    {
      // printf("|_Channel: %i | ADC: %i [%i,%i]\n", crt.Channel(), crt.ADC(), crt.T0(), crt.T1());
      nCrtHits++;
    }
  }

  printf("\nSize of McParts and IDEs: (%lu,%i)\n", (*truthHandle).size(), nIDE);
  printf("--Number of MCParts: %i\n", nParts);
  printf("--Number of IDEs with energy deposits: %i\n", nEnergyDeposits);
  printf("--Number of (non-null) CRT Hits: %i\n", nCrtHits);

  // Fill tree and finish event loop
  tTree->Fill();
  if (fVerbose) {printf("---------------------------------------------\n\n");}
  return;
} // END function analyze

// Name that will be used by the .fcl to invoke the module
DEFINE_ART_MODULE(AttenuationStudy)

#endif // END def AttenuationStudy_module

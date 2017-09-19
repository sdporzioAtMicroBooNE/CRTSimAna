#ifndef AttenuationStudy_module
#define AttenuationStudy_module

// c++ includes
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <exception>

// root includes
#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1.h"
#include "TH2D.h"
#include "TH2I.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TClonesArray.h"
#include "TCanvas.h"
#include "TGraph.h"

// framework includes
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Optional/TFileDirectory.h"
#include "fhiclcpp/ParameterSet.h"

// art includes
#include "canvas/Utilities/InputTag.h"
#include "canvas/Persistency/Common/FindMany.h"

// larsoft object includes
#include "larcorealg/Geometry/geo.h"
#include "larcore/Geometry/Geometry.h"
#include "larcore/CoreUtils/ServiceUtil.h" // lar::providerFrom<>()
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"

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
  std::string fInstanceName;

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
    fInstanceName(pset.get<std::string>("InstanceName")),
{} // END constructor AttenuationStudy

AttenuationStudy::~AttenuationStudy()
{} // END destructor AttenuationStudy

void AttenuationStudy::beginJob()
{
  // Declare tree variables
  art::ServiceHandle< art::TFileService > tfs;

  metaTree = tfs->make<TTree>("Metadata","");
  metaTree->Branch("instanceName",&fInstanceName);
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

} // END function analyze


// Name that will be used by the .fcl to invoke the module
DEFINE_ART_MODULE(AttenuationStudy)

#endif // END def AttenuationStudy_module
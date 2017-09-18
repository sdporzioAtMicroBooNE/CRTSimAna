#ifndef ScanRecoSelectionParameters_module
#define ScanRecoSelectionParameters_module

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
#include "canvas/Persistency/Common/FindManyP.h"
#include "canvas/Persistency/Common/FindOne.h"
#include "canvas/Persistency/Common/FindOneP.h"


// larsoft object includes
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/Shower.h"
#include "lardataobj/RecoBase/Vertex.h"
#include "lardataobj/RecoBase/PFParticle.h"
#include "lardataobj/RecoBase/Wire.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/TrackingTypes.h"
#include "lardataobj/Simulation/SimChannel.h"
#include "lardataobj/RawData/RawDigit.h"
#include "larcorealg/Geometry/geo.h"
#include "larcore/Geometry/Geometry.h"
#include "larcore/CoreUtils/ServiceUtil.h" // lar::providerFrom<>()
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"

// Auxiliary objects includes
#include "DataObjects/DecayVertex.h"


// Analyzer class
class ScanRecoSelectionParameters : public art::EDAnalyzer
{
public:
  explicit ScanRecoSelectionParameters(fhicl::ParameterSet const & pset);
  virtual ~ScanRecoSelectionParameters();
  void analyze(art::Event const & evt);
  void beginJob();
  void endJob();
private:
  // Declare fhiclcpp variables
  std::string fInstanceName;
  int fIteration;
  std::vector<double> fMinTpcBound;
  std::vector<double> fMaxTpcBound;
  std::string fDataType;
  std::string fPfpLabel;
  std::string fHitLabel;
  int fDecayChannel;
  double fSterileMass;
  double fDistanceCut;
  double fCaloCut;
  double fChannelNorm;
  double fTickNorm;
  bool fPrimaryOnly;
  bool fEndVerticesAlso;
  std::string fAnaType;
  bool fVerbose;
  bool fSaveDrawTree;

  // Declare services
  geo::GeometryCore const* fGeometry; // Pointer to the Geometry service
  detinfo::DetectorProperties const* fDetectorProperties; // Pointer to the Detector Properties

  // Declare trees
  TTree *tTree;
  TTree *metaTree;
  TTree *drawTree;

  // Declare analysis variables
  Int_t run, subrun, event, nTracks, nShowers, nPairs, nTrackVertices, nShowerVertices, nPotVertices, nCleanVertices, pandora_nPrimaryVertices, pandora_nCleanVertices, nCleanVerticesOutsideTPC, nTotHits;
  std::vector<float> pairDistance, potPairDistance, totChargeInRadius, par1ChargeInRadius, par2ChargeInRadius, caloRatio;
  std::vector<int> pandora_primaryVertexPDG, pandora_nDaughters, pandora_nTracks, pandora_nShowers, pandora_nNearTracks, pandora_nNearShowers, nTrackHits, nShowerHits, nTotHitsInRadius, nPar1HitsInRadius, nPar2HitsInRadius;

  // Declare drawTree variables
  std::vector<std::vector<int>> dv_wireCoordinates,
    par1_wireCoordinates, par2_wireCoordinates,
    par1_hits_p0_wireCoordinates, par1_hits_p1_wireCoordinates, par1_hits_p2_wireCoordinates,
    par2_hits_p0_wireCoordinates, par2_hits_p1_wireCoordinates, par2_hits_p2_wireCoordinates,
    tot_hits_p0_wireCoordinates, tot_hits_p1_wireCoordinates, tot_hits_p2_wireCoordinates;

  std::vector<std::vector<double>> dv_xyzCoordinates, dv_tickCoordinates,
    par1_xyzCoordinates, par1_tickCoordinates,
    par2_xyzCoordinates, par2_tickCoordinates,
    par1_hits_p0_tickCoordinates, par1_hits_p1_tickCoordinates, par1_hits_p2_tickCoordinates,
    par2_hits_p0_tickCoordinates, par2_hits_p1_tickCoordinates, par2_hits_p2_tickCoordinates,
    tot_hits_p0_tickCoordinates, tot_hits_p1_tickCoordinates, tot_hits_p2_tickCoordinates;

  // Declare geometry functions
  void SetDetectorCoordinates(AuxVertex::DecayVertex& vert);

  // Declare analysis functions
  void ClearData();
  void GetTrackShowerVectors(art::Event const & evt,
          std::vector<recob::PFParticle const*>& pandora_primaryPFP,
          std::vector<recob::PFParticle const*>& tracks,
          std::vector<recob::PFParticle const*>& showers);
  void GetOriginVertices(art::Event const & evt,
          const std::vector<recob::PFParticle const*>& tracks,
          const std::vector<recob::PFParticle const*>& showers,
          std::vector<AuxVertex::DecayVertex>& trackVertices,
          std::vector<AuxVertex::DecayVertex>& showerVertices);
  void GetDecayVertices(const std::vector<AuxVertex::DecayVertex>& trackVertices,
          const std::vector<AuxVertex::DecayVertex>& showerVertices,
          std::vector<AuxVertex::DecayVertex>& potVertices,
          std::vector<AuxVertex::DecayVertex>& cleanVertices);
  void GetHitVectors(art::Event const & evt,
          const std::vector<recob::PFParticle const*>& tracks,
          const std::vector<recob::PFParticle const*>& showers,
          std::vector<recob::Hit const*>& totHits,
          std::vector<std::vector<recob::Hit const*>>& trackHits,
          std::vector<std::vector<recob::Hit const*>>& showerHits);
  void PerformCalorimetry(const std::vector<AuxVertex::DecayVertex>& cleanVertices,
          const std::vector<recob::Hit const*>& totHits,
          const std::vector<std::vector<recob::Hit const*>>& trackHits,
          const std::vector<std::vector<recob::Hit const*>>& showerHits,
          std::vector<std::vector<recob::Hit const*>>& totHitsInRadius);
  void FillDrawTree(
          const std::vector<AuxVertex::DecayVertex>& cleanVertices,
          const std::vector<std::vector<recob::Hit const*>>& totHitsInRadius,
          const std::vector<std::vector<recob::Hit const*>>& trackHits,
          const std::vector<std::vector<recob::Hit const*>>& showerHits);
}; // End class ScanRecoSelectionParameters

ScanRecoSelectionParameters::ScanRecoSelectionParameters(fhicl::ParameterSet const & pset) :
    EDAnalyzer(pset),
    fInstanceName(pset.get<std::string>("InstanceName")),
    fIteration(pset.get<int>("Iteration")),
    fMinTpcBound(pset.get<std::vector<double>>("MinTpcBound")),
    fMaxTpcBound(pset.get<std::vector<double>>("MaxTpcBound")),
    fDataType(pset.get<std::string>("DataType")),
    fPfpLabel(pset.get<std::string>("PfpLabel")),
    fHitLabel(pset.get<std::string>("HitLabel")),
    fDecayChannel(pset.get<int>("DecayChannel")),
    fSterileMass(pset.get<double>("SterileMass")),
    fDistanceCut(pset.get<double>("DistanceCut")),
    fCaloCut(pset.get<double>("CaloCut")),
    fChannelNorm(pset.get<double>("ChannelNorm")),
    fTickNorm(pset.get<double>("TickNorm")),
    fPrimaryOnly(pset.get<bool>("PrimaryOnly")),
    fEndVerticesAlso(pset.get<bool>("EndVerticesAlso")),
    fAnaType(pset.get<std::string>("AnalysisType")),
    fVerbose(pset.get<bool>("VerboseMode")),
    fSaveDrawTree(pset.get<bool>("SaveDrawTree"))
{} // END constructor ScanRecoSelectionParameters

ScanRecoSelectionParameters::~ScanRecoSelectionParameters()
{} // END destructor ScanRecoSelectionParameters

void ScanRecoSelectionParameters::beginJob()
{
  // Declare tree variables
  art::ServiceHandle< art::TFileService > tfs;

  metaTree = tfs->make<TTree>("Metadata","");
  metaTree->Branch("instanceName",&fInstanceName);
  metaTree->Branch("iteration",&fIteration,"iteration/I"); 
  metaTree->Branch("minTpcBound",&fMinTpcBound);
  metaTree->Branch("maxTpcBound",&fMaxTpcBound);  
  metaTree->Branch("dataType",&fDataType);
  metaTree->Branch("pfpLabel",&fPfpLabel);
  metaTree->Branch("hitLabel",&fHitLabel);
  metaTree->Branch("decayChannel",&fDecayChannel,"decayChannel/I");
  metaTree->Branch("distanceCut",&fDistanceCut,"distanceCut/D");
  metaTree->Branch("caloCut",&fCaloCut,"caloCut/D");
  metaTree->Branch("channelNorm",&fChannelNorm,"channelNorm/D");
  metaTree->Branch("tickNorm",&fTickNorm,"tickNorm/D");
  metaTree->Branch("sterileMass",&fSterileMass,"sterileMass/D");
  metaTree->Branch("primaryOnly",&fPrimaryOnly,"primaryOnly/O");
  metaTree->Branch("endVerticesAlso",&fEndVerticesAlso,"endVerticesAlso/O");
  metaTree->Branch("anaType",&fAnaType);
  metaTree->Branch("saveDrawTree",&fSaveDrawTree,"saveDrawTree/O");

  metaTree->Fill();

  tTree = tfs->make<TTree>("Data","");
  tTree->Branch("run",&run,"run/I");
  tTree->Branch("subrun",&subrun,"subrun/I");
  tTree->Branch("event",&event,"event/I");
  tTree->Branch("nTracks",&nTracks,"nTracks/I");
  tTree->Branch("nShowers",&nShowers,"nShowers/I");
  tTree->Branch("pairDistance",&pairDistance);
  tTree->Branch("nPairs",&nPairs,"nPairs/I");
  tTree->Branch("nTrackVertices",&nTrackVertices,"nTrackVertices/I");
  tTree->Branch("nShowerVertices",&nShowerVertices,"nShowerVertices/I");
  tTree->Branch("potPairDistance",&potPairDistance);
  tTree->Branch("nPotVertices",&nPotVertices,"nPotVertices/I");
  tTree->Branch("nCleanVertices",&nCleanVertices,"nCleanVertices/I");

  tTree->Branch("nTotHits",&nTotHits,"nTotHits/I");
  tTree->Branch("nTrackHits",&nTrackHits);
  tTree->Branch("nShowerHits",&nShowerHits);
  tTree->Branch("nTotHitsInRadius",&nTotHitsInRadius);
  tTree->Branch("nPar1HitsInRadius",&nPar1HitsInRadius);
  tTree->Branch("nPar2HitsInRadius",&nPar2HitsInRadius);
  tTree->Branch("totChargeInRadius",&totChargeInRadius);
  tTree->Branch("par1ChargeInRadius",&par1ChargeInRadius);
  tTree->Branch("par2ChargeInRadius",&par2ChargeInRadius);
  tTree->Branch("caloRatio",&caloRatio);

  tTree->Branch("pandora_nPrimaryVertices",&pandora_nPrimaryVertices,"pandora_nPrimaryVertices/I");
  tTree->Branch("pandora_primaryVertexPDG",&pandora_primaryVertexPDG);
  tTree->Branch("pandora_nDaughters",&pandora_nDaughters);
  tTree->Branch("pandora_nTracks",&pandora_nTracks);
  tTree->Branch("pandora_nShowers",&pandora_nShowers);
  tTree->Branch("pandora_nNearTracks",&pandora_nNearTracks);
  tTree->Branch("pandora_nNearShowers",&pandora_nNearShowers);
  tTree->Branch("pandora_nCleanVertices",&pandora_nCleanVertices,"pandora_nCleanVertices/I");

  if (fSaveDrawTree)
  {
    drawTree = tfs->make<TTree>("DrawTree","");
    drawTree->Branch("dv_xyzCoordinates",&dv_xyzCoordinates);
    drawTree->Branch("dv_wireCoordinates",&dv_wireCoordinates);
    drawTree->Branch("dv_tickCoordinates",&dv_tickCoordinates);

    drawTree->Branch("par1_xyzCoordinates",&par1_xyzCoordinates);
    drawTree->Branch("par1_wireCoordinates",&par1_wireCoordinates);
    drawTree->Branch("par1_tickCoordinates",&par1_tickCoordinates);

    drawTree->Branch("par2_xyzCoordinates",&par2_xyzCoordinates);
    drawTree->Branch("par2_wireCoordinates",&par2_wireCoordinates);
    drawTree->Branch("par2_tickCoordinates",&par2_tickCoordinates);

    drawTree->Branch("par1_hits_p0_wireCoordinates",&par1_hits_p0_wireCoordinates);
    drawTree->Branch("par1_hits_p1_wireCoordinates",&par1_hits_p1_wireCoordinates);
    drawTree->Branch("par1_hits_p2_wireCoordinates",&par1_hits_p2_wireCoordinates);
    drawTree->Branch("par1_hits_p0_tickCoordinates",&par1_hits_p0_tickCoordinates);
    drawTree->Branch("par1_hits_p1_tickCoordinates",&par1_hits_p1_tickCoordinates);
    drawTree->Branch("par1_hits_p2_tickCoordinates",&par1_hits_p2_tickCoordinates);

    drawTree->Branch("par2_hits_p0_wireCoordinates",&par2_hits_p0_wireCoordinates);
    drawTree->Branch("par2_hits_p1_wireCoordinates",&par2_hits_p1_wireCoordinates);
    drawTree->Branch("par2_hits_p2_wireCoordinates",&par2_hits_p2_wireCoordinates);
    drawTree->Branch("par2_hits_p0_tickCoordinates",&par2_hits_p0_tickCoordinates);
    drawTree->Branch("par2_hits_p1_tickCoordinates",&par2_hits_p1_tickCoordinates);
    drawTree->Branch("par2_hits_p2_tickCoordinates",&par2_hits_p2_tickCoordinates);

    drawTree->Branch("tot_hits_p0_wireCoordinates",&tot_hits_p0_wireCoordinates);
    drawTree->Branch("tot_hits_p1_wireCoordinates",&tot_hits_p1_wireCoordinates);
    drawTree->Branch("tot_hits_p2_wireCoordinates",&tot_hits_p2_wireCoordinates);
    drawTree->Branch("tot_hits_p0_tickCoordinates",&tot_hits_p0_tickCoordinates);
    drawTree->Branch("tot_hits_p1_tickCoordinates",&tot_hits_p1_tickCoordinates);
    drawTree->Branch("tot_hits_p2_tickCoordinates",&tot_hits_p2_tickCoordinates);
  }

  fGeometry = lar::providerFrom<geo::Geometry>();
  fDetectorProperties = lar::providerFrom<detinfo::DetectorPropertiesService>();
} // END function beginJob

void ScanRecoSelectionParameters::endJob()
{
} // END function endJob

void ScanRecoSelectionParameters::ClearData()
{
  run = -1;
  subrun = -1;
  event = -1;
  nTracks = 0;
  nShowers= 0;
  nPairs = 0;
  nTrackVertices = 0;
  nShowerVertices = 0;
  nPotVertices = 0;
  nCleanVertices = 0;
  pandora_nPrimaryVertices = 0;
  pandora_nCleanVertices = 0;
  nCleanVerticesOutsideTPC = 0;
  pairDistance.clear();
  potPairDistance.clear();
  pandora_primaryVertexPDG.clear();
  pandora_nDaughters.clear();
  pandora_nTracks.clear();
  pandora_nShowers.clear();
  pandora_nNearTracks.clear();
  pandora_nNearShowers.clear();
  nTrackHits.clear();
  nShowerHits.clear();
  nTotHitsInRadius.clear();
  nPar1HitsInRadius.clear();
  nPar2HitsInRadius.clear();
  totChargeInRadius.clear();
  par1ChargeInRadius.clear();
  par2ChargeInRadius.clear();
  caloRatio.clear();

  // Drawing vectors
  dv_xyzCoordinates.clear();
  dv_wireCoordinates.clear();
  dv_tickCoordinates.clear();
  par1_xyzCoordinates.clear();
  par1_wireCoordinates.clear();
  par1_tickCoordinates.clear();
  par2_xyzCoordinates.clear();
  par2_wireCoordinates.clear();
  par2_tickCoordinates.clear();
  par1_hits_p0_wireCoordinates.clear();
  par1_hits_p1_wireCoordinates.clear();
  par1_hits_p2_wireCoordinates.clear();
  par1_hits_p0_tickCoordinates.clear();
  par1_hits_p1_tickCoordinates.clear();
  par1_hits_p2_tickCoordinates.clear();
  par2_hits_p0_wireCoordinates.clear();
  par2_hits_p1_wireCoordinates.clear();
  par2_hits_p2_wireCoordinates.clear();
  par2_hits_p0_tickCoordinates.clear();
  par2_hits_p1_tickCoordinates.clear();
  par2_hits_p2_tickCoordinates.clear();
  tot_hits_p0_wireCoordinates.clear();
  tot_hits_p1_wireCoordinates.clear();
  tot_hits_p2_wireCoordinates.clear();
  tot_hits_p0_tickCoordinates.clear();
  tot_hits_p1_tickCoordinates.clear();
  tot_hits_p2_tickCoordinates.clear();
} // END function ClearData

void ScanRecoSelectionParameters::SetDetectorCoordinates(AuxVertex::DecayVertex& vert)
{
  // Get spatial coordinates and mark vertex as assigned
  double xyz[3] = {vert.GetX(),vert.GetY(),vert.GetZ()};
  double par1_xyz[3] = {vert.GetParX(0),vert.GetParY(0),vert.GetParZ(0)};
  double par2_xyz[3] = {vert.GetParX(1),vert.GetParY(1),vert.GetParZ(1)};

  vert.SetIsDetLocAssigned(true);

  // Check whether coordinates are inside TPC 
  bool isInsideX = (xyz[0]>fMinTpcBound[0]+fDistanceCut &&
    xyz[0]<fMaxTpcBound[0]-fDistanceCut);
  bool isInsideY = (xyz[1]>fMinTpcBound[1]+fDistanceCut &&
    xyz[1]<fMaxTpcBound[1]-fDistanceCut);
  bool isInsideZ = (xyz[2]>fMinTpcBound[2]+fDistanceCut &&
    xyz[2]<fMaxTpcBound[2]-fDistanceCut);

  // If vertex is inside TPC, determine channel/tick coordinates and assign them
  if (isInsideX && isInsideY && isInsideZ)
  {
    vert.SetIsInsideTPC(true);
    raw::ChannelID_t channel0 = fGeometry->NearestChannel(xyz,0);
    raw::ChannelID_t channel1 = fGeometry->NearestChannel(xyz,1);
    raw::ChannelID_t channel2 = fGeometry->NearestChannel(xyz,2);
    double tick0 = fDetectorProperties->ConvertXToTicks(xyz[0], 0, 0, 0);
    double tick1 = fDetectorProperties->ConvertXToTicks(xyz[0], 1, 0, 0);
    double tick2 = fDetectorProperties->ConvertXToTicks(xyz[0], 2, 0, 0);
    vert.SetChannelLoc(channel0, channel1, channel2);
    vert.SetTickLoc(tick0, tick1, tick2);

    raw::ChannelID_t par1_channel0 = fGeometry->NearestChannel(par1_xyz,0);
    raw::ChannelID_t par1_channel1 = fGeometry->NearestChannel(par1_xyz,1);
    raw::ChannelID_t par1_channel2 = fGeometry->NearestChannel(par1_xyz,2);
    double par1_tick0 = fDetectorProperties->ConvertXToTicks(par1_xyz[0], 0, 0, 0);
    double par1_tick1 = fDetectorProperties->ConvertXToTicks(par1_xyz[0], 1, 0, 0);
    double par1_tick2 = fDetectorProperties->ConvertXToTicks(par1_xyz[0], 2, 0, 0);
    vert.SetParChannelLoc(0, par1_channel0, par1_channel1, par1_channel2);
    vert.SetParTickLoc(0, par1_tick0, par1_tick1, par1_tick2);

    raw::ChannelID_t par2_channel0 = fGeometry->NearestChannel(par2_xyz,0);
    raw::ChannelID_t par2_channel1 = fGeometry->NearestChannel(par2_xyz,1);
    raw::ChannelID_t par2_channel2 = fGeometry->NearestChannel(par2_xyz,2);
    double par2_tick0 = fDetectorProperties->ConvertXToTicks(par2_xyz[0], 0, 0, 0);
    double par2_tick1 = fDetectorProperties->ConvertXToTicks(par2_xyz[0], 1, 0, 0);
    double par2_tick2 = fDetectorProperties->ConvertXToTicks(par2_xyz[0], 2, 0, 0);
    vert.SetParChannelLoc(1, par2_channel0, par2_channel1, par2_channel2);
    vert.SetParTickLoc(1, par2_tick0, par2_tick1, par2_tick2);
    return;
  }

  // Else flag it as outside the TPC and exit function
  else {vert.SetIsInsideTPC(false); return;}
} // END function SetDetectorCoordinates

void ScanRecoSelectionParameters::GetTrackShowerVectors(art::Event const & evt, std::vector<recob::PFParticle const*>& pandora_primaryPFP, std::vector<recob::PFParticle const*>&tracks, std::vector<recob::PFParticle const*>& showers)
{
  // Take an event (evt), then fill the (empty!) track, shower and primaries pfp vectors (tracks,showers,pandora_primaryPFP) with all the PFParticles that are tracks, showers and primaries pfps respectively in the event. These vectors will then be used to determine all the possible vertices.

  // Prepare handle labels
  art::InputTag pfpTag {fPfpLabel};

  // Find associations from PFP to tracks and showers
  const auto& pfpHandle = evt.getValidHandle< std::vector<recob::PFParticle> >(pfpTag);
  for (auto const& pfp : (*pfpHandle)){
    if (pfp.IsPrimary()) {pandora_primaryPFP.push_back(&pfp);}
    else
    {
      // Find current pfp parent and check if parent is primary (so that pfp are only secondaries, and not tertiaries, etc.), then fill the correspondending vector with the type of particle found
      if (fPrimaryOnly){
        auto const& parent = (*pfpHandle).at(pfp.Parent());
        if (parent.IsPrimary()){
          if (pfp.PdgCode()==13) {tracks.push_back(&pfp);}
          if (pfp.PdgCode()==11) {showers.push_back(&pfp);}
        }
      }
      // Fill the track and shower vectors with the products associated with the pfp (in any case, regardless of their "genealogy")
      else {
        if (pfp.PdgCode()==13) {tracks.push_back(&pfp);}
        if (pfp.PdgCode()==11) {showers.push_back(&pfp);}
      }
    }
  } // End of pfp loop

  // Calculate tree quantities
  nTracks = tracks.size();
  nShowers = showers.size();

  // Diagnostic message
  if (fVerbose) {printf("Loading %lu secondary tracks and %lu secondary showers.\n", tracks.size(), showers.size());}
  return;
} // END function GetTrackShowerVectors

void ScanRecoSelectionParameters::GetOriginVertices(art::Event const & evt, const std::vector<recob::PFParticle const*>& tracks, const std::vector<recob::PFParticle const*>& showers, std::vector<AuxVertex::DecayVertex>& trackVertices, std::vector<AuxVertex::DecayVertex>& showerVertices)
{
  // Given track and shower vectors (tracks,showers), fill vertices vectors (trackVertices,showerVertices) taking all the start and end points for tracks and start points for showers. The last two numbers (j,j) indicate only their idx in the xxxVertices vector (which is reduntant at this stage). However, later on, it will be used to identify the vertices uniquely, and the idxs from two vertices will be used to define the parent idx of the mean vertex created between them.

  // Initialize associations
  art::InputTag pfpTag {fPfpLabel};
  art::FindMany<recob::Track> pta(tracks,evt,pfpTag);
  art::FindMany<recob::Shower> psa(showers,evt,pfpTag);

  // Perform for tracks
  for(std::vector<int>::size_type i=0; i!=tracks.size(); i++)
  {
    // Annoying stupid way of getting associated object
    std::vector<recob::Track const*> vTrack;
    pta.get(i,vTrack);
    if (vTrack.size()==1)
    {
      recob::Track const* track = vTrack[0];
      // Create vertex
      AuxVertex::DecayVertex tempV1(track->Vertex().X(),track->Vertex().Y(),track->Vertex().Z(),i,i,"t","t","front","front");
      SetDetectorCoordinates(tempV1);
      trackVertices.push_back(tempV1);
      if (fEndVerticesAlso)
      {
        AuxVertex::DecayVertex tempV2(track->End().X(),track->End().Y(),track->End().Z(),i,i,"t","t","end","end");
        SetDetectorCoordinates(tempV2);
        trackVertices.push_back(tempV2);
      }
    }
    else {printf("WHAT THE HELL! Why does the association doesn't lead to an object?\nIt looks like one track/object might be missing, don't trust this event.\n");}
  }


  // And for showers
  for(std::vector<int>::size_type i=0; i!=showers.size(); i++)
  {
    // Annoying stupid way of getting associated object
    std::vector<recob::Shower const*> vShower;
    psa.get(i,vShower);
    if (vShower.size()==1)
    {
      recob::Shower const* shower = vShower[0];
      // Create vertex
      AuxVertex::DecayVertex tempV(shower->ShowerStart().X(),shower->ShowerStart().Y(),shower->ShowerStart().Z(),i,i,"s","s","front","front");
      SetDetectorCoordinates(tempV);
      showerVertices.push_back(tempV);
    }
    else {printf("WHAT THE HELL! Why does the association doesn't lead to an object?\nIt looks like one track/object might be missing, don't trust this event.\n");}
  }

  // Calculate tree quantities
  nTrackVertices = trackVertices.size();
  nShowerVertices = showerVertices.size();

  // Diagnostic message
  if (fVerbose) {printf("Processing %lu origin vertices (%lu from tracks, %lu from showers).\n", trackVertices.size()+showerVertices.size(), trackVertices.size(), showerVertices.size());}
  return;
} // END function GetOriginVertices

void ScanRecoSelectionParameters::GetDecayVertices(const std::vector<AuxVertex::DecayVertex>& trackVertices, const std::vector<AuxVertex::DecayVertex>& showerVertices, std::vector<AuxVertex::DecayVertex>& potVertices, std::vector<AuxVertex::DecayVertex>& cleanVertices)
{
  // Given the reco vertices establishing the origin of tracks and showers (trackVertices,showerVertices), take all the possible mean vertices by paring them two by two (potVertices), then impose that in radius (fDistanceCut) only two original vertices are present (cleanVertices). Now the previously used (j,j) index are used to define the parent vertices (which track or shower originating vertex was used to determine the mean vertex between them).
  // For track-track
  if (fAnaType == "tt")
  {
    for(std::vector<int>::size_type i=0; i!=trackVertices.size(); i++)
    {
      for(std::vector<int>::size_type j=i+1; j!=trackVertices.size(); j++)
      {
        AuxVertex::DecayVertex v1 = trackVertices[i];
        AuxVertex::DecayVertex v2 = trackVertices[j];
        float distance = Distance(v1,v2);
        pairDistance.push_back(distance);
        bool isInRadius = (distance<fDistanceCut);
        if (isInRadius)
        {
          AuxVertex::DecayVertex v3 = MeanVertex(v1, v2);
          SetDetectorCoordinates(v3);
          potVertices.push_back(v3);
          potPairDistance.push_back(distance);
        }
      }
    }
    // Make sure the potential mean vertices are clean (two decay vertices only in radius)
    // (start with assumption that isGoodVertex == true, then if you find extra particles in radius, turn it in bad vertex. Only good vertices are saved)
    for(std::vector<int>::size_type i=0; i!=potVertices.size(); i++)
    {
      AuxVertex::DecayVertex mv = potVertices[i];
      bool isGoodVertex = true;
      for(std::vector<int>::size_type j=0; j!=trackVertices.size(); j++)
      {
        AuxVertex::DecayVertex v1 = trackVertices[j];
        bool isParent1 = (mv.GetParIdx1() == v1.GetParIdx1());
        bool isParent2 = (mv.GetParIdx2() == v1.GetParIdx2());
        bool notParent = !(isParent1 || isParent2);
        bool isInRadius = (Distance(mv,v1)<fDistanceCut);
        if (isInRadius && notParent) isGoodVertex = false;
      }
      if (isGoodVertex) cleanVertices.push_back(mv);
    }
  }

  // And for track-shower
  else if (fAnaType == "ts"){
    for(std::vector<int>::size_type i=0; i!=trackVertices.size(); i++)
    {
      for(std::vector<int>::size_type j=0; j!=showerVertices.size(); j++)
      {
        AuxVertex::DecayVertex v1 = trackVertices[i];
        AuxVertex::DecayVertex v2 = showerVertices[j];
        float distance = Distance(v1,v2);
        pairDistance.push_back(distance);
        bool isInRadius = (distance<fDistanceCut);
        if (isInRadius)
        {
          AuxVertex::DecayVertex v3 = MeanVertex(v1, v2);
          SetDetectorCoordinates(v3);
          potVertices.push_back(v3);
          potPairDistance.push_back(distance);
        }
      }
    }
    // Make sure the potential mean vertices are clean (two decay vertices only in radius)
    // (start with assumption that isGoodVertex == true, then if you find extra particles in radius, turn it in bad vertex. Only good vertices are saved)
    for(std::vector<int>::size_type i=0; i!=potVertices.size(); i++)
    {
      AuxVertex::DecayVertex mv = potVertices[i];
      bool isGoodVertex = true;
      for(std::vector<int>::size_type j=0; j!=trackVertices.size(); j++)
      {
        AuxVertex::DecayVertex v1 = trackVertices[j];
        bool notParent = (mv.GetParIdx1() != v1.GetParIdx1());
        bool isInRadius = (Distance(mv,v1)<fDistanceCut);
        if (isInRadius && notParent) isGoodVertex = false;
      }
      for(std::vector<int>::size_type j=0; j!=showerVertices.size(); j++)
      {
        AuxVertex::DecayVertex v2 = showerVertices[j];
        bool notParent = (mv.GetParIdx2() != v2.GetParIdx2());
        bool isInRadius = (Distance(mv,v2)<fDistanceCut);
        if (isInRadius && notParent) isGoodVertex = false;
      }
      if (isGoodVertex) {cleanVertices.push_back(mv);}
    }
  }
  // Throw error if fAnaType wasn't right.
  else {
    throw std::invalid_argument("Invalid fAnaType. Must be 'tt' or 'ts'!");
  }

  // Calculate tree quantities
  nPairs = pairDistance.size();
  nPotVertices = potVertices.size();
  nCleanVertices = cleanVertices.size();

  // Diagnostic message
  if (fVerbose) {printf("Selecting %lu potential vertices, %lu of which are clean.\n", potVertices.size(), cleanVertices.size());}
  return;
} // END function GetDecayVertices

void ScanRecoSelectionParameters::GetHitVectors(art::Event const & evt, const std::vector<recob::PFParticle const*>& tracks, const std::vector<recob::PFParticle const*>& showers, std::vector<recob::Hit const*>& totHits, std::vector<std::vector<recob::Hit const*>>& trackHits, std::vector<std::vector<recob::Hit const*>>& showerHits)
{
  // Fill three vectors, one containing all the hits in the event (totHits), one containing vectors of all the hits pertaining to tracks (trackHits), one for each track, and finally one containing vectors of all the hits form showers (showerHits), one for each shower. These hits will be used to perform calorimetry analysis.

  // Prepare pfpTag
  art::InputTag pfpTag {fPfpLabel};
  art::FindMany<recob::Track> pta(tracks,evt,pfpTag);
  art::FindMany<recob::Shower> psa(showers,evt,pfpTag);
  std::vector<recob::Track const*> realTracks;
  std::vector<recob::Shower const*> realShowers;
  int totNTrackHits, totNShowerHits;

  // Get the actual recob::Track and recob::Shower objects from the tracks and shower arrays (which instead contain only recob::PFParticles).
  // Hits are associated with tracks/showers objects, not with PFParticles, which is why we must follow this annoying chain of associations (recob::PFParticle->recob::Track/Shower->recob::Hit)

  // Find recob::Tracks
  for(std::vector<int>::size_type i=0; i!=tracks.size(); i++)
  {
    // Annoying stupid way of getting associated object
    std::vector<recob::Track const*> vTrack;
    pta.get(i,vTrack);
    if (vTrack.size()==1)
    {
      recob::Track const* track = vTrack[0];
      realTracks.push_back(track);
    }
  }
  // Find recob::Showers
  for(std::vector<int>::size_type i=0; i!=showers.size(); i++)
  {
    // Annoying stupid way of getting associated object
    std::vector<recob::Shower const*> vShower;
    psa.get(i,vShower);
    if (vShower.size()==1){
      recob::Shower const* shower = vShower[0];
      realShowers.push_back(shower);
    }
  }

  // Find all hits
  art::Handle<std::vector<recob::Hit>> hitHandle;
  evt.getByLabel(fHitLabel, hitHandle);
  std::vector<recob::Hit> const& tempTotHits(*hitHandle);
  for (auto const& tempTotHit: tempTotHits) {totHits.push_back(&tempTotHit);}
  
  // Find hits associated with recob::Tracks
  art::FindMany<recob::Hit> tha(realTracks,evt,pfpTag);
  totNTrackHits = 0;
  for (std::vector<int>::size_type i=0; i!=realTracks.size(); i++)
  {
    std::vector<recob::Hit const*> tempHitVector;
    tha.get(i,tempHitVector);
    trackHits.push_back(tempHitVector);
    nTrackHits.push_back(tempHitVector.size());
    totNTrackHits += tempHitVector.size();
  }

  // Find hits associated with recob::Showers
  art::FindMany<recob::Hit> sha(realShowers,evt,pfpTag);
  totNShowerHits = 0;
  for (std::vector<int>::size_type i=0; i!=realShowers.size(); i++)
  {
    std::vector<recob::Hit const*> tempHitVector;
    sha.get(i,tempHitVector);
    showerHits.push_back(tempHitVector);
    nShowerHits.push_back(tempHitVector.size());
    totNShowerHits += tempHitVector.size();
  }

  // Calculate tree quantities
  // Nothing this time

  // Diagnostic message
  if (fVerbose) {printf("Loading %lu hits (%i from %lu secondary tracks, %i from %lu secondary showers).\n", totHits.size(), totNTrackHits, tracks.size(), totNShowerHits, showers.size());}
  return;
} // END function GetHitVectors

void ScanRecoSelectionParameters::PerformCalorimetry(const std::vector<AuxVertex::DecayVertex>& cleanVertices, const std::vector<recob::Hit const*>& totHits, const std::vector<std::vector<recob::Hit const*>>& trackHits, const std::vector<std::vector<recob::Hit const*>>& showerHits, std::vector<std::vector<recob::Hit const*>>& totHitsInRadius)
{
  // Perform calorimetry analysis. At this stage we finally calculate all the charge deposited by the hits of track1 and track2 (or shower) within a radius (fCaloCut) from the assumed HSN decay vertex (cleanVertices[i]), for each decay vertex.
  // The second step looks at all the charge deposited by any hit in radius (which may come from hadronic interaction, in the case of background). And we finally calculate the ratio between the two (caloRatio). We would expect this ratio to be closer to 1 for signal, since HSN decaying in the detector don't interact with any particle, and we'd expect charge deposited by the two decay products to be the only charge within a certain radius from the decay point.


  // Loop through each clean vertex
  int vertInd = 0;
  std::cout << vertInd;
  for (std::vector<int>::size_type i=0; i!=cleanVertices.size(); i++)
  {
    // Initialize the vector of hits that will be pushed back to the vector of vector of hits and returned by the function
    std::vector<recob::Hit const*> totHitsInRadius_thisDv;

    // Get useful quantities about the clean vertex currently being analyzed, like coordinates and parent indices
    int channel0[3] = {cleanVertices[i].GetChannelLoc(0),cleanVertices[i].GetChannelLoc(1),cleanVertices[i].GetChannelLoc(2)};
    double tick0[3] = {cleanVertices[i].GetTickLoc(0),cleanVertices[i].GetTickLoc(1),cleanVertices[i].GetTickLoc(2)};
    int parIdx1 = cleanVertices[i].GetParIdx1();
    int parIdx2 = cleanVertices[i].GetParIdx2();
    if (fVerbose) {cleanVertices[i].PrintInformation();}

    // Calculate calorimetry for track 1 within radius
    double parCharge1 = 0.;
    for (auto hit : trackHits[parIdx1])
    {
      int hitChannel = hit->Channel();
      double hitTick = (hit->EndTick() + hit->StartTick())/2.;
      int hitPlane = hit->View();
      bool isInsideRadius = (pow(((hitChannel-channel0[hitPlane])/fChannelNorm),2.) + pow(((hitTick-tick0[hitPlane])/fTickNorm),2.) < pow(fCaloCut,2.));
      if (isInsideRadius)
      {
        double hitCharge = hit->Integral();
        parCharge1 += hitCharge;
      }
    }

    // Calculate calorimetry for track 2 within radius
    double parCharge2 = 0.;
    for (auto hit : trackHits[parIdx2])
    {
      int hitChannel = hit->Channel();
      double hitTick = (hit->EndTick() + hit->StartTick())/2.;
      int hitPlane = hit->View();
      bool isInsideRadius = (pow(((hitChannel-channel0[hitPlane])/fChannelNorm),2.) + pow(((hitTick-tick0[hitPlane])/fTickNorm),2.) < pow(fCaloCut,2.));
      if (isInsideRadius)
      {
        double hitCharge = hit->Integral();
        parCharge2 += hitCharge;
      }
    }

    // Calculate total calorimetry within radius
    double totCharge = 0.;
    for (auto hit : totHits)
    {
      int hitChannel = hit->Channel();
      double hitTick = (hit->EndTick() + hit->StartTick())/2.;
      int hitPlane = hit->View();
      bool isInsideRadius = (pow(((hitChannel-channel0[hitPlane])/fChannelNorm),2.) + pow(((hitTick-tick0[hitPlane])/fTickNorm),2.) < pow(fCaloCut,2.));
      if (isInsideRadius)
      {
        totHitsInRadius_thisDv.push_back(hit);
        double hitCharge = hit->Integral();
        totCharge += hitCharge;
      }
    }
    totHitsInRadius.push_back(totHitsInRadius_thisDv);

    // Determine the ratio for this clean vertex
    double thisCaloRatio = (parCharge1+parCharge2)/totCharge;

    // Calculate tree quantities
    par1ChargeInRadius.push_back(parCharge1);
    par2ChargeInRadius.push_back(parCharge2);
    totChargeInRadius.push_back(totCharge);
    caloRatio.push_back(thisCaloRatio);

    // Diagnostic message
    if (fVerbose) {printf("\n-|Clean Vertex %i|\n|_Deposit IP1: %.1f\n|_Deposit IP2: %.1f\n|_Total deposit: %.1f\n|_RATIO: %.1f\n",vertInd,parCharge1,parCharge2,totCharge,thisCaloRatio);}

    vertInd++;
  } // End clean vertex loop
  return;
} // END function PerformCalorimetry

void ScanRecoSelectionParameters::FillDrawTree(const std::vector<AuxVertex::DecayVertex>& cleanVertices, const std::vector<std::vector<recob::Hit const*>>& totHitsInRadius, const std::vector<std::vector<recob::Hit const*>>& trackHits, const std::vector<std::vector<recob::Hit const*>>& showerHits)
{
  for (std::vector<int>::size_type i=0; i!=cleanVertices.size(); i++)
  {
    // Get clean vertex
    auto dv = cleanVertices[i];
    int parIdx1 = dv.GetParIdx1();
    int parIdx2 = dv.GetParIdx2();
    std::vector<recob::Hit const*> par1_hits = trackHits[parIdx1];
    std::vector<recob::Hit const*> par2_hits = trackHits[parIdx2];
    std::vector<recob::Hit const*> thisTot_hits = totHitsInRadius[i];
    std::vector<double> thisPar1_hits_p0_tickCoordinates,
      thisPar1_hits_p1_tickCoordinates,
      thisPar1_hits_p2_tickCoordinates,
      thisPar2_hits_p0_tickCoordinates,
      thisPar2_hits_p1_tickCoordinates,
      thisPar2_hits_p2_tickCoordinates,
      thisTot_hits_p0_tickCoordinates,
      thisTot_hits_p1_tickCoordinates,
      thisTot_hits_p2_tickCoordinates;
    std::vector<int> thisPar1_hits_p0_wireCoordinates,
      thisPar1_hits_p1_wireCoordinates,
      thisPar1_hits_p2_wireCoordinates,
      thisPar2_hits_p0_wireCoordinates,
      thisPar2_hits_p1_wireCoordinates,
      thisPar2_hits_p2_wireCoordinates,
      thisTot_hits_p0_wireCoordinates,
      thisTot_hits_p1_wireCoordinates,
      thisTot_hits_p2_wireCoordinates;

    for (auto hit : par1_hits)
    {
      if (hit->View() == 0) {
        thisPar1_hits_p0_wireCoordinates.push_back(hit->Channel());
        thisPar1_hits_p0_tickCoordinates.push_back((hit->StartTick() + hit->EndTick())/2.);
      }
      if (hit->View() == 1) {
        thisPar1_hits_p1_wireCoordinates.push_back(hit->Channel());
        thisPar1_hits_p1_tickCoordinates.push_back((hit->StartTick() + hit->EndTick())/2.);
      }
      if (hit->View() == 2) {
        thisPar1_hits_p2_wireCoordinates.push_back(hit->Channel());
        thisPar1_hits_p2_tickCoordinates.push_back((hit->StartTick() + hit->EndTick())/2.);
      }
    }

    for (auto hit : par2_hits)
    {
      if (hit->View() == 0) {
        thisPar2_hits_p0_wireCoordinates.push_back(hit->Channel());
        thisPar2_hits_p0_tickCoordinates.push_back((hit->StartTick() + hit->EndTick())/2.);
      }
      if (hit->View() == 1) {
        thisPar2_hits_p1_wireCoordinates.push_back(hit->Channel());
        thisPar2_hits_p1_tickCoordinates.push_back((hit->StartTick() + hit->EndTick())/2.);
      }
      if (hit->View() == 2) {
        thisPar2_hits_p2_wireCoordinates.push_back(hit->Channel());
        thisPar2_hits_p2_tickCoordinates.push_back((hit->StartTick() + hit->EndTick())/2.);
      }
    }

    for (auto hit : thisTot_hits)
    {
      if (hit->View() == 0) {
        thisTot_hits_p0_wireCoordinates.push_back(hit->Channel());
        thisTot_hits_p0_tickCoordinates.push_back((hit->StartTick() + hit->EndTick())/2.);
      }
      if (hit->View() == 1) {
        thisTot_hits_p1_wireCoordinates.push_back(hit->Channel());
        thisTot_hits_p1_tickCoordinates.push_back((hit->StartTick() + hit->EndTick())/2.);
      }
      if (hit->View() == 2) {
        thisTot_hits_p2_wireCoordinates.push_back(hit->Channel());
        thisTot_hits_p2_tickCoordinates.push_back((hit->StartTick() + hit->EndTick())/2.);
      }
    }

    // Get coordinates
    dv_xyzCoordinates.push_back({dv.GetX(),dv.GetY(),dv.GetZ()});
    dv_wireCoordinates.push_back({dv.GetChannelLoc(0),dv.GetChannelLoc(1),dv.GetChannelLoc(2)});
    dv_tickCoordinates.push_back({dv.GetTickLoc(0),dv.GetTickLoc(1),dv.GetTickLoc(2)});
    par1_xyzCoordinates.push_back({dv.GetParX(0),dv.GetParY(0),dv.GetParZ(0)});
    par1_wireCoordinates.push_back({dv.GetParChannelLoc(0,0),dv.GetParChannelLoc(0,1),dv.GetParChannelLoc(0,2)});
    par1_tickCoordinates.push_back({dv.GetParTickLoc(0,0),dv.GetParTickLoc(0,1),dv.GetParTickLoc(0,2)});
    par2_xyzCoordinates.push_back({dv.GetParX(1),dv.GetParY(1),dv.GetParZ(1)});
    par2_wireCoordinates.push_back({dv.GetParChannelLoc(1,0),dv.GetParChannelLoc(1,1),dv.GetParChannelLoc(1,2)});
    par2_tickCoordinates.push_back({dv.GetParTickLoc(1,0),dv.GetParTickLoc(1,1),dv.GetParTickLoc(1,2)});

    par1_hits_p0_wireCoordinates.push_back(thisPar1_hits_p0_wireCoordinates);
    par1_hits_p1_wireCoordinates.push_back(thisPar1_hits_p1_wireCoordinates);
    par1_hits_p2_wireCoordinates.push_back(thisPar1_hits_p2_wireCoordinates);
    par1_hits_p0_tickCoordinates.push_back(thisPar1_hits_p0_tickCoordinates);
    par1_hits_p1_tickCoordinates.push_back(thisPar1_hits_p1_tickCoordinates);
    par1_hits_p2_tickCoordinates.push_back(thisPar1_hits_p2_tickCoordinates);

    par2_hits_p0_wireCoordinates.push_back(thisPar2_hits_p0_wireCoordinates);
    par2_hits_p1_wireCoordinates.push_back(thisPar2_hits_p1_wireCoordinates);
    par2_hits_p2_wireCoordinates.push_back(thisPar2_hits_p2_wireCoordinates);
    par2_hits_p0_tickCoordinates.push_back(thisPar2_hits_p0_tickCoordinates);
    par2_hits_p1_tickCoordinates.push_back(thisPar2_hits_p1_tickCoordinates);
    par2_hits_p2_tickCoordinates.push_back(thisPar2_hits_p2_tickCoordinates);

    tot_hits_p0_wireCoordinates.push_back(thisTot_hits_p0_wireCoordinates);
    tot_hits_p1_wireCoordinates.push_back(thisTot_hits_p1_wireCoordinates);
    tot_hits_p2_wireCoordinates.push_back(thisTot_hits_p2_wireCoordinates);
    tot_hits_p0_tickCoordinates.push_back(thisTot_hits_p0_tickCoordinates);
    tot_hits_p1_tickCoordinates.push_back(thisTot_hits_p1_tickCoordinates);
    tot_hits_p2_tickCoordinates.push_back(thisTot_hits_p2_tickCoordinates);
  }
  return;
} // END function FillDrawTree


void ScanRecoSelectionParameters::analyze(art::Event const & evt)
{
  // Core analysis. Use all the previously defined functions to determine success rate. This will be repeated event by event.
  if (fVerbose) {printf("\n------------------------------------------------\n");}

  // Start by clearing all the vectors.
  ClearData();

  // Determine event ID
  run = evt.id().run();
  subrun = evt.id().subRun();
  event = evt.id().event();
  if (fVerbose) {printf("||INFORMATION FOR EVENT %i [RUN %i, SUBRUN %i]||\n", event, run, subrun);}

  // Prepare track, shower and Pandora primary vectors
  std::vector<recob::PFParticle const*> tracks, showers, pandora_primaryPFP;
  GetTrackShowerVectors(evt, pandora_primaryPFP, tracks, showers);

  // Determine origin vertices for tracks and showers
  std::vector<AuxVertex::DecayVertex> trackVertices, showerVertices;
  GetOriginVertices(evt, tracks, showers, trackVertices, showerVertices);

  // Determine potential and clean decay vertices satisfying selection
  std::vector<AuxVertex::DecayVertex> potVertices, cleanVertices;
  GetDecayVertices(trackVertices, showerVertices, potVertices, cleanVertices);

  // Just being smart, if there's no clean vertices, there's no reason to waste time loading hits that won't be used
  if (cleanVertices.size()==0) {printf("No clean vertex candidates found. Moving to next event...\n");}
  else
  {
    // Get vectors containing hits for each track/shower object in order to perform calorimetry
    std::vector<recob::Hit const*> totHits;
    std::vector<std::vector<recob::Hit const*>> trackHits, showerHits;
    GetHitVectors(evt, tracks, showers, totHits, trackHits, showerHits);

    // Perform calorimetry analysis
    std::vector<std::vector<recob::Hit const*>> totHitsInRadius;
    PerformCalorimetry(cleanVertices, totHits, trackHits, showerHits, totHitsInRadius);

    // Fill draw tree (optional)
    if (fSaveDrawTree)
    {
      FillDrawTree(cleanVertices, totHitsInRadius, trackHits, showerHits);
    }
  }


  // Fill tree and finish event loop
  tTree->Fill();
  if (fSaveDrawTree) {drawTree->Fill();}
  printf("------------------------------------------------\n\n");
} // END function analyze


// Name that will be used by the .fcl to invoke the module
DEFINE_ART_MODULE(ScanRecoSelectionParameters)

#endif // END def ScanRecoSelectionParameters_module
// ----------------------------
//
// SIDISKinematicsReco.C()
// 
// Written by Gregory Matousek
// 
// ----------------------------

#include "SIDISKinematicsReco.h"

using namespace std;

SIDISKinematicsReco::SIDISKinematicsReco(std::string outfilename):
  _ievent(0),
  _tfile(nullptr),
  _tree_MC(nullptr),
  _tree_Reco(nullptr),
  _electron_beam_energy(0)
{
  _outfilename = outfilename;
  cout << "Initialized SIDISKinematicsReco" << endl;
}

int SIDISKinematicsReco::Init()
{
  
  // Create TFile
  // -------------------------

  _tfile = new TFile(_outfilename.c_str(),"RECREATE");

  // Create event variable map 
  // -------------------------  
  float dummy = 0;
  std::vector<float> vdummy;

  _map_event.insert( make_pair( "nParticles" , dummy ) );
  _map_event.insert( make_pair( "x" , dummy ) );
  _map_event.insert( make_pair( "y" , dummy ) );
  _map_event.insert( make_pair( "Q2" , dummy ) );
  _map_event.insert( make_pair( "W" , dummy ) );
  _map_event.insert( make_pair( "nu" , dummy ) );
  _map_event.insert( make_pair( "helicity" , dummy ) );
  _map_event.insert( make_pair( "polarization" , dummy ) );
  _map_event.insert( make_pair( "evnum" , dummy ) );
  
  // Create particle map 
  // -------------------------  
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_pid , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_px , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_py , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_pz , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_pt , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_p , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_E , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_theta , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_eta , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_phi , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_vx , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_vy , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_vz , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_vt , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_pindex , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_beta , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_chi2 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_status , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_parentID , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::part_parentPID , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_sector , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_path1 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_path2 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_path3 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_det1 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_det2 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_det3 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_x1 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_x2 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_x3 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_y1 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_y2 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_y3 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_z1 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_z2 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::traj_z3 , vdummy) );
  
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_sector_PCAL , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_sector_ECIN , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_sector_ECOUT , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_energy_PCAL , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_energy_ECIN , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_energy_ECOUT , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_time_PCAL , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_time_ECIN , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_time_ECOUT , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_path_PCAL , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_path_ECIN , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_path_ECOUT , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_lu_PCAL , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_lu_ECIN , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_lu_ECOUT , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_lv_PCAL , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_lv_ECIN , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_lv_ECOUT , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_lw_PCAL , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_lw_ECIN , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::cal_lw_ECOUT , vdummy) );


  // ----------------- MONTE CARLO VERSION ------------------- //
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_pid , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_px , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_py , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_pz , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_pt , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_p , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_E , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_theta , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_eta , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_phi , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_vx , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_vy , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_vz , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_vt , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_pindex , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_beta , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_chi2 , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_status , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_parentID , vdummy) );
  _map_particle.insert( make_pair( SIDISParticle::PROPERTY::evtgen_part_parentPID , vdummy) );

  // Create Monte Carlo TTree
  // -------------------------

  _tree_MC = new TTree("tree_MC","A Tree with *true* event and particle information");
  _tree_MC->Branch("event_truth", &_ievent, "event/I");
  
  // Add Event Branches
  // -------------------------
  for(map< string , float >::iterator it = _map_event.begin(); it!= _map_event.end(); ++it)
    {
      _tree_MC->Branch( (it->first).c_str() , &(it->second) );
    }
  
  // Add Particle Branches
  // -------------------------
  for (map< SIDISParticle::PROPERTY, std::vector<float> >::iterator it = _map_particle.begin(); it!=_map_particle.end(); ++it)
    {
      _tree_MC->Branch( SIDISParticle::get_property_info( (it->first) ).first.c_str(), &(it->second));
    }
 
  // Create Reconstructed TTree
  // as Monte Carlo copy
  // -------------------------
  _tree_Reco = _tree_MC->CloneTree();
  _tree_Reco->SetName("tree_reco");
  _tree_Reco->SetTitle("A Tree with *reconstructred* event and particle information");


  // Load in HipoFiles
  // -------------------------
  if(_settings._hipoFileStrings.size()==0){
    std::cout << "ERROR in SIDISKinematicsReco::Init() -- No files added to hipoChain. Double-check that 'addHipoFile' in Settings.h was called by processing script" << std::endl;
    return -1;
  }

  for(unsigned int idx = 0 ; idx < _settings._hipoFileStrings.size() ; idx ++){
    _chain.Add(_settings._hipoFileStrings.at(idx).c_str());

  }

  // Set beam energy
  // -------------------------
  _electron_beam_energy = _settings._electronBeamEnergy;

  // Configure CLAS12Reader
  // -------------------------
  _config_c12=_chain.GetC12Reader();
  
  // Configure PID helper
  // -------------------------
  _pidhelper = PID();
  _pidhelper.ImportSettings(_settings);

  // Initialize Hipo file settings
  // -------------------------------
  InitHipo();
  return 0;
}

int SIDISKinematicsReco::InitHipo()
{
  // -----------------------------------------------------
  // Using the _config_c12 object, cut PIDs for each event
  // -----------------------------------------------------
  std::vector<int> finalStatePIDs = _settings._fPID;
  for(unsigned int idx = 0 ; idx < finalStatePIDs.size(); idx++){
    int pid = finalStatePIDs.at(idx);
    int npid = _settings.getN_fromPID(pid);
    bool exactpid = _settings.isExact_fromPID(pid);
    if(exactpid==true)
      _config_c12->addExactPid(pid,npid);
    else
      _config_c12->addAtLeastPid(pid,npid);
  }

  // -----------------------------------------------------
  // -----------------------------------------------------
  // Add banks to the _config_c12 object
  // -----------------------------------------------------

  // Add REC::Kinematics bank
  if(_settings._eventRecoMethod == Settings::eventRecoMethod::useRecKinematicsBank){
    _idx_RECKin = _config_c12->addBank("REC::Kinematics");
    _ix = _config_c12->getBankOrder(_idx_RECKin,"x");
    _iQ2 = _config_c12->getBankOrder(_idx_RECKin,"Q2"); 
    _iy = _config_c12->getBankOrder(_idx_RECKin,"y");
    _inu = _config_c12->getBankOrder(_idx_RECKin,"nu");
    _iW = _config_c12->getBankOrder(_idx_RECKin,"W");
  }

  // Add RUN::config bank
  _idx_RUNconfig = _config_c12->addBank("RUN::config");
  _irun = _config_c12->getBankOrder(_idx_RUNconfig,"run");
  _ievnum = _config_c12->getBankOrder(_idx_RUNconfig,"event");

  return 0;
}
int SIDISKinematicsReco::process_events()
{
  // Establish CLAS12 event parser
  // -------------------------
  auto &_c12= _chain.C12ref();

  // Configure HipoBankInterface
  // -------------------------
  _hipoInterface = HipoBankInterface(_c12); 

  // Create Fiducial Cuts Object
  // -----------------------------------------------------
  if(_settings._doFiducialCuts)
    _fiducial = FiducialCuts();


  // Move to the next event in the Hipo chain
  while(_chain.Next()==true){
    if(_verbosity > 0 && (_ievent)%_printEvery==0 && _ievent!=0){
       std::cout << _ievent << " events completed | " << _tree_Reco->GetEntriesFast() << " passed cuts --> " << _tree_Reco->GetEntriesFast()*100.0/_ievent << "%" << std::endl;
    }
    if(_ievent == 10)
      break;
    // Get the run number from the RUN::config bank
    // Also get the event number too
    // -----------------------------------------------------
    _runNumber = _c12->getBank(_idx_RUNconfig)->getInt(_irun,0);   
    _evnum = _c12->getBank(_idx_RUNconfig)->getInt(_ievnum,0);   

    // Set torus bending in fiducial
    // -----------------------------------------------------
    _fiducial.setTorusBending(runTorusBending(_runNumber));

    /* Increase event # */
    _ievent++;

    if(_c12->getDetParticles().empty())
      continue;
    
    // Create map to store reconstructed SIDIS particles
    // Use the pindex as a unique key
    type_map_part recoparticleMap;

    // Create map to store true SIDIS particles
    type_map_part particleMap;

    
    // Parse through reconstructed particle data
    if(_settings._doReco)
      {
		
	/* Add reco particle information */
	/* Skip event if certain cuts are not satisfied */
	if(CollectParticlesFromReco( _c12, recoparticleMap )!=0)
       	  continue;
      }
    
    // Parse through true Monte Carlo particle data
    if(_settings._doMC)
      {
	
	/* Add particle information */
	CollectParticlesFromTruth( _c12, particleMap );
	
	/* Connect MC and Reco particle info in TTrees */
	if(_settings._connectMC2Reco)
	  {
	    ConnectTruth2Reco( particleMap, recoparticleMap );
	  }
      }

    // ---------------------------
    // Writing to TTrees 
    // ---------------------------

    if(_settings._doReco)
      {
	/* Reset branch map */
	ResetBranchMap();

	/* Add event information */
	/* Skip event if it fails cuts */
	if(AddRecoEventInfo( _c12 )!=0)
	  continue;
	/* Write particle information to Tree */
       	WriteParticlesToTree( recoparticleMap );

	/* fill reco tree */
       	_tree_Reco->Fill();

	/* Free up memory taken by elements of the map */
	DeleteParticlePointers( recoparticleMap );
      }
    if(_settings._doMC)
      {
	/* Reset branch map */
	ResetBranchMap();

	/* Add event information */
	AddTruthEventInfo( _c12 );
	/* Write particle information to Tree */	
	WriteParticlesToTree( particleMap );
	
	/* Fill MC tree */
	_tree_MC->Fill();

	/* Free up memory taken by elements of the map */
	DeleteParticlePointers( particleMap );
      }
  }
  
  std::cout << " All events completed \n Done." << std::endl;

  return 0;
}

int SIDISKinematicsReco::CollectParticlesFromTruth(const std::unique_ptr<clas12::clas12reader>& _c12,
						   type_map_part& particleMap )
{
  // Get pointer to Monte Carlo particles in event
  auto mcparticles=_c12->mcparts();

  // Loop over all particles
  for(int idx = 0 ; idx < mcparticles->getRows() ; idx++){
    
    // Get particle in MC::Lund
    mcparticles->setEntry(idx);
    if(mcparticles->getType()!=1) // Reject non-final state
      continue; 
    
    // Create new SIDISParticle
    SIDISParticlev1 *sp = new SIDISParticlev1();
    sp->set_candidate_id( particleMap.size() );

    int pid = mcparticles->getPid();
    float px = mcparticles->getPx();
    float py = mcparticles->getPy();
    float pz = mcparticles->getPz();
    float m = mcparticles->getMass();
    
    float pt = _kin.Pt(px,py);
    float p  = _kin.P(px,py,pz);
    float E  = _kin.E(m,p);

    float theta = _kin.th(pt,pz);
    float eta = _kin.eta(theta);
    float phi   = _kin.phi(px,py);

    float vx = mcparticles->getVx();
    float vy = mcparticles->getVy();
    float vz = mcparticles->getVz();
    //    float vt = mcparticles->getVt();

    sp->set_property( SIDISParticle::evtgen_part_pid, pid);
    sp->set_property( SIDISParticle::evtgen_part_px,  px);
    sp->set_property( SIDISParticle::evtgen_part_py,  py);
    sp->set_property( SIDISParticle::evtgen_part_pz,  pz);
    sp->set_property( SIDISParticle::evtgen_part_pt,  pt);
    sp->set_property( SIDISParticle::evtgen_part_p,  p);
    sp->set_property( SIDISParticle::evtgen_part_E,   E);
    sp->set_property( SIDISParticle::evtgen_part_theta,   theta);
    sp->set_property( SIDISParticle::evtgen_part_eta,   eta);
    sp->set_property( SIDISParticle::evtgen_part_phi,   phi);
    sp->set_property( SIDISParticle::evtgen_part_vx,   vx);
    sp->set_property( SIDISParticle::evtgen_part_vy,   vy);
    sp->set_property( SIDISParticle::evtgen_part_vz,   vz);
    //    sp->set_property( SIDISParticle::evtgen_part_vt,   vt);
    sp->set_property( SIDISParticle::evtgen_part_pindex,   ineg999);
    sp->set_property( SIDISParticle::evtgen_part_beta,   fneg999);
    sp->set_property( SIDISParticle::evtgen_part_chi2,   fneg999);
    sp->set_property( SIDISParticle::evtgen_part_ID,   mcparticles->getIndex());
    sp->set_property( SIDISParticle::evtgen_part_parentID,  mcparticles->getParent());
    sp->set_property( SIDISParticle::evtgen_part_parentPID, mcparticles->getPid(mcparticles->getParent()-1));
      
    // Add SIDISParticle to the collection
    particleMap.insert ( make_pair( sp->get_candidate_id() , sp) );
    
  }
  return 0;
}


int SIDISKinematicsReco::CollectParticlesFromReco(const std::unique_ptr<clas12::clas12reader>& _c12,
						   type_map_part& recoparticleMap )
{
  // Get std::vector<> of particles in event
  // -------------------------------------------------------
  auto particles=_c12->getDetParticles();
  // Loop over all particles
  // -------------------------------------------------------
  for(unsigned int idx = 0 ; idx < particles.size() ; idx++){
    // Extract each particle from event one-at-a-time
    // -------------------------------------------------------
    auto particle = particles.at(idx);
    int pid = particle->getPid();
    float chi2 = particle->getChi2Pid();
    float theta = particle->getTheta();
    float eta = _kin.eta(theta);
    float phi = particle->getPhi();
    float p = particle->getP();
    float px = _kin.Px(p,theta,phi);
    float py = _kin.Py(p,theta,phi);
    float pz = _kin.Pz(p,theta,phi);
    float pt = _kin.Pt(px,py);
    float m = 0.0;
    if(pid!=22)
      m = particle->getPdgMass();
    float E  = _kin.E(m,p);
    float beta = particle->getBeta();
    int pindex = particle->getIndex();
    float vx = particle->par()->getVx();
    float vy = particle->par()->getVy();
    float vz = particle->par()->getVz();
    //    float vt = particle->par()->getVt();

    // Create new particle structure
    // -------------------------------------------------------
    SIDISParticlev1 *sp = new SIDISParticlev1();
    sp->set_candidate_id( pindex );
    
    sp->set_property( SIDISParticle::part_pid, pid);
    sp->set_property( SIDISParticle::part_px,  px);
    sp->set_property( SIDISParticle::part_py,  py);
    sp->set_property( SIDISParticle::part_pz,  pz);
    sp->set_property( SIDISParticle::part_pt,  pt);
    sp->set_property( SIDISParticle::part_p,  p);
    sp->set_property( SIDISParticle::part_E,   E);
    sp->set_property( SIDISParticle::part_theta,   theta);
    sp->set_property( SIDISParticle::part_eta,   eta);
    sp->set_property( SIDISParticle::part_phi,   phi);
    sp->set_property( SIDISParticle::part_vx,   vx);
    sp->set_property( SIDISParticle::part_vy,   vy);
    sp->set_property( SIDISParticle::part_vz,   vz);
    //    sp->set_property( SIDISParticle::part_vt,   vt);
    sp->set_property( SIDISParticle::part_pindex,   pindex);
    sp->set_property( SIDISParticle::part_beta,   beta);
    sp->set_property( SIDISParticle::part_chi2,   chi2);
    sp->set_property( SIDISParticle::part_ID, pindex);
    sp->set_property( SIDISParticle::part_parentID, -999);
    sp->set_property( SIDISParticle::part_parentPID, -999);

    // ------------------- MONTE CARLO ----------------------------//
    sp->set_property( SIDISParticle::evtgen_part_pid, -999);
    sp->set_property( SIDISParticle::evtgen_part_px,  (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_py,  (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_pz,  (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_pt,  (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_p,  (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_E,  (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_theta,   (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_eta,   (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_phi,   (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_vx,   (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_vy,   (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_vz,   (float)-999);
    //    sp->set_property( SIDISParticle::evtgen_part_vt,   (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_pindex,   -999);
    sp->set_property( SIDISParticle::evtgen_part_beta,   (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_chi2,   (float)-999);
    sp->set_property( SIDISParticle::evtgen_part_ID, -999);
    sp->set_property( SIDISParticle::evtgen_part_parentID, -999);
    sp->set_property( SIDISParticle::evtgen_part_parentPID, -999);

    // Add detector info to SIDISParticle
    // --------------------------------------------------------------------------
    
    if(_hipoInterface.loadBankData(_c12, sp)==false)
      continue;
    
    // CUT REC::Particle
    // --------------------------------------------------------------------------
    if(_pidhelper.performPIDCuts(sp)==false)
      continue;

    // CUT Fiducial
    // --------------------------------------------------------------------------
    if(_settings._doFiducialCuts){
      if(_fiducial.FidCutParticle(_c12,_runNumber,sp) == false)
	  continue;
    }
    
    // Add SIDISParticle to the collection
    // --------------------------------------------------------------------------
    recoparticleMap.insert ( make_pair( sp->get_candidate_id() , sp) );
  }

  // Parse through the recoparticleMap to check if the desired event trigger was reco'd
  // In this case, we make sure that, after all cuts, the event contains the desired minimum/exact
  // number of particles
  // ---------------------------------------------------------------------------------------
  for (unsigned int i = 0 ; i < _settings._fPID.size() ; i++){
    int pid = _settings._fPID.at(i);
    int npart = _settings._fNpart.at(i);
    bool exact = _settings._fExact.at(i);
    int npart_map = 0;
    for (type_map_part::iterator it_reco = recoparticleMap.begin(); it_reco!= recoparticleMap.end() ; ++it_reco){
      if( (it_reco->second)->get_property_int(SIDISParticle::part_pid) == pid)
	npart_map++;
    }
    
    if(exact == true && npart_map!=npart)
      return -1;
    else if(exact == false && npart_map<npart)
      return -1;
  }

  return 0;
}

int SIDISKinematicsReco::ConnectTruth2Reco( type_map_part& particleMap,
					    type_map_part& recoparticleMap )
{
  /* Loop over all reco particles */
  for (type_map_part::iterator it_reco = recoparticleMap.begin(); it_reco!= recoparticleMap.end() ; ++it_reco){

    double reco_theta = (it_reco->second)->get_property_float(SIDISParticle::part_theta); 
    double reco_phi = (it_reco->second)->get_property_float(SIDISParticle::part_phi); 
    double reco_E = (it_reco->second)->get_property_float(SIDISParticle::part_E);
      /* Loop over all MC particles */
      for(type_map_part::iterator it_mc = particleMap.begin(); it_mc!= particleMap.end(); ++it_mc){
	double mc_theta = (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_theta); 
	double mc_phi = (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_phi); 
	double mc_E = (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_E);
	/* Match the *theta* and *phi* of two particles. For details, see https://www.jlab.org/Hall-B/general/thesis/THayward_thesis.pdf */
	double dth = abs(reco_theta-mc_theta);
	double dphi = abs(reco_phi-mc_phi);
	double dE = abs(reco_E - mc_E);
	//	int mcpid = (it_mc->second)->get_property_int(SIDISParticle::part_pid);
	//	int recopid = (it_reco->second)->get_property_int(SIDISParticle::part_pid);
	if((dE < 0.5) &&
	   (dth < 6*degtorad) && 
	    (dphi < 2*degtorad || abs(dphi - 2*PI) < 2*degtorad)){
	  (it_mc->second)->set_property( SIDISParticle::part_pindex, (it_reco->second)->get_property_int(SIDISParticle::part_pindex));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_pid , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_pid));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_px , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_px));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_py , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_py));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_pz , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_pz));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_pt , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_pt));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_p , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_p));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_vx , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_vx));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_vy , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_vy));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_vz , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_vz));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_E , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_E));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_theta , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_theta));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_eta , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_eta));
	  (it_reco->second)->set_property( SIDISParticle::evtgen_part_phi , (it_mc->second)->get_property_float(SIDISParticle::evtgen_part_phi));
	  (it_reco->second)->set_property( SIDISParticle::part_parentID , (it_mc->second)->get_property_int(SIDISParticle::part_parentID));
	  (it_reco->second)->set_property( SIDISParticle::part_parentPID , (it_mc->second)->get_property_int(SIDISParticle::part_parentPID));
	}
      }
  }
  
  return 0;
  
}

int SIDISKinematicsReco::AddTruthEventInfo(const std::unique_ptr<clas12::clas12reader>& _c12)
{
  // Get pointer to Monte Carlo particles in event
  auto mcparticles=_c12->mcparts();

  // Loop over all particles
  for(int idx = 0 ; idx < mcparticles->getRows() ; idx++){
   
    // Get particle in MC::Lund
    mcparticles->setEntry(idx);
    
    // Test if scattered lepton
    if(mcparticles->getParent()==1 && mcparticles->getPid()==11){
      float px = mcparticles->getPx();
      float py = mcparticles->getPy();
      float pz = mcparticles->getPz();
      float m = mcparticles->getMass();
      float p = _kin.P(px,py,pz);
      float E = _kin.E(m,p);
      float cth = _kin.cth(px,py,pz);
      float Q2 = _kin.Q2(_electron_beam_energy,E,cth);
      float y  = _kin.y(_electron_beam_energy, E);
      float nu  = _kin.nu(_electron_beam_energy, E);
      float W  = _kin.W(Q2,protonMass,nu);
      float s  = protonMass*protonMass+electronMass*electronMass+2*protonMass*_electron_beam_energy;
      float x = _kin.x(Q2,s,y);

      (_map_event.find("x"))->second = x;
      (_map_event.find("Q2"))->second = Q2;
      (_map_event.find("y"))->second = y;
      (_map_event.find("nu"))->second = nu;
      (_map_event.find("W"))->second = W;
      (_map_event.find("polarization"))->second = 0;
      (_map_event.find("evnum"))->second = _evnum;
      break;
    }
  }
  
  return 0;
}


int SIDISKinematicsReco::AddRecoEventInfo(const std::unique_ptr<clas12::clas12reader>& _c12)
{

  double reco_x = ineg999;
  double reco_Q2 = ineg999;
  double reco_y = ineg999;
  double reco_nu = ineg999;
  double reco_W = ineg999;

  /* METHOD 1: If available, use REC::Kinematics bank for event reco */
  if(_settings._eventRecoMethod == Settings::eventRecoMethod::useRecKinematicsBank){
    reco_x=_c12->getBank(_idx_RECKin)->getDouble(_ix,0);
    reco_Q2=_c12->getBank(_idx_RECKin)->getDouble(_iQ2,0);
    reco_y=_c12->getBank(_idx_RECKin)->getDouble(_iy,0);
    reco_nu=_c12->getBank(_idx_RECKin)->getDouble(_inu,0);
    reco_W=_c12->getBank(_idx_RECKin)->getDouble(_iW,0);
  }
  /* METHOD 2: Use the scattered electron to reconstruct event variables */
  else if(_settings._eventRecoMethod == Settings::eventRecoMethod::useLargestPinFD){
    // Get std::vector<> of particles in event
    auto particles=_c12->getDetParticles();
    
    // Loop over all particles
    for(unsigned int idx = 0 ; idx < particles.size() ; idx++){
      // Extract each particle from event one-at-a-time
      auto particle = particles.at(idx);
      if(particle->getStatus()<0){
	float p = particle->getP();
	float theta = particle->getTheta();
	float phi = particle->getPhi();
	float px = _kin.Px(p,theta,phi);
	float py = _kin.Py(p,theta,phi);
	float pz = _kin.Pz(p,theta,phi);
    	float m = particle->getCalcMass();
	float E = _kin.E(m,p);
	float cth = _kin.cth(px,py,pz);
	reco_Q2 = _kin.Q2(_electron_beam_energy,E,cth);
	reco_y  = _kin.y(_electron_beam_energy, E);
	reco_nu  = _kin.nu(_electron_beam_energy, E);
	reco_W  = _kin.W(reco_Q2,protonMass,reco_nu);
	float s  = protonMass*protonMass+electronMass*electronMass+2*protonMass*_electron_beam_energy;
	reco_x = _kin.x(reco_Q2,s,reco_y);
	
	// Found scattered electron, just break out of here
	break;
      }
    }
  }
  
  if(reco_W < _settings._Wmin || reco_W > _settings._Wmax)
    return -1;
  else if(reco_y < _settings._ymin || reco_y > _settings._ymax)
    return -1;
  else if(reco_Q2 < _settings._Q2min || reco_Q2 > _settings._Q2max)
    return -1;
  else{


    (_map_event.find("x"))->second = reco_x; 
    (_map_event.find("Q2"))->second = reco_Q2;
    (_map_event.find("y"))->second = reco_y;
    (_map_event.find("nu"))->second = reco_nu;
    (_map_event.find("W"))->second = reco_W;
    auto event = _c12->event();
    if(runHelicityFlip(_runNumber))
      (_map_event.find("helicity"))->second = -event->getHelicity();
    else
      (_map_event.find("helicity"))->second = event->getHelicity();
    (_map_event.find("polarization"))->second = runPolarization(_runNumber,true);
    (_map_event.find("evnum"))->second = _evnum;
  }
  
  return 0;
}


int SIDISKinematicsReco::WriteParticlesToTree(type_map_part& particleMap )
{
  /* Get number of particles */
  ( _map_event.find("nParticles") )->second = particleMap.size();
  
  /* Loop over all Particles and add them to tree */
  for (type_map_part::iterator it = particleMap.begin(); it!= particleMap.end(); ++it)
    {
      
      for (map< SIDISParticle::PROPERTY , std::vector<float> >::iterator it_prop = _map_particle.begin(); it_prop!= _map_particle.end(); ++it_prop)
	{
	  switch ( SIDISParticle::get_property_info( (it_prop->first) ).second ) {
	    
          case SIDISParticle::type_float :
            (it_prop->second).push_back( (it->second)->get_property_float( (it_prop->first) ) );
            break;

          case SIDISParticle::type_int :
            (it_prop->second).push_back( (it->second)->get_property_int( (it_prop->first) ) );
            break;
	  }
	}
    }
  
  return 0;
}
void SIDISKinematicsReco::ResetBranchMap()
{
  // Event branch
  for(map<std::string,float>::iterator it = _map_event.begin(); it!=_map_event.end();++it)
    {
      (it->second) = NAN;
    }   

  // Particle branch
  for(map<SIDISParticle::PROPERTY , std::vector<float>>::iterator it = _map_particle.begin(); it!=_map_particle.end(); ++it)
    {
      
      (it->second).clear();
    }
  return;
}

void SIDISKinematicsReco::DeleteParticlePointers(type_map_part& particleMap)
{
  // Loop of particle map
  for(type_map_part::iterator it = particleMap.begin(); it!=particleMap.end();++it)
    {
      delete (it->second);
    }   
  return;
}

int SIDISKinematicsReco::PostProcessReco()
{

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Beginning of PostProcessing. Total events = " << _tree_Reco->GetEntriesFast() << std::endl;
  std::cout << "----------------------------------------" << std::endl;
 
  _tree_PostProcess = new TTree("tree_postprocess","Tree created after all hipo events are filtered");

  _postprocess.Init(_tree_Reco,_electron_beam_energy);
  
  if(_postprocess.setPostProcessMethod(_settings._postProcessMethod)!=0)
    return -1;
  
  // This line is quite the mouthful...
  _postprocess.Process(_tree_PostProcess);

  
  return 0;
}




int SIDISKinematicsReco::End()
{

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Number of events saved to TTRee = " << _tree_Reco->GetEntriesFast() << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  _tfile->cd();
  
  if(_settings._doMC)
    _tree_MC->Write();
  
  if(_settings._doReco)
    _tree_Reco->Write();

  if(_settings._doPostProcess)
    _tree_PostProcess->Write();

  _tfile->Close();
  
  return 0;
}

#ifndef PostProcess_h
#define PostProcess_h

#include <TTree.h>
#include <TLorentzVector.h>
#include <iostream>
#include <fstream>
#include "Constants.h"
#include <math.h>
#include "Kinematics.h"

using namespace std;
class PostProcess{

 public:
  PostProcess();

  enum PROCESS_ID{
    
    pipluspi0 = 0,
    pipluspi0_MC = 1,
    piminuspi0 = 10,
    piminuspi0_MC = 11,

    Inclusive = 20,
    Elastic = 21,

    Exclusive_pipi = 50,
    Exclusive_pipiproton = 51
    
  
  };
  int Init(TTree *, double);
  
  int Process(TTree *);

  int setPostProcessMethod(const char * method){
  
    if(strcmp(method, "pipluspi0")==0){
      _process_id = PROCESS_ID::pipluspi0;
      _piPID=211;
    }

    else if(strcmp(method, "piminuspi0")==0){
      _process_id = PROCESS_ID::piminuspi0;
      _piPID=-211;
    }

    else if(strcmp(method, "pipluspi0_MC")==0){
      _process_id = PROCESS_ID::pipluspi0_MC;
      _piPID=211;
    }

    else if(strcmp(method, "piminuspi0_MC")==0){
      _process_id = PROCESS_ID::piminuspi0_MC;
      _piPID=-211;
    }

    else if(strcmp(method, "inclusive")==0){
      _process_id = PROCESS_ID::Inclusive;
    }

    else if(strcmp(method, "elastic")==0){
      _process_id = PROCESS_ID::Elastic;
    }

    else if(strcmp(method, "exclusive_pipi")==0){
      _process_id = PROCESS_ID::Exclusive_pipi;
    }
    else if(strcmp(method, "exclusive_pipiproton")==0){
      _process_id = PROCESS_ID::Exclusive_pipiproton;
    }

    else{
      std::cout << "ERROR in PostProcess::setPostProcessMethod -- Unrecognized method " << method << " . Breaking . . . " << std::endl;
      return -1;
    }
    return 0;
  }

  int pipi0(TTree *, int);
  int inclusive(TTree *);
  int elastic(TTree *);
  int exclusive_pipi(TTree *);
  int exclusive_pipiproton(TTree *);
  
  void set_electron_beam_energy(double E){ _electron_beam_energy = E; }

  // private:
  Int_t           event_truth=0;
  Float_t         Mx=0;
  Float_t         Q2=0;
  Float_t         W=0;
  Float_t         trueQ2=0;
  Float_t         trueW=0;
  Float_t         evnum=0;
  Float_t         helicity=0;
  Float_t         nParticles=0;
  Float_t         nu=0;
  Float_t         truenu=0;
  Float_t         polarization=0;
  Float_t         run=0;
  Float_t         x=0;
  Float_t         y=0;
  Float_t         truex=0;
  Float_t         truey=0;
  vector<float>   *evtgen_parentPID=0;
  vector<float>   *evtgen_parentparentPID=0;
  vector<float>   *evtgen_parentID=0;
  vector<float>   *evtgen_status=0;
  vector<float>   *evtgen_chi2=0;
  vector<float>   *evtgen_beta=0;
  vector<float>   *evtgen_pindex=0;
  vector<float>   *evtgen_vt=0;
  vector<float>   *evtgen_vz=0;
  vector<float>   *evtgen_vy=0;
  vector<float>   *evtgen_vx=0;
  vector<float>   *evtgen_phi=0;
  vector<float>   *evtgen_eta=0;
  vector<float>   *evtgen_theta=0;
  vector<float>   *evtgen_E=0;
  vector<float>   *evtgen_p=0;
  vector<float>   *evtgen_pt=0;
  vector<float>   *evtgen_pz=0;
  vector<float>   *evtgen_py=0;
  vector<float>   *evtgen_px=0;
  vector<float>   *evtgen_pid=0;
  vector<float>   *pid=0;
  vector<float>   *px=0;
  vector<float>   *py=0;
  vector<float>   *pz=0;
  vector<float>   *pt=0;
  vector<float>   *p=0;
  vector<float>   *E=0;
  vector<float>   *theta=0;
  vector<float>   *eta=0;
  vector<float>   *phi=0;
  vector<float>   *vx=0;
  vector<float>   *vy=0;
  vector<float>   *vz=0;
  vector<float>   *vt=0;
  vector<float>   *pindex=0;
  vector<float>   *beta=0;
  vector<float>   *chi2=0;
  vector<float>   *status=0;
  vector<float>   *parentID=0;
  vector<float>   *parentPID=0;
  vector<float>   *traj_sector=0;
  vector<float>   *traj_path1=0;
  vector<float>   *traj_path2=0;
  vector<float>   *traj_path3=0;
  vector<float>   *traj_det1=0;
  vector<float>   *traj_det2=0;
  vector<float>   *traj_det3=0;
  vector<float>   *traj_x1=0;
  vector<float>   *traj_x2=0;
  vector<float>   *traj_x3=0;
  vector<float>   *traj_y1=0;
  vector<float>   *traj_y2=0;
  vector<float>   *traj_y3=0;
  vector<float>   *traj_z1=0;
  vector<float>   *traj_z2=0;
  vector<float>   *traj_z3=0;
  vector<float>   *cal_sector_PCAL=0;
  vector<float>   *cal_sector_ECIN=0;
  vector<float>   *cal_sector_ECOUT=0;
  vector<float>   *cal_energy_PCAL=0;
  vector<float>   *cal_energy_ECIN=0;
  vector<float>   *cal_energy_ECOUT=0;
  vector<float>   *cal_time_PCAL=0;
  vector<float>   *cal_time_ECIN=0;
  vector<float>   *cal_time_ECOUT=0;
  vector<float>   *cal_path_PCAL=0;
  vector<float>   *cal_path_ECIN=0;
  vector<float>   *cal_path_ECOUT=0;
  vector<float>   *cal_lu_PCAL=0;
  vector<float>   *cal_lu_ECIN=0;
  vector<float>   *cal_lu_ECOUT=0;
  vector<float>   *cal_lv_PCAL=0;
  vector<float>   *cal_lv_ECIN=0;
  vector<float>   *cal_lv_ECOUT=0;
  vector<float>   *cal_lw_PCAL=0;
  vector<float>   *cal_lw_ECIN=0;
  vector<float>   *cal_lw_ECOUT=0;
 
  // List of branches      
  TBranch        *b_event;   //!
  TBranch        *b_Mx;   //!
  TBranch        *b_Q2;   //!
  TBranch        *b_trueQ2;   //!
  TBranch        *b_W;   //!
  TBranch        *b_trueW;   //!
  TBranch        *b_evnum;   //!
  TBranch        *b_helicity;   //!
  TBranch        *b_nParticles;   //!
  TBranch        *b_nu;   //!
  TBranch        *b_truenu;   //!
  TBranch        *b_polarization;   //!
  TBranch        *b_run;   //!
  TBranch        *b_x;   //!
  TBranch        *b_truex;   //!
  TBranch        *b_y;   //!
  TBranch        *b_truey;   //!
  TBranch        *b_evtgen_parentPID;   //!
  TBranch        *b_evtgen_parentparentPID;   //!
  TBranch        *b_evtgen_parentID;   //!
  TBranch        *b_evtgen_status;   //!
  TBranch        *b_evtgen_chi2;   //!
  TBranch        *b_evtgen_beta;   //!
  TBranch        *b_evtgen_pindex;   //!
  TBranch        *b_evtgen_vt;   //!
  TBranch        *b_evtgen_vz;   //!
  TBranch        *b_evtgen_vy;   //!
  TBranch        *b_evtgen_vx;   //!
  TBranch        *b_evtgen_phi;   //!
  TBranch        *b_evtgen_eta;   //!
  TBranch        *b_evtgen_theta;   //!
  TBranch        *b_evtgen_E;   //!
  TBranch        *b_evtgen_p;   //!
  TBranch        *b_evtgen_pt;   //!
  TBranch        *b_evtgen_pz;   //!
  TBranch        *b_evtgen_py;   //!
  TBranch        *b_evtgen_px;   //!
  TBranch        *b_evtgen_pid;   //!
  TBranch        *b_pid;   //!
  TBranch        *b_px;   //!
  TBranch        *b_py;   //!
  TBranch        *b_pz;   //!
  TBranch        *b_pt;   //!
  TBranch        *b_p;   //!
  TBranch        *b_E;   //!
  TBranch        *b_theta;   //!
  TBranch        *b_eta;   //!
  TBranch        *b_phi;   //!
  TBranch        *b_vx;   //!
  TBranch        *b_vy;   //!
  TBranch        *b_vz;   //!
  TBranch        *b_vt;   //!
  TBranch        *b_pindex;   //!
  TBranch        *b_beta;   //!
  TBranch        *b_chi2;   //!
  TBranch        *b_status;   //!
  TBranch        *b_parentID;   //!
  TBranch        *b_parentPID;   //!
  TBranch        *b_traj_sector;   //!
  TBranch        *b_traj_path1;   //!
  TBranch        *b_traj_path2;   //!
  TBranch        *b_traj_path3;   //!
  TBranch        *b_traj_det1;   //!
  TBranch        *b_traj_det2;   //!
  TBranch        *b_traj_det3;   //!
  TBranch        *b_traj_x1;   //!
  TBranch        *b_traj_x2;   //!
  TBranch        *b_traj_x3;   //!
  TBranch        *b_traj_y1;   //!
  TBranch        *b_traj_y2;   //!
  TBranch        *b_traj_y3;   //!
  TBranch        *b_traj_z1;   //!
  TBranch        *b_traj_z2;   //!
  TBranch        *b_traj_z3;   //!
  TBranch        *b_cal_sector_PCAL;   //!
  TBranch        *b_cal_sector_ECIN;   //!
  TBranch        *b_cal_sector_ECOUT;   //!
  TBranch        *b_cal_energy_PCAL;   //!
  TBranch        *b_cal_energy_ECIN;   //!
  TBranch        *b_cal_energy_ECOUT;   //!
  TBranch        *b_cal_time_PCAL;   //!
  TBranch        *b_cal_time_ECIN;   //!
  TBranch        *b_cal_time_ECOUT;   //!
  TBranch        *b_cal_path_PCAL;   //!
  TBranch        *b_cal_path_ECIN;   //!
  TBranch        *b_cal_path_ECOUT;   //!
  TBranch        *b_cal_lu_PCAL;   //!
  TBranch        *b_cal_lu_ECIN;   //!
  TBranch        *b_cal_lu_ECOUT;   //!
  TBranch        *b_cal_lv_PCAL;   //!
  TBranch        *b_cal_lv_ECIN;   //!
  TBranch        *b_cal_lv_ECOUT;   //!
  TBranch        *b_cal_lw_PCAL;   //!
  TBranch        *b_cal_lw_ECIN;   //!
  TBranch        *b_cal_lw_ECOUT;   //!

  TTree *_tree_Reco;
  Long64_t _nentries;
  
  int _piPID;

  PROCESS_ID _process_id;

  double _electron_beam_energy;

  Kinematics _kin;
};
#endif

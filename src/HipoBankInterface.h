#ifndef HipoBankInterface_h
#define HipoBankInterface_h

/* Src Includes */
#include "Constants.h"
#include "Kinematics.h"
#include "SIDISParticlev1.h"
/* CLAS12 Includes */
#include "HipoChain.h"
#include "clas12reader.h"

using namespace std;

class HipoBankInterface{
 public:
  HipoBankInterface();
  HipoBankInterface(const std::unique_ptr<clas12::clas12reader>&);

// Using the pindex from the SIDISParticle, attach extra bank info
  bool loadBankData(const std::unique_ptr<clas12::clas12reader>&, SIDISParticlev1 *);
 protected:
  bool importDataToParticle(SIDISParticlev1 *);
  int determineSectorDC(float, float, float);
  void clear();
  
  int _idx_RECCal;
  int _ipindex_RECCal;
  int _ilu_RECCal;
  int _ilv_RECCal;
  int _ilw_RECCal;
  int _ilayer_RECCal;
  int _isector_RECCal;
  int _itime_RECCal;
  int _ipath_RECCal;
  int _ienergy_RECCal;

  float _Ele_PCAL_e, _Ele_ECIN_e, _Ele_ECOUT_e;
  int _sector_Cal[3]={0,0,0};
  float _time_Cal[3]={0,0,0};
  float _path_Cal[3]={0,0,0};
  float _lu_Cal[3]={0,0,0};
  float _lv_Cal[3]={0,0,0};
  float _lw_Cal[3]={0,0,0};

  int _idx_RECTraj;
  int _ipindex_RECTraj;
  int _ilayer_RECTraj;
  int _idet_RECTraj;
  int _ipath_RECTraj;
  int _ix_RECTraj;
  int _iy_RECTraj;
  int _iz_RECTraj;

  int _sector_DC; 

  int _det_DC[3]={0,0,0};  
  float _path_DC[3]={0,0,0};
  float _x_DC[3]={0,0,0};
  float _y_DC[3]={0,0,0};
  float _z_DC[3]={0,0,0};

};  
#endif

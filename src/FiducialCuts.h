#ifndef FiducialCuts_h
#define FiducialCuts_h

/* Src Includes */
#include "Constants.h"
/* CLAS12 Includes */
#include "HipoChain.h"
#include "clas12reader.h"

using namespace std;

class FiducialCuts{
 public:
  FiducialCuts();
  FiducialCuts(const std::unique_ptr<clas12::clas12reader>&);

  void setTorusBending(int);
  bool FidCutParticle(const std::unique_ptr<clas12::clas12reader>&, int, int , int, float,float);

  bool DC_fiducial_cut_theta_phi(float,float,float,int, int, int);

  bool DC_fiducial_cut_XY(float,float,float,int, int, int);

  bool CheckSampFrac(float);

  int determineSectorDC(float,float,float);

 protected:
  int _idx_RECCal;
  int _ipindex_Cal;
  int _ilv;
  int _ilw;
  int _ilayer_Cal;
  int _isector;
  float _ienergy;
  
  int _idx_RECTraj;
  int _ipindex_Traj;
  int _ilayer_Traj;
  int _icx;
  int _icy;
  int _icz;

  float _cx[3]={0,0,0};
  float _cy[3]={0,0,0};
  float _cz[3]={0,0,0};

  float Ele_PCAL_e, Ele_ECIN_e, Ele_ECOUT_e;
  int Ele_sector;

  int _torusBending=0;
  int _runNumber=0;
};
#endif

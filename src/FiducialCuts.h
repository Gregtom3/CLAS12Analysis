#ifndef FiducialCuts_h
#define FiducialCuts_h

/* Src Includes */
#include "Constants.h"
#include "SIDISParticlev1.h"
/* CLAS12 Includes */
#include "HipoChain.h"
#include "clas12reader.h"

using namespace std;

class FiducialCuts{
 public:
  FiducialCuts();

  void setTorusBending(int);
  bool FidCutParticle(const std::unique_ptr<clas12::clas12reader>&, int, SIDISParticlev1*);

  bool DC_fiducial_cut_theta_phi(float,float,float,int, int, int);

  bool DC_fiducial_cut_XY(float,float,float,int, int, int);
  
  bool CheckSampFrac(float);
 protected:
  float _x[3]={0,0,0};
  float _y[3]={0,0,0};
  float _z[3]={0,0,0};

  float Ele_PCAL_e, Ele_ECIN_e, Ele_ECOUT_e;
  int Ele_sector;
  float lu, lv, lw;

  int _torusBending=0;
  int _runNumber=0;
};
#endif

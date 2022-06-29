#include "HipoBankInterface.h"

HipoBankInterface::HipoBankInterface(){}

HipoBankInterface::HipoBankInterface(const std::unique_ptr<clas12::clas12reader>& _c12){
  
  // Add REC::Cal info
  _idx_RECCal = _c12->addBank("REC::Calorimeter");
  _ipindex_RECCal = _c12->getBankOrder(_idx_RECCal,"pindex");
  _ilu_RECCal = _c12->getBankOrder(_idx_RECCal,"lu");
  _ilv_RECCal = _c12->getBankOrder(_idx_RECCal,"lv");
  _ilw_RECCal = _c12->getBankOrder(_idx_RECCal,"lw");
  _ilayer_RECCal = _c12->getBankOrder(_idx_RECCal,"layer");
  _isector_RECCal = _c12->getBankOrder(_idx_RECCal,"sector");
  _itime_RECCal = _c12->getBankOrder(_idx_RECCal,"time");
  _ipath_RECCal = _c12->getBankOrder(_idx_RECCal,"path");
  _ienergy_RECCal = _c12->getBankOrder(_idx_RECCal,"energy");

  // Add REC::Traj info
  _idx_RECTraj = _c12->addBank("REC::Traj");
  _ipindex_RECTraj = _c12->getBankOrder(_idx_RECTraj,"pindex");
  _ilayer_RECTraj = _c12->getBankOrder(_idx_RECTraj,"layer");
  _idet_RECTraj = _c12->getBankOrder(_idx_RECTraj,"detector");
  _ipath_RECTraj = _c12->getBankOrder(_idx_RECTraj,"path");
  _ix_RECTraj = _c12->getBankOrder(_idx_RECTraj,"x");
  _iy_RECTraj = _c12->getBankOrder(_idx_RECTraj,"y");
  _iz_RECTraj = _c12->getBankOrder(_idx_RECTraj,"z");

}

bool HipoBankInterface::loadBankData(const std::unique_ptr<clas12::clas12reader>& _c12 , SIDISParticlev1 *sp){
  this.clear();
  
  // Grab necessary particle info
  // -------------------------------------------------------------
  int pid = sp->get_property_int(SIDISParticle::part_pid);
  int pindex = sp->get_property_int(SIDISParticle::part_pindex);

  // -------------------------------------------------------------
  // Parse the REC::Calorimeter
  // -------------------------------------------------------------

  for(auto i = 0 ; i < _c12->getBank(_idx_RECCal)->getRows() ; i++){
    // Continue loop if the pindex in the calo bank does not match
    if(_c12->getBank(_idx_RECCal)->getInt(_ipindex_RECCal,i)!=pindex)
      continue;
    
    int sectorCal = _c12->getBank(_idx_RECCal)->getFloat(_isector_RECCal,i);
    float timeCal = _c12->getBank(_idx_RECCal)->getFloat(_itime_RECCal,i);
    float pathCal = _c12->getBank(_idx_RECCal)->getFloat(_ipath_RECCal,i);
    float lu = _c12->getBank(_idx_RECCal)->getFloat(_ilu_RECCal,i);
    float lv = _c12->getBank(_idx_RECCal)->getFloat(_ilv_RECCal,i);
    float lw = _c12->getBank(_idx_RECCal)->getFloat(_ilw_RECCal,i);
    int layerCal = _c12->getBank(_idx_RECCal)->getInt(_ilayer_RECCal,i);
    int calidx = -1; //Array index for lu, lv, lw

    switch(layerCal){
    case 1: //PCal
      calidx = 0;
      _Ele_PCAL_e = c12->getBank(_idx_RECCal)->getFloat(_ienergy_RECCal,i);
      break;
    case 4: //ECIN
      calidx = 1;
      _Ele_ECIN_e = c12->getBank(_idx_RECCal)->getFloat(_ienergy_RECCal,i);
      break;
    case 7: //ECOUT
      calidx = 2;
      _Ele_ECOUT_e = c12->getBank(_idx_RECCal)->getFloat(_ienergy_RECCal,i);
      break;   
    }
    
    // If there was a pindex attached to one of the calo layers...
    if(calidx!=-1){
      _lu_Cal[calidx]=lu;
      _lv_Cal[calidx]=lv;
      _lw_Cal[calidx]=lw;

      _sector_Cal[calidx]=sectorCal;
      _time_Cal[calidx]=timeCal;
      _path_Cal[calidx]=pathCal;
    }
  }


  // -------------------------------------------------------------
  // Parse the REC::Trajectory
  // -------------------------------------------------------------

  for(auto i = 0 ; i < _c12->getBank(_idx_RECTraj)->getRows() ; i++){
    // Continue loop if the pindex in the traj bank does not match
    if(_c12->getBank(_idx_RECTraj)->getInt(_ipindex_RECTraj,i)!=pindex)
      continue;
    
    if(_c12->getBank(_idx_RECTraj)->getInt(_ilayer_RECTraj,i)==6){
      _det_DC[0] = _c12->getBank(_idx_RECTraj)->getInt(_idet_RECTraj,i);
      _path_DC[0] = _c12->getBank(_idx_RECTraj)->getInt(_ipath_RECTraj,i);
      _x_DC[0] = _c12->getBank(_idx_RECTraj)->getFloat(_ix_RECTraj,i);
      _y_DC[0] = _c12->getBank(_idx_RECTraj)->getFloat(_iy_RECTraj,i);
      _z_DC[0] = _c12->getBank(_idx_RECTraj)->getFloat(_iz_RECTraj,i);
    }else if(_c12->getBank(_idx_RECTraj)->getInt(_ilayer_Traj,i)==18){
      _det_DC[1] = _c12->getBank(_idx_RECTraj)->getInt(_idet_RECTraj,i);
      _path_DC[1] = _c12->getBank(_idx_RECTraj)->getInt(_ipath_RECTraj,i);
      _x_DC[1] = _c12->getBank(_idx_RECTraj)->getFloat(_ix_RECTraj,i);
      _y_DC[1] = _c12->getBank(_idx_RECTraj)->getFloat(_iy_RECTraj,i);
      _z_DC[1] = _c12->getBank(_idx_RECTraj)->getFloat(_iz_RECTraj,i);
    }else if(_c12->getBank(_idx_RECTraj)->getInt(_ilayer_Traj,i)==36){
      _det_DC[2] = _c12->getBank(_idx_RECTraj)->getInt(_idet_RECTraj,i);
      _path_DC[2] = _c12->getBank(_idx_RECTraj)->getInt(_ipath_RECTraj,i);
      _x_DC[2] = _c12->getBank(_idx_RECTraj)->getFloat(_ix_RECTraj,i);
      _y_DC[2] = _c12->getBank(_idx_RECTraj)->getFloat(_iy_RECTraj,i);
      _z_DC[2] = _c12->getBank(_idx_RECTraj)->getFloat(_iz_RECTraj,i);
    }
  }
  
  // Get the azimuthal sector # from the middle drift chamber
  int _sector_DC = determineSectorDC(_x_DC[1], _y_DC[1], _z_DC[1]);

  this.importDataToParticle(sp);
  
  return true;
}

bool importDataToParticle(SIDISParticlev1 *sp)
{
  
  // -------------------------------------------------------------
  // Import the REC::Calorimeter data
  // -------------------------------------------------------------
  sp->set_property(SIDISParticle::cal_sector_PCAL, _sector_Cal[0]);
  sp->set_property(SIDISParticle::cal_sector_ECIN, _sector_Cal[1]);
  sp->set_property(SIDISParticle::cal_sector_ECOUT, _sector_Cal[2]);

  sp->set_property(SIDISParticle::cal_time_PCAL, _time_Cal[0]);
  sp->set_property(SIDISParticle::cal_time_ECIN, _time_Cal[1]);
  sp->set_property(SIDISParticle::cal_time_ECOUT, _time_Cal[2]);

  sp->set_property(SIDISParticle::cal_path_PCAL, _path_Cal[0]);
  sp->set_property(SIDISParticle::cal_path_ECIN, _path_Cal[1]);
  sp->set_property(SIDISParticle::cal_path_ECOUT, _path_Cal[2]);

  sp->set_property(SIDISParticle::cal_energy_PCAL, _Ele_PCAL_e);
  sp->set_property(SIDISParticle::cal_energy_ECIN, _Ele_ECIN_e);
  sp->set_property(SIDISParticle::cal_energy_ECOUT, _Ele_ECOUT_e);

  sp->set_property(SIDISParticle::cal_lu_PCAL, _lu_RECCal[0]);
  sp->set_property(SIDISParticle::cal_lu_ECIN, _lu_RECCal[1]);
  sp->set_property(SIDISParticle::cal_lu_ECOUT, _lu_RECCal[2]);

  sp->set_property(SIDISParticle::cal_lv_PCAL, _lv_RECCal[0]);
  sp->set_property(SIDISParticle::cal_lv_ECIN, _lv_RECCal[1]);
  sp->set_property(SIDISParticle::cal_lv_ECOUT, _lv_RECCal[2]);

  sp->set_property(SIDISParticle::cal_lw_PCAL, _lw_RECCal[0]);
  sp->set_property(SIDISParticle::cal_lw_ECIN, _lw_RECCal[1]);
  sp->set_property(SIDISParticle::cal_lw_ECOUT, _lw_RECCal[2]);

  // -------------------------------------------------------------
  // Import the REC::Trajectory data
  // -------------------------------------------------------------
  sp->set_property(SIDISParticle::traj_sector, _sector_DC);

  sp->set_property(SIDISParticle::traj_path1, _path_DC[0]);
  sp->set_property(SIDISParticle::traj_path2, _path_DC[1]);
  sp->set_property(SIDISParticle::traj_path3, _path_DC[2]);

  sp->set_property(SIDISParticle::traj_det1, _det_DC[0]);
  sp->set_property(SIDISParticle::traj_det2, _det_DC[1]);
  sp->set_property(SIDISParticle::traj_det3, _det_DC[2]);

  sp->set_property(SIDISParticle::traj_x1, _x_DC[0]);
  sp->set_property(SIDISParticle::traj_x2, _x_DC[1]);
  sp->set_property(SIDISParticle::traj_x3, _x_DC[2]);

  sp->set_property(SIDISParticle::traj_y1, _y_DC[0]);
  sp->set_property(SIDISParticle::traj_y2, _y_DC[1]);
  sp->set_property(SIDISParticle::traj_y3, _y_DC[2]);

  sp->set_property(SIDISParticle::traj_z1, _z_DC[0]);
  sp->set_property(SIDISParticle::traj_z2, _z_DC[1]);
  sp->set_property(SIDISParticle::traj_z3, _z_DC[2]);

  return true;
}
int HipoBankInterface::determineSectorDC(float x, float y, float z){
  float phi = 180 / PI * atan2(y / sqrt(pow(x,2) + pow(y,2) + pow(z,2)),
			       x /sqrt(pow(x,2) + pow(y,2) + pow(z,2)));
  if(phi<30 && phi>=-30){return 1;}
  else if(phi<90 && phi>=30){return 2;}
  else if(phi<150 && phi>=90){return 3;}
  else if(phi>=150 || phi<-150){return 4;}
  else if(phi<-90 && phi>=-150){return 5;}
  else if(phi<-30 && phi>-90){return 6;}

  return 0;

}


void HipoBankInterface::clear(){
  _Ele_PCAL_e = 0.0;
  _Ele_ECIN_e = 0.0;
  _Ele_ECOUT_e = 0.0;
  _sector_Cal={0,0,0};
  _time_Cal={0,0,0};
  _path_Cal={0,0,0};
  _lu_Cal={0,0,0};
  _lv_Cal={0,0,0};
  _lw_Cal={0,0,0};

  _sector_DC = -1;
  _det_DC={-1,-1,-1};
  _path_DC={0,0,0};
  _x_DC={0,0,0};
  _y_DC={0,0,0};
  _z_DC={0,0,0};
}

#ifndef SIDISParticle_h
#define SIDISParticle_h

#include <utility>
#include <iostream>
#include <climits>
class SIDISParticle
{
 public: 
  SIDISParticle(){}
  virtual ~SIDISParticle() {}
  
  // How to add new PROPERTY tag:
  // 1. Add new tag below with a unique value
  // 2. Go to SIDISParticle.C and write a short name to SIDISParticle::get_property_info
  enum PROPERTY
  {//
    
    // -- 1-10   Generic Information -- //
    part_nParticle   = 1,
    part_nPhoton   = 2,
    // -- 10-99 Particle Kinematics and Identification -- //
    part_pid = 10,
    part_px  = 11,
    part_py  = 12,
    part_pz  = 13,
    part_pt  = 14,
    part_p   = 15,
    part_E   = 16,
    part_theta = 18,
    part_eta   = 19,
    part_phi   = 20,
    part_vx   = 30,
    part_vy   = 31,
    part_vz   = 32,
    part_vt   = 33,

    part_pindex = 50,
    part_beta   = 51,
    part_chi2   = 52,
    part_status = 53,
    part_ID   = 54,
    part_parentID = 55,
    part_parentPID = 56,

    // -- 100-199 Trajectory Bank Information 
    traj_sector = 100,
    traj_path1 = 101,
    traj_path2 = 102,
    traj_path3 = 103,
    
    traj_det1 = 110,
    traj_det2 = 111,
    traj_det3 = 112,

    traj_x1 = 130,
    traj_x2 = 131,
    traj_x3 = 132,
    traj_y1 = 133,
    traj_y2 = 134,
    traj_y3 = 135,
    traj_z1 = 136,
    traj_z2 = 137,
    traj_z3 = 138,
    
    // -- 200-299 Calorimeter Bank Information
    cal_sector_PCAL = 200,
    cal_sector_ECIN = 201,
    cal_sector_ECOUT = 202,

    cal_energy_PCAL = 210,
    cal_energy_ECIN = 211,
    cal_energy_ECOUT = 212,

    cal_time_PCAL = 220,
    cal_time_ECIN = 221,
    cal_time_ECOUT = 222,

    cal_path_PCAL = 230,
    cal_path_ECIN = 231,
    cal_path_ECOUT = 232,

    cal_lu_PCAL = 240,
    cal_lu_ECIN = 241,
    cal_lu_ECOUT = 242,
    cal_lv_PCAL = 243,
    cal_lv_ECIN = 244,
    cal_lv_ECOUT = 245,
    cal_lw_PCAL = 246,
    cal_lw_ECIN = 247,
    cal_lw_ECOUT = 248,


    // ------------------ MONTE CARLO VERSION ----------------------- //
    // --  1000-1010   Generic Information -- //
    evtgen_part_nParticle   = 1001,
    evtgen_part_nPhoton   = 1002,
    // -- 1010-1099 Particle Kinematics and Identification -- //
    evtgen_part_pid = 1010,
    evtgen_part_px  = 1011,
    evtgen_part_py  = 1012,
    evtgen_part_pz  = 1013,
    evtgen_part_pt  = 1014,
    evtgen_part_p   = 1015,
    evtgen_part_E   = 1016,
    evtgen_part_theta = 1018,
    evtgen_part_eta   = 1019,
    evtgen_part_phi   = 1020,
    evtgen_part_vx   = 1030,
    evtgen_part_vy   = 1031,
    evtgen_part_vz   = 1032,
    evtgen_part_vt   = 1033,

    evtgen_part_pindex = 1050,
    evtgen_part_beta   = 1051,
    evtgen_part_chi2   = 1052,
    evtgen_part_status = 1053,
    evtgen_part_ID   = 1054,
    evtgen_part_parentID = 1055,
    evtgen_part_parentPID = 1056
  };

  enum PROPERTY_TYPE
  {//
    type_int = 1,
    type_float = 2
  };

  static std::pair<const std::string,PROPERTY_TYPE> get_property_info(PROPERTY prop_id);
  virtual float get_property_float(const PROPERTY prop_id) const {return -999.999;}
  virtual int   get_property_int(const PROPERTY prop_id) const {return INT_MIN;}
  virtual unsigned int   get_property_uint(const PROPERTY prop_id) const {return UINT_MAX;}

  virtual void  set_property(const PROPERTY prop_id, const float value) {return;}
  virtual void  set_property(const PROPERTY prop_id, const int value) {return;}
  virtual void  set_property(const PROPERTY prop_id, const unsigned int value) {return;}
  

};
#endif

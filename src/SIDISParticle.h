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
    // -- 10-100 Particle Kinematics and Identification -- //
    part_pid = 10,
    part_px  = 11,
    part_py  = 12,
    part_pz  = 13,
    part_pt  = 14,
    part_p   = 15,
    part_E   = 16,
    part_theta = 17,
    part_eta   = 18,
    part_phi   = 19,
    
    part_pindex = 50,
    part_beta   = 51,
    part_chi2   = 52
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

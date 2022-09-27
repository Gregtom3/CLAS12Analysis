#include "SIDISParticle.h"


std::pair<const std::string, SIDISParticle::PROPERTY_TYPE> SIDISParticle::get_property_info(const PROPERTY prop_id)
{
  switch(prop_id)
    {
      
      // ---------------

    case part_pid:
      return std::make_pair("pid", SIDISParticle::type_int);
    case part_px:
      return std::make_pair("px", SIDISParticle::type_float);
    case part_py:
      return std::make_pair("py", SIDISParticle::type_float);
    case part_pz:
      return std::make_pair("pz", SIDISParticle::type_float);
    case part_pt:
      return std::make_pair("pt", SIDISParticle::type_float);
    case part_p:
      return std::make_pair("p", SIDISParticle::type_float);
    case part_E:
      return std::make_pair("E", SIDISParticle::type_float);
    case part_theta:
      return std::make_pair("theta", SIDISParticle::type_float);
    case part_eta:
      return std::make_pair("eta", SIDISParticle::type_float);
    case part_phi:
      return std::make_pair("phi", SIDISParticle::type_float);
    case part_vx:
      return std::make_pair("vx", SIDISParticle::type_float);
    case part_vy:
      return std::make_pair("vy", SIDISParticle::type_float);
    case part_vz:
      return std::make_pair("vz", SIDISParticle::type_float);
    case part_vt:
      return std::make_pair("vt", SIDISParticle::type_float);
    case part_pindex:
      return std::make_pair("pindex", SIDISParticle::type_int);
    case part_beta:
      return std::make_pair("beta", SIDISParticle::type_float);
    case part_chi2:
      return std::make_pair("chi2", SIDISParticle::type_float);
    case part_status:
      return std::make_pair("status", SIDISParticle::type_int);
    case part_ID:
      return std::make_pair("ID", SIDISParticle::type_int);
    case part_parentID:
      return std::make_pair("parentID", SIDISParticle::type_int);
    case part_parentPID:
      return std::make_pair("parentPID", SIDISParticle::type_int);

      // ----------------
     
    case traj_sector:
      return std::make_pair("traj_sector",SIDISParticle::type_int);
    case traj_path1:
      return std::make_pair("traj_path1",SIDISParticle::type_float);
    case traj_path2:
      return std::make_pair("traj_path2",SIDISParticle::type_float);
    case traj_path3:
      return std::make_pair("traj_path3",SIDISParticle::type_float);
    case traj_det1:
      return std::make_pair("traj_det1",SIDISParticle::type_int);
    case traj_det2:
      return std::make_pair("traj_det2",SIDISParticle::type_int);
    case traj_det3:
      return std::make_pair("traj_det3",SIDISParticle::type_int);
    case traj_x1:
      return std::make_pair("traj_x1",SIDISParticle::type_float);
    case traj_x2:
      return std::make_pair("traj_x2",SIDISParticle::type_float);
    case traj_x3:
      return std::make_pair("traj_x3",SIDISParticle::type_float);
    case traj_y1:
      return std::make_pair("traj_y1",SIDISParticle::type_float);
    case traj_y2:
      return std::make_pair("traj_y2",SIDISParticle::type_float);
    case traj_y3:
      return std::make_pair("traj_y3",SIDISParticle::type_float);
    case traj_z1:
      return std::make_pair("traj_z1",SIDISParticle::type_float);
    case traj_z2:
      return std::make_pair("traj_z2",SIDISParticle::type_float);
    case traj_z3:
      return std::make_pair("traj_z3",SIDISParticle::type_float);
     
      // ----------------
      
    case cal_sector_PCAL:
      return std::make_pair("cal_sector_PCAL",SIDISParticle::type_int);
    case cal_sector_ECIN:
      return std::make_pair("cal_sector_ECIN",SIDISParticle::type_int);
    case cal_sector_ECOUT:
      return std::make_pair("cal_sector_ECOUT",SIDISParticle::type_int);
    case cal_energy_PCAL:
      return std::make_pair("cal_energy_PCAL",SIDISParticle::type_float);
    case cal_energy_ECIN:
      return std::make_pair("cal_energy_ECIN",SIDISParticle::type_float);
    case cal_energy_ECOUT:
      return std::make_pair("cal_energy_ECOUT",SIDISParticle::type_float);
    case cal_time_PCAL:
      return std::make_pair("cal_time_PCAL",SIDISParticle::type_float);
    case cal_time_ECIN:
      return std::make_pair("cal_time_ECIN",SIDISParticle::type_float);
    case cal_time_ECOUT:
      return std::make_pair("cal_time_ECOUT",SIDISParticle::type_float);
    case cal_path_PCAL:
      return std::make_pair("cal_path_PCAL",SIDISParticle::type_float);
    case cal_path_ECIN:
      return std::make_pair("cal_path_ECIN",SIDISParticle::type_float);
    case cal_path_ECOUT:
      return std::make_pair("cal_path_ECOUT",SIDISParticle::type_float);
    case cal_lu_PCAL:
      return std::make_pair("cal_lu_PCAL",SIDISParticle::type_float);
    case cal_lu_ECIN:
      return std::make_pair("cal_lu_ECIN",SIDISParticle::type_float);
    case cal_lu_ECOUT:
      return std::make_pair("cal_lu_ECOUT",SIDISParticle::type_float);
    case cal_lv_PCAL:
      return std::make_pair("cal_lv_PCAL",SIDISParticle::type_float);
    case cal_lv_ECIN:
      return std::make_pair("cal_lv_ECIN",SIDISParticle::type_float);
    case cal_lv_ECOUT:
      return std::make_pair("cal_lv_ECOUT",SIDISParticle::type_float);
    case cal_lw_PCAL:
      return std::make_pair("cal_lw_PCAL",SIDISParticle::type_float);
    case cal_lw_ECIN:
      return std::make_pair("cal_lw_ECIN",SIDISParticle::type_float);
    case cal_lw_ECOUT:
      return std::make_pair("cal_lw_ECOUT",SIDISParticle::type_float);



      // ------------------ MONTE CARLO VERSION --------------------- //

    case evtgen_part_pid:
      return std::make_pair("evtgen_pid", SIDISParticle::type_int);
    case evtgen_part_px:
      return std::make_pair("evtgen_px", SIDISParticle::type_float);
    case evtgen_part_py:
      return std::make_pair("evtgen_py", SIDISParticle::type_float);
    case evtgen_part_pz:
      return std::make_pair("evtgen_pz", SIDISParticle::type_float);
    case evtgen_part_pt:
      return std::make_pair("evtgen_pt", SIDISParticle::type_float);
    case evtgen_part_p:
      return std::make_pair("evtgen_p", SIDISParticle::type_float);
    case evtgen_part_E:
      return std::make_pair("evtgen_E", SIDISParticle::type_float);
    case evtgen_part_theta:
      return std::make_pair("evtgen_theta", SIDISParticle::type_float);
    case evtgen_part_eta:
      return std::make_pair("evtgen_eta", SIDISParticle::type_float);
    case evtgen_part_phi:
      return std::make_pair("evtgen_phi", SIDISParticle::type_float);
    case evtgen_part_vx:
      return std::make_pair("evtgen_vx", SIDISParticle::type_float);
    case evtgen_part_vy:
      return std::make_pair("evtgen_vy", SIDISParticle::type_float);
    case evtgen_part_vz:
      return std::make_pair("evtgen_vz", SIDISParticle::type_float);
    case evtgen_part_vt:
      return std::make_pair("evtgen_vt", SIDISParticle::type_float);
    case evtgen_part_pindex:
      return std::make_pair("evtgen_pindex", SIDISParticle::type_int);
    case evtgen_part_beta:
      return std::make_pair("evtgen_beta", SIDISParticle::type_float);
    case evtgen_part_chi2:
      return std::make_pair("evtgen_chi2", SIDISParticle::type_float);
    case evtgen_part_status:
      return std::make_pair("evtgen_status", SIDISParticle::type_int);
    case evtgen_part_ID:
      return std::make_pair("evtgen_ID", SIDISParticle::type_int);
    case evtgen_part_parentID:
      return std::make_pair("evtgen_parentID", SIDISParticle::type_int);
    case evtgen_part_parentPID:
      return std::make_pair("evtgen_parentPID", SIDISParticle::type_int);
    case evtgen_part_parentparentPID:
      return std::make_pair("evtgen_parentparentPID", SIDISParticle::type_int);

    default:
      std::cout << "SIDISParticle::get_property_info - Fatal Error - unknown prop_id " << prop_id << std::endl;
      return std::make_pair("-1", SIDISParticle::type_int);
    }
}



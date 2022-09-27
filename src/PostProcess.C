#include "PostProcess.h"
PostProcess::PostProcess(){}
int PostProcess::Init(TTree * tree_Reco, double eBeam)
{
  _electron_beam_energy = eBeam;
  _tree_Reco = tree_Reco;
  _tree_Reco->SetBranchAddress("event_truth", &event_truth, &b_event);
  _tree_Reco->SetBranchAddress("Mx", &Mx, &b_Mx);
  _tree_Reco->SetBranchAddress("Q2", &Q2, &b_Q2);
  _tree_Reco->SetBranchAddress("trueQ2", &trueQ2, &b_trueQ2);
  _tree_Reco->SetBranchAddress("W", &W, &b_W);
  _tree_Reco->SetBranchAddress("trueW", &trueW, &b_trueW);
  _tree_Reco->SetBranchAddress("evnum", &evnum, &b_evnum);
  _tree_Reco->SetBranchAddress("helicity", &helicity, &b_helicity);
  _tree_Reco->SetBranchAddress("nParticles", &nParticles, &b_nParticles);
  _tree_Reco->SetBranchAddress("nu", &nu, &b_nu);
  _tree_Reco->SetBranchAddress("truenu", &truenu, &b_truenu);
  _tree_Reco->SetBranchAddress("polarization", &polarization, &b_polarization);
  _tree_Reco->SetBranchAddress("run", &run, &b_run);
  _tree_Reco->SetBranchAddress("x", &x, &b_x);
  _tree_Reco->SetBranchAddress("truex", &truex, &b_truex);
  _tree_Reco->SetBranchAddress("y", &y, &b_y);
  _tree_Reco->SetBranchAddress("truey", &truey, &b_truey);
  _tree_Reco->SetBranchAddress("evtgen_parentPID", &evtgen_parentPID, &b_evtgen_parentPID);
  _tree_Reco->SetBranchAddress("evtgen_parentparentPID", &evtgen_parentparentPID, &b_evtgen_parentparentPID);
  _tree_Reco->SetBranchAddress("evtgen_parentID", &evtgen_parentID, &b_evtgen_parentID);
  _tree_Reco->SetBranchAddress("evtgen_status", &evtgen_status, &b_evtgen_status);
  _tree_Reco->SetBranchAddress("evtgen_chi2", &evtgen_chi2, &b_evtgen_chi2);
  _tree_Reco->SetBranchAddress("evtgen_beta", &evtgen_beta, &b_evtgen_beta);
  _tree_Reco->SetBranchAddress("evtgen_pindex", &evtgen_pindex, &b_evtgen_pindex);
  _tree_Reco->SetBranchAddress("evtgen_vt", &evtgen_vt, &b_evtgen_vt);
  _tree_Reco->SetBranchAddress("evtgen_vz", &evtgen_vz, &b_evtgen_vz);
  _tree_Reco->SetBranchAddress("evtgen_vy", &evtgen_vy, &b_evtgen_vy);
  _tree_Reco->SetBranchAddress("evtgen_vx", &evtgen_vx, &b_evtgen_vx);
  _tree_Reco->SetBranchAddress("evtgen_phi", &evtgen_phi, &b_evtgen_phi);
  _tree_Reco->SetBranchAddress("evtgen_eta", &evtgen_eta, &b_evtgen_eta);
  _tree_Reco->SetBranchAddress("evtgen_theta", &evtgen_theta, &b_evtgen_theta);
  _tree_Reco->SetBranchAddress("evtgen_E", &evtgen_E, &b_evtgen_E);
  _tree_Reco->SetBranchAddress("evtgen_p", &evtgen_p, &b_evtgen_p);
  _tree_Reco->SetBranchAddress("evtgen_pt", &evtgen_pt, &b_evtgen_pt);
  _tree_Reco->SetBranchAddress("evtgen_pz", &evtgen_pz, &b_evtgen_pz);
  _tree_Reco->SetBranchAddress("evtgen_py", &evtgen_py, &b_evtgen_py);
  _tree_Reco->SetBranchAddress("evtgen_px", &evtgen_px, &b_evtgen_px);
  _tree_Reco->SetBranchAddress("evtgen_pid", &evtgen_pid, &b_evtgen_pid);
  _tree_Reco->SetBranchAddress("pid", &pid, &b_pid);
  _tree_Reco->SetBranchAddress("px", &px, &b_px);
  _tree_Reco->SetBranchAddress("py", &py, &b_py);
  _tree_Reco->SetBranchAddress("pz", &pz, &b_pz);
  _tree_Reco->SetBranchAddress("pt", &pt, &b_pt);
  _tree_Reco->SetBranchAddress("p", &p, &b_p);
  _tree_Reco->SetBranchAddress("E", &E, &b_E);
  _tree_Reco->SetBranchAddress("theta", &theta, &b_theta);
  _tree_Reco->SetBranchAddress("eta", &eta, &b_eta);
  _tree_Reco->SetBranchAddress("phi", &phi, &b_phi);
  _tree_Reco->SetBranchAddress("vx", &vx, &b_vx);
  _tree_Reco->SetBranchAddress("vy", &vy, &b_vy);
  _tree_Reco->SetBranchAddress("vz", &vz, &b_vz);
  _tree_Reco->SetBranchAddress("vt", &vt, &b_vt);
  _tree_Reco->SetBranchAddress("pindex", &pindex, &b_pindex);
  _tree_Reco->SetBranchAddress("beta", &beta, &b_beta);
  _tree_Reco->SetBranchAddress("chi2", &chi2, &b_chi2);
  _tree_Reco->SetBranchAddress("status", &status, &b_status);
  _tree_Reco->SetBranchAddress("parentID", &parentID, &b_parentID);
  _tree_Reco->SetBranchAddress("parentPID", &parentPID, &b_parentPID);
  _tree_Reco->SetBranchAddress("traj_sector", &traj_sector, &b_traj_sector);
  _tree_Reco->SetBranchAddress("traj_path1", &traj_path1, &b_traj_path1);
  _tree_Reco->SetBranchAddress("traj_path2", &traj_path2, &b_traj_path2);
  _tree_Reco->SetBranchAddress("traj_path3", &traj_path3, &b_traj_path3);
  _tree_Reco->SetBranchAddress("traj_det1", &traj_det1, &b_traj_det1);
  _tree_Reco->SetBranchAddress("traj_det2", &traj_det2, &b_traj_det2);
  _tree_Reco->SetBranchAddress("traj_det3", &traj_det3, &b_traj_det3);
  _tree_Reco->SetBranchAddress("traj_x1", &traj_x1, &b_traj_x1);
  _tree_Reco->SetBranchAddress("traj_x2", &traj_x2, &b_traj_x2);
  _tree_Reco->SetBranchAddress("traj_x3", &traj_x3, &b_traj_x3);
  _tree_Reco->SetBranchAddress("traj_y1", &traj_y1, &b_traj_y1);
  _tree_Reco->SetBranchAddress("traj_y2", &traj_y2, &b_traj_y2);
  _tree_Reco->SetBranchAddress("traj_y3", &traj_y3, &b_traj_y3);
  _tree_Reco->SetBranchAddress("traj_z1", &traj_z1, &b_traj_z1);
  _tree_Reco->SetBranchAddress("traj_z2", &traj_z2, &b_traj_z2);
  _tree_Reco->SetBranchAddress("traj_z3", &traj_z3, &b_traj_z3);
  _tree_Reco->SetBranchAddress("cal_sector_PCAL", &cal_sector_PCAL, &b_cal_sector_PCAL);
  _tree_Reco->SetBranchAddress("cal_sector_ECIN", &cal_sector_ECIN, &b_cal_sector_ECIN);
  _tree_Reco->SetBranchAddress("cal_sector_ECOUT", &cal_sector_ECOUT, &b_cal_sector_ECOUT);
  _tree_Reco->SetBranchAddress("cal_energy_PCAL", &cal_energy_PCAL, &b_cal_energy_PCAL);
  _tree_Reco->SetBranchAddress("cal_energy_ECIN", &cal_energy_ECIN, &b_cal_energy_ECIN);
  _tree_Reco->SetBranchAddress("cal_energy_ECOUT", &cal_energy_ECOUT, &b_cal_energy_ECOUT);
  _tree_Reco->SetBranchAddress("cal_time_PCAL", &cal_time_PCAL, &b_cal_time_PCAL);
  _tree_Reco->SetBranchAddress("cal_time_ECIN", &cal_time_ECIN, &b_cal_time_ECIN);
  _tree_Reco->SetBranchAddress("cal_time_ECOUT", &cal_time_ECOUT, &b_cal_time_ECOUT);
  _tree_Reco->SetBranchAddress("cal_path_PCAL", &cal_path_PCAL, &b_cal_path_PCAL);
  _tree_Reco->SetBranchAddress("cal_path_ECIN", &cal_path_ECIN, &b_cal_path_ECIN);
  _tree_Reco->SetBranchAddress("cal_path_ECOUT", &cal_path_ECOUT, &b_cal_path_ECOUT);
  _tree_Reco->SetBranchAddress("cal_lu_PCAL", &cal_lu_PCAL, &b_cal_lu_PCAL);
  _tree_Reco->SetBranchAddress("cal_lu_ECIN", &cal_lu_ECIN, &b_cal_lu_ECIN);
  _tree_Reco->SetBranchAddress("cal_lu_ECOUT", &cal_lu_ECOUT, &b_cal_lu_ECOUT);
  _tree_Reco->SetBranchAddress("cal_lv_PCAL", &cal_lv_PCAL, &b_cal_lv_PCAL);
  _tree_Reco->SetBranchAddress("cal_lv_ECIN", &cal_lv_ECIN, &b_cal_lv_ECIN);
  _tree_Reco->SetBranchAddress("cal_lv_ECOUT", &cal_lv_ECOUT, &b_cal_lv_ECOUT);
  _tree_Reco->SetBranchAddress("cal_lw_PCAL", &cal_lw_PCAL, &b_cal_lw_PCAL);
  _tree_Reco->SetBranchAddress("cal_lw_ECIN", &cal_lw_ECIN, &b_cal_lw_ECIN);
  _tree_Reco->SetBranchAddress("cal_lw_ECOUT", &cal_lw_ECOUT, &b_cal_lw_ECOUT);


  _nentries = _tree_Reco->GetEntriesFast();
  return 0;
}

int PostProcess::Process(TTree *_tree_postprocess){
  switch(_process_id){
  case PROCESS_ID::pipluspi0:
    pipi0(_tree_postprocess,0);
    break;
  case PROCESS_ID::piminuspi0:
    pipi0(_tree_postprocess,0);
    break;
  case PROCESS_ID::pipluspi0_MC:
    pipi0(_tree_postprocess,1);
    break;
  case PROCESS_ID::piminuspi0_MC:
    pipi0(_tree_postprocess,1);
    break;
  case PROCESS_ID::Inclusive:
    inclusive(_tree_postprocess);
    break;
  case PROCESS_ID::Elastic:
    elastic(_tree_postprocess);
    break;
  }
  return 0;
}

int PostProcess::pipi0(TTree * _tree_postprocess, int isMC){
  int fid = 0;
  double Mgg;
  double E1;
  double E2;
  double angle1;
  double angle2;
  double Mdihadron;
  double beta1;
  double beta2;
  double phi_R0;
  double phi_R1;
  double phi_h;
  double th;
  double zpair = 0.0;


  double truephi_R0;
  double truephi_R1;
  double truephi_h;
  double trueth;
  double trueMgg;
  double trueMdihadron;
  double truezpair = 0.0;
  int flag;
  
  
  double px1;
  double py1;
  double pz1;
  double pt1;
  double p1;
  double theta1;
  double phi1;
  double vx1;
  double vy1;
  double vz1;
  double status1;
  double parentPID1;
  double parentparentPID1;
  double PID1;
  double traj_sector1;
  double traj_path1_1;
  double traj_path2_1;
  double traj_path3_1;
  double traj_det1_1;
  double traj_det2_1;
  double traj_det3_1;
  double traj_x1_1;
  double traj_x2_1;
  double traj_x3_1;
  double traj_y1_1;
  double traj_y2_1;
  double traj_y3_1;
  double traj_z1_1;
  double traj_z2_1;
  double traj_z3_1;
  double cal_sector_PCAL_1;
  double cal_sector_ECIN_1;
  double cal_sector_ECOUT_1;
  double cal_energy_PCAL_1;
  double cal_energy_ECIN_1;
  double cal_energy_ECOUT_1;
  double cal_time_PCAL_1;
  double cal_time_ECIN_1;
  double cal_time_ECOUT_1;
  double cal_path_PCAL_1;
  double cal_path_ECIN_1;
  double cal_path_ECOUT_1;
  double cal_lu_PCAL_1;
  double cal_lu_ECIN_1;
  double cal_lu_ECOUT_1;
  double cal_lv_PCAL_1;
  double cal_lv_ECIN_1;
  double cal_lv_ECOUT_1;
  double cal_lw_PCAL_1;
  double cal_lw_ECIN_1;
  double cal_lw_ECOUT_1;

  double px2;
  double py2;
  double pz2;
  double pt2;
  double p2;
  double theta2;
  double phi2;
  double vx2;
  double vy2;
  double vz2;
  double status2;
  double parentPID2;
  double parentparentPID2;
  double PID2;
  double traj_sector2;
  double traj_path1_2;
  double traj_path2_2;
  double traj_path3_2;
  double traj_det1_2;
  double traj_det2_2;
  double traj_det3_2;
  double traj_x1_2;
  double traj_x2_2;
  double traj_x3_2;
  double traj_y1_2;
  double traj_y2_2;
  double traj_y3_2;
  double traj_z1_2;
  double traj_z2_2;
  double traj_z3_2;
  double cal_sector_PCAL_2;
  double cal_sector_ECIN_2;
  double cal_sector_ECOUT_2;
  double cal_energy_PCAL_2;
  double cal_energy_ECIN_2;
  double cal_energy_ECOUT_2;
  double cal_time_PCAL_2;
  double cal_time_ECIN_2;
  double cal_time_ECOUT_2;
  double cal_path_PCAL_2;
  double cal_path_ECIN_2;
  double cal_path_ECOUT_2;
  double cal_lu_PCAL_2;
  double cal_lu_ECIN_2;
  double cal_lu_ECOUT_2;
  double cal_lv_PCAL_2;
  double cal_lv_ECIN_2;
  double cal_lv_ECOUT_2;
  double cal_lw_PCAL_2;
  double cal_lw_ECIN_2;
  double cal_lw_ECOUT_2;

  double pxpi;
  double pypi;
  double pzpi;
  double ptpi;
  double ppi;
  double thetapi;
  double phipi;
  double vxpi;
  double vypi;
  double vzpi;
  double statuspi;
  double parentPIDpi;
  double parentparentPIDpi;
  double PIDpi;
  double traj_sectorpi;
  double traj_path1_pi;
  double traj_path2_pi;
  double traj_path3_pi;
  double traj_det1_pi;
  double traj_det2_pi;
  double traj_det3_pi;
  double traj_x1_pi;
  double traj_x2_pi;
  double traj_x3_pi;
  double traj_y1_pi;
  double traj_y2_pi;
  double traj_y3_pi;
  double traj_z1_pi;
  double traj_z2_pi;
  double traj_z3_pi;
  double cal_sector_PCAL_pi;
  double cal_sector_ECIN_pi;
  double cal_sector_ECOUT_pi;
  double cal_energy_PCAL_pi;
  double cal_energy_ECIN_pi;
  double cal_energy_ECOUT_pi;
  double cal_time_PCAL_pi;
  double cal_time_ECIN_pi;
  double cal_time_ECOUT_pi;
  double cal_path_PCAL_pi;
  double cal_path_ECIN_pi;
  double cal_path_ECOUT_pi;
  double cal_lu_PCAL_pi;
  double cal_lu_ECIN_pi;
  double cal_lu_ECOUT_pi;
  double cal_lv_PCAL_pi;
  double cal_lv_ECIN_pi;
  double cal_lv_ECOUT_pi;
  double cal_lw_PCAL_pi;
  double cal_lw_ECIN_pi;
  double cal_lw_ECOUT_pi;

  
  _tree_postprocess->Branch("fid",&fid);
  _tree_postprocess->Branch("Mdiphoton",&Mgg);
  _tree_postprocess->Branch("trueMdiphoton",&trueMgg);
  _tree_postprocess->Branch("E1",&E1);
  _tree_postprocess->Branch("E2",&E2);
  _tree_postprocess->Branch("angle1",&angle1);
  _tree_postprocess->Branch("angle2",&angle2);
  _tree_postprocess->Branch("Mdihadron",&Mdihadron);
  _tree_postprocess->Branch("trueMdihadron",&trueMdihadron);
  _tree_postprocess->Branch("beta1",&beta1);
  _tree_postprocess->Branch("beta2",&beta2);
  _tree_postprocess->Branch("helicity",&helicity);
  _tree_postprocess->Branch("phi_R0",&phi_R0);
  _tree_postprocess->Branch("phi_R1",&phi_R1);
  _tree_postprocess->Branch("phi_h",&phi_h);
  _tree_postprocess->Branch("truephi_R0",&truephi_R0);
  _tree_postprocess->Branch("truephi_R1",&truephi_R1);
  _tree_postprocess->Branch("truephi_h",&truephi_h);
  _tree_postprocess->Branch("th",&th);
  _tree_postprocess->Branch("trueth",&trueth);
  _tree_postprocess->Branch("Q2",&Q2);
  _tree_postprocess->Branch("W",&W);
  _tree_postprocess->Branch("nu",&nu);
  _tree_postprocess->Branch("x",&x);
  _tree_postprocess->Branch("y",&y);
  _tree_postprocess->Branch("z",&zpair);

  _tree_postprocess->Branch("trueQ2",&trueQ2);
  _tree_postprocess->Branch("trueW",&trueW);
  _tree_postprocess->Branch("truenu",&truenu);
  _tree_postprocess->Branch("truex",&truex);
  _tree_postprocess->Branch("truey",&truey);
  _tree_postprocess->Branch("truez",&truezpair);

  _tree_postprocess->Branch("polarization",&polarization);
  _tree_postprocess->Branch("flag",&flag); 

  _tree_postprocess->Branch("px1",&px1);
  _tree_postprocess->Branch("py1",&py1);
  _tree_postprocess->Branch("pz1",&pz1);
  _tree_postprocess->Branch("pt1",&pt1);
  _tree_postprocess->Branch("p1",&p1);
  _tree_postprocess->Branch("theta1",&theta1);
  _tree_postprocess->Branch("phi1",&phi1);
  _tree_postprocess->Branch("vx1",&vx1);
  _tree_postprocess->Branch("vy1",&vy1);
  _tree_postprocess->Branch("vz1",&vz1);
  _tree_postprocess->Branch("status1",&status1);
  _tree_postprocess->Branch("PID1",&PID1);
  _tree_postprocess->Branch("parentPID1",&parentPID1);
  _tree_postprocess->Branch("parentparentPID1",&parentparentPID1);
  _tree_postprocess->Branch("traj_sector1",&traj_sector1);
  _tree_postprocess->Branch("traj_path1_1",&traj_path1_1);
  _tree_postprocess->Branch("traj_path2_1",&traj_path2_1);
  _tree_postprocess->Branch("traj_path3_1",&traj_path3_1);
  _tree_postprocess->Branch("traj_det1_1",&traj_det1_1);
  _tree_postprocess->Branch("traj_det2_1",&traj_det2_1);
  _tree_postprocess->Branch("traj_det3_1",&traj_det3_1);
  _tree_postprocess->Branch("traj_x1_1",&traj_x1_1);
  _tree_postprocess->Branch("traj_x2_1",&traj_x2_1);
  _tree_postprocess->Branch("traj_x3_1",&traj_x3_1);
  _tree_postprocess->Branch("traj_y1_1",&traj_y1_1);
  _tree_postprocess->Branch("traj_y2_1",&traj_y2_1);
  _tree_postprocess->Branch("traj_y3_1",&traj_y3_1);
  _tree_postprocess->Branch("traj_z1_1",&traj_z1_1);
  _tree_postprocess->Branch("traj_z2_1",&traj_z2_1);
  _tree_postprocess->Branch("traj_z3_1",&traj_z3_1);
  _tree_postprocess->Branch("cal_sector_PCAL_1",&cal_sector_PCAL_1);
  _tree_postprocess->Branch("cal_sector_ECIN_1",&cal_sector_ECIN_1);
  _tree_postprocess->Branch("cal_sector_ECOUT_1",&cal_sector_ECOUT_1);
  _tree_postprocess->Branch("cal_energy_PCAL_1",&cal_energy_PCAL_1);
  _tree_postprocess->Branch("cal_energy_ECIN_1",&cal_energy_ECIN_1);
  _tree_postprocess->Branch("cal_energy_ECOUT_1",&cal_energy_ECOUT_1);
  _tree_postprocess->Branch("cal_time_PCAL_1",&cal_time_PCAL_1);
  _tree_postprocess->Branch("cal_time_ECIN_1",&cal_time_ECIN_1);
  _tree_postprocess->Branch("cal_time_ECOUT_1",&cal_time_ECOUT_1);
  _tree_postprocess->Branch("cal_path_PCAL_1",&cal_path_PCAL_1);
  _tree_postprocess->Branch("cal_path_ECIN_1",&cal_path_ECIN_1);
  _tree_postprocess->Branch("cal_path_ECOUT_1",&cal_path_ECOUT_1);
  _tree_postprocess->Branch("cal_lu_PCAL_1",&cal_lu_PCAL_1);
  _tree_postprocess->Branch("cal_lu_ECIN_1",&cal_lu_ECIN_1);
  _tree_postprocess->Branch("cal_lu_ECOUT_1",&cal_lu_ECOUT_1);
  _tree_postprocess->Branch("cal_lv_PCAL_1",&cal_lv_PCAL_1);
  _tree_postprocess->Branch("cal_lv_ECIN_1",&cal_lv_ECIN_1);
  _tree_postprocess->Branch("cal_lv_ECOUT_1",&cal_lv_ECOUT_1);
  _tree_postprocess->Branch("cal_lw_PCAL_1",&cal_lw_PCAL_1);
  _tree_postprocess->Branch("cal_lw_ECIN_1",&cal_lw_ECIN_1);
  _tree_postprocess->Branch("cal_lw_ECOUT_1",&cal_lw_ECOUT_1);

  _tree_postprocess->Branch("px2",&px2);
  _tree_postprocess->Branch("py2",&py2);
  _tree_postprocess->Branch("pz2",&pz2);
  _tree_postprocess->Branch("pt2",&pt2);
  _tree_postprocess->Branch("p2",&p2);
  _tree_postprocess->Branch("theta2",&theta2);
  _tree_postprocess->Branch("phi2",&phi2);
  _tree_postprocess->Branch("vx2",&vx2);
  _tree_postprocess->Branch("vy2",&vy2);
  _tree_postprocess->Branch("vz2",&vz2);
  _tree_postprocess->Branch("status2",&status2);
  _tree_postprocess->Branch("PID2",&PID2);
  _tree_postprocess->Branch("parentPID2",&parentPID2);
  _tree_postprocess->Branch("parentparentPID2",&parentparentPID2);
  _tree_postprocess->Branch("traj_sector2",&traj_sector2);
  _tree_postprocess->Branch("traj_path1_2",&traj_path1_2);
  _tree_postprocess->Branch("traj_path2_2",&traj_path2_2);
  _tree_postprocess->Branch("traj_path3_2",&traj_path3_2);
  _tree_postprocess->Branch("traj_det1_2",&traj_det1_2);
  _tree_postprocess->Branch("traj_det2_2",&traj_det2_2);
  _tree_postprocess->Branch("traj_det3_2",&traj_det3_2);
  _tree_postprocess->Branch("traj_x1_2",&traj_x1_2);
  _tree_postprocess->Branch("traj_x2_2",&traj_x2_2);
  _tree_postprocess->Branch("traj_x3_2",&traj_x3_2);
  _tree_postprocess->Branch("traj_y1_2",&traj_y1_2);
  _tree_postprocess->Branch("traj_y2_2",&traj_y2_2);
  _tree_postprocess->Branch("traj_y3_2",&traj_y3_2);
  _tree_postprocess->Branch("traj_z1_2",&traj_z1_2);
  _tree_postprocess->Branch("traj_z2_2",&traj_z2_2);
  _tree_postprocess->Branch("traj_z3_2",&traj_z3_2);
  _tree_postprocess->Branch("cal_sector_PCAL_2",&cal_sector_PCAL_2);
  _tree_postprocess->Branch("cal_sector_ECIN_2",&cal_sector_ECIN_2);
  _tree_postprocess->Branch("cal_sector_ECOUT_2",&cal_sector_ECOUT_2);
  _tree_postprocess->Branch("cal_energy_PCAL_2",&cal_energy_PCAL_2);
  _tree_postprocess->Branch("cal_energy_ECIN_2",&cal_energy_ECIN_2);
  _tree_postprocess->Branch("cal_energy_ECOUT_2",&cal_energy_ECOUT_2);
  _tree_postprocess->Branch("cal_time_PCAL_2",&cal_time_PCAL_2);
  _tree_postprocess->Branch("cal_time_ECIN_2",&cal_time_ECIN_2);
  _tree_postprocess->Branch("cal_time_ECOUT_2",&cal_time_ECOUT_2);
  _tree_postprocess->Branch("cal_path_PCAL_2",&cal_path_PCAL_2);
  _tree_postprocess->Branch("cal_path_ECIN_2",&cal_path_ECIN_2);
  _tree_postprocess->Branch("cal_path_ECOUT_2",&cal_path_ECOUT_2);
  _tree_postprocess->Branch("cal_lu_PCAL_2",&cal_lu_PCAL_2);
  _tree_postprocess->Branch("cal_lu_ECIN_2",&cal_lu_ECIN_2);
  _tree_postprocess->Branch("cal_lu_ECOUT_2",&cal_lu_ECOUT_2);
  _tree_postprocess->Branch("cal_lv_PCAL_2",&cal_lv_PCAL_2);
  _tree_postprocess->Branch("cal_lv_ECIN_2",&cal_lv_ECIN_2);
  _tree_postprocess->Branch("cal_lv_ECOUT_2",&cal_lv_ECOUT_2);
  _tree_postprocess->Branch("cal_lw_PCAL_2",&cal_lw_PCAL_2);
  _tree_postprocess->Branch("cal_lw_ECIN_2",&cal_lw_ECIN_2);
  _tree_postprocess->Branch("cal_lw_ECOUT_2",&cal_lw_ECOUT_2);

  _tree_postprocess->Branch("pxpi",&pxpi);
  _tree_postprocess->Branch("pypi",&pypi);
  _tree_postprocess->Branch("pzpi",&pzpi);
  _tree_postprocess->Branch("ptpi",&ptpi);
  _tree_postprocess->Branch("ppi",&ppi);
  _tree_postprocess->Branch("thetapi",&thetapi);
  _tree_postprocess->Branch("phipi",&phipi);
  _tree_postprocess->Branch("vxpi",&vxpi);
  _tree_postprocess->Branch("vypi",&vypi);
  _tree_postprocess->Branch("vzpi",&vzpi);
  _tree_postprocess->Branch("statuspi",&statuspi);
  _tree_postprocess->Branch("PIDpi",&PIDpi);
  _tree_postprocess->Branch("parentPIDpi",&parentPIDpi);
  _tree_postprocess->Branch("parentparentPIDpi",&parentparentPIDpi);
  _tree_postprocess->Branch("traj_sectorpi",&traj_sectorpi);
  _tree_postprocess->Branch("traj_path1_pi",&traj_path1_pi);
  _tree_postprocess->Branch("traj_path2_pi",&traj_path2_pi);
  _tree_postprocess->Branch("traj_path3_pi",&traj_path3_pi);
  _tree_postprocess->Branch("traj_det1_pi",&traj_det1_pi);
  _tree_postprocess->Branch("traj_det2_pi",&traj_det2_pi);
  _tree_postprocess->Branch("traj_det3_pi",&traj_det3_pi);
  _tree_postprocess->Branch("traj_x1_pi",&traj_x1_pi);
  _tree_postprocess->Branch("traj_x2_pi",&traj_x2_pi);
  _tree_postprocess->Branch("traj_x3_pi",&traj_x3_pi);
  _tree_postprocess->Branch("traj_y1_pi",&traj_y1_pi);
  _tree_postprocess->Branch("traj_y2_pi",&traj_y2_pi);
  _tree_postprocess->Branch("traj_y3_pi",&traj_y3_pi);
  _tree_postprocess->Branch("traj_z1_pi",&traj_z1_pi);
  _tree_postprocess->Branch("traj_z2_pi",&traj_z2_pi);
  _tree_postprocess->Branch("traj_z3_pi",&traj_z3_pi);
  _tree_postprocess->Branch("cal_sector_PCAL_pi",&cal_sector_PCAL_pi);
  _tree_postprocess->Branch("cal_sector_ECIN_pi",&cal_sector_ECIN_pi);
  _tree_postprocess->Branch("cal_sector_ECOUT_pi",&cal_sector_ECOUT_pi);
  _tree_postprocess->Branch("cal_energy_PCAL_pi",&cal_energy_PCAL_pi);
  _tree_postprocess->Branch("cal_energy_ECIN_pi",&cal_energy_ECIN_pi);
  _tree_postprocess->Branch("cal_energy_ECOUT_pi",&cal_energy_ECOUT_pi);
  _tree_postprocess->Branch("cal_time_PCAL_pi",&cal_time_PCAL_pi);
  _tree_postprocess->Branch("cal_time_ECIN_pi",&cal_time_ECIN_pi);
  _tree_postprocess->Branch("cal_time_ECOUT_pi",&cal_time_ECOUT_pi);
  _tree_postprocess->Branch("cal_path_PCAL_pi",&cal_path_PCAL_pi);
  _tree_postprocess->Branch("cal_path_ECIN_pi",&cal_path_ECIN_pi);
  _tree_postprocess->Branch("cal_path_ECOUT_pi",&cal_path_ECOUT_pi);
  _tree_postprocess->Branch("cal_lu_PCAL_pi",&cal_lu_PCAL_pi);
  _tree_postprocess->Branch("cal_lu_ECIN_pi",&cal_lu_ECIN_pi);
  _tree_postprocess->Branch("cal_lu_ECOUT_pi",&cal_lu_ECOUT_pi);
  _tree_postprocess->Branch("cal_lv_PCAL_pi",&cal_lv_PCAL_pi);
  _tree_postprocess->Branch("cal_lv_ECIN_pi",&cal_lv_ECIN_pi);
  _tree_postprocess->Branch("cal_lv_ECOUT_pi",&cal_lv_ECOUT_pi);
  _tree_postprocess->Branch("cal_lw_PCAL_pi",&cal_lw_PCAL_pi);
  _tree_postprocess->Branch("cal_lw_ECIN_pi",&cal_lw_ECIN_pi);
  _tree_postprocess->Branch("cal_lw_ECOUT_pi",&cal_lw_ECOUT_pi);


  TLorentzVector init_electron;
  init_electron.SetPxPyPzE(0,0,sqrt(_electron_beam_energy*_electron_beam_energy - electronMass * electronMass),_electron_beam_energy);
  TLorentzVector init_target;
  init_target.SetPxPyPzE(0,0,0,protonMass);

  TLorentzVector electron;
  TLorentzVector q;
  TLorentzVector pi;
  TLorentzVector pi0;
  TLorentzVector gamma1;
  TLorentzVector gamma2;
  TLorentzVector dihadron;

  TLorentzVector trueelectron;
  TLorentzVector trueq;
  TLorentzVector truepi;
  TLorentzVector truepi0;
  TLorentzVector truegamma1;
  TLorentzVector truegamma2;
  TLorentzVector truedihadron;

  double vz_electron = 0.0;
  double vz_pi = 0.0;
  double vz_pi0 = 0.0;

  double xF_pi = 0.0;
  double xF_pi0 = 0.0;

  double zpi = 0.0;
  double zpi0 = 0.0;

  double truezpi = 0.0;
  double truezpi0 = 0.0;

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {
    nb = _tree_Reco->GetEntry(jentry);   nbytes += nb;
    
    Mgg=0.0;
    trueMgg=0.0;
    E1=0.0;
    E2=0.0;
    Mdihadron=0.0;
    trueMdihadron=0.0;
    beta1=0.0;
    beta2=0.0;
    phi_h=0.0;
    phi_R0=0.0;
    phi_R1=0.0;
    th=0.0;
    zpair=0.0;

    truephi_h=0.0;
    truephi_R0=0.0;
    truephi_R1=0.0;
    trueth=0.0;
    truezpair=0.0;

    flag = 0;

    px1=0;
    py1=0;
    pz1=0;
    pt1=0;
    p1=0;
    theta1=0;
    phi1=0;
    vx1=0;
    vy1=0;
    vz1=0;
    status1=0;
    PID1=0;
    parentPID1=0;
    parentparentPID1=0;
    traj_sector1=0;
    traj_path1_1=0;
    traj_path2_1=0;
    traj_path3_1=0;
    traj_det1_1=0;
    traj_det2_1=0;
    traj_det3_1=0;
    traj_x1_1=0;
    traj_x2_1=0;
    traj_x3_1=0;
    traj_y1_1=0;
    traj_y2_1=0;
    traj_y3_1=0;
    traj_z1_1=0;
    traj_z2_1=0;
    traj_z3_1=0;
    cal_sector_PCAL_1=0;
    cal_sector_ECIN_1=0;
    cal_sector_ECOUT_1=0;
    cal_energy_PCAL_1=0;
    cal_energy_ECIN_1=0;
    cal_energy_ECOUT_1=0;
    cal_time_PCAL_1=0;
    cal_time_ECIN_1=0;
    cal_time_ECOUT_1=0;
    cal_path_PCAL_1=0;
    cal_path_ECIN_1=0;
    cal_path_ECOUT_1=0;
    cal_lu_PCAL_1=0;
    cal_lu_ECIN_1=0;
    cal_lu_ECOUT_1=0;
    cal_lv_PCAL_1=0;
    cal_lv_ECIN_1=0;
    cal_lv_ECOUT_1=0;
    cal_lw_PCAL_1=0;
    cal_lw_ECIN_1=0;
    cal_lw_ECOUT_1=0;

    px2=0;
    py2=0;
    pz2=0;
    pt2=0;
    p2=0;
    theta2=0;
    phi2=0;
    vx2=0;
    vy2=0;
    vz2=0;
    status2=0;
    PID2=0;
    parentPID2=0;
    parentparentPID2=0;
    traj_sector2=0;
    traj_path1_2=0;
    traj_path2_2=0;
    traj_path3_2=0;
    traj_det1_2=0;
    traj_det2_2=0;
    traj_det3_2=0;
    traj_x1_2=0;
    traj_x2_2=0;
    traj_x3_2=0;
    traj_y1_2=0;
    traj_y2_2=0;
    traj_y3_2=0;
    traj_z1_2=0;
    traj_z2_2=0;
    traj_z3_2=0;
    cal_sector_PCAL_2=0;
    cal_sector_ECIN_2=0;
    cal_sector_ECOUT_2=0;
    cal_energy_PCAL_2=0;
    cal_energy_ECIN_2=0;
    cal_energy_ECOUT_2=0;
    cal_time_PCAL_2=0;
    cal_time_ECIN_2=0;
    cal_time_ECOUT_2=0;
    cal_path_PCAL_2=0;
    cal_path_ECIN_2=0;
    cal_path_ECOUT_2=0;
    cal_lu_PCAL_2=0;
    cal_lu_ECIN_2=0;
    cal_lu_ECOUT_2=0;
    cal_lv_PCAL_2=0;
    cal_lv_ECIN_2=0;
    cal_lv_ECOUT_2=0;
    cal_lw_PCAL_2=0;
    cal_lw_ECIN_2=0;
    cal_lw_ECOUT_2=0;

    pxpi=0;
    pypi=0;
    pzpi=0;
    ptpi=0;
    ppi=0;
    thetapi=0;
    phipi=0;
    vxpi=0;
    vypi=0;
    vzpi=0;
    statuspi=0;
    PIDpi=0;
    parentPIDpi=0;
    parentparentPIDpi=0;
    traj_sectorpi=0;
    traj_path1_pi=0;
    traj_path2_pi=0;
    traj_path3_pi=0;
    traj_det1_pi=0;
    traj_det2_pi=0;
    traj_det3_pi=0;
    traj_x1_pi=0;
    traj_x2_pi=0;
    traj_x3_pi=0;
    traj_y1_pi=0;
    traj_y2_pi=0;
    traj_y3_pi=0;
    traj_z1_pi=0;
    traj_z2_pi=0;
    traj_z3_pi=0;
    cal_sector_PCAL_pi=0;
    cal_sector_ECIN_pi=0;
    cal_sector_ECOUT_pi=0;
    cal_energy_PCAL_pi=0;
    cal_energy_ECIN_pi=0;
    cal_energy_ECOUT_pi=0;
    cal_time_PCAL_pi=0;
    cal_time_ECIN_pi=0;
    cal_time_ECOUT_pi=0;
    cal_path_PCAL_pi=0;
    cal_path_ECIN_pi=0;
    cal_path_ECOUT_pi=0;
    cal_lu_PCAL_pi=0;
    cal_lu_ECIN_pi=0;
    cal_lu_ECOUT_pi=0;
    cal_lv_PCAL_pi=0;
    cal_lv_ECIN_pi=0;
    cal_lv_ECOUT_pi=0;
    cal_lw_PCAL_pi=0;
    cal_lw_ECIN_pi=0;
    cal_lw_ECOUT_pi=0;





    for(unsigned int i = 0 ; i < pid->size() ; i++){
      // Identify the scattered electron
      if(pid->at(i)==11){
	electron.SetPxPyPzE(px->at(i),py->at(i),pz->at(i),E->at(i));
	trueelectron.SetPxPyPzE(evtgen_px->at(i),evtgen_py->at(i),evtgen_pz->at(i),evtgen_E->at(i));
	vz_electron=vz->at(i);
      }
    }

   
    // Set virtual photon
    q = init_electron - electron;
    trueq = init_electron - trueelectron;
    
    // Next, identify pairs of photons
    for(unsigned int i = 0 ; i < pid->size() ; i++){
      if(pid->at(i)==22){
	for(unsigned int j = i+1 ; j < pid->size() ; j++){
	  if(pid->at(j)==22){
	    // Next, identify a pion
	    for(unsigned int k = 0 ; k < pid->size() ; k++){
	      if(pid->at(k)==_piPID){
		// Create pion
		pi.SetPxPyPzE(px->at(k),py->at(k),pz->at(k),E->at(k));
		truepi.SetPxPyPzE(evtgen_px->at(k),evtgen_py->at(k),evtgen_pz->at(k),evtgen_E->at(k));
		
		vz_pi=vz->at(k);      
		zpi = (init_target*pi)/(init_target*q);
		truezpi = (init_target*truepi)/(init_target*trueq);

		// Create gammas
		gamma1.SetPxPyPzE(px->at(i),py->at(i),pz->at(i),E->at(i));
		gamma2.SetPxPyPzE(px->at(j),py->at(j),pz->at(j),E->at(j));
		truegamma1.SetPxPyPzE(evtgen_px->at(i),evtgen_py->at(i),evtgen_pz->at(i),evtgen_E->at(i));
		truegamma2.SetPxPyPzE(evtgen_px->at(j),evtgen_py->at(j),evtgen_pz->at(j),evtgen_E->at(j));
		
		angle1=gamma1.Angle(electron.Vect())*180/PI;
		angle2=gamma2.Angle(electron.Vect())*180/PI;

		// Create pi0
		pi0=gamma1+gamma2;
		truepi=truegamma1+truegamma2;

		vz_pi0=(vz->at(i)+vz->at(j))/2.0;
		zpi0 = (init_target*pi0)/(init_target*q);
		truezpi0 = (init_target*truepi0)/(init_target*trueq);

		// Create dihadron
		dihadron = pi0+pi;
		truedihadron = truepi0+truepi;

		xF_pi = Kinematics::xF(q,pi,init_target,W);
		xF_pi0 = Kinematics::xF(q,pi0,init_target,W);

		zpair = zpi+zpi0;
		truezpair = truezpi + truezpi0;

		if(angle1>8 && angle2>8 &&
		   xF_pi>0 && xF_pi0>0 &&
		   zpair<0.95 &&
		   abs(vz_electron-vz_pi)<20 && abs(vz_electron-vz_pi0)<20){
	            

	      
		  Mgg=((gamma1+gamma2).M());
		  trueMgg=((truegamma1+truegamma2).M());
		  E1=(gamma1.E());
		  E2=(gamma2.E());
		  Mdihadron=(dihadron.M());
		  trueMdihadron=(truedihadron.M());
		  beta1=(beta->at(i));
		  beta2=(beta->at(j));

		  if(parentID->at(i)==parentID->at(j) && parentPID->at(i)==111)
		    flag=(1);
		  else
		    flag=(-1);
			      
		  phi_R0=(_kin.phi_R(q,init_electron,pi,pi0,0));
		  phi_R1=(_kin.phi_R(q,init_electron,pi,pi0,1));
		  phi_h=(_kin.phi_h(q,init_electron,pi,pi0));
		  th = (_kin.com_th(pi,pi0));

		  truephi_R0=(_kin.phi_R(trueq,init_electron,truepi,truepi0,0));
		  truephi_R1=(_kin.phi_R(trueq,init_electron,truepi,truepi0,1));
		  truephi_h=(_kin.phi_h(q,init_electron,truepi,truepi0));
		  trueth = (_kin.com_th(truepi,truepi0));
		  

		  px1=px->at(i);
		  py1=py->at(i);
		  pz1=pz->at(i);
		  pt1=pt->at(i);
		  p1=p->at(i);
		  theta1=theta->at(i);
		  phi1=phi->at(i);
		  vx1=vx->at(i);
		  vy1=vy->at(i);
		  vz1=vz->at(i);
		  status1=status->at(i);
		  parentPID1=evtgen_parentPID->at(i);
		  parentparentPID1=evtgen_parentparentPID->at(i);
		  PID1=evtgen_pid->at(i);
		  traj_sector1=traj_sector->at(i);
		  traj_path1_1=traj_path1->at(i);
		  traj_path2_1=traj_path2->at(i);
		  traj_path3_1=traj_path3->at(i);
		  traj_det1_1=traj_det1->at(i);
		  traj_det2_1=traj_det2->at(i);
		  traj_det3_1=traj_det3->at(i);
		  traj_x1_1=traj_x1->at(i);
		  traj_x2_1=traj_x2->at(i);
		  traj_x3_1=traj_x3->at(i);
		  traj_y1_1=traj_y1->at(i);
		  traj_y2_1=traj_y2->at(i);
		  traj_y3_1=traj_y3->at(i);
		  traj_z1_1=traj_z1->at(i);
		  traj_z2_1=traj_z2->at(i);
		  traj_z3_1=traj_z3->at(i);
		  cal_sector_PCAL_1=cal_sector_PCAL->at(i);
		  cal_sector_ECIN_1=cal_sector_ECIN->at(i);
		  cal_sector_ECOUT_1=cal_sector_ECOUT->at(i);
		  cal_energy_PCAL_1=cal_energy_PCAL->at(i);
		  cal_energy_ECIN_1=cal_energy_ECIN->at(i);
		  cal_energy_ECOUT_1=cal_energy_ECOUT->at(i);
		  cal_time_PCAL_1=cal_time_PCAL->at(i);
		  cal_time_ECIN_1=cal_time_ECIN->at(i);
		  cal_time_ECOUT_1=cal_time_ECOUT->at(i);
		  cal_path_PCAL_1=cal_path_PCAL->at(i);
		  cal_path_ECIN_1=cal_path_ECIN->at(i);
		  cal_path_ECOUT_1=cal_path_ECOUT->at(i);
		  cal_lu_PCAL_1=cal_lu_PCAL->at(i);
		  cal_lu_ECIN_1=cal_lu_ECIN->at(i);
		  cal_lu_ECOUT_1=cal_lu_ECOUT->at(i);
		  cal_lv_PCAL_1=cal_lv_PCAL->at(i);
		  cal_lv_ECIN_1=cal_lv_ECIN->at(i);
		  cal_lv_ECOUT_1=cal_lv_ECOUT->at(i);
		  cal_lw_PCAL_1=cal_lw_PCAL->at(i);
		  cal_lw_ECIN_1=cal_lw_ECIN->at(i);
		  cal_lw_ECOUT_1=cal_lw_ECOUT->at(i);

		  px2=px->at(j);
		  py2=py->at(j);
		  pz2=pz->at(j);
		  pt2=pt->at(j);
		  p2=p->at(j);
		  theta2=theta->at(j);
		  phi2=phi->at(j);
		  vx2=vx->at(j);
		  vy2=vy->at(j);
		  vz2=vz->at(j);
		  status2=status->at(j);
		  parentPID2=evtgen_parentPID->at(j);
		  parentparentPID2=evtgen_parentparentPID->at(j);
		  PID2=evtgen_pid->at(j);
		  traj_sector2=traj_sector->at(j);
		  traj_path1_2=traj_path1->at(j);
		  traj_path2_2=traj_path2->at(j);
		  traj_path3_2=traj_path3->at(j);
		  traj_det1_2=traj_det1->at(j);
		  traj_det2_2=traj_det2->at(j);
		  traj_det3_2=traj_det3->at(j);
		  traj_x1_2=traj_x1->at(j);
		  traj_x2_2=traj_x2->at(j);
		  traj_x3_2=traj_x3->at(j);
		  traj_y1_2=traj_y1->at(j);
		  traj_y2_2=traj_y2->at(j);
		  traj_y3_2=traj_y3->at(j);
		  traj_z1_2=traj_z1->at(j);
		  traj_z2_2=traj_z2->at(j);
		  traj_z3_2=traj_z3->at(j);
		  cal_sector_PCAL_2=cal_sector_PCAL->at(j);
		  cal_sector_ECIN_2=cal_sector_ECIN->at(j);
		  cal_sector_ECOUT_2=cal_sector_ECOUT->at(j);
		  cal_energy_PCAL_2=cal_energy_PCAL->at(j);
		  cal_energy_ECIN_2=cal_energy_ECIN->at(j);
		  cal_energy_ECOUT_2=cal_energy_ECOUT->at(j);
		  cal_time_PCAL_2=cal_time_PCAL->at(j);
		  cal_time_ECIN_2=cal_time_ECIN->at(j);
		  cal_time_ECOUT_2=cal_time_ECOUT->at(j);
		  cal_path_PCAL_2=cal_path_PCAL->at(j);
		  cal_path_ECIN_2=cal_path_ECIN->at(j);
		  cal_path_ECOUT_2=cal_path_ECOUT->at(j);
		  cal_lu_PCAL_2=cal_lu_PCAL->at(j);
		  cal_lu_ECIN_2=cal_lu_ECIN->at(j);
		  cal_lu_ECOUT_2=cal_lu_ECOUT->at(j);
		  cal_lv_PCAL_2=cal_lv_PCAL->at(j);
		  cal_lv_ECIN_2=cal_lv_ECIN->at(j);
		  cal_lv_ECOUT_2=cal_lv_ECOUT->at(j);
		  cal_lw_PCAL_2=cal_lw_PCAL->at(j);
		  cal_lw_ECIN_2=cal_lw_ECIN->at(j);
		  cal_lw_ECOUT_2=cal_lw_ECOUT->at(j);

		  pxpi=px->at(k);
		  pypi=py->at(k);
		  pzpi=pz->at(k);
		  ptpi=pt->at(k);
		  ppi=p->at(k);
		  thetapi=theta->at(k);
		  phipi=phi->at(k);
		  vxpi=vx->at(k);
		  vypi=vy->at(k);
		  vzpi=vz->at(k);
		  statuspi=status->at(k);
		  parentPIDpi=evtgen_parentPID->at(k);
		  parentparentPIDpi=evtgen_parentparentPID->at(k);
		  PIDpi=evtgen_pid->at(k);
		  traj_sectorpi=traj_sector->at(k);
		  traj_path1_pi=traj_path1->at(k);
		  traj_path2_pi=traj_path2->at(k);
		  traj_path3_pi=traj_path3->at(k);
		  traj_det1_pi=traj_det1->at(k);
		  traj_det2_pi=traj_det2->at(k);
		  traj_det3_pi=traj_det3->at(k);
		  traj_x1_pi=traj_x1->at(k);
		  traj_x2_pi=traj_x2->at(k);
		  traj_x3_pi=traj_x3->at(k);
		  traj_y1_pi=traj_y1->at(k);
		  traj_y2_pi=traj_y2->at(k);
		  traj_y3_pi=traj_y3->at(k);
		  traj_z1_pi=traj_z1->at(k);
		  traj_z2_pi=traj_z2->at(k);
		  traj_z3_pi=traj_z3->at(k);
		  cal_sector_PCAL_pi=cal_sector_PCAL->at(k);
		  cal_sector_ECIN_pi=cal_sector_ECIN->at(k);
		  cal_sector_ECOUT_pi=cal_sector_ECOUT->at(k);
		  cal_energy_PCAL_pi=cal_energy_PCAL->at(k);
		  cal_energy_ECIN_pi=cal_energy_ECIN->at(k);
		  cal_energy_ECOUT_pi=cal_energy_ECOUT->at(k);
		  cal_time_PCAL_pi=cal_time_PCAL->at(k);
		  cal_time_ECIN_pi=cal_time_ECIN->at(k);
		  cal_time_ECOUT_pi=cal_time_ECOUT->at(k);
		  cal_path_PCAL_pi=cal_path_PCAL->at(k);
		  cal_path_ECIN_pi=cal_path_ECIN->at(k);
		  cal_path_ECOUT_pi=cal_path_ECOUT->at(k);
		  cal_lu_PCAL_pi=cal_lu_PCAL->at(k);
		  cal_lu_ECIN_pi=cal_lu_ECIN->at(k);
		  cal_lu_ECOUT_pi=cal_lu_ECOUT->at(k);
		  cal_lv_PCAL_pi=cal_lv_PCAL->at(k);
		  cal_lv_ECIN_pi=cal_lv_ECIN->at(k);
		  cal_lv_ECOUT_pi=cal_lv_ECOUT->at(k);
		  cal_lw_PCAL_pi=cal_lw_PCAL->at(k);
		  cal_lw_ECIN_pi=cal_lw_ECIN->at(k);
		  cal_lw_ECOUT_pi=cal_lw_ECOUT->at(k);

		  




		  _tree_postprocess->Fill();
		  fid++;
		}
	      }
	    }
	  }
	}
      }
    }
  }
  return 0;
}



int PostProcess::inclusive(TTree * _tree_postprocess){
  double px_e;
  double py_e;
  double pz_e;
  double p_e;
  double th_e;
  double phi_e;
  double z_e;
  double vz_e;
  
  TLorentzVector init_electron;
  init_electron.SetPxPyPzE(0,0,sqrt(_electron_beam_energy*_electron_beam_energy - electronMass * electronMass),_electron_beam_energy);
  TLorentzVector init_target;
  init_target.SetPxPyPzE(0,0,0,protonMass);

  _tree_postprocess->Branch("px_e",&px_e);
  _tree_postprocess->Branch("py_e",&py_e);
  _tree_postprocess->Branch("pz_e",&pz_e);
  _tree_postprocess->Branch("p_e",&p_e);
  _tree_postprocess->Branch("th_e",&th_e);
  _tree_postprocess->Branch("phi_e",&phi_e);
  _tree_postprocess->Branch("z_e",&z_e);
  _tree_postprocess->Branch("vz_e",&vz_e);
  _tree_postprocess->Branch("Q2",&Q2);
  _tree_postprocess->Branch("x",&x);
  _tree_postprocess->Branch("y",&y);
  _tree_postprocess->Branch("W",&W);
  _tree_postprocess->Branch("helicity",&helicity);

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {
    nb = _tree_Reco->GetEntry(jentry);   nbytes += nb;
    px_e=0;
    py_e=0;
    pz_e=0;
    p_e=0;
    th_e=0;
    phi_e=0;
    z_e=0;
    vz_e=0;

    TLorentzVector electron;
    TLorentzVector q;
    
    bool foundscattered = false;
    for(unsigned int i = 0 ; i < pid->size() ; i++){
      // Identify the scattered electron
      if(pid->at(i)==11){
	electron.SetPxPyPzE(px->at(i),py->at(i),pz->at(i),E->at(i));
	px_e = px->at(i);
	py_e = py->at(i);
	pz_e = pz->at(i);
	p_e = p->at(i);
	th_e = theta->at(i);
	phi_e = phi->at(i);
	q = init_electron - electron;
	z_e = (init_target*electron)/(init_target*q);	
	vz_e = vz->at(i);
	if(status->at(i)<0){
	  foundscattered = true;
	  break;
	}
      }
    }
    
    if(foundscattered)
      _tree_postprocess->Fill();    
  }  
  return 0;
}


int PostProcess::elastic(TTree * _tree_postprocess){
  double px_e;
  double py_e;
  double pz_e;
  double p_e;
  double th_e;
  double phi_e;
  double z_e;
  double vz_e;
  double px_p;
  double py_p;
  double pz_p;
  double p_p;
  double th_p;
  double phi_p;
  double z_p;
  double vz_p;
  double Mmissing;
  double Ptmissing;
  double th_p_predict;

  TLorentzVector init_electron;
  init_electron.SetPxPyPzE(0,0,sqrt(_electron_beam_energy*_electron_beam_energy - electronMass * electronMass),_electron_beam_energy);
  TLorentzVector init_target;
  init_target.SetPxPyPzE(0,0,0,protonMass);

  _tree_postprocess->Branch("px_e",&px_e);
  _tree_postprocess->Branch("py_e",&py_e);
  _tree_postprocess->Branch("pz_e",&pz_e);
  _tree_postprocess->Branch("p_e",&p_e);
  _tree_postprocess->Branch("th_e",&th_e);
  _tree_postprocess->Branch("phi_e",&phi_e);
  _tree_postprocess->Branch("z_e",&z_e);
  _tree_postprocess->Branch("vz_e",&vz_e);
  _tree_postprocess->Branch("px_p",&px_p);
  _tree_postprocess->Branch("py_p",&py_p);
  _tree_postprocess->Branch("pz_p",&pz_p);
  _tree_postprocess->Branch("p_p",&p_p);
  _tree_postprocess->Branch("th_p",&th_p);
  _tree_postprocess->Branch("th_p_predict",&th_p_predict);
  _tree_postprocess->Branch("phi_p",&phi_p);
  _tree_postprocess->Branch("z_p",&z_p);
  _tree_postprocess->Branch("vz_p",&vz_p);
  _tree_postprocess->Branch("Q2",&Q2);
  _tree_postprocess->Branch("x",&x);
  _tree_postprocess->Branch("y",&y);
  _tree_postprocess->Branch("W",&W);
  _tree_postprocess->Branch("helicity",&helicity);
  _tree_postprocess->Branch("Mmiss",&Mmissing);
  _tree_postprocess->Branch("Ptmiss",&Ptmissing);

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {
    nb = _tree_Reco->GetEntry(jentry);   nbytes += nb;
    px_e=0;
    py_e=0;
    pz_e=0;
    p_e=0;
    th_e=0;
    phi_e=0;
    z_e=0;
    vz_e=0;
    px_p=0;
    py_p=0;
    pz_p=0;
    p_p=0;
    th_p=0;
    phi_p=0;
    z_p=0;
    vz_p=0;
    Mmissing = 0;
    Ptmissing = 0;
    th_p_predict = 0;

    TLorentzVector electron;
    TLorentzVector proton;
    TLorentzVector q;
    
    for(unsigned int i = 0 ; i < pid->size() ; i++){
      // Identify the scattered electron
      if(pid->at(i)==11){
	electron.SetPxPyPzE(px->at(i),py->at(i),pz->at(i),E->at(i));
	px_e = px->at(i);
	py_e = py->at(i);
	pz_e = pz->at(i);
	p_e = p->at(i);
	th_e = theta->at(i);
	phi_e = phi->at(i);
	q = init_electron - electron;
	z_e = (init_target*electron)/(init_target*q);	
	vz_e = vz->at(i);
      }
      if(pid->at(i)==2212){
	proton.SetPxPyPzE(px->at(i),py->at(i),pz->at(i),E->at(i));
	px_p = px->at(i);
	py_p = py->at(i);
	pz_p = pz->at(i);
	p_p = p->at(i);
	th_p = theta->at(i);
	phi_p = phi->at(i);
	z_p = (init_target*proton)/(init_target*q);	
	vz_p = vz->at(i);
      }
    }
    
    Mmissing = (init_target+init_electron-electron-proton).M();
    Ptmissing = (init_target+init_electron-electron-proton).Pt();
    TLorentzVector proton_predict = (init_target+init_electron-electron);
    th_p_predict = proton_predict.Angle(proton.Vect());

    _tree_postprocess->Fill();    
  }  
  return 0;
}




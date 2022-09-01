#include "PostProcess.h"
PostProcess::PostProcess(){}
int PostProcess::Init(TTree * tree_Reco, double eBeam)
{
  _electron_beam_energy = eBeam;
  _tree_Reco = tree_Reco;
  _tree_Reco->SetBranchAddress("event_truth", &event_truth, &b_event);
  _tree_Reco->SetBranchAddress("Mx", &Mx, &b_Mx);
  _tree_Reco->SetBranchAddress("Q2", &Q2, &b_Q2);
  _tree_Reco->SetBranchAddress("W", &W, &b_W);
  _tree_Reco->SetBranchAddress("evnum", &evnum, &b_evnum);
  _tree_Reco->SetBranchAddress("helicity", &helicity, &b_helicity);
  _tree_Reco->SetBranchAddress("nParticles", &nParticles, &b_nParticles);
  _tree_Reco->SetBranchAddress("nu", &nu, &b_nu);
  _tree_Reco->SetBranchAddress("polarization", &polarization, &b_polarization);
  _tree_Reco->SetBranchAddress("run", &run, &b_run);
  _tree_Reco->SetBranchAddress("x", &x, &b_x);
  _tree_Reco->SetBranchAddress("y", &y, &b_y);
  _tree_Reco->SetBranchAddress("evtgen_parentPID", &evtgen_parentPID, &b_evtgen_parentPID);
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
  int flag;
  _tree_postprocess->Branch("fid",&fid);
  _tree_postprocess->Branch("Mdiphoton",&Mgg);
  _tree_postprocess->Branch("E1",&E1);
  _tree_postprocess->Branch("E2",&E2);
  _tree_postprocess->Branch("angle1",&angle1);
  _tree_postprocess->Branch("angle2",&angle2);
  _tree_postprocess->Branch("Mdihadron",&Mdihadron);
  _tree_postprocess->Branch("beta1",&beta1);
  _tree_postprocess->Branch("beta2",&beta2);
  _tree_postprocess->Branch("helicity",&helicity);
  _tree_postprocess->Branch("phi_R0",&phi_R0);
  _tree_postprocess->Branch("phi_R1",&phi_R1);
  _tree_postprocess->Branch("phi_h",&phi_h);
  _tree_postprocess->Branch("th",&th);
  _tree_postprocess->Branch("Q2",&Q2);
  _tree_postprocess->Branch("W",&W);
  _tree_postprocess->Branch("nu",&nu);
  _tree_postprocess->Branch("x",&x);
  _tree_postprocess->Branch("y",&y);
  _tree_postprocess->Branch("z",&zpair);
  _tree_postprocess->Branch("polarization",&polarization);
  _tree_postprocess->Branch("flag",&flag);
  
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

  double vz_electron = 0.0;
  double vz_pi = 0.0;
  double vz_pi0 = 0.0;

  double xF_pi = 0.0;
  double xF_pi0 = 0.0;

  double zpi = 0.0;
  double zpi0 = 0.0;

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {
    nb = _tree_Reco->GetEntry(jentry);   nbytes += nb;
    
    Mgg=0.0;
    E1=0.0;
    E2=0.0;
    Mdihadron=0.0;
    beta1=0.0;
    beta2=0.0;
    phi_h=0.0;
    phi_R0=0.0;
    phi_R1=0.0;
    th=0.0;
    zpair=0.0;
    flag = 0;
    for(unsigned int i = 0 ; i < pid->size() ; i++){
      // Identify the scattered electron
      if(pid->at(i)==11){
	electron.SetPxPyPzE(px->at(i),py->at(i),pz->at(i),E->at(i));
	vz_electron=vz->at(i);
      }
      // Identify the single charged pion
      if(pid->at(i)==_piPID){
      }
    }

   
    // Set virtual photon
    q = init_electron - electron;
    
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
		vz_pi=vz->at(k);      
		zpi = (init_target*pi)/(init_target*q);
		
		// Create gammas
		gamma1.SetPxPyPzE(px->at(i),py->at(i),pz->at(i),E->at(i));
		gamma2.SetPxPyPzE(px->at(j),py->at(j),pz->at(j),E->at(j));
		angle1=gamma1.Angle(electron.Vect())*180/PI;
		angle2=gamma2.Angle(electron.Vect())*180/PI;

		// Create pi0
		pi0=gamma1+gamma2;
		vz_pi0=(vz->at(i)+vz->at(j))/2.0;
		zpi0 = (init_target*pi0)/(init_target*q);

		// Create dihadron
		dihadron = pi0+pi;

		xF_pi = Kinematics::xF(q,pi,init_target,W);
		xF_pi0 = Kinematics::xF(q,pi0,init_target,W);

		zpair = zpi+zpi0;
	    
		if(angle1>8 && angle2>8 &&
		   xF_pi>0 && xF_pi0>0 &&
		   zpair<0.95 &&
		   abs(vz_electron-vz_pi)<20 && abs(vz_electron-vz_pi0)<20){
	            

	      
		  Mgg=((gamma1+gamma2).M());
		  E1=(gamma1.E());
		  E2=(gamma2.E());
		  Mdihadron=(dihadron.M());
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
	break;
      }
    }

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
  double phi_p_predict;

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
  _tree_postprocess->Branch("phi_p_predict",&phi_p_predict);
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
    phi_p_predict = 0;

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
    th_p_predict = (init_target+init_electron-electron).Theta();
    phi_p_predict = (init_target+init_electron-electron).Phi();

    _tree_postprocess->Fill();    
  }  
  return 0;
}




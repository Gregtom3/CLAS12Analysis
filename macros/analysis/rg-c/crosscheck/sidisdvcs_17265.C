/*
  
  Created October 31st 2022
  Author: Gregory Matousek

  Purpose: To analyze Run 17265 and extract the Faraday Cup accumulation for h=+1, h=-1 beam helicities and the x-Q2 for each event. This is all the necessary information required to produce the target polarization of the run later

  Reads in the HBT, coatjava8.3.2 sidisdvcs train for Run 17265

  We make DIS electron cuts of the following
      Ee > 2.6 GeV
      8 < th_e < 35 degrees (FD)
      |vz_e + 4.5| < 4 [cm]

 */

int sidisdvcs_17265(){
  // Constants
  const char * hipofile = "/volatile/clas12/rg-c/production/ana_data/HBT/8.3.2/dst/train/sidisdvcs/sidisdvcs_017265.hipo";
  const double mE = 0.000511;
  const double mP = 0.938272;
  const double beamE = 10.5563; // From RCDB
  TLorentzVector init_electron;
  init_electron.SetPxPyPzE(0,0,sqrt(beamE*beamE-mE*mE),beamE);
  TLorentzVector init_proton;
  init_proton.SetPxPyPzE(0,0,0,mP);
  TLorentzVector electron;
  TLorentzVector q;

  // Create output rootfile, output ttree for x-Q2, and output histogram to store fcup accumulations
  TFile *fOut = new TFile("data17265.root","RECREATE");
  TTree *tOut = new TTree("tree","");
  TH1F *hOut = new TH1F("fcup","",2,0,2);
  int hel = 0;
  double x = 0.0;
  double Q2 = 0.0;
  tOut->Branch("helicity",&hel,"helicity/I");
  tOut->Branch("x",&x,"x/D");
  tOut->Branch("Q2",&Q2,"Q2/D");

  // Create HipoChain for reading the sidisdvcs hipo file
  HipoChain chain;
  chain.Add(hipofile);
  auto config_c12=chain.GetC12Reader();

  // Making basic DIS electron cuts
  config_c12->addAtLeastPid(11,1);    //at least 1 electron
  config_c12->db()->turnOffQADB();    //no QADB for RGC yet
  auto& c12=chain.C12ref();

  // Loop over events in the HipoChain
  int ievent=0;
  while(chain.Next()==true){
    if((++ievent)%100000==0) cout << "Event " << ievent << " completed" << endl;
    if(c12->getDetParticles().empty())
      continue;
    auto particles = c12->getDetParticles();
    // Loop over all particles
    for(unsigned int idx = 0 ; idx < particles.size() ; idx++){
      auto particle = particles.at(idx);
      int pid = particle->getPid();
      if(pid!=11) continue; // Skip non electrons
      int status = particle->getStatus();
      if(status>0) continue; // Skip non-scattered electrons
      double P_e = particle->getP();
      double E_e = sqrt(P_e * P_e + (mE*mE));
      double th_e = particle->getTheta();
      double phi_e = particle->getPhi(); 
      double vz_e = particle->par()->getVz();
      // Set TLorentzVectors for scattered electron and virtual photon
      double px_e = P_e * sin(th_e) * cos(phi_e);
      double py_e = P_e * sin(th_e) * sin(phi_e);
      double pz_e = P_e * cos(th_e);
      electron.SetPxPyPzE(px_e,py_e,pz_e,E_e);
      q=(init_electron-electron);

      if(E_e<2.6) continue; // Ecut
      if(th_e*(180/3.14159265)<8 || th_e*(180/3.14159265)>35) continue; // Theta cut
      if(abs(vz_e+4.5)>4) continue; // vz cut
      
      // Set TTree Branch variables
      hel = c12->event()->getHelicity();
      Q2 = -(q*q);
      x = Q2/(2*init_proton*q);
      
      // Fill the TTree
      tOut->Fill();
    }
  }

  /* 
     
     Now read HEL::scaler for the hipo file

   */
  
  double fcupgated = 0.0;
  double fcupgated_pos = 0.0;
  double fcupgated_neg = 0.0;
  
  hipo::reader reader;
  hipo::event event;
  hipo::dictionary factory;
  
  reader.setTags(1);
  reader.open(hipofile);
  reader.readDictionary(factory);
  hipo::bank HEL(factory.getSchema("HEL::scaler"));

  // Loop over every HEL::scaler bank entry
  while(reader.next()){
    reader.read(event);
    event.getStructure(HEL);

    hel = HEL.getInt("helicity",0); // Pull helicity from HEL::scaler
    fcupgated = HEL.getFloat("fcupgated",0); // Pull fcupgated from HEL::scaler
    
    if(HEL.getRows()==0) // Continue if bad entry
      continue;
    else if(hel==1) // Store +1
      fcupgated_pos+=fcupgated;
    else if(hel==-1) // Store -1
      fcupgated_neg+=fcupgated;
  }

  // Set Bin Content of output histogram
  // First bin --> fcupgated_pos
  // Second bin --> fcupgated_neg

  hOut->SetBinContent(1,fcupgated_pos);
  hOut->SetBinContent(2,fcupgated_neg);

  // Write TObjects
  tOut->Write();
  hOut->Write();
  fOut->Close();

  return 0;
}

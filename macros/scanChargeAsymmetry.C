// Purpose : Scan through HIPO file without cuts to accumulate...
//           HEL::scaler accumulated beam charge from faraday cup (for helicities -1, 0, 1)
//           RUN::config  helicity counts (+1, 0, -1) **USING RECON BANKS**
//           RUN::config  helicity counts (+1, 0, -1) **USING TRAIN BANKS**
//           The results of which are appended to a row in CLAS12Analysis/util/runHelicityCounts.csv


int scanChargeAsymmetry(std::string fileprefix_recon = "/volatile/clas12/rg-c/production/ana_data/dst/recon/016904/rec_clas_016904.evio.",
			std::string filename_train = "/volatile/clas12/rg-c/production/ana_data/dst/train/sidisdvcs/sidisdvcs_016904.hipo", 
			const char * csvfile = "/work/clas12/users/gmat/CLAS12Analysis/util/runHelicityCounts.csv",
			int max_files = 99999, 
			int verbosity = 0)
{
  cout << "READING TRAIN" << endl;
  /* Now read in the TRAIN HIPO files */
  clas12root::HipoChain chain_train;
  chain_train.Add(Form("%s",filename_train.c_str()));
  int polarization_train = 0;
  int npos_train = 0;
  int nneg_train = 0;
  int nzero_train = 0;
  int run_train = 0; 

  auto config_c12_train=chain_train.GetC12Reader();
  config_c12_train->db()->turnOffQADB();
  auto idx_RECEvent_train= config_c12_train->addBank("REC::Event");
  auto idx_RUNConfig_train= config_c12_train->addBank("RUN::config");
  auto iHelicity_train = config_c12_train->getBankOrder(idx_RECEvent_train,"helicity");
  auto iRun_train = config_c12_train->getBankOrder(idx_RUNConfig_train,"run");
  auto& c12_train=chain_train.C12ref();

  // LOOP OVER THE HIPO FILE
  while(  chain_train.Next() ){
    run_train = c12_train->getBank(idx_RUNConfig_train)->getInt(iRun_train,0);
    polarization_train = c12_train->getBank(idx_RECEvent_train)->getInt(iHelicity_train,0);
    if(polarization_train==1)
      npos_train++;
    else if(polarization_train==-1)
      nneg_train++;
    else if(polarization_train==0)
      nzero_train++;
    else
      cout << "ERROR" << endl;
    
  }
  cout << "READING RECON" << endl;
  //  Now read in the RECON HIPO files
  clas12root::HipoChain chain_recon;
  chain_recon.Add(Form("%s*",fileprefix_recon.c_str()));
  int polarization_recon = 0;
  int npos_recon = 0;
  int nneg_recon = 0;
  int nzero_recon = 0;
  int run_recon = 0; 
  /*
  auto config_c12_recon=chain_recon.GetC12Reader();
  config_c12_recon->db()->turnOffQADB();
  auto idx_RECEvent_recon= config_c12_recon->addBank("REC::Event");
  auto idx_RUNConfig_recon= config_c12_recon->addBank("RUN::config");
  auto iHelicity_recon = config_c12_recon->getBankOrder(idx_RECEvent_recon,"helicity");
  auto iRun_recon = config_c12_recon->getBankOrder(idx_RUNConfig_recon,"run");
  auto& c12_recon=chain_recon.C12ref();

  // LOOP OVER THE HIPO FILE
  while(  chain_recon.Next() ){
    // DO NOT LOOP RECON
    break;
    run_recon = c12_recon->getBank(idx_RUNConfig_recon)->getInt(iRun_recon,0);
    polarization_recon = c12_recon->getBank(idx_RECEvent_recon)->getInt(iHelicity_recon,0);
    if(polarization_recon==1)
      npos_recon++;
    else if(polarization_recon==-1)
      nneg_recon++;
    else if(polarization_recon==0)
      nzero_recon++;
    else
      cout << "ERROR" << endl;
    
  }

  */
  /* Read HEL::scaler */
  /* For each entry in HEL::scaler, we use the entry's helicity to accumulate the correspond faraday cup charge */
  /* Any 'preference' for +/- helicity will be seen as an artificial asymmetry in our studies, one we must correct for */
  cout << "READING HEL::scaler" << endl; 
  double fcup = 0.0;
  double fcup_pos = 0.0;
  double fcup_neg = 0.0;
  double fcup_zero = 0.0;
  double fcup_bad = 0.0;
  int hel = -999;
  size_t maxZeros = 5;
  int leadingZeros;

  std::string filename = "";
  std::string filesuffix = "";

  hipo::reader     reader_;
  hipo::event      event_;
  hipo::dictionary  factory_;

  for(int idx_file = 0 ; idx_file < max_files ; idx_file ++){
    
    filesuffix = std::to_string(idx_file);
    leadingZeros = maxZeros - std::min(maxZeros,filesuffix.size());
    filename = Form("%s%s.hipo",fileprefix_recon.c_str(), std::string(leadingZeros,'0').append(filesuffix).c_str()   );
    
    if(filename.empty() || gSystem->AccessPathName(filename.c_str()))
      {
	cout << "File " << filename << " not found...Aborting..." << endl;
	break;
      }
    else
      {
	if(verbosity > 0){
	  cout << "Reading in HEL::scaler for file " << filename << endl;
	}
      }

    reader_.setTags(1);
    reader_.open(filename.data()); //keep a pointer to the reader
    reader_.readDictionary(factory_);
    hipo::bank HEL(factory_.getSchema("HEL::scaler"));

    while(reader_.next()){
      reader_.read(event_);
      event_.getStructure(HEL);

      hel = HEL.getInt("helicity",0);
      fcup = HEL.getFloat("fcupgated",0);
      
    
      if(HEL.getRows()==0){
	fcup_bad+=fcup;
      }
      else if(hel==1)
	fcup_pos+=fcup;
      else if(hel==0)
	fcup_zero+=fcup;
      else if(hel==-1)
	fcup_neg+=fcup;
      else
	cout << "ERROR" << endl;
    }
    if(verbosity > 2){
    cout << "FaradayCup for Helicity +1 " << fcup_pos << endl;
    cout << "FaradayCup for Helicity  0 " << fcup_zero << endl;
    cout << "FaradayCup for Helicity -1 " << fcup_neg << endl;
    cout << "FaradayCup for Helicity ?? " << fcup_bad << endl;
    cout << "------------------------------------------" << endl;
    }
  }
  if(verbosity>0){
  cout << "\n-------------- TOTAL ---------------------" << endl;
  cout << "FaradayCup for Helicity +1 " << fcup_pos << endl;
  cout << "FaradayCup for Helicity  0 " << fcup_zero << endl;
  cout << "FaradayCup for Helicity -1 " << fcup_neg << endl;
  cout << "FaradayCup for Helicity ?? " << fcup_bad << endl;
  cout << "------------------------------------------" << endl;
  }



  


  // Add helicity counts to csv
  ofstream outFile(csvfile, fstream::app);
  if(!outFile.is_open()){
    cout << "ERROR: Could not find runHelicityCounts.csv at " << csvfile << "...Aborting..." << endl;
    return -1;
  }
  else{
    outFile << run_train << "," << npos_recon << "," << nneg_recon << "," << nzero_recon << "," << npos_train << "," << nneg_train << "," << nzero_train << "," << fcup_pos << "," << fcup_neg << "," << fcup_zero << "," << fcup_bad << "\n";
  }
  outFile.close();
  
  return 0 ;
}

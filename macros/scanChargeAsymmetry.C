// Purpose : Scan through HIPO file without cuts to accumulate...
//           HEL::scaler accumulated beam charge from faraday cup (for helicities -1, 0, 1)
//           RUN::config  helicity counts (+1, 0, -1)
//           The results of which are appended to a row in CLAS12Analysis/util/runHelicityCounts.csv


int scanChargeAsymmetry(std::string fileprefix = "/volatile/clas12/rg-c/production/ana_data/dst/recon/016292/rec_clas_016292.evio.", 
			const char * csvfile = "/work/clas12/users/gmat/CLAS12Analysis/util/runHelicityCounts.csv",
			int max_files = 99999, 
			int verbosity = 0)
{

  /* Read HEL::scaler */
  /* For each entry in HEL::scaler, we use the entry's helicity to accumulate the correspond faraday cup charge */
  /* Any 'preference' for +/- helicity will be seen as an artificial asymmetry in our studies, one we must correct for */

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
    filename = Form("%s%s.hipo",fileprefix.c_str(), std::string(leadingZeros,'0').append(filesuffix).c_str()   );
    
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




  /* Now read in the HIPO chain */
  clas12root::HipoChain chain;
  chain.Add(Form("%s*",fileprefix.c_str()));
  int polarization = 0;
  int npos = 0;
  int nneg = 0;
  int nzero = 0;
  int run = 0; 

  auto config_c12=chain.GetC12Reader();
  config_c12->db()->turnOffQADB();
  auto idx_RECEvent= config_c12->addBank("REC::Event");
  auto idx_RUNConfig= config_c12->addBank("RUN::config");
  auto iHelicity = config_c12->getBankOrder(idx_RECEvent,"helicity");
  auto iRun = config_c12->getBankOrder(idx_RUNConfig,"run");
  auto& c12=chain.C12ref();

  // LOOP OVER THE HIPO FILE
  while(  chain.Next() ){

    run = c12->getBank(idx_RUNConfig)->getInt(iRun,0);
    polarization = c12->getBank(idx_RECEvent)->getInt(iHelicity,0);
    if(polarization==1)
      npos++;
    else if(polarization==-1)
      nneg++;
    else if(polarization==0)
      nzero++;
    else
      cout << "ERROR" << endl;
    
  }

  // Add helicity counts to csv
  ofstream outFile(csvfile, fstream::app);
  if(!outFile.is_open()){
    cout << "ERROR: Could not find runHelicityCounts.csv at " << csvfile << "...Aborting..." << endl;
    return -1;
  }
  else{
    outFile << run << "," << npos << "," << nneg << "," << nzero << "," << fcup_pos << "," << fcup_neg << "," << fcup_zero << "," << fcup_bad << "\n";
  }
  outFile.close();
  return 0 ;
}

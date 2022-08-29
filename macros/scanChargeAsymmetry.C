// Purpose : Scan through HIPO file without cuts to accumulate helicity counts (+1, 0, -1)
//           The results of which are appended to a row in CLAS12Analysis/util/runHelicityCounts.csv

int scanChargeAsymmetry(const char * hipofile = "/volatile/clas12/rg-c/production/ana_data/dst/recon/016412/*",
			const char * csvfile = "/work/clas12/users/gmat/CLAS12Analysis/util/runHelicityCounts.csv")
{


  clas12root::HipoChain chain;
  chain.Add(hipofile);
  int i = 0;
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
    outFile << run << "," << npos << "," << nneg << "," << nzero << "\n";
  }
  outFile.close();
  return 0;
}

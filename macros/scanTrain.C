// scanTrain
// Purpose: Parse through HEL::scaler for each run and save information to csv's
// To execute properly, simply perform ./run.sh

int scanTrain(int run = 16770,
	      std::string prefix = "/farm_out/gmat/rgc-scaler-run",
	      std::string cook="TBT/8.3.2"){
  // Verbosity
  int verbosity = 0;
  
  // Filenames
  std::string file = Form("/volatile/clas12/rg-c/production/ana_data/%s/dst/train/sidisdvcs/sidisdvcs_0%d.hipo",cook.c_str(),run);
  std::string outHELScaler = Form("%s-%d-HELScaler-train.csv",prefix.c_str(),run);

  // HEL::Scaler bank info
  double HEL_fcupgated = 0.0;
  double HEL_fcup = 0.0;
  double HEL_slmgated = 0.0;
  double HEL_slm = 0.0;
  double HEL_clockgated = 0.0;
  double HEL_clock = 0.0;
  int HEL_helicity = 0;
  int HEL_helicityRaw = 0;
  int entry_idx=0;
  ofstream outFile_HEL(outHELScaler,fstream::trunc);

  int leadingZeros;

  std::string filename = "";
  std::string filesuffix = "";

  hipo::reader     reader_;
  hipo::event      event_;
  hipo::dictionary  factory_;

  reader_.setTags(1);
  reader_.open(file.data()); //keep a pointer to the reader
  reader_.readDictionary(factory_);
  hipo::bank HEL(factory_.getSchema("HEL::scaler"));

  while(reader_.next()){
    reader_.read(event_);
    event_.getStructure(HEL);
    if(HEL.getRows()==0){
      continue;
    }
    
    HEL_fcupgated = HEL.getFloat("fcupgated",0);
    HEL_fcup = HEL.getFloat("fcup",0);
    HEL_slmgated = HEL.getFloat("slmgated",0);
    HEL_slm = HEL.getFloat("slm",0);
    HEL_clockgated = HEL.getFloat("clockgated",0);
    HEL_clock = HEL.getFloat("clock",0);
    HEL_helicity = HEL.getInt("helicity",0);
    HEL_helicityRaw = HEL.getInt("helicityRaw",0);
      
    entry_idx++;

    outFile_HEL << run << "," << entry_idx << "," << HEL_fcupgated << "," << HEL_fcup << "," << HEL_slmgated << "," << HEL_slm << "," << HEL_clockgated << "," << HEL_clock << "," << HEL_helicity << "," << HEL_helicityRaw << "\n";
  
  }

  outFile_HEL.close();


  return 0 ;
}

#ifndef Constants_h
#define Constants_h

const double electronMass = 0.000511;
const double protonMass   = 0.938272;
const double alpha_em     = 1/137.036;
const double PI         = 3.14159265359;
const double radtodeg     = 57.2957795131;
const double degtorad     = 0.01745329251;
const int    ineg999      = -999;
const float  fneg999      = -999.;

// Returns run polarization if "v" is true
// Returns run polarization error if "v" is false
inline float runPolarization(int run, bool v=true){
  if     (run>= 5032 && run<= 5332) return v? 0.8592 : 0.01290; // rga_fa18, before Wien angle change
  else if(run>= 5333 && run<= 5666) return v? 0.8922 : 0.02509; // rga_fa18, after Wien angle change
  else if(run>= 6616 && run<= 6783) return v? 0.8453 : 0.01474; // rga_sp19 https://logbooks.jlab.org/entry/3677077
  else
    return 0.0;
};

// Returns beam energy based on run
inline float runBeamEnergy(int run){
  if     (run>= 5032 && run<= 5666) return 10.6041; // rga fall 18
  else if(run>= 6616 && run<= 6783) return 10.1998; // rga spring 19
  else if(run>= 6120 && run<= 6399) return 10.5986; // rgb spring 19
  else if(run>= 6409 && run<= 6604) return 10.1998; // rgb spring 19
  else if(run>=11093 && run<=11283) return 10.4096; // rgb fall 19
  else if(run>=11284 && run<=11300) return 4.17179; // rgb fall BAND_FT 19
  else if(run>=11323 && run<=11571) return 10.3894; // rgb winter 20 (RCDB may still be incorrect)
  else if(run==11)                  return 10.6041; // MC for RGA inbending
  else {
    return 0.0;
  };
};

inline bool runHelicityFlip(int run){
  if     (run>= 5032 && run<= 5666) return true;  // rga fall 18
  else if(run>= 6616 && run<= 6783) return true;  // rga spring 19
  else if(run>= 6120 && run<= 6604) return true;  // rgb spring 19
  else if(run>=11093 && run<=11283) return false; // rgb fall 19
  else if(run>=11284 && run<=11300) return true;  // rgb fall BAND_FT 19
  else if(run>=11323 && run<=11571) return false; // rgb winter 20
  else
    return false;
};

inline int runTorusBending(int run){
  if     (run>= 5032 && run<= 5332) return -1; // rga_fa18, before Wien angle change
  else if(run>= 5333 && run<= 5666) return +1; // rga_fa18, after Wien angle change
  else if(run>= 6616 && run<= 6783) return -1; // rga_sp19 https://logbooks.jlab.org/entry/3677077
  else
    return 0.0;
};

#endif 

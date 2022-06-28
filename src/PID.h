#ifndef PID_h
#define PID_h

/* STL Includes */
#include <math.h>
/* Src Includes */
#include "Settings.h"
#include "SIDISParticlev1.h"

class Settings;

class PID{
 public:
  PID(Settings);
  
  // All cuts based on REC::Particle information alone
  bool performPIDCuts(SIDISParticlev1*)

 protected:
  Settings _settings;
};
#endif

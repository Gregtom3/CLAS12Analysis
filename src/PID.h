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
  PID();
  void ImportSettings(Settings _settingsFromMain);
  
  // Particle specific cuts
  bool performPIDCuts(SIDISParticlev1*);

 protected:
  // Simple cuts from REC::Particle
  // Typically editable by the user in ./macros/
  bool basicRECParticleCuts(SIDISParticlev1*);

  // More involved cuts from REC::Particle
  bool advancedRECParticleCuts(SIDISParticlev1*);

  // Sampling fraction cut
  bool samplingFractionCuts(SIDISParticlev1*);
  
  Settings _settings;
  
};
#endif

#include "PID.h"

PID::PID(){}

void PID::ImportSettings(Settings _settingsFromMain){
  _settings = _settingsFromMain;
}

bool PID::performPIDCuts(SIDISParticlev1* sp){
  
  if(basicRECParticleCuts(sp) == false)
    return false;
  else if(advancedRECParticleCuts(sp) == false)
    return false;
  else if(samplingFractionCuts(sp) == false)
    return false;

  // All particle cuts have been successfully passed!
  return true;
} 

bool PID::basicRECParticleCuts(SIDISParticlev1* sp){
  
  int pid = sp->get_property_int(SIDISParticle::part_pid);
  float E = sp->get_property_float(SIDISParticle::part_E);
  float p = sp->get_property_float(SIDISParticle::part_p);  
  float chi2 = sp->get_property_float(SIDISParticle::part_chi2);
  float beta = sp->get_property_float(SIDISParticle::part_beta);
  float vz = sp->get_property_float(SIDISParticle::part_vz);
 
  // CUT pid --------------------------------------------------------------
  if(_settings.getN_fromPID(pid)==0 && _settings._ignoreOtherRecoParticles)
    return false;

  // CUT chi2 -------------------------------------------------------------
  if(abs(chi2) > _settings.getChi2max_fromPID(pid))
    return false;

  // CUT beta -------------------------------------------------------------
  if(abs(beta) > _settings.getBetamax_fromPID(pid) || abs(beta) < _settings.getBetamin_fromPID(pid))
    return false;

  // CUT p -------------------------------------------------------------
  if(_settings.getPmin_fromPID(pid) > p)      
    return false;

  // CUT E -------------------------------------------------------------
  if(_settings.getEmin_fromPID(pid) > E)
    return false;

  // CUT vz -------------------------------------------------------------
  if(vz < _settings.getVzmin_fromPID(pid) || vz > _settings.getVzmax_fromPID(pid))
    return false;
  
  return true;
}

bool PID::advancedRECParticleCuts(SIDISParticlev1* sp){

  int pid = sp->get_property_int(SIDISParticle::part_pid);
  float p = sp->get_property_float(SIDISParticle::part_p);  
  float chi2 = sp->get_property_float(SIDISParticle::part_chi2);
  
  bool passChargedPionChi2=true;
  // For charged pions, perform additional chi2 cuts
  // See RGA analysis note for details
  if(pid==211||pid==-211){
    // Only do chargedPionChi2cut if user desires it
    if(_settings._chargedPionChi2cut!=Settings::chargedPionChi2cut::none){
      // Determine pion charge dependent C value
      float C = 0.0;
      (pid==211 ? C=0.88 : C=0.93);
      // 2 different pion chi2pid regions
      // standard
      // strict
      if(_settings._chargedPionChi2cut==Settings::chargedPionChi2cut::standard){
	if(p<2.44)
	  passChargedPionChi2=chi2<C*3;
	else
	  passChargedPionChi2=chi2<C*(0.00869 + 14.98587 * exp(-p/1.18236) + 1.81751 * exp(-p/4.86394));
      } 
      else if(_settings._chargedPionChi2cut==Settings::chargedPionChi2cut::strict){
	if(p<2.44)
	  passChargedPionChi2=chi2<C*3;
	else if(p<4.6)
	  passChargedPionChi2=chi2< C * (0.00869 + 14.98587 * exp(-p/1.18236) + 1.81751 * exp(-p/4.86394));
	else
	  passChargedPionChi2=chi2< C * (-1.14099 + 24.14992 * exp(-p/1.36554) + 2.66876 * exp(-p/6.80552));
      } 
      else {
	std::cout << " UNKNOWN chargedPionChi2cut value, returning -1" << std::endl;
	return false;
      }
    }
  }
  
  if(passChargedPionChi2 == false)
    return false;
    
  return true;
}

bool PID::samplingFractionCuts(SIDISParticlev1* sp){
  return true;
}

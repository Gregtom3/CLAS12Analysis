#include "Settings.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
Settings::Settings()
{
  
}

void Settings::addFinalState(int pid, int n, bool exact=false) {
  // Make sure pid isn't a duplicate
  if(std::count(_fPID.begin(), _fPID.end(), pid)){
    std::cout << "ERROR in Settings::addFinalState -- " << pid << " is a repeat! Skipping..." << std::endl;
  }
  else if(n<0)
    {
      std::cout << "ERROR in Settings::addFinalState -- " << "n cannot be negative! Skipping..." << std::endl;
    }
  else
    {
      _fPID.push_back(pid);
      _fNpart.push_back(n);
      _fExact.push_back(exact);
    }
  return;
}

void Settings::addHipoFile(std::string filename){
  _hipoFileStrings.push_back(filename);
}

void Settings::addPIDforEmin(int pid, double Emin){
  _EminPID.push_back(pid);
  _Emin.push_back(Emin);
}

void Settings::addPIDforPmin(int pid, double Pmin){
  _PminPID.push_back(pid);
  _Pmin.push_back(Pmin);
}

void Settings::addPIDforVzrange(int pid, double Vzmin, double Vzmax) {
  _VzPID.push_back(pid);
  _Vzmin.push_back(Vzmin);
  _Vzmax.push_back(Vzmax);
  return;
}

void Settings::addPIDforBetarange(int pid, double Betamin, double Betamax) {
  _BetaPID.push_back(pid);
  _Betamin.push_back(Betamin);
  _Betamax.push_back(Betamax);
  return;
}

void Settings::addPIDforChi2max(int pid, double chi2max){
  _chi2PID.push_back(pid);
  _chi2max.push_back(chi2max);
  return;
}

int Settings::getN_fromPID(int pid){
  for(unsigned int idx = 0 ; idx < _fPID.size() ; idx++)
    {
      if(_fPID.at(idx)==pid)
	return _fNpart.at(idx);
    }
  
  return 0;
}

bool Settings::isExact_fromPID(int pid){
  for(unsigned int idx = 0 ; idx < _fPID.size() ; idx++)
    {
      if(_fPID.at(idx)==pid)
	return _fExact.at(idx);
    }
  return false;
}

double Settings::getEmin_fromPID(int pid){
 for(unsigned int idx = 0 ; idx < _EminPID.size() ; idx++)
   {
     if(_EminPID.at(idx)==pid)
       return _Emin.at(idx);
   }
 return 0;
} 

double Settings::getPmin_fromPID(int pid){
 for(unsigned int idx = 0 ; idx < _PminPID.size() ; idx++)
   {
     if(_PminPID.at(idx)==pid)
       return _Pmin.at(idx);
   }
 return 0;
} 


double Settings::getVzmin_fromPID(int pid){
 for(unsigned int idx = 0 ; idx < _VzPID.size() ; idx++)
   {
     if(_VzPID.at(idx)==pid)
       return _Vzmin.at(idx);
   }
 return -99999;
} 

double Settings::getVzmax_fromPID(int pid){
 for(unsigned int idx = 0 ; idx < _VzPID.size() ; idx++)
   {
     if(_VzPID.at(idx)==pid)
       return _Vzmax.at(idx);
   }
 return 99999;
} 

double Settings::getBetamin_fromPID(int pid){
 for(unsigned int idx = 0 ; idx < _BetaPID.size() ; idx++)
   {
     if(_BetaPID.at(idx)==pid)
       return _Betamin.at(idx);
   }
 return -99999;
} 

double Settings::getBetamax_fromPID(int pid){
 for(unsigned int idx = 0 ; idx < _BetaPID.size() ; idx++)
   {
     if(_BetaPID.at(idx)==pid)
       return _Betamax.at(idx);
   }
 return 99999;
} 

double Settings::getChi2max_fromPID(int pid){
 for(unsigned int idx = 0 ; idx < _chi2PID.size() ; idx++)
   {
     if(_chi2PID.at(idx)==pid)
       return _chi2max.at(idx);
   }
 return 99999999;
} 

bool Settings::needsChi2PidCut(int pid){
  for(unsigned int idx = 0 ; idx < _chi2PID.size() ; idx++)
    {
      if(_chi2PID.at(idx)==pid)
	return true;
    }
  return false;
}


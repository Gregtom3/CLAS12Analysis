#ifndef Kinematics_h
#define Kinematics_h

#include "TLorentzVector.h"
#include "TVector3.h"
#include <iostream>

using namespace std;

class Kinematics {
 public:
  static double Q2(double E1, double E2, double cth);
  static double x(double Q2, double s, double y);
  static double Mx(double,double,double,double,double,double);
  static double Px(double P, double th, double phi);
  static double Py(double P, double th, double phi);
  static double Pz(double P, double th, double phi);
  static double Pt(double Px, double Py);
  static double P(double Px, double Py, double Pz);
  static double E(double M, double P);
  static double cth(double Px, double Py, double Pz);
  static double y(double E1, double E2);
  static double nu(double E1, double E2);
  static double W(double Q2, double mT, double nu);
  static double th(double Pt, double Pz);
  static double eta(double th);
  static double phi(double Px, double Py);
  static double xF(TLorentzVector, TLorentzVector, TLorentzVector, double);
  static double Pt_COM(TLorentzVector, TLorentzVector, TLorentzVector);
  static double phi_h(TLorentzVector,TLorentzVector,TLorentzVector,TLorentzVector);
  static double phi_R(TLorentzVector,TLorentzVector,TLorentzVector,TLorentzVector, int);
  static double com_th(TLorentzVector, TLorentzVector);
};
#endif

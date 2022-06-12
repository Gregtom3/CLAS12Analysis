#include "Kinematics.h"
#include "Constants.h"
#include <math.h>
using namespace std;

double Kinematics::Q2(double E1, double E2, double cth){
  return 2.0 * E1 * E2 * (1.0 - cth);
}

double Kinematics::x(double Q2, double s, double y){
  return Q2/s/y;
}

double Kinematics::Px(double P, double th, double phi){
  return P * sin(th) * cos(phi);
}

double Kinematics::Py(double P, double th, double phi){
  return P * sin(th) * sin(phi);
}

double Kinematics::Pz(double P, double th, double phi){
  return P * cos(th);
}

double Kinematics::Pt(double Px, double Py){
  return pow(Px*Px + Py*Py,0.5);
}

double Kinematics::P(double Px, double Py, double Pz){
  return pow(Px*Px + Py*Py + Pz*Pz,0.5);
}

double Kinematics::E(double M, double P){
  return pow(M*M+P*P,0.5);
}

double Kinematics::cth(double Px, double Py, double Pz){
  double Pt = Kinematics::Pt(Px,Py);
  return Pz / (pow(Pz*Pz+Pt*Pt,0.5));
}

double Kinematics::y(double E1, double E2){
  return (E1-E2)/E1;
}

double Kinematics::nu(double E1, double E2){
  return E1-E2;
}

double Kinematics::W(double Q2, double mT, double nu){
  return sqrt(-Q2 + pow(mT,2) + 2 * mT * nu);
} 

double Kinematics::th(double Pt, double Pz){
  return abs(atan(Pt/Pz));
}

double Kinematics::eta(double th){
  return -1.0 * log ( tan ( th / 2.0 ) );
}

double Kinematics::phi(double Px, double Py){
  return atan2(Py,Px);
}

double Kinematics::phi_h(TLorentzVector Q, TLorentzVector L, TLorentzVector p1, TLorentzVector p2){
  TLorentzVector ph = p1 + p2;
  TLorentzVector r = 0.5*(p1-p2);

  TVector3 q(Q.Px(), Q.Py(), Q.Pz());
  TVector3 l(L.Px(), L.Py(), L.Pz());
  TVector3 Ph(ph.Px(), ph.Py(), ph.Pz());

  TVector3 qcrossl = q.Cross(l);
  TVector3 qcrossPh = q.Cross(Ph);

  double factor1 = (qcrossl*Ph)/abs(qcrossl*Ph);
  double factor2 = (qcrossl*qcrossPh)/qcrossl.Mag()/qcrossPh.Mag();

  return factor1*acos(factor2);
}

double Kinematics::xF(TLorentzVector q, TLorentzVector p, TLorentzVector init_target, double W){
  TLorentzVector com = q+init_target;
  TVector3 comBOOST = com.BoostVector();
  TLorentzVector qq = q;
  TLorentzVector pp = p;
  qq.Boost(-comBOOST);
  pp.Boost(-comBOOST);
  return 2*(qq.Vect().Dot(pp.Vect()))/(qq.Vect().Mag()*W);
}

double Kinematics::phi_R(TLorentzVector Q, TLorentzVector L, TLorentzVector p1, TLorentzVector p2, int method){
  TLorentzVector ph = p1 + p2;
  TLorentzVector r = 0.5*(p1-p2);

  TVector3 q(Q.Px(), Q.Py(), Q.Pz());
  TVector3 l(L.Px(), L.Py(), L.Pz());
  TVector3 R(r.Px(), r.Py(), r.Pz());

  TVector3 Rperp;
  // Different methods for defining Rperp
  switch(method){
  case 0:   // -- HERMES 0803.2367 angle
    Rperp=R-(q*R)/(q*q)*q;
    break;
  case 1: // -- HERMES 0803.2367 angle, but used Matevosyan et al 1707.04999
          //    to obtain R_T vector
    TLorentzVector init_target;
    init_target.SetPxPyPzE(0,0,0,0.938272);
    double z1 = (init_target*p1)/(init_target*Q);
    double z2 = (init_target*p2)/(init_target*Q);
    TVector3 P1(p1.Px(), p1.Py(), p1.Pz());
    TVector3 P2(p2.Px(), p2.Py(), p2.Pz());
    TVector3 P1perp = P1-(q*P1)/(q*q)*q;
    TVector3 P2perp = P2-(q*P2)/(q*q)*q;
    Rperp = (z2*P1perp-z1*P2perp)*((1)/(z1+z2));
    break;
  }

  TVector3 qcrossl = q.Cross(l);
  TVector3 qcrossRperp = q.Cross(Rperp);

  double factor1 = (qcrossl*Rperp)/abs(qcrossl*Rperp);
  double factor2 = (qcrossl*qcrossRperp)/qcrossl.Mag()/qcrossRperp.Mag();

  return factor1*acos(factor2);
}

double Kinematics::com_th(TLorentzVector P1, TLorentzVector P2){
  TLorentzVector Ptotal = P1+P2;
  TVector3 comBOOST = Ptotal.BoostVector();
  Ptotal.Boost(-comBOOST);
  P1.Boost(-comBOOST);
  return P1.Angle(comBOOST);
}

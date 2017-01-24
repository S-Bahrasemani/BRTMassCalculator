#ifndef BRT_INTERFACE_H
#define BRT_INTERFACE_H

// stdl includes
#include <iostream>
#include <vector>
#include <string>
#include <python2.6/Python.h>
// #include <numpy/arrayobject.h>
// #include <numpy/npy_common.h>

// local includes
#include "xJobOptions/xJobOptions.h"
#include "xTools/xTools.h"

class Python 
{
 public: 
  Python();
  Python(int argc, wchar_t** argv);
  // destructor to close python
  ~ Python() ;
}; 

// FIX ME! the order of variables matters, this is the order which has been used for training
// KEEP IT THE SAME PLS.
struct hh_features_t
{
  double ditau_dr;
  double met_et;
  double ditau_scal_sum_pt;
  double ditau_vect_sum_pt;
  double ditau_mt_lep0_met;
  double ditau_mt_lep1_met;
  double ditau_vis_mass;
  double ditau_dphi;
  double ditau_tau0_pt;
  double ditau_tau1_pt;
  double ditau_met_min_dphi;
  double ditau_met_lep0_cos_dphi;
  double ditau_met_lep1_cos_dphi;
  double ditau_dpt;
  
  //* low correlation features *//
  /*
      double met_sumet;
      double ditau_mt;
      double ditau_pt_diff;
      double ditau_cos_theta;
      double ditau_coll_approx_m;
      double ditau_coll_approx_x0;
      double ditau_coll_approx_x1;
      double ditau_pt_ratio;
  */
};

struct lh_features_t
{
  double lephad transverse_mass;

}; // struct lh_features

struct features_t
{
  hh_features_t hh_features;
  lh_features_t lh_features;
};

class BRTMassCalculator
{
  Python* scope;
  PyObject* pModule, pName;
  
 public: 
  BRTMassCalculator();
  BRTMassCalculator(int argc, wchar_t** argv);
  virtual ~ BRTMassCalculator();
  
  double CalculateMass(features_t *pfeatures,
		       std::sring channel,
		       const xAOD::EventInfo *peventinfo,
		       const xAOD::IParticle *ptau0,
		       const xAOD::IParticle *ptau1,
		       const xAOD::JetContainer *pjets,
		       const xAOD::MissingET *pmet);
  
  std::vector <double> CalculateTrainingFeature (features_t *pfeatures,
						 std::sring channel,
						 const xAOD::EventInfo *peventinfo,
						 const xAOD::IParticle *ptau0,
						 const xAOD::IParticle *ptau1,
						 const xAOD::JetContainer *pjets,
						 const xAOD::MissingET *pmet);
 private:
  
						 
}; //end class BRTMassCalculator

#endif // BRT_INTERFACE_H

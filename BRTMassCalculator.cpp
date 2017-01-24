#include"BRTMassCalculator.h"

// this module helps to unpickle the trained model (in python),
// predic the di-tau mass for every event from the xAOD files:


// // constructure 
// Python::Python()
// {
//   Py_Initialize();
// }

// Python::Python(int argc, wchar_t** argv)
// {
//   Py_Initialize();
//   //Py_Sys_SetArgv(argc, argv);
// }
  
// // destructor to close python
// Python::~ Python() {Py_Finalize();}

BRTMassCalculator::BRTMassCalculator()
{}

// BRTMassCalculator::BRTMassCalculator(int argc, wchar_t** argv)
// {
//   // Python* scope;
//   // PyObject* pModule;
  
//   // scope = new Python(argc, argv);
//   // //pName = PyString_FromString("BRTMassCalculator");
//   // pModule = PyImport_Import(PyString_FromString("BRTMassCalculator"));
//   // if (!pModule) std::cout<< "Failed to load BRTMassCalculator.py"<<std::endl;
//   //Py_DECREF(pName);
// }
  
/* release BRTMassCalculator.py */
BRTMassCalculator::~ BRTMassCalculator()
{
  // if (pModule) Py_DECREF(pModule);
  // else{}
  
  // if (scope) delete scope;
  // else{}
}

std::vector <double> CalculateTrainingFeatures(std::sring channel,
					       const xAOD::EventInfo *peventinfo,
					       const xAOD::IParticle *ptau0,
					       const xAOD::IParticle *ptau1,
					       const xAOD::JetContainer *pjets,
					       const xAOD::MissingET *pmet,
					       const xTools *ptool)
{
  /* To caculate the kinematics needed for the BRT training on the fly */
  if (channel == "hadhad")
    {
      hh_features_t hh_features = features.hh_features;
      xTools *ptools = new xTools();
      std::vector <double> train_features;
      // calculate training features
      if (channel!="hadhad")
	{
	  throw "Channel "<< channel << "is not implemented yet";
	}
      
      hh_features.ditau_dr = m_tools.DeltaR(ptau0,ptau1);
      train_features.push_back(hh_features.ditau_dr);
      
      hh_features.met_et = pmet->met() * energyMetric();
      train_features.push_back(hh_features.met_et);
      
      hh_features.ditau_scal_sum_pt = m_tools->ScalarSumPt(ptau0,ptau1)* energyMetric();
      train_features.push_back(hh_features.ditau_scal_sum_pt);
      
      hh_features.ditau_vect_sum_pt = m_tools->VectorSumPt(ptau0,ptau1)* energyMetric();
      train_features.push_back(hh_features.ditau_vect_sum_pt);
      
      hh_features.ditau_mt_lep0_met = m_tools->MT(ptau0, pmet)*energyMetric();;
      train_features.push_back(hh_features.ditau_mt_lep0_met);
      
      hh_features.ditau_mt_lep1_met = m_tools->MT(ptau1, pmet)*energyMetric();;
      train_features.push_back(hh_features.ditau_mt_lep1_met);
      
      hh_features.ditau_vis_mass = m_tools->MassVisible(ptau0,ptau1)* energyMetric();
      train_features.push_back(hh_features.ditau_vis_mass);
      
      hh_features.ditau_dphi = m_tools.DeltaPhi(ptau0,ptau1);
      train_features.push_back(hh_features.ditau_dphi);
      
      hh_features.ditau_ptau0_pt = ptau0->pt();
      train_features.push_back(hh_features.ditau_ptau0_pt);
      
      hh_features.ditau_ptau1_pt = ptau1->pt();
      train_features.push_back(hh_features.ditau_ptau1_pt);
      
      hh_features.ditau_met_min_dphi = m_tools->METmindeltaphi(p0, p1, pmet);;
      train_features.push_back(hh_features.ditau_met_min_dphi);
      
      hh_features.ditau_met_lep0_cos_dphi = m_tools->DeltaPhi(ptau0->phi(), pmet->phi());;
      train_features.push_back(hh_features.ditau_met_lep0_cos_dphi);
      
      hh_features.ditau_met_lep1_cos_dphi = tools->DeltaPhi(ptau1->phi(), pmet->phi());;
      train_features.push_back(hh_features.ditau_met_lep1_cos_dphi);
      
      hh_features.ditau_dpt = (ptau0->pt - ptau1->pt()) * energyMetric();
      train_features.push_back(hh_features.hh_features.ditau_dpt);
      
      return train_features;
    }
  
  double CalculateMass(std::string channel,
		       const xAOD::EventInfo *peventinfo,
		       const xAOD::IParticle *ptau0,
		       const xAOD::IParticle *ptau1,
		       const xAOD::JetContainer *pjets,
		       const xAOD::MissingET *pmet)
  {
    /* calculate features on fly*/
    try
      {
	std::vector<double> train_features = BRTMassCalculator::CalculateTrainingFeature(channel,
										     peventinfo,
											 ptau0,
										     pjets,
										     pmet);
      }
    catch(const char* msg)
      {
	std::cerr<<msg<<std::endl;
      }
    // To evaluate the mass from the input features for a  specific channel  
    Py_Initialize();
    PyObject  *pModule, *pFunc, *pArgs, *pValue;
    PyObject *ptrain_features;
    double mass(-1111.);
    
    //scope = new Python(argc, argv);
    //pName = PyString_FromString("BRTMassCalculator");
    PyRun_SimpleString("import sys; import os");
    PyRun_SimpleString("sys.path.append(os.getcwd()+\"/\")");
    pModule = PyImport_Import(PyString_FromString("BRTMassCalculator"));
    
    if (pModule == NULL)
      {
	std::cout<< "Failed to load BRTMassCalculator.py"<<std::endl;
	exit(-1);
      }
  
  pFunc = PyObject_GetAttrString(pModule, "CalculateMass");
  if (!pFunc) 
    {
      std::cout<<"ERROR in accessing CalculateMass attribute"<<std::endl;
      exit(-1);
    }

  unsigned int  feats_size = train_features.size();
  ptrain_features = PyList_New(feats_size);
  //Py_XINCREF(ptrain_features);
  for(unsigned int i = 0; i < feats_size; ++i )
    {
      double curVal = train_features.at(i);
      PyList_SetItem(ptrain_features, i, PyFloat_FromDouble(curVal));
    }
  
  /* prepare the function argument which is a list */
  pArgs = PyTuple_New(1);
  PyTuple_SetItem(pArgs, 0, ptrain_features);
  
  if (pArgs==NULL)  std::cout<<"HERE!!"<<std::endl;
  //Py_INCREF(pFunc);
  pValue = PyObject_CallObject(pFunc, pArgs);
  mass = PyFloat_AsDouble(pValue);
  // // Py_XDECREF(pValue);
  Py_Finalize();    
  return mass;
} //end CalculateMass(...)


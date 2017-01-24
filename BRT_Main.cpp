#include <iostream>
#include <vector>
#include <string>
#include "BRT_Interface.h"  


int main()
{
  double mass = 10.0;
  
  std::vector<double> feats(14,100.);
  BRTMassCalculator brt;
  //brt = new BRTMassCalculator;
  
  std::string channel = "hh";
  try
    {
      feats = brt.CalculateTrainingFeatures(features_t *pfeatures,
					    std::sring channel,
					    const xAOD::EventInfo *peventinfo,
					    const xAOD::IParticle *ptau0,
					    const xAOD::IParticle *ptau1,
					    const xAOD::JetContainer *pjets,
					    const xAOD::MissingET *pmet
					    );
    }
  catch(const char* msg)
    {
      std::cerr<<msg<<std::endl;
    } 
  mass = brt.CalculateMass(feats,channel);
  std::cout<<"BRT MASS: "<<mass<<std::endl;
  
  return 0;
}

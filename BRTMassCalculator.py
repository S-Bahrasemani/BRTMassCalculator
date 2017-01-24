import os, array, logging
import pickle
# import compiledtrees
# import sklearn.ensemble
import numpy as np

trainedModel = '03302016_GradientBoostingRegressor_mix_reco_hh_None.pkl'
def CalculateMass(input_features):
    """ load the pickled trained regressor, 
    calculate the mass and return it
    
    Parameters
    ----------
    input_features: list, 
    1-d list of input features.
    
    Return
    ------
    mass: float,
    predicted mass

    """
    
    with open (trainedModel, 'rb') as model:
        print (50*"==")
        print ("loading trained model << %s >>"%trainedModel)
        print (50*"==")
        regressor = pickle.load(model)
        model.close()
        # compiled_predictor = compiledtrees.CompiledRegressionPredictor(regressor)
        # mass = compiled_predictor.predict(input_features)
        mass = regressor.predict(input_features)
    return mass

# ## small test
# input_features =np.array([[10.]*14])
# print input_features.shape
# m = CalculateMass(input_features)
# print m

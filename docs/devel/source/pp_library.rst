Physical property library
=========================

For the OpSim project a new physical property library will be developed. The library will be able to handle multiple physical properties as described in the OpSim specs. Properties that will be initially supported are; 

  - Molweight
  
  - Vapor pressure
  
  - Viscosity
  
    - Liquid/solid
	
    - Vapor
	
  - Density
  
  - Thermal conductivity
  
    - Liquid/solid
	
    - Vapor
	
  - Surface tension
  
  - Enthalpy
  
    - Liquid/solid
	
    - Vapor
	
  - Entropy
  
    - Liquid/solid
	
    - Vapor

The library will calculate these properties according to different models and over different ranges, as appropriate. A property database will hold the following data;

  - Component name
  
  - Atom data
  
  - Property models
  
    - Model type
	
    - Model coefficients
	
    - Temperature validity range
	
    - Pressure validity range
	
    - Data source

A single component will possibly support multiple models that can overlap or extend the range. For the first release overlapping will not be supported, but future versions might support overlapping and implement some sort of smooth transition from one to the other model.

Calculating a property
----------------------
As mentioned the library will handle multiple models but expose only a simple API to the user. Below is an example on how this works when a user requests to calculate the vapour pressure for a component.


Registering models
------------------
All the models in the library are “hard coded”. Per physical parameter a separate unit is made that implements the basic calculations. Per calculation a flag is created and in the API function this flag is used to call the appropriate model function.

New parameter models
--------------------
A GUI application will be created that allows to enter model values and convert them to model parameters. For instance Antoine coefficients are often given in mmHg. The library will expect them in predefined (SI) units as will be described in the library documentation. The GUI application will allow these parameters to be converted. The source data will be stored in a separate database for future reference. It implements additionally the possibility to do regression on measured data.
The GUI application will allow to regress data provided by the user in any unit desired and converted to the correct unit.
Unit converter
==============
OpSim has its own built in unit converter. The unit converter allows conversion from an to different physical quantities. The units are also classified as being part of one of the following unit systems:

  - metric
  - imperial
  - US customary

The default setting of the unit system can be set in the settings window. Once set, the model will display its variables that are linked to a unit according to the default unit system. Within OpSim each numerical variable is always linked to a unit system.

Converting units
----------------
The unit converter stores for each unit the multiplier and bias to get to the base unit, which is always defined as the first unit. A variable in OpSim is defined in an arbitrary unit that is defined on creation of the variable. When the user selects a different unit, OpSim will convert the unit from the internal defined unit to the desired unit and display this in the user interface. Internally OpSim will reamin calculating in the defined unit, until the definition of the variable changes.

Unit difference
---------------
Checking the box Unit difference enables you to convert the units of a difference. As an example the unit conversion is given of a temperature and of a temperature difference:

.. note::

   A temperature of 1C = 274K or 1C = 33.8F
   
   A temperature difference of 1C = 1K or 1C = 1.8F

Absolute units
--------------
Some units might be missing while they are actually available. For instance when doing a heat transfer coefficient unit conversion and you would like to convert 1 kW/m2/K to Btu/hr/ft2/F the last unit does not exist. In this case Btu/hr/ft2/R can be used. Zero degree on both Kelvin and Rankine scale are the absolute zero. Celsius scales with Kelvin as Fahrenheit scales with Rankine. Therefore:

.. note::

   1 kW/m2/K = 1 kW/m2/C (unit is not available)
   
   1 kW/m2/K = 176 Btu/hr/ft2/R (unit is available)
   
   176 Btu/hr/ft2/R = 176 Btu/hr/ft2/F (unit is not available)
 
Defining custom units
---------------------
Unit conversions definitions can changed and added by adjusting the be unit script that is being loaded at startup of OpSim. The script is written in xxx and defines a conversion in the following way.

::

   UNC_define_conversion(quantity, unit: string; multiplier, bias: double; flag: word);
   
The provided flag can be anything of the following;

  - UNC_FLAG_METRIC
  - UNC_FLAG_IMPERIAL
  - UNC_FLAG_US_CUSTOMARY
  
The flags can be ORed to assign the defined unit to multiple unit systems.
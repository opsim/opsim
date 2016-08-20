Physical properties
===================
The physical properties library is able to handle multiple physical properties and models. Properties that will be initially supported are; 

Andrade                    
-------
Andrade equation for calculating the liquid viscosity

.. math::
  \ln(η) = A + \frac{B}{T} + C \cdot \ln(T)


Antoine
-------
The Antoine equation is a vapor pressure equation and describes the relation between vapor pressure and temperature for pure components. The Antoine equation is derived from the Clausius–Clapeyron relation.

.. math::
  \log(p) = A - \frac{B}{C + T}

Antoine viscosity
-----------------
Antoine equation for the viscosity

.. math:: 
  \ln(η) = a + \frac{b}{T+c}

Barin
-----
Barin equations for thermophysical property data

.. math::
  G = a + b \cdot T + c \cdot (T \cdot \ln(T)) + d \cdot T^2 + e \cdot T^3 + f \cdot T^4 + \frac{g}{T} + \frac{h}{T^2}
  
BWR
---
BWR-equation of state

.. math::
  p = R \cdot T \cdot d + (b0 \cdot R \cdot T - a0 ­ \frac{c0}{T^2} \cdot d2 + (b0 \cdot R \cdot T - a0) \cdot d3 +a \cdot α \cdot d6 + (c \cdot \frac{d3}{T^2}) \cdot (1 + γ \cdot d2) \cdot \exp{(-γ \cdot d2)}

Cragoe
------
Cragoe vapor pressure equation

.. math::
  \log(p) = a + \frac{b}{T} + c \cdot T + d \cdot T^2

DIPPR101
--------
.. math::
  property = \exp{(A + \frac{B}{T} + C \cdot \ln(T) + D \cdot T^E)}

DIPPR102
--------
DIPPR equation for the gas viscosity at 0 atm pressure and the gas thermal conductivity

.. math::
  property = A \cdot T \frac{B}{1 + \frac{C}{T} + \frac{D}{T^2}}

DIPPR104
--------

.. math::
  property = A + \frac{B}{T} + \frac{C}{T^3} + \frac{D}{T^8} + \frac{E}{T^9}

DIPPR105
--------
Liquid density equation.

.. math::
  property = \frac{A}{B^{(1 + (1 - \frac{T}{C}))^D}}

DIPPR106
--------

.. math::
  property = A \cdot (1-T_r)^{(B + C \cdot T_r + D \cdot T_r^2)}
.. math::
  T_r = \frac{T}{T_{crit}}

DIPPR107
--------
DIPPR equation for the ideal heat capacity

.. math::
  property = A + B \cdot \Bigg(\frac{\frac{C}{T}}{\sinh(\frac{C}{T})}\Bigg)^2 + D \cdot \Bigg(\frac{\frac{E}{T}}{\cosh(\frac{E}{T})}\Bigg)^2

heat capacity (ASPEN)
---------------------
[7]-equation for the solid heat capacity (page 3-102)

.. math::
  Cp = c1 + c2 \cdot T + c3 \cdot T^2 + \frac{c4}{T} + \frac{c5}{T^2} + \frac{c6}{\sqrt{T}}

Jones-Dole
----------
Jones-Dole equation

.. math::
  \frac{η}{η_0} = 1 + a \cdot \sqrt{c} + b \cdot c

liquid viscosity (DIPPR)
------------------------
DIPPR equation for the liquid viscosity

.. math::
  \ln(η) = c1 + \frac{c2}{T} + c3 \cdot \ln(T) + c4 \cdot T^{c5}

mod.Antoine( Aspen)
-------------------
modified Antoine vapor pressure equation ([7], page 3-80)

.. math::
  \ln(p) = A + \frac{B}{T+C} + D \cdot \ln(T) + E \cdot T^F

mod.Antoine( Hysys)
-------------------
modified Antoine vapor pressure equation (Hysys[9], page A-36)

.. math::
  \ln(p) = A + \frac{B}{T+C} + D \cdot T + E \cdot \ln(T) + F \cdot T^G

Peng-Robinson
-------------
standard Peng-Robinson equation of state ([7], page 3-34)

.. math::
  p = R \cdot T/(v_m-b) ­a/[v_m \cdot (v_m+b)+b \cdot (v_m-b)]

Peng-Robinson-Boston-Mathias
----------------------------
Peng-Robinson-Boston-Mathias equation of state ([7], page 3­25)

.. math::
  p = R \cdot T/(v_m-b) ­a/[v_m \cdot (v_m+b)+b \cdot (v_m-b)]
  
Polynomial
----------
Polynomial function where x can be any property.

.. math::
  y = a + b \cdot x + c \cdot x^2 + ...+ n \cdot x^n
  
Redlich-Kwong
-------------
Redlich-Kwong equation of state ([7], page 3-27)

.. math::
  a = \frac{0.42748 \cdot R^2 \cdot T^{2.5}}{P_c}
.. math::
  b = \frac{0.08664 \cdot R \cdot T_c}{P_c}
.. math::
  p = {\frac{R \cdot T}{v_m-b}} - {\frac{a}{\sqrt{T} \cdot v_m \cdot (v_m+b)}}
  
Redlich-Kwong-Aspen
-------------------
Aspen modification of the Redlich-Kwong equation of state( [7], page 3-28)

.. math::
  p = \frac{R \cdot T}{v_m-b} - \frac{a}{v_m \cdot (v_m+b)} 
  
with mixing rules
  
Redlich-Kwong-Soave
-------------------
standard Redlich-Kwong-Soave equation of state ([7], page 3­35)

.. math::
  p = \frac{R \cdot T}{v_m-b} - \frac{a}{v_m \cdot (v_m+b)} 
  
with mixing rules

Redlich-Kwong-Soave-Boston-Mathias
----------------------------------
Redlich-Kwong equation of state with Boston-Mathias alpha function ([7], page 3-29)

.. math::
  p = \frac{R \cdot T}{v_m-b} - \frac{a}{v_m \cdot (v_m+b)} 
  
with mixing rules

Riedel
------
Riedel vapor pressure equation

.. math::
  \ln(p) = a - \frac{b}{T} + c \cdot T + d \cdot T^2 + e \cdot \ln(T)

Riedel therm.cond.
------------------
Riedel equation for thermal conductivities

.. math::
  κ = a  \cdot  (1 + 20/3 \cdot (1 - \frac{T}{T_{crit}})^\frac{2}{3})
  
suface tension (DIPPR)
----------------------
DIPPR correlation for surface tension

.. math::
  T_r = \frac{T}{T_{crit}}
.. math::
  σ = c1 \cdot (1-T_r)^{(c2 + c3 \cdot T_r + c4 \cdot T_r^2 + c5 \cdot T_r^3)}

thermal conductivity (NEL)
--------------------------
NEL equation for thermal conductivity

.. math::
  x=1-\frac{T}{T_{crit}}
.. math::
  κ = a \cdot (1 + b \cdot x^\frac{1}{3} + c \cdot x^\frac{2}{3} + d \cdot x)

vapor pressure\_1
-----------------
vapor pressure equation

.. math::
  \ln(p) = a + b \cdot T + \frac{c}{T} + \frac{d}{T^2}

Wagner
------
Wagner vapor pressure equation

.. math::
  x = 1 - \frac{T}{T_{crit}}
.. math::
  \ln(\frac{p}{p_{crit}}) = \frac{a \cdot x + b \cdot x^\frac{3}{2} + c \cdot x^3 + d \cdot x^6}{\frac{T}{T_{crit}}}

Wagner2
-------
2nd Wagner vapor pressure equation

.. math::
  x = 1 - \frac{T}{T_{crit}}
.. math::
  \ln(\frac{p}{p_{crit}}) = \frac{a \cdot x + b \cdot x^\frac{3}{2} + c \cdot x^3 + d \cdot x^7 + e \cdot x^9}{\frac{T}{T_{crit}}}

Wagner3
-------
Wagner vapor pressure equation

.. math::
  x = 1 - \frac{T}{T_{crit}}
.. math::
  \ln(\frac{p}{p_{crit}}) = \frac{a \cdot x + b \cdot x^\frac{3}{2} + c \cdot x^3 + d \cdot x^4}{\frac{T}{T_{crit}}}

Wrede
-----
Wrede vapor pressure equation

.. math::
  \log(p) = a - \frac{b}{T}

Wrede-ln
--------
Wrede vapor pressure equation

.. math::
  \ln(p) = a - \frac{b}{T}

Yuan/Mok
--------
Yuan - Mok equation for the heat capacity

.. math::
  cp = a + b  \cdot  \exp{\frac{-c}{T_n}}

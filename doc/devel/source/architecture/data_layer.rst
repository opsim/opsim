Data Access Layer
=================
OpSim will implement it's own file format that is in essence a binary representation of the internal data structure. The file format is 


1) The simulator will have input/output (I/O) capabilities to save and
   reload a simulation case from a file.

2) All the data comprising one simulation case is stored in only one
   file.

3) The file I/O operation will be performed through the embedded
   Relational Database Management System (DBMS): Firebird.

4) The technical reasoning to use a DBMS is:

   1. Low level storage operations are isolated from the simulator.

   2. We do not need to worry about a file format. This is left to the
      DBMS. If the file format of the DBMS changes, we only need to dump
      the data and inject in the new version. A custom file format would
      require exhaustive implementation of converting routines.

   3. Reliable data storage. DBMS uses sophisticated schemes to access
      the hard disk in order to protect data.

   4. Fragmented access to the data file. A custom data file commonly
      imposes a three stage cycle: reading the file at once, and storing
      it in the RAM; modifying the data in RAM and writing the whole
      file at once. DBMS allows read/write small pieces of information
      in a much more efficient manner.

   5. Concurrent access to the data in the simulation file. DBMS permits
      multiple applications accessing the very same simulation case,
      what is not allowed with a custom formatted file. A simulation
      case could be built simultaneously by many individuals remotely
      apart, via the internet, for example, or a local/wide area network
      (LAN/WAN).

5) The technical reasoning to use a Firebird is:

   1. Firebird is an industrial strength DBMS with decades of corporate
      use.

   2. Firebird is open source, free of charge, with a very proud active
      community. The licensing agreement even allows its use with
      commercial applications, without restrictions (LGPL like).

   3. Can be easily embedded into the simulator.

   4. Can be easily scaled up to client/server, allowing simultaneous
      access of multiple users.

   5. Available in several operating systems.

   6. Have a very small footprint and the most advantageous relation
      [feature set]/[footprint].

   7. Very fine integration support with Lazarus. Some components are
      directly compatible with Lazarus, but regarding the best one
      (IBObjects), I am not so sure. Anyway, there are native components
      for a data access in Lazarus.

Database Structure
------------------

1) The data utilized by the simulator is stored in two databases:

   1. *System Database*: SYSDATA.ODB, used to store intrinsic to the
      whole system of the simulator, e.g. preferences, environment
      conjurations, etc. The extension “ODB” stands for “Opsim DataBase”

   2. *Model Database*: [model-name].OSC, that store the whole data of
      the simulation model (case) currently opened at the simulator
      environment. The extension “OSC” stands for “Opsim Simulation
      Case”

The System Database
~~~~~~~~~~~~~~~~~~~

The system database is comprised by the following tables:

1) Units of measures used in conversions.

2) Sets of units.

3) What more???????

1. UNITS OF MEASUREMENT: holds data regarding the units of measurement
   used in the simulator. This table will be used to build custom sets
   of units.

+--------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+
| PHISICAL TABLE NAME: UNIT\_MEASURE   |                                                                      |                            |                          |
+======================================+======================================================================+============================+==========================+
| **FIELD NAME**                       | **FIELD DESCRIPTION**                                                | **FIELD TYPE**             | **FIELD SIZE (BYTES)**   |
+--------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+
| ID                                   | Internal identification used in referential integrity of the data.   | INTEGER (AUTO INCREMENT)   |                          |
+--------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+
| NAME                                 | Name of the unit.                                                    | TEXT                       | 70                       |
+--------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+
| SYMBOL                               | Symbol of the unit in normalized text (e.g. W/m2K or W/m^2.K)        | TEXT                       | 30                       |
+--------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+
| CONV\_FACTOR\_INTERSECT\_SI          | Intersect conversion factor to the metric system of measurement.     | TEXT                       | 100                      |
+--------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+
| CONV\_FACTOR\_SLOPE\_SI              | Slope conversion factor to the metric system of measurement.         | TEXT                       | 100                      |
+--------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+
| SOMETHING MORE??????????             |                                                                      |                            |                          |
+--------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+

1. UNIT SETS OF MEASUREMENT: hold unit sets of measurement. A set of
   units should be used while building the simulation model. Each unit
   of measurement contained in the set, gets an entry on this table.

+-------------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+
| PHISICAL TABLE NAME: UNIT\_MEASURE\_SET   |                                                                      |                            |                          |
+===========================================+======================================================================+============================+==========================+
| **FIELD NAME**                            | **FIELD DESCRIPTION**                                                | **FIELD TYPE**             | **FIELD SIZE (BYTES)**   |
+-------------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+
| ID                                        | Internal identification used in referential integrity of the data.   | INTEGER (AUTO INCREMENT)   |                          |
+-------------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+
| NAME                                      | Name of the unit set (e.g. SI, EuroSI, English System, etc.)         | TEXT                       | 50                       |
+-------------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+
| UNIT\_MEASURE\_ID                         | ID of the unit of measurement.                                       | INTEGER                    |                          |
+-------------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+
| SOMETHING MORE??????????                  |                                                                      |                            |                          |
+-------------------------------------------+----------------------------------------------------------------------+----------------------------+--------------------------+

1. PROPERTIES OF PURE SUBSTANCES: holds data regarding the properties of
   the pure substances.

+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| PHISICAL TABLE NAME: PROP\_PURE\_SUBST   |                                                                                                                             |                            |                          |
+==========================================+=============================================================================================================================+============================+==========================+
| **FIELD NAME**                           | **FIELD DESCRIPTION**                                                                                                       | **FIELD TYPE**             | **FIELD SIZE (BYTES)**   |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| ID                                       | Internal identification used in referential integrity of the data.                                                          | INTEGER (AUTO INCREMENT)   |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| CODE                                     | This is a custom code that may be show to the user. It is not used to create relationship between tables of the database.   | INTEGER                    |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| NAME                                     | Name of the substance.                                                                                                      | TEXT                       | 70                       |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| NAME\_IUPAC                              | IUPAC name o the substance.                                                                                                 | TEXT                       | 100                      |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| CHEMICAL\_FORMULA                        | The chemical formula of the substance (e.g. CH4)                                                                            | TEXT                       | 100                      |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| MOLECULAR\_WEIGHT                        | The molecular weight.                                                                                                       | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| TC                                       | Critical Temperature (K)                                                                                                    | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| PC                                       | Critical Pressure (Pa)                                                                                                      | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| VC                                       | Critical Volume (m3/kmol)                                                                                                   | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| API                                      | API gravity                                                                                                                 | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| ACC                                      | Accentic Factor                                                                                                             | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| NBP                                      | Normal Boiling Point (K)                                                                                                    | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| VNB                                      | Viscosity at NBP (kgf.hr/m2)                                                                                                | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| HF                                       | Ideal Gas Heat of Formation at Standard temperature (J/kg)                                                                  | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| GF                                       | Ideal Gas Free Energy of Formation at Standard Temperature (J/kg)                                                           | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| IG\_COEFF\_1                             | Coefficient 1 to calculate Ideal Gas Thermodynamic properties (J/kg)                                                        | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| IG\_COEFF\_2                             | Coefficient 2 to calculate Ideal Gas Thermodynamic properties (J/kg/K)                                                      | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| IG\_COEFF\_3                             | Coefficient 3 to calculate Ideal Gas Thermodynamic properties (J/kg/K2)                                                     | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| IG\_COEFF\_4                             | Coefficient 4 to calculate Ideal Gas Thermodynamic properties (J/kg/K3)                                                     | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| IG\_COEFF\_5                             | Coefficient 5 to calculate Ideal Gas Thermodynamic properties (J/kg/K4)                                                     | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| IG\_COEFF\_6                             | Coefficient 6 to calculate Ideal Gas Thermodynamic properties (J/kg/K5)                                                     | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| IG\_COEFF\_7                             | Coefficient 7 to calculate Ideal Gas Thermodynamic properties (J/kg/K6)                                                     | REAL                       |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| SOMETHING MORE??????????                 |                                                                                                                             |                            |                          |
+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+

The Model Database
~~~~~~~~~~~~~~~~~~

The model database contains:

1) Global options of the simulation model.

2) One different table for each type of component added to the model,
   being it a unit operation, a stream, or whatever.

3) Other tables??????

1. PUMPS ON THE SIMULATION MODEL: holds data regarding all the pumps
   components used in the simulation model. Each pump placed by the user
   on the PFD diagram, generates a record on this table. Since we may
   need other tables to keep other others unit operation or components,
   let establish a prefix now on “PFD\_” used to name each table
   containing unit operation/component of a simulation case.

+----------------------------------+------------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| PHISICAL TABLE NAME: PFD\_PUMP   |                                                                                                                              |                            |                          |
+==================================+==============================================================================================================================+============================+==========================+
| **FIELD NAME**                   | **FIELD DESCRIPTION**                                                                                                        | **FIELD TYPE**             | **FIELD SIZE (BYTES)**   |
+----------------------------------+------------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| ID                               | Internal identification used in referential integrity of the data.                                                           | INTEGER (AUTO INCREMENT)   |                          |
+----------------------------------+------------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| CODE                             | This is a custom code that may be shown to the user. It is not used to create relationship between tables of the database.   | INTEGER                    |                          |
+----------------------------------+------------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| NAME                             | Descriptive name of the pump.                                                                                                | TEXT                       | 70                       |
+----------------------------------+------------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| LABEL                            | Label used to identification on the PFD.                                                                                     | TEXT                       | 50                       |
+----------------------------------+------------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| HEAD                             | The head supported by the pump with zero flow rate                                                                           | REAL                       |                          |
+----------------------------------+------------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+
| HEAD\_UNIT\_ID                   | ID of the unit of measurement used to specify the HEAD field. Needed to do conversions.                                      | INTEGER                    |                          |
+----------------------------------+------------------------------------------------------------------------------------------------------------------------------+----------------------------+--------------------------+

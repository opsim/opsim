Interface
=========
The simulator will have an intuitive and friendly graphical user interface. The user interface is based on a native OpenGL toolkit. The main interface elements are show in the below figure.

.. figure:: ../media/UI_structures.svg
   :width: 75%
   :alt: OpSim main UI structures

The main structure is the `Window`. This structure contains a handle to the window context and to a list of `Areas` that are located on the window. OpSim will be capable to handle multiple windows simultaneously that interface with the datalayer and simulation engine. An `Area` is a structure that contains the information about the position on the screen in a regular grid type data structure. The user can resize, create and destroy `Areas` as desired.  
   

Spaces
------
OpSim will have a number of spaces that allow the user to manipulate the data from the model and display it in numerous forms. Some of the spaces that have been identified to be implemented are listed in the below table;

+---------------------+--------------------------------------------------------+
| Space               | Description                                            |
+=====================+========================================================+
| Component palette   | Used for unit operations and other components.         |
+---------------------+--------------------------------------------------------+
| Flowsheet editor    | To build the flowsheet. This space has a drag-drop     |
|                     | style, so that elements of the simulation model can be |
|                     | point-clicked and dragged from the component palette.  |
+---------------------+--------------------------------------------------------+
| Python console      | Used to write extensions for the UI and create models. | 
+---------------------+--------------------------------------------------------+
| Message console     | Display messages from the solver and generally from    |
|                     | the UI.                                                |
+---------------------+--------------------------------------------------------+
| Settings            | OpSim settings.                                        |
+---------------------+--------------------------------------------------------+
| Report              | Adds reporting capabilities to OpSim.                  |
+---------------------+--------------------------------------------------------+
| Spreadsheet         | Displays a representation of the model data and allows |
|                     | interaction with the model.                            | 
+---------------------+--------------------------------------------------------+
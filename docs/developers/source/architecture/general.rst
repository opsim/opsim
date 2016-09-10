Architecture design
===================

The whole simulator structure is divided in three major parts: user interface, engine and data accesses layer. Each one aspect has its own functionality and provides services to each other, through precisely defined boundaries. They exchange information only through these boundaries.

A diagram illustrating this is shown below:

.. figure:: ../media/architecture.svg
   :width: 80%
   :alt: OpSim general architecture

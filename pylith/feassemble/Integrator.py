#!/usr/bin/env python
#
# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# <LicenseText>
#
# ----------------------------------------------------------------------
#

## @file pylith/feassemble/Integrator.py
##
## @brief Python abstract base class for integration of operator
## actions with finite-elements.
##
## Factory: fe_integrator.

from pyre.components.Component import Component

# Integrator class
class Integrator(Component):
  """
  Python abstract base class for integration of actions with
  finite-elements.

  Factory: integrator.
  """

  # PUBLIC METHODS /////////////////////////////////////////////////////

  def __init__(self, name="integrator"):
    """
    Constructor.
    """
    Component.__init__(self, name, facility="integrator")
    self.cppHandle = None
    self.quadrature = None
    self.mesh = None
    return


  def setMesh(self, mesh):
    """
    Set mesh.
    """
    self.mesh = mesh
    return
  

  def initQuadrature(self, quadrature):
    """
    Initialize quadrature.
    """
    if self.cppHandle is None:
      raise ValueError("C++ handle not set.")
    
    self._info.log("Initialize quadrature")
    quadrature.initialize()
    self.quadrature = quadrature
    self.cppHandle.quadrature = self.quadrature.cppHandle
    return
  
  
# End of file 

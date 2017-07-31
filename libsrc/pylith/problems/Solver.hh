// -*- C++ -*-
//
// ======================================================================
//
// Brad T. Aagaard, U.S. Geological Survey
// Charles A. Williams, GNS Science
// Matthew G. Knepley, University of Chicago
//
// This code was developed as part of the Computational Infrastructure
// for Geodynamics (http://geodynamics.org).
//
// Copyright (c) 2010-2017 University of California, Davis
//
// See COPYING for license information.
//
// ======================================================================
//

/**
 * @file libsrc/problems/Solver.hh
 *
 * @brief Abstract C++ base class for using PETSc linear and nonlinear
 * solvers.
 */

#if !defined(pylith_problems_solver_hh)
#define pylith_problems_solver_hh

// Include directives ---------------------------------------------------
#include "problemsfwd.hh" // forward declarations

#include "pylith/topology/topologyfwd.hh" // USES SolutionFields
#include "pylith/utils/utilsfwd.hh" // USES EventLogger
#include "pylith/utils/petscfwd.h" // USES PetscMat

typedef struct {
  PetscPC pc;
  PetscMat A;
  const char *faultFieldName;
  PetscMat faultA;
} FaultPreconCtx;


// Solver ---------------------------------------------------------
/** @brief Abstract C++ base class for using PETSc linear and
 * nonlinear solvers.
 */
class pylith::problems::Solver
{ // Solver
  friend class TestSolver; // unit testing

// PUBLIC MEMBERS ///////////////////////////////////////////////////////
public :

  /// Constructor.
  Solver(void);

  /// Destructor
  virtual
  ~Solver(void);

  /// Deallocate PETSc and local data structures.
  virtual
  void deallocate(void);
  
  /** Create null space associated with rigid body motion for multiple
   * bodies..
   *
   * @param fields Solution fields.
   * @param numBodies Number of bodies in domain.
   * @param numMaterialsInBodies Array with number of materials in each body.
   * @param bodiesMaterialIds Array of material ids for each body.
   */
    void createNullSpaceBodies(const topology::SolutionFields& fields,
			       const PetscInt numBodies,
			       const PetscInt numMaterialsInBodies[],
			       const PetscInt bodiesMaterialIds[]);

  /** Initialize solver.
   *
   * @param fields Solution fields.
   * @param jacobian Jacobian of system.
   * @param formulation Formulation of system of equations.
   */
  virtual
  void initialize(const topology::SolutionFields& fields,
		  const topology::Jacobian& jacobian,
		  Formulation* const formulation);

// PROTECTED METHODS ////////////////////////////////////////////////////
protected :

  /** Setup preconditioner for preconditioning using split fields.
   *
   * @param pc PETSc preconditioner.
   * @param formulation Formulation of system of equations.
   * @param jacobian System Jacobian matrix.
   * @param fields Solution fields.
   */
  void _setupFieldSplit(PetscPC* const pc,
			Formulation* const formulation,
			const topology::Jacobian& jacobian,
			const topology::SolutionFields& fields);
  
  /** :MATT: :TODO: DOCUMENT THIS.
   */
  static
  int _epsilon(int i,
	       int j,
	       int k);

// PROTECTED MEMBERS ////////////////////////////////////////////////////
protected :

  Formulation* _formulation; ///< Handle to formulation for system of eqns.
  utils::EventLogger* _logger; ///< Event logger.
  PetscMat _jacobianPC; ///< Global preconditioning matrix.
  PetscMat _jacobianPCFault; ///< Preconditioning matrix for Lagrange constraints.
  FaultPreconCtx _ctx; ///< Context for preconditioning matrix for Lagrange constraints.

// NOT IMPLEMENTED //////////////////////////////////////////////////////
private :

  Solver(const Solver&); ///< Not implemented
  const Solver& operator=(const Solver&); ///< Not implemented

}; // Solver

#endif // pylith_problems_solver_hh


// End of file 

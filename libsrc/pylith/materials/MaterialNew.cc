// -*- C++ -*-
//
// ----------------------------------------------------------------------
//
// Brad T. Aagaard, U.S. Geological Survey
// Charles A. Williams, GNS Science
// Matthew G. Knepley, University of Chicago
//
// This code was developed as part of the Computational Infrastructure
// for Geodynamics (http://geodynamics.org).
//
// Copyright (c) 2010-2015 University of California, Davis
//
// See COPYING for license information.
//
// ----------------------------------------------------------------------
//

#include <portinfo>

#include "MaterialNew.hh" // implementation of object methods

#include "pylith/topology/Mesh.hh" // USES Mesh
#include "pylith/topology/Field.hh" // USES Field
#include "pylith/topology/FieldQuery.hh" // HOLDSA FieldQuery
#include "pylith/topology/CoordsVisitor.hh" // USES CoordsVisitor
#include "pylith/topology/Stratum.hh" // USES StratumIS

#include "spatialdata/spatialdb/SpatialDB.hh" // USES SpatialDB
#include "spatialdata/units/Nondimensional.hh" // USES Nondimensional
#include "spatialdata/spatialdb/GravityField.hh" // HASA GravityField

#include "petscds.h" // USES PetscDS

#include "journal/debug.h" // USES journal::debug_t
#include "journal/error.h" // USES journal::error_t

#include <cassert> // USES assert()
#include <stdexcept> // USES std::runtime_error

extern "C" PetscErrorCode DMPlexComputeResidual_Internal(DM dm, PetscInt cStart, PetscInt cEnd, PetscReal time, Vec locX, Vec locX_t, Vec locF, void *user);
extern "C" PetscErrorCode DMPlexComputeJacobian_Internal(DM dm, PetscInt cStart, PetscInt cEnd, PetscReal t, PetscReal X_tShift, Vec X, Vec X_t, Mat Jac, Mat JacP,void *user);
extern "C" PetscErrorCode DMPlexComputeJacobianAction_Internal(DM dm, PetscInt cStart, PetscInt cEnd, PetscReal t, PetscReal X_tShift, Vec X, Vec X_t, Vec Y, Vec z, void *user);


// ----------------------------------------------------------------------
// Default constructor.
pylith::materials::MaterialNew::MaterialNew(const int dimension) :
    _materialIS(NULL),
    _dimension(dimension),
    _id(0),
    _label(""),
    _gravityField(0)
{ // constructor
    const topology::FieldBase::DiscretizeInfo defaultInfo = {-1, -1, true};
    _auxFieldsFEInfo["default"] = defaultInfo;
} // constructor

// ----------------------------------------------------------------------
// Destructor.
pylith::materials::MaterialNew::~MaterialNew(void)
{ // destructor
    deallocate();
} // destructor

// ----------------------------------------------------------------------
// Deallocate PETSc and local data structures.
void
pylith::materials::MaterialNew::deallocate(void)
{ // deallocate
    PYLITH_METHOD_BEGIN;

    IntegratorPointwise::deallocate();
    delete _materialIS; _materialIS = NULL;
    delete _gravityField; _gravityField = NULL;

    PYLITH_METHOD_END;
} // deallocate

// ----------------------------------------------------------------------
// Get physical property parameters and initial state (if used) from database.
void
pylith::materials::MaterialNew::initialize(const pylith::topology::Field& solution)
{ // initialize
    PYLITH_METHOD_BEGIN;

    journal::debug_t debug("material");
    debug << journal::at(__HERE__)
          << "MaterialNew::intialize(solution="<<solution.label()<<")" << journal::endl;

    // Get cells associated with material
    const pylith::topology::Mesh& mesh = solution.mesh();
    PetscDM dmMesh = mesh.dmMesh(); assert(dmMesh);
    topology::CoordsVisitor::optimizeClosure(dmMesh);

    const bool includeOnlyCells = true;
    delete _materialIS; _materialIS = new topology::StratumIS(dmMesh, "material-id", _id, includeOnlyCells); assert(_materialIS);

    delete _auxFields; _auxFields = new topology::Field(mesh); assert(_auxFields);
    delete _auxFieldsQuery; _auxFieldsQuery = new topology::FieldQuery(*_auxFields); assert(_auxFieldsQuery);
    _auxFields->label("auxiliary fields");
    _auxFieldsSetup();
    _auxFields->subfieldsSetup();
    _auxFields->allocate();
    _auxFields->zeroAll();

    if (_auxFieldsDB) {
        assert(_normalizer);
        _auxFieldsQuery->openDB(_auxFieldsDB, _normalizer->lengthScale());
        _auxFieldsQuery->queryDB();
        _auxFieldsQuery->closeDB(_auxFieldsDB);
    } else { // else
        journal::error_t error("material");
        error << "Unknown case for setting up auxiliary fields." << journal::endl;
        throw std::logic_error("Unknown case for setting up auxiliary fields.");
    } // if/else
    _auxFields->complete();

    PYLITH_METHOD_END;
} // initialize


// ----------------------------------------------------------------------
// Compute RHS residual for G(t,s).
void
pylith::materials::MaterialNew::computeRHSResidual(PetscVec residualVec,
                                                   const PylithReal t,
                                                   const PylithReal dt,
                                                   const pylith::topology::Field& solution)
{ // computeRHSResidual
    PYLITH_METHOD_BEGIN;

    journal::debug_t debug("material");
    debug << journal::at(__HERE__)
          << "MaterialNew::computeRHSResidual(residualVec="<<residualVec<<", t="<<t<<", dt="<<dt<<", solution="<<solution.label()<<")" << journal::endl;

    _setFEKernelsRHSResidual(solution);

    PetscVec solutionDotNull = NULL; // No dependence on time derivative of solution in RHS.
    _computeResidual(residualVec, t, dt, solution, solutionDotNull);

    PYLITH_METHOD_END;
} // computeRHSResidual


// ----------------------------------------------------------------------
// Compute RHS Jacobian for G(t,s).
void
pylith::materials::MaterialNew::computeRHSJacobian(PetscMat jacobianMat,
                                                   PetscMat precondMat,
                                                   const PylithReal t,
                                                   const PylithReal dt,
                                                   const pylith::topology::Field& solution)
{ // computeRHSJacobian
    PYLITH_METHOD_BEGIN;

    journal::debug_t debug("material");
    debug << journal::at(__HERE__)
          << "MaterialNew::computeRHSJacobian(jacobianMat="<<jacobianMat<<", precondMat="<<precondMat<<", t="<<t<<", dt="<<dt<<", solution="<<solution.label()<<")" << journal::endl;

    _setFEKernelsRHSJacobian(solution);

    PetscVec solutionDotNull = NULL; // No dependence on time derivative of solution in RHS.
    const PylithReal tshift = 0.0; // No dependence on time derivative of solution in RHS, so shift isn't applicable.
    _computeJacobian(jacobianMat, precondMat, t, dt, tshift, solution, solutionDotNull);
    _needNewRHSJacobian = false;

    PYLITH_METHOD_END;
} // computeRHSJacobian


// ----------------------------------------------------------------------
// Compute LHS residual for F(t,s,\dot{s}).
void
pylith::materials::MaterialNew::computeLHSResidual(PetscVec residualVec,
                                                   const PylithReal t,
                                                   const PylithReal dt,
                                                   const pylith::topology::Field& solution,
                                                   PetscVec solutionDotVec)
{ // computeLHSResidual
    PYLITH_METHOD_BEGIN;

    journal::debug_t debug("material");
    debug << journal::at(__HERE__)
          << "MaterialNew::computeLHSResidual(residualVec="<<residualVec<<", t="<<t<<", dt="<<dt<<", solution="<<solution.label()<<", solutionDotVec="<<solutionDotVec<<")" << journal::endl;

    _setFEKernelsLHSResidual(solution);

    _computeResidual(residualVec, t, dt, solution, solutionDotVec);

    PYLITH_METHOD_END;
} // computeLHSResidual

// ----------------------------------------------------------------------
// Compute LHS Jacobian for F(t,s,\dot{s}).
void
pylith::materials::MaterialNew::computeLHSJacobianImplicit(PetscMat jacobianMat,
                                                           PetscMat precondMat,
                                                           const PylithReal t,
                                                           const PylithReal dt,
                                                           const PylithReal tshift,
                                                           const pylith::topology::Field& solution,
                                                           PetscVec solutionDotVec)
{ // computeLHSJacobianImplicit
    PYLITH_METHOD_BEGIN;

    journal::debug_t debug("material");
    debug << journal::at(__HERE__)
          << "MaterialNew::computeLHSJacobianImplicit(jacobianMat="<<jacobianMat<<", precondMat="<<precondMat<<", t="<<t<<", dt="<<dt<<", tshift="<<tshift<<", solution="<<solution.label()<<", solutionDotVec="<<solutionDotVec<<")" << journal::endl;

    _setFEKernelsLHSJacobianImplicit(solution);

    _computeJacobian(jacobianMat, precondMat, t, dt, tshift, solution, solutionDotVec);
    _needNewLHSJacobian = false;

    PYLITH_METHOD_END;
} // computeLHSJacobianImplicit


// ----------------------------------------------------------------------
// Compute LHS Jacobian for F(t,s,\dot{s}).
void
pylith::materials::MaterialNew::computeLHSJacobianLumpedInv(pylith::topology::Field* jacobianInv,
                                                            const PylithReal t,
                                                            const PylithReal dt,
                                                            const pylith::topology::Field& solution)
{ // computeLHSJacobianInverseExplicit
    PYLITH_METHOD_BEGIN;

    journal::debug_t debug("material");
    debug << journal::at(__HERE__)
          << "MaterialNew::computeLHSJacobianLumpedInv(jacobianInv="<<jacobianInv<<", t="<<t<<", dt="<<dt<<", solution="<<solution.label()<<")" << journal::endl;

    _setFEKernelsLHSJacobianExplicit(solution);

    PetscDS prob = NULL;
    PetscInt cStart = 0, cEnd = 0;
    PetscErrorCode err;

    PetscDM dmMesh = solution.dmMesh();
    PetscDM dmAux = _auxFields->dmMesh();
    PetscDMLabel dmLabel;

    // Pointwise function have been set in DS
    err = DMGetDS(dmMesh, &prob); PYLITH_CHECK_ERROR(err);

    // Get auxiliary data
    err = PetscObjectCompose((PetscObject) dmMesh, "dmAux", (PetscObject) dmAux); PYLITH_CHECK_ERROR(err);
    err = PetscObjectCompose((PetscObject) dmMesh, "A", (PetscObject) auxFields().localVector()); PYLITH_CHECK_ERROR(err);

    PetscVec vecRowSum = NULL;
    err = DMGetGlobalVector(dmMesh, &vecRowSum); PYLITH_CHECK_ERROR(err);
    err = VecSet(vecRowSum, 1.0); PYLITH_CHECK_ERROR(err);

    // Compute the local Jacobian action
    err = DMGetLabel(dmMesh, "material-id", &dmLabel); PYLITH_CHECK_ERROR(err);
    err = DMLabelGetStratumBounds(dmLabel, id(), &cStart, &cEnd); PYLITH_CHECK_ERROR(err);

    journal::error_t error("material");
    error << journal::at(__HERE__)
          << "DMPlexComputeJacobianAction_Internal() not yet implemented in PETSc knepley/pylith." << journal::endl;
#if 0 // NOT YET IMPLEMENTED IN petsc-dev knepley/pylith
    err = DMPlexComputeJacobianAction_Internal(dmMesh, cStart, cEnd, t, tshift, vecRowSum, NULL, vecRowSum, jacobianInv->localVector(), NULL); PYLITH_CHECK_ERROR(err);

    // Compute the Jacobian inverse.
    err = VecReciprocal(jacobianInv->localVector()); PYLITH_CHECK_ERROR(err);
#endif

    _needNewLHSJacobian = false;

    PYLITH_METHOD_END;
} // computeLHSJacobianInverseExplicit


// ----------------------------------------------------------------------
// Update state variables as needed.
void
pylith::materials::MaterialNew::updateStateVars(const pylith::topology::Field& solution)
{ // updateStateVars
    PYLITH_METHOD_BEGIN;

    journal::debug_t debug("material");
    debug << journal::at(__HERE__)
          << "MaterialNew::updateStateVars(solution="<<solution.label()<<")" << journal::endl;

    throw std::logic_error("MaterialNew::updateStateVars() not implemented");

    PYLITH_METHOD_END;
} // updateStateVars

// ----------------------------------------------------------------------
// Compute residual using current kernels.
void
pylith::materials::MaterialNew::_computeResidual(PetscVec residualVec,
                                                 const PylithReal t,
                                                 const PylithReal dt,
                                                 const pylith::topology::Field& solution,
                                                 PetscVec solutionDotVec)
{ // _computeResidual
    PYLITH_METHOD_BEGIN;

    assert(residualVec);
    assert(_auxFields);

    PetscDS prob = NULL;
    PetscInt cStart = 0, cEnd = 0;
    PetscErrorCode err;

    PetscDM dmMesh = solution.dmMesh();
    PetscDM dmAux = _auxFields->dmMesh();
    PetscDMLabel dmLabel;

    // Pointwise function have been set in DS
    err = DMGetDS(dmMesh, &prob); PYLITH_CHECK_ERROR(err);

    // Get auxiliary data
    err = PetscObjectCompose((PetscObject) dmMesh, "dmAux", (PetscObject) dmAux); PYLITH_CHECK_ERROR(err);
    err = PetscObjectCompose((PetscObject) dmMesh, "A", (PetscObject) _auxFields->localVector()); PYLITH_CHECK_ERROR(err);

    // Compute the local residual
    assert(solution.localVector());
    err = DMGetLabel(dmMesh, "material-id", &dmLabel); PYLITH_CHECK_ERROR(err);
    err = DMLabelGetStratumBounds(dmLabel, id(), &cStart, &cEnd); PYLITH_CHECK_ERROR(err);

    journal::debug_t debug("material");
    debug << journal::at(__HERE__)
          << "MaterialNew::_computeResidual() with material-id '"<<id()<<"' and cells ["<<cStart<<","<<cEnd<<")" << journal::endl;

    err = DMPlexComputeResidual_Internal(dmMesh, cStart, cEnd, PETSC_MIN_REAL, solution.localVector(), solutionDotVec, residualVec, NULL); PYLITH_CHECK_ERROR(err);

    PYLITH_METHOD_END;
} // _computeResidual


// ----------------------------------------------------------------------
// Compute Jacobian using current kernels.
void
pylith::materials::MaterialNew::_computeJacobian(PetscMat jacobianMat,
                                                 PetscMat precondMat,
                                                 const PylithReal t,
                                                 const PylithReal dt,
                                                 const PylithReal tshift,
                                                 const pylith::topology::Field& solution,
                                                 PetscVec solutionDotVec)
{ // _computeJacobian
    PYLITH_METHOD_BEGIN;

    assert(jacobianMat);
    assert(precondMat);

    PetscDS prob = NULL;
    PetscInt cStart = 0, cEnd = 0;
    PetscErrorCode err;
    PetscDM dmMesh = solution.dmMesh();
    PetscDM dmAux = _auxFields->dmMesh();
    PetscDMLabel dmLabel;

    // Pointwise function have been set in DS
    err = DMGetDS(dmMesh, &prob); PYLITH_CHECK_ERROR(err);

    // Get auxiliary data
    err = PetscObjectCompose((PetscObject) dmMesh, "dmAux", (PetscObject) dmAux); PYLITH_CHECK_ERROR(err);
    err = PetscObjectCompose((PetscObject) dmMesh, "A", (PetscObject) auxFields().localVector()); PYLITH_CHECK_ERROR(err);

    // Compute the local Jacobian
    assert(solution.localVector());
    err = DMGetLabel(dmMesh, "material-id", &dmLabel); PYLITH_CHECK_ERROR(err);
    err = DMLabelGetStratumBounds(dmLabel, id(), &cStart, &cEnd); PYLITH_CHECK_ERROR(err);

    journal::debug_t debug("material");
    debug << journal::at(__HERE__)
          << "MaterialNew::_computeJacobian() with material-id '"<<id()<<"' and cells ["<<cStart<< ","<<cEnd<<")" << journal::endl;
    err = DMPlexComputeJacobian_Internal(dmMesh, cStart, cEnd, t, tshift, solution.localVector(), solutionDotVec, jacobianMat, precondMat, NULL); PYLITH_CHECK_ERROR(err);

    PYLITH_METHOD_END;
} // _computeJacobian


// End of file
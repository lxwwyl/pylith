// -*- C++ -*-
//
// ---------------------------------------------------------------------------------------------------------------------
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
// ---------------------------------------------------------------------------------------------------------------------
//

#include <portinfo>

#include "IntegratorBoundary.hh" // implementation of object methods

#include "pylith/topology/Mesh.hh" // USES Mesh
#include "pylith/topology/Field.hh" // USES Field
#include "pylith/topology/CoordsVisitor.hh" // USES CoordsVisitor::optimizeClosure()

#include "spatialdata/spatialdb/GravityField.hh" // HASA GravityField

#include "pylith/utils/journals.hh" // USES PYLITH_JOURNAL_*

#include "petscds.h" // USES PetscDS

#include <cassert> // USES assert()
#include <stdexcept> // USES std::runtime_error

// ---------------------------------------------------------------------------------------------------------------------
// Local "private" functions.
namespace pylith {
    namespace feassemble {
        class _IntegratorBoundary {
public:

            /** Compute residual using current kernels.
             *
             * @param[out] residual Field for residual.
             * @param[in] integrator Integrator for boundary.
             * @param[in] kernels Kernels for computing residual.
             * @param[in] t Current time.
             * @param[in] dt Current time step.
             * @param[in] solution Field with current trial solution.
             * @param[in] solutionDot Field with time derivative of current trial solution.
             */
            static
            void computeResidual(pylith::topology::Field* residual,
                                 const pylith::feassemble::IntegratorBoundary* integrator,
                                 const std::vector<pylith::feassemble::IntegratorBoundary::ResidualKernels>& kernels,
                                 const PylithReal t,
                                 const PylithReal dt,
                                 const pylith::topology::Field& solution,
                                 const pylith::topology::Field& solutionDot);

        }; // _IntegratorBoundary

    } // feassemble
} // pylith

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
pylith::feassemble::IntegratorBoundary::IntegratorBoundary(pylith::problems::Physics* const physics) :
    Integrator(physics),
    _boundaryMesh(NULL),
    _boundaryLabel("") {
    _needNewRHSJacobian = false;
    _needNewLHSJacobian = false;
} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Destructor.
pylith::feassemble::IntegratorBoundary::~IntegratorBoundary(void) {
    deallocate();
} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Deallocate PETSc and local data structures.
void
pylith::feassemble::IntegratorBoundary::deallocate(void) {
    PYLITH_METHOD_BEGIN;

    Integrator::deallocate();

    delete _boundaryMesh;_boundaryMesh = NULL;

    PYLITH_METHOD_END;
} // deallocate


// ---------------------------------------------------------------------------------------------------------------------
// Set label marking boundary associated with boundary condition surface.
void
pylith::feassemble::IntegratorBoundary::setMarkerLabel(const char* value) {
    if (strlen(value) == 0) {
        throw std::runtime_error("Empty string given for boundary condition integrator label.");
    } // if

    _boundaryLabel = value;
} // setMarkerLabel


// ---------------------------------------------------------------------------------------------------------------------
// Get label marking boundary associated with boundary condition surface.
const char*
pylith::feassemble::IntegratorBoundary::getMarkerLabel(void) const {
    return _boundaryLabel.c_str();
} // getMarkerLabel


// ---------------------------------------------------------------------------------------------------------------------
// Get mesh associated with integration domain.
const pylith::topology::Mesh&
pylith::feassemble::IntegratorBoundary::getIntegrationDomainMesh(void) const {
    assert(_boundaryMesh);
    return *_boundaryMesh;
} // domainMesh


// ---------------------------------------------------------------------------------------------------------------------
void
pylith::feassemble::IntegratorBoundary::setKernelsRHSResidual(const std::vector<ResidualKernels>& kernels) {
    PYLITH_METHOD_BEGIN;
    PYLITH_JOURNAL_DEBUG("setKernelsRHSResidual(# kernels="<<kernels.size()<<")");

    _kernelsRHSResidual = kernels;

    PYLITH_METHOD_END;
} // setKernelsRHSResidual


// ---------------------------------------------------------------------------------------------------------------------
void
pylith::feassemble::IntegratorBoundary::setKernelsLHSResidual(const std::vector<ResidualKernels>& kernels) {
    PYLITH_METHOD_BEGIN;
    PYLITH_JOURNAL_DEBUG("setKernelsLHSResidual(# kernels="<<kernels.size()<<")");

    _kernelsLHSResidual = kernels;

    PYLITH_METHOD_END;
} // setKernelsLHSResidual


// ---------------------------------------------------------------------------------------------------------------------
// Initialize integration domain, auxiliary field, and derived field. Update observers.
void
pylith::feassemble::IntegratorBoundary::initialize(const pylith::topology::Field& solution) {
    PYLITH_METHOD_BEGIN;
    PYLITH_JOURNAL_DEBUG("intialize(solution="<<solution.label()<<")");

    delete _boundaryMesh;_boundaryMesh = new pylith::topology::Mesh(solution.mesh(), _boundaryLabel.c_str());assert(_boundaryMesh);
    PetscDM dmBoundary = _boundaryMesh->dmMesh();assert(dmBoundary);
    pylith::topology::CoordsVisitor::optimizeClosure(dmBoundary);

    Integrator::initialize(solution);

    PYLITH_METHOD_END;
} // initialize


// ---------------------------------------------------------------------------------------------------------------------
// Compute RHS residual for G(t,s).
void
pylith::feassemble::IntegratorBoundary::computeRHSResidual(pylith::topology::Field* residual,
                                                           const PylithReal t,
                                                           const PylithReal dt,
                                                           const pylith::topology::Field& solution) {
    PYLITH_METHOD_BEGIN;
    PYLITH_JOURNAL_DEBUG("computeRHSResidual(residual="<<residual<<", t="<<t<<", dt="<<dt<<", solution="<<solution.label()<<")");

    if (0 == _kernelsRHSResidual.size()) { PYLITH_METHOD_END;}

    _setKernelConstants(solution, dt);

    pylith::topology::Field solutionDot(solution.mesh()); // No dependence on time derivative of solution in RHS.
    solutionDot.label("solution_dot");
    _IntegratorBoundary::computeResidual(residual, this, _kernelsRHSResidual, t, dt, solution, solutionDot);

    PYLITH_METHOD_END;
} // computeRHSResidual


// ---------------------------------------------------------------------------------------------------------------------
// Compute RHS Jacobian for G(t,s).
void
pylith::feassemble::IntegratorBoundary::computeRHSJacobian(PetscMat jacobianMat,
                                                           PetscMat precondMat,
                                                           const PylithReal t,
                                                           const PylithReal dt,
                                                           const pylith::topology::Field& solution) {
    PYLITH_METHOD_BEGIN;
    PYLITH_JOURNAL_DEBUG("computeRHSJacobian(jacobianMat="<<jacobianMat<<", precondMat="<<precondMat<<", t="<<t<<", dt="<<dt<<", solution="<<solution.label()<<") empty method");

    // No implementation needed for boundary.

    PYLITH_METHOD_END;
} // computeRHSJacobian


// ---------------------------------------------------------------------------------------------------------------------
// Compute LHS residual for F(t,s,\dot{s}).
void
pylith::feassemble::IntegratorBoundary::computeLHSResidual(pylith::topology::Field* residual,
                                                           const PylithReal t,
                                                           const PylithReal dt,
                                                           const pylith::topology::Field& solution,
                                                           const pylith::topology::Field& solutionDot) {
    PYLITH_METHOD_BEGIN;
    PYLITH_JOURNAL_DEBUG("computeLHSResidual(residual="<<residual<<", t="<<t<<", dt="<<dt<<", solution="<<solution.label()<<")");

    if (0 == _kernelsLHSResidual.size()) { PYLITH_METHOD_END;}

    _setKernelConstants(solution, dt);

    _IntegratorBoundary::computeResidual(residual, this, _kernelsLHSResidual, t, dt, solution, solutionDot);
} // computeLHSResidual


// ---------------------------------------------------------------------------------------------------------------------
// Compute LHS Jacobian for F(t,s,\dot{s}).
void
pylith::feassemble::IntegratorBoundary::computeLHSJacobian(PetscMat jacobianMat,
                                                           PetscMat precondMat,
                                                           const PylithReal t,
                                                           const PylithReal dt,
                                                           const PylithReal s_tshift,
                                                           const pylith::topology::Field& solution,
                                                           const pylith::topology::Field& solutionDot) {
    PYLITH_METHOD_BEGIN;
    PYLITH_JOURNAL_DEBUG("computeLHSJacobian(jacobianMat="<<jacobianMat<<", precondMat="<<precondMat<<", t="<<t<<", dt="<<dt<<", solution="<<solution.label()<<", solutionDot="<<solutionDot.label()<<") empty method");

    // No implementation needed for boundary.

    PYLITH_METHOD_END;
} // computeLHSJacobian


// ---------------------------------------------------------------------------------------------------------------------
// Compute LHS Jacobian for F(t,s,\dot{s}).
void
pylith::feassemble::IntegratorBoundary::computeLHSJacobianLumpedInv(pylith::topology::Field* jacobianInv,
                                                                    const PylithReal t,
                                                                    const PylithReal dt,
                                                                    const PylithReal s_tshift,
                                                                    const pylith::topology::Field& solution) {
    PYLITH_METHOD_BEGIN;
    PYLITH_JOURNAL_DEBUG("computeLHSJacobianLumpedInv(jacobianInv="<<jacobianInv<<", t="<<t<<", dt="<<dt<<", solution="<<solution.label()<<") empty method");

    // No implementation needed for boundary.

    PYLITH_METHOD_END;
} // computeLHSJacobianLumpedInv


// ---------------------------------------------------------------------------------------------------------------------
// Compute residual.
void
pylith::feassemble::_IntegratorBoundary::computeResidual(pylith::topology::Field* residual,
                                                         const pylith::feassemble::IntegratorBoundary* integrator,
                                                         const std::vector<pylith::feassemble::IntegratorBoundary::ResidualKernels>& kernels,
                                                         const PylithReal t,
                                                         const PylithReal dt,
                                                         const pylith::topology::Field& solution,
                                                         const pylith::topology::Field& solutionDot) {
    PYLITH_METHOD_BEGIN;
    //PYLITH_COMPONENT_DEBUG("computeRHSResidual(residual="<<residual<<", t="<<t<<", dt="<<dt<<",
    // solution="<<solution.label()<<")");

    assert(integrator);
    assert(residual);

    const pylith::topology::Field* auxiliaryField = integrator->getAuxiliaryField();assert(auxiliaryField);

    PetscErrorCode err;

    PetscDM dmSoln = solution.dmMesh();
    PetscDM dmAux = auxiliaryField->dmMesh();
    PetscDMLabel dmLabel = NULL;

    PetscDS prob = NULL;
    err = DMGetDS(dmSoln, &prob);PYLITH_CHECK_ERROR(err);

    // Get auxiliary data
    err = PetscObjectCompose((PetscObject) dmSoln, "dmAux", (PetscObject) dmAux);PYLITH_CHECK_ERROR(err);
    err = PetscObjectCompose((PetscObject) dmSoln, "A", (PetscObject) auxiliaryField->localVector());PYLITH_CHECK_ERROR(err);

    // Compute the local residual
    assert(solution.localVector());
    assert(residual->localVector());
    err = DMGetLabel(dmSoln, integrator->getMarkerLabel(), &dmLabel);PYLITH_CHECK_ERROR(err);
    const int labelId = 1;

    //solution.mesh().view(":mesh.txt:ascii_info_detail"); // :DEBUG:

    //PYLITH_COMPONENT_DEBUG("DMPlexComputeBdResidualSingle() for boundary '"<<label()<<"')");
    for (size_t i = 0; i < kernels.size(); ++i) {
        const PetscInt i_field = solution.subfieldInfo(kernels[i].subfield.c_str()).index;
        err = PetscDSSetBdResidual(prob, i_field, kernels[i].r0, kernels[i].r1);PYLITH_CHECK_ERROR(err);
        err = DMPlexComputeBdResidualSingle(dmSoln, t, dmLabel, 1, &labelId, i_field, solution.localVector(), solutionDot.localVector(), residual->localVector());PYLITH_CHECK_ERROR(err);
    } // for

    PYLITH_METHOD_END;
} // _computeResidual


// End of file
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
// Copyright (c) 2010-2016 University of California, Davis
//
// See COPYING for license information.
//
// ----------------------------------------------------------------------
//

/** @file modulesrc/materials/MaterialNew.i
 *
 * Python interface to C++ abstract base MaterialNew.
 */

namespace pylith {
  namespace materials {

    class MaterialNew
    { // class MaterialNew

      // PUBLIC METHODS /////////////////////////////////////////////////////
    public:

      /** Default constructor.
       *
       * @param dimension Spatial dimension associated with material.
       */
      MaterialNew(const int dimension);

      /// Destructor.
      virtual
	~MaterialNew(void);
      
      /// Deallocate PETSc and local data structures.
      virtual
	void deallocate(void);

      /** Get spatial dimension of material.
       *
       * @returns Spatial dimension.
       */
      int dimension(void) const;

      /** Set identifier of material.
       *
       * @param value Material identifier
       */
      void id(const int value);

      /** Get identifier of material.
       *
       * @returns Material identifier
       */
      int id(void) const;
      
      /** Set label of material.
       *
       * @param value Label of material
       */
      void label(const char* value);
      
      /** Get label of material.
       *
       * @returns Label of material
       */
      const char* label(void) const;
      
      /** Initialize material. Setup auxiliary fields.
       *
       * @param solution Solution field.
       */
      void initialize(const pylith::topology::Field& solution);
      
      // PROTECTED METHODS //////////////////////////////////////////////////
    protected:

      /** Setup auxiliary subfields (discretization and query fns).
       *
       * Create subfields in auxiliary fields (includes name of the field,
       * vector field type, discretization, and scale for
       * nondimensionalization) and set query functions for filling them
       * from a spatial database.
       *
       * @attention The order of the calls to subfieldAdd() must match the
       * order of the auxiliary fields in the FE kernels.
       */
      virtual
      void _auxFieldsSetup(void) = 0;

      /** Set kernels for RHS residual G(t,s).
       *
       * Potentially, there are g0 and g1 kernels for each equation. If no
       * kernel is needed, then set the kernel function to NULL.
       *
       * @param solution Solution field.
       */
      virtual
      void _setFEKernelsRHSResidual(const topology::Field& solution) const = 0;


      /** Set kernels for RHS Jacobian G(t,s).
       *
       * Potentially, there are Jg0, Jg1, Jg2, and Jg3 kernels for each
       * combination of equations. If no kernel is needed, then set the
       * kernel function to NULL.
       *
       * - Jg0(ifield, jfield)
       * - Jg1(ifield, jfield, jdim)
       * - Jg2(ifield, jfield, idim)
       * - Jg3(ifield, jfield, idim, jdim)
       *
       * @param solution Solution field.
       */
      virtual
      void _setFEKernelsRHSJacobian(const topology::Field& solution) const = 0;

      
      /** Set kernels for LHS residual F(t,s,\dot{s}).
       *
       * Potentially, there are f0 and f1 kernels for each equation. If no
       * kernel is needed, then set the kernel function to NULL.
       *
       * @param solution Solution field.
       */
      virtual
      void _setFEKernelsLHSResidual(const topology::Field& solution) const = 0;
      

      /** Set kernels for LHS Jacobian F(t,s,\dot{s}) when implicit time-stepping.
       *
       * - Jf0(ifield, jfield)
       * - Jf1(ifield, jfield, jdim)
       * - Jf2(ifield, jfield, idim)
       * - Jf3(ifield, jfield, idim, jdim)
       *
       * @param solution Solution field.
       */
      virtual
      void _setFEKernelsLHSJacobianImplicit(const topology::Field& solution) const = 0;


      /** Set kernels for LHS Jacobian F(t,s,\dot{s}) when explicit time-stepping.
       *
       * - Jf0(ifield, jfield)
       * - Jf1(ifield, jfield, jdim)
       * - Jf2(ifield, jfield, idim)
       * - Jf3(ifield, jfield, idim, jdim)
       *
       * @param solution Solution field.
       */
      virtual
      void _setFEKernelsLHSJacobianExplicit(const topology::Field& solution) const = 0;
      
    }; // class MaterialNew

  } // materials
} // pylith


// End of file 

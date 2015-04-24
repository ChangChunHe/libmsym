//
//  msym.h
//  libmsym
//
//  Created by Marcus Johansson on 30/01/15.
//  Copyright (c) 2015 Marcus Johansson. 
//
//  Distributed under the MIT License ( See LICENSE file or copy at http://opensource.org/licenses/MIT )
//

#ifndef __MSYM_H
#define __MSYM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "msym_error.h"

    typedef struct _msym_context * msym_context;

    typedef enum _msym_symmetry_operation_type {
        IDENTITY = 0,
        PROPER_ROTATION = 1,
        IMPROPER_ROTATION = 2,
        REFLECTION = 3,
        INVERSION = 4
    } msym_symmetry_operation_type_t;
    
    typedef enum _msym_basis_type {
        ATOMIC_ORBITAL,
        MASS_WEIGHTED_COORDINATES
    } msym_basis_type_t;
    
    typedef struct _msym_symmetry_operation {
        msym_symmetry_operation_type_t type;
        int order;                              // Order of proper/improper rotation
        int power;                              // Power (e.g. C2^2 = I)
        double v[3];                            // Proper/improper rotation vector or reflection plane normal
        int cla;                                // Class of symmetry operation (point group dependant)
    } msym_symmetry_operation_t;
    
    typedef struct _msym_thresholds {
        double zero;                            // For determining if something is zero (e.g. vectors close to center of mass)
        double geometry;                        // For translating inertial tensor eigenvalues to geometric structures
        double angle;                           // For determining angles, (e.g. if vectors are parallel)
        double equivalence;                     // Equivalence test threshold
        double eigfact;                         // Jacobi eigenvalue algorithm threshold
        double permutation;                     // Equality test when determining permutation for symmetry operation
        double orthogonalization;               // For orthogonalizing orbital subspaces
    } msym_thresholds_t;
    
    typedef struct _msym_orbital {
        int n;                                  // Principal
        int l;                                  // Azimuthal
        int m;                                  // Liniear combination of magnetic quantum number (e.g. 2pz = 0, 2px = 1, 2py = -1)
        char name[8];                           // Name
    } msym_orbital_t;
    
    
    typedef struct _msym_displacement {
        double v[3];
    } msym_displacement_t;
    
    typedef struct _msym_subspace {
        msym_basis_type_t type;                 // Type
        double *space;                          // double[d][basisl] with coefficients of orbitals
        union {
            msym_orbital_t **o;                 // Atomic orbital basis
            msym_displacement_t **q;            // Mass weighted coordinates vibration basis NYI
        } basis;                                // Basis functions
        struct _msym_subspace *subspace;        // Subspaces
        int d;                                  // Dimension of subspace
        int basisl;                             // Length of basis vectors
        int irrep;                              // Irreducable representation
        int subspacel;                          // Number of subspaces
    } msym_subspace_t;
    
    typedef struct _msym_element {
        msym_orbital_t **ao;                    // Pointers into block allocated list of atomic orbitals
        double m;                               // Mass
        double v[3];                            // Position
        int n;                                  // Nuclear charge
        int aol;                                // Number of atomic orbitals
        char name[4];                           // Name
    } msym_element_t;
    
    typedef struct _msym_equivalence_set {
        msym_element_t **elements;              // Pointers to elements
        double err;                             // Maximum error when detecting this equivalence set
        int length;                             // Number of elements
    } msym_equivalence_set_t ;
    
    msym_context msymCreateContext();
    msym_error_t msymReleaseContext(msym_context ctx);
    
    msym_error_t msymSetThresholds(msym_context ctx, msym_thresholds_t *thresholds);
    msym_error_t msymGetThresholds(msym_context ctx, msym_thresholds_t **thresholds);
    msym_error_t msymSetElements(msym_context ctx, int length, msym_element_t elements[length]);
    msym_error_t msymGetElements(msym_context ctx, int *length, msym_element_t **elements);
    msym_error_t msymSetPointGroup(msym_context ctx, char *name);
    msym_error_t msymGetPointGroup(msym_context ctx, int l, char buf[l]);
    msym_error_t msymGetSymmetryOperations(msym_context ctx, int *sopsl, msym_symmetry_operation_t **sops);
    msym_error_t msymGetEquivalenceSets(msym_context ctx, int *l, msym_equivalence_set_t **es);
    
    msym_error_t msymFindEquivalenceSets(msym_context ctx);
    msym_error_t msymFindEquivalenceSetPermutations(msym_context ctx);
    msym_error_t msymFindSymmetry(msym_context ctx);
    msym_error_t msymSymmetrizeMolecule(msym_context context, double *err);
    msym_error_t msymApplyTranslation(msym_context ctx, msym_element_t *element, double v[3]);
    msym_error_t msymSymmetrizeOrbitals(msym_context ctx, int l, double c[l][l]);
    msym_error_t msymGenerateElements(msym_context ctx, int length, msym_element_t elements[length]);
    msym_error_t msymGenerateOrbitalSubspaces(msym_context ctx);
    msym_error_t msymGetOrbitalSubspaces(msym_context ctx, int l, double c[l][l]);
    msym_error_t msymAlignAxes(msym_context ctx);
    
    msym_error_t msymGetCenterOfMass(msym_context ctx, double v[3]);
    msym_error_t msymGetRadius(msym_context ctx, double *radius);
    msym_error_t msymGetAlignmentAxes(msym_context ctx, double primary[3], double secondary[3]);
    msym_error_t msymSetAlignmentAxes(msym_context ctx, double primary[3], double secondary[3]);
    msym_error_t msymGetAlignmentTransform(msym_context ctx, double transform[3][3]);
    msym_error_t msymSetAlignmentTransform(msym_context ctx, double transform[3][3]);
    
    //Testing only, crap implementation
    msym_error_t msymSetOrbitalsMB(msym_context ctx);
    
    
#ifdef __cplusplus
}
#endif
    
#endif /* defined(__MSYM_H) */

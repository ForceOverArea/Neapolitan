#ifndef SOLVER_H_
#define SOLVER_H_

#include "elems.h"
#include "matrix.h"

/**
 * "Useful" (bad) approximation for `1 / infinity`
 */
#define _DX 0.00001;

typedef Vec_S NodalProblem_T;

/**
 * A struct used for retaining important information about
 * a particular nodal analysis problem 
 */
typedef struct ProblemProperties_S
{
    /**
     * The dimension of the nodal and flux quantities
     * used in the problem.
     */
    size_t vecDimension;
    /**
     * The number of free nodes in the problem
     */
    size_t dof;
    /**
     * The edge length of the jacobian matrix
     * needed to solve this problem.
     */
    size_t matrixEdgeLen;
}
ProblemProperties_S;

/**
 * ### Desription
 * Populates a struct with useful information 
 * about the problem being solved.
 * 
 * ### Arguments
 * `props`      (out)   - A reference to the struct to populate 
 *                        with information regarding the problem
 * `problem`    (inout) - A reference to the problem to pull 
 *                        information from
 * 
 * ### Returns
 * An `NpStatus_E` indicating success or information about what went
 * wrong while trying to solve.
 */
void getProblemProps(ProblemProperties_S* props, const NodalProblem_T* const problem);

/**
 * ### Description
 * Attempts to solve a nodal analysis problem, 
 * populating `soln` with the correct nodal values.
 * 
 * ### Arguments
 * `soln`       (out)   - A vector pointer to populate with the
 *                        nodal potential values, once they have 
 *                        have been solved by the solver
 * `problem`    (in)    - A pointer to the nodal analysis problem to
 *                        solve.
 * 
 * ### Returns
 * An `NpStatus_E` indicating success or information about what went
 * wrong while trying to solve.
 */
NpStatus_E solveNodalProblem(Vec_S* soln, NodalProblem_T* problem);

#endif
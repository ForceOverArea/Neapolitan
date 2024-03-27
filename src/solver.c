#include "solver.h"

void getProblemProps(ProblemProperties_S* props, NodalProblem_T* problem)
{
    GenericNode_S* node0 = (GenericNode_S*)(problem->elements[0].pointer);
    props->vecDimension  = node0->potentialVector->len;
    props->dof           = problem->len;
    props->matrixEdgeLen = props->vecDimension * props->dof;
}

/**
 * ### Description
 * Populates a single column in the jacobian matrix needed to
 * solve this problem. 
 * 
 * ### Arguments
 * `problem`    (in)    - The problem to solve
 * `jacobian`   (inout) - The jacobian matrix to add a column to
 * `iNode`      (in)    - The index of the node in the problem to 
 *                        change to calculate the matrix column
 * `iVar`       (in)    - The index of the variable in the node 
 *                        to change to calculate the matrix column
 * 
 * ### Returns
 * An `NpStatus_E` indicating success or information about what went
 * wrong while trying to solve.
 */
static inline NpStatus_E populateJacobianColumn(
    const NodalProblem_T* problem, 
    Matrix_S* jacobian, 
    size_t iNode, 
    size_t iVar)
{
    ProblemProperties_S props;
    getProblemProps(&props, problem);

    GenericNode_S* node = (GenericNode_S*)(problem->elements[iNode].pointer);
    double* var = &(node->potentialVector->elements[iVar].floating);

    Vec_S* tmpFlux = newVecWithLen(props.vecDimension);
    if (NULL == tmpFlux)
    {
        return OUT_OF_MEMORY;
    }

    // Calculate F(X + DX)
    *var += _DX; 
    for (size_t rowIdx = 0; rowIdx < jacobian->rows; rowIdx++)
    {
        for (size_t nodeIdx = 0; nodeIdx < props.dof; nodeIdx++)
        {
            PROPOGATE_ERROR(fluxDiscrepancy(
                tmpFlux, 
                (GenericNode_S*)(problem->elements[iNode].pointer)))

            for (size_t varIdx = 0; varIdx < props.vecDimension; varIdx++)
            {
                size_t matrixCol = nodeIdx + varIdx;
                // Assign F(X) values to column
                *(indexMatrix(&jacobian, rowIdx, matrixCol)) = tmpFlux->elements[varIdx].floating;
            }
        }
    }
    *var -= _DX;

    // Calculate F(X)
    for (size_t rowIdx = 0; rowIdx < jacobian->rows; rowIdx++)
    {
        for (size_t nodeIdx = 0; nodeIdx < props.dof; nodeIdx++)
        {
            PROPOGATE_ERROR(fluxDiscrepancy(
                tmpFlux, 
                (GenericNode_S*)(problem->elements[iNode].pointer)))

            for (size_t varIdx = 0; varIdx < props.vecDimension; varIdx++)
            {
                size_t matrixCol = nodeIdx + varIdx;
                // Subassign F(X + DX) to the same column
                *(indexMatrix(&jacobian, rowIdx, matrixCol)) -= tmpFlux->elements[varIdx].floating;
            }
        }
    }

    free(tmpFlux);
    return OK;
}

NpStatus_E solveNodalProblem(Vec_S* soln, NodalProblem_T* problem)
{
    /* TODO: make validation function/routine for 
    asserting all nodes are of same dimension */

    ProblemProperties_S props;
    getProblemProps(&props, problem);

    Matrix_S jacobian, delta;
    PROPOGATE_ERROR(tryNewMatrix(&jacobian, props.matrixEdgeLen, props.matrixEdgeLen))
    PROPOGATE_ERROR(tryNewMatrix(&delta, jacobian.rows, 1));

    // Build up jacobian column-by-column
    for (size_t iNode = 0; iNode < props.dof; iNode++)
    {
        for (size_t iVar = 0; iVar < props.vecDimension; iVar++)
        {
            PROPOGATE_ERROR(populateJacobianColumn(problem, &jacobian, iNode, iVar))
        }
    }

    // Invert jacobian matrix and calculate delta
    PROPOGATE_ERROR(tryInplaceInvert(&jacobian))
    PROPOGATE_ERROR(tryMultiplyMatrix(&delta, &jacobian, &error))

    free(jacobian.elements); // Release vec stored in Jacobian
    free(delta.elements);
    return OK;
}
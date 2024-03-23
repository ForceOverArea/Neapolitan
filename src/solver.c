#include "solver.h"

void getProblemProps(ProblemProperties_T* props, NodalProblem_T* problem)
{
    GenericNode_T* node0 = (GenericNode_T*)(problem->elements[0].pointer);
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
 * An `NpStatus_T` indicating success or information about what went
 * wrong while trying to solve.
 */
static inline NpStatus_T populateJacobianColumn(
    const NodalProblem_T* problem, 
    Matrix_T* jacobian, 
    size_t iNode, 
    size_t iVar)
{
    ProblemProperties_T props;
    getProblemProps(&props, problem);

    GenericNode_T* node = (GenericNode_T*)(problem->elements[iNode].pointer);
    double* var = &(node->potentialVector->elements[iVar].floating);

    Vec_T* tmpFlux = newVecWithLen(props.vecDimension);
    if (NULL == tmpFlux)
    {
        return OUT_OF_MEMORY;
    }

    // Calculate F(X)
    for (size_t rowIdx = 0; rowIdx < jacobian->rows; rowIdx++)
    {
        for (size_t nodeIdx = 0; nodeIdx < props.dof; nodeIdx++)
        {
            PROPOGATE_ERROR(fluxDiscrepancy(
                tmpFlux, 
                (GenericNode_T*)(problem->elements[iNode].pointer)))

            for (size_t varIdx = 0; varIdx < props.vecDimension; varIdx++)
            {
                size_t matrixCol = nodeIdx + varIdx;
                
                *(indexMatrix(&jacobian, rowIdx, matrixCol)) = 
                    (-(tmpFlux->elements[varIdx].floating));
            }
        }
    }

    // Calculate F(X + DX)
    // FIXME : This needs to be updated from the above source code
    for (size_t rowIdx = 0; rowIdx < jacobian->rows; rowIdx++)
    {
        for (size_t nodeIdx = 0; nodeIdx < props.dof; nodeIdx++)
        {
            PROPOGATE_ERROR(fluxDiscrepancy(
                tmpFlux, 
                (GenericNode_T*)(problem->elements[iNode].pointer)))

            for (size_t varIdx = 0; varIdx < props.vecDimension; varIdx++)
            {
                size_t matrixCol = nodeIdx + varIdx;
                
                *(indexMatrix(&jacobian, rowIdx, matrixCol)) = 
                    (-(tmpFlux->elements[varIdx].floating));
            }
        }
    }

    *var += _DX;


    return OK;
}

NpStatus_T solveNodalProblem(Vec_T* soln, NodalProblem_T* problem)
{
    /* TODO: make validation function/routine for 
    asserting all nodes are of same dimension */

    

    Matrix_T jacobian;
    PROPOGATE_ERROR(tryNewMatrix(&jacobian, matrixEdgeLen, matrixEdgeLen))

    // one variable per column, one function per row
    for (size_t ni = 0; ni < numNodes; ni++)
    {
        for (size_t vi = 0; vi < vecDimension; vi++)
        {
            size_t matrixCol = ni + vi;
            indexMatrix(&jacobian, )
        }
    }

    free(jacobian.elements); // Release vec stored in Jacobian
}
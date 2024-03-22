#include "elemprimitives.h"

#define NO_VEC ((Vec_T*)0)

typedef FLOATING (*MathOp_T)(FLOATING opnd1, FLOATING opnd2, FLOATING g);

static inline NpStatus_T applyElementWise(
    MathOp_T op, 
    Vec_T* result,
    Vec_T* input, 
    Vec_T* output, 
    Vec_T* gain)
{
    if (output->len != input->len ||
        output->len != gain->len ||
        gain->len != input->len)
    {
        return VECTOR_LEN_MISMATCH;
    }

    // Calculate element-wise potential 
    for (size_t i = 0; i < input->len; i++)
    {
        FLOATING po, pi, g;
        po = output->elements[i].floating;
        pi =  input->elements[i].floating;
        g  =   gain->elements[i].floating;

        result->elements[i].floating = op(po, pi, g);
    }

    return OK;
}

static inline NpStatus_T addAssignElementWise(
    Vec_T* sum,
    Vec_T* addend)
{
    if (sum->len != addend->len)
    {
        return VECTOR_LEN_MISMATCH;
    }

    // Calculate element-wise potential 
    for (size_t i = 0; i < sum->len; i++)
    {
        sum->elements[i].floating += addend->elements[i].floating;
    }

    return OK;
}

/**
 * 1 - (out) result
 * 2 - (in)  output potential
 * 3 - (in)  input potential
 * 4 - (in)  gain 
 */
static inline FLOATING prodDeltaGain(FLOATING po, FLOATING pi, FLOATING g)
{
    return (po - pi) * g;
}

/**
 * 1 - (out) sum
 * 2 - (in)  left addend
 * 3 - (in)  DO NOT USE
 * 4 - (in)  right addend
 */
static inline FLOATING vectorSum(FLOATING adnd1, FLOATING _dnu, FLOATING adnd2)
{
    return adnd1 + adnd2;
}

/**
 * 1 - (out) result
 * 2 - (in)  minuend
 * 3 - (in)  DO NOT USE
 * 4 - (in)  subtrahend 
 */
static inline FLOATING vectorDiff(FLOATING adnd1, FLOATING _dnu, FLOATING adnd2)
{
    return adnd1 - adnd2;
}

NpStatus_T tryNewElement(
    GenericElement_T* elem, 
    size_t dimension, 
    FluxCalculation_P func)
{
    elem->gainVector = newVec(dimension);
    if (NULL == elem->gainVector)
    {
        return OUT_OF_MEMORY;
    }
    elem->inputNode = NULL;
    elem->outputNode = NULL;
    elem->flux = func;

    return OK;
}

NpStatus_T tryNewNode(GenericNode_T* node, size_t dimension)
{   
    node->lockingElement = NULL;

    node->potentialVector = newVec(dimension);
    if (NULL == node->potentialVector)
    {
        return OUT_OF_MEMORY;
    }

    node->inputs = newVec(0);
    if (NULL == node->inputs)
    {
        free(node->potentialVector);
        return OUT_OF_MEMORY;
    }

    node->outputs = newVec(0);
    if (NULL == node->outputs)
    {
        free(node->potentialVector);
        free(node->inputs);
        return OUT_OF_MEMORY;
    }

    return OK;
}

NpStatus_T fluxDiscrepancy(Vec_T* fluxDiscrep, GenericNode_T* node)
{
    size_t n = node->inputs->len;

    Vec_T* totalInput, * totalOutput, * tempFlux;
    totalInput = newVec(n);
    if (NULL == totalInput)
    {
        return OUT_OF_MEMORY;   
    }

    totalOutput = newVec(n);
    if (NULL == totalOutput)
    {
        free(totalInput);
        return OUT_OF_MEMORY;
    }

    tempFlux = newVec(n); 
    if (NULL == tempFlux)
    {
        free(totalInput);
        free(totalOutput);
        return OUT_OF_MEMORY;
    }

    for (size_t i = 0; i < node->inputs->len; i++)
    {
        // Get the element from the inputs list
        GenericElement_T* input = (GenericElement_T*)(node->inputs->elements[i].pointer);

        // Perform it's flux calculation
        input->flux(
            tempFlux,
            input->gainVector, 
            input->inputNode, 
            input->outputNode,
            input->drivesOutputPotential);

        // Sum up the flux discrepancy
        if (addAssignElementWise(totalInput, tempFlux))
        {
            free(totalInput);
            free(totalOutput);
            free(tempFlux);
            return VECTOR_LEN_MISMATCH; // This is the only possible error to report
        }
    }

    // Rinse and repeat for outputs
    for (size_t i = 0; i < node->outputs->len; i++)
    {
        // Get the element from the outputs list
        GenericElement_T* output = (GenericElement_T*)(node->outputs->elements[i].pointer);

        // Perform it's flux calculation
        output->flux(
            tempFlux,
            output->gainVector, 
            output->inputNode, 
            output->outputNode,
            output->drivesOutputPotential);

        // Sum up the flux discrepancy
        if (!addAssignElementWise(totalOutput, tempFlux))
        {
            free(totalInput);
            free(totalOutput);
            free(tempFlux);
            return VECTOR_LEN_MISMATCH; // This is the only possible error to report
        }
    }

    if(!applyElementWise(vectorDiff,
        fluxDiscrep,
        totalInput,
        NO_VEC,
        totalOutput))
    {
        free(totalInput);
        free(totalOutput);
        free(tempFlux);
        return VECTOR_LEN_MISMATCH; // This is the only possible error to report
    }

    free(totalInput);
    free(totalOutput);
    free(tempFlux);

    return OK;
}

NpStatus_T normalFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* input, GenericNode_T* output, bool _dnu)
{
    if (!applyElementWise(prodDeltaGain, 
        flux, 
        output->potentialVector, 
        input->potentialVector, 
        gain))
    {
        return VECTOR_LEN_MISMATCH; // This is the only possible error to report
    }

    return OK;
}

NpStatus_T observeFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* input, GenericNode_T* output, bool drivesOutput)
{
    if (drivesOutput)
    {
        if (!applyElementWise(vectorSum, 
            output->potentialVector, 
            input->potentialVector, 
            NO_VEC, 
            gain))
        {
            return VECTOR_LEN_MISMATCH;
        }

        PROPOGATE_ERROR(
            fluxDiscrepancy(flux, output)
        )
    }
    else
    {
        if (!applyElementWise(vectorSum, 
            input->potentialVector, 
            output->potentialVector, 
            NO_VEC, 
            gain))
        {
            return VECTOR_LEN_MISMATCH;
        }

        PROPOGATE_ERROR(
            fluxDiscrepancy(flux, input)
        )
    }

    return OK;
}

NpStatus_T forceFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* _dnu1, GenericNode_T* _dnu2, bool _dnu3)
{
    // FIXME: is this correct usage?
    (void)memcpy(flux, gain, sizeof *gain);
    return OK;
}

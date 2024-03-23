#include "elemprimitives.h"
#include <stdio.h>

static inline NpStatus_T prodDeltaGain(
    Vec_T* result,
    Vec_T* input, 
    Vec_T* output,
    Vec_T* gain)
{
    if (result->len != input->len ||
        input->len != output->len ||
        output->len != gain->len)
    {
        return VECTOR_LEN_MISMATCH;   
    }

    size_t n = result->len;
    for (size_t i = 0; i < n; i++)
    {
        result->elements[i].floating = 
            (output->elements[i].floating - input->elements[i].floating) * gain->elements[i].floating;
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

NpStatus_T tryNewElement(
    GenericElement_T* elem, 
    size_t dimension, 
    FluxCalculation_F func)
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
    size_t n = node->potentialVector->len;

    Vec_T* totalInput, * totalOutput, * tmpFlux;
    totalInput = newVecWithLen(n);
    if (NULL == totalInput)
    {
        return OUT_OF_MEMORY;   
    }

    totalOutput = newVecWithLen(n);
    if (NULL == totalOutput)
    {
        free(totalInput);
        return OUT_OF_MEMORY;
    }

    tmpFlux = newVecWithLen(n); 
    if (NULL == tmpFlux)
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
            tmpFlux,
            input->gainVector, 
            input->inputNode, 
            input->outputNode,
            input->drivesOutputPotential);

        // Sum up the flux discrepancy
        if (addAssignElementWise(totalInput, tmpFlux))
        {
            free(totalInput);
            free(totalOutput);
            free(tmpFlux);
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
            tmpFlux,
            output->gainVector, 
            output->inputNode, 
            output->outputNode,
            output->drivesOutputPotential);

        // Sum up the flux discrepancy
        if (addAssignElementWise(totalOutput, tmpFlux))
        {
            free(totalInput);
            free(totalOutput);
            free(tmpFlux);
            return VECTOR_LEN_MISMATCH; // This is the only possible error to report
        }
    }

    if (elementWiseDiff(fluxDiscrep, totalInput, totalOutput, true))
    {
        free(totalInput);
        free(totalOutput);
        free(tmpFlux);
        return VECTOR_LEN_MISMATCH; // This is the only possible error to report
    }

    free(totalInput);
    free(totalOutput);
    free(tmpFlux);

    return OK;
}

NpStatus_T normalFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* input, GenericNode_T* output, bool _dnu)
{
    if (prodDeltaGain(flux, output->potentialVector, input->potentialVector, gain))
    {
        return VECTOR_LEN_MISMATCH; // This is the only possible error to report
    }

    return OK;
}

NpStatus_T observeFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* input, GenericNode_T* output, bool drivesOutput)
{
    if (drivesOutput)
    {
        PROPOGATE_ERROR(elementWiseAdd(output->potentialVector, input->potentialVector, gain, true))
        PROPOGATE_ERROR(fluxDiscrepancy(flux, output))
    }
    else
    {
        PROPOGATE_ERROR(elementWiseAdd(input->potentialVector, output->potentialVector, gain, true))
        PROPOGATE_ERROR(fluxDiscrepancy(flux, input))
    }

    return OK;
}

NpStatus_T forceFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* _dnu1, GenericNode_T* _dnu2, bool _dnu3)
{
    // FIXME: is this correct usage?
    (void)memcpy(flux, gain, sizeof *gain);
    return OK;
}

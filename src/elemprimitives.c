#include "elemprimitives.h"

#define NO_ELEMENT ((GenericElement_T*)0)

typedef FLOATING (*MathOp_T)(FLOATING opnd1, FLOATING opnd2, FLOATING g);

static inline bool applyElementWise(
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
        return false;
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

    return true;
}

static inline bool addAssignElementWise(
    Vec_T* sum,
    Vec_T* addend)
{
    if (sum->len != addend->len)
    {
        return false;
    }

    // Calculate element-wise potential 
    for (size_t i = 0; i < sum->len; i++)
    {
        sum->elements[i].floating += addend->elements[i].floating;
    }

    return true;
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

bool fluxDiscrepancy(Vec_T* fluxDiscrep, GenericNode_T* node)
{
    size_t n = node->inputs->len;

    Vec_T* totalInput, * totalOutput;
    totalInput  = newVec(n);
    totalOutput = newVec(n);
    if (NULL == totalInput)
    {
        return NULL;   
    }
    else if (NULL == totalOutput)
    {
        free(totalInput);
        return NULL;
    }

    for (size_t i = 0; i < node->inputs->len; i++)
    {
        // Get the element from the inputs list
        GenericElement_T* input = (GenericElement_T*)(node->inputs->elements[i].pointer);

        // Perform it's flux calculation
        Vec_T* inputFlux;
        input->flux(
            inputFlux,
            input->gainVector, 
            input->inputNode, 
            input->outputNode);

        // Abort if NULL
        if (NULL == inputFlux)
        {
            return NULL;
        }

        // Sum up the flux discrepancy
        if (!addAssignElementWise(totalInput, inputFlux))
        {
            return NULL;
        }
    }

    // Rinse and repeat for outputs
    for (size_t i = 0; i < node->outputs->len; i++)
    {
        // Get the element from the outputs list
        GenericElement_T* output = (GenericElement_T*)(node->outputs->elements[i].pointer);

        // Perform it's flux calculation
        Vec_T* outputFlux;
        output->flux(
            outputFlux,
            output->gainVector, 
            output->inputNode, 
            output->outputNode);

        // Abort if NULL
        if (NULL == outputFlux)
        {
            return NULL;
        }

        // Sum up the flux discrepancy
        if (!addAssignElementWise(totalOutput, outputFlux))
        {
            return NULL;
        }
    }

    if(!applyElementWise(
        vectorDiff,
        fluxDiscrep,
        totalInput,
        NO_ELEMENT,
        totalOutput))
    {

    }
}

bool normalFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* input, GenericNode_T* output, bool _dnu)
{
    size_t n = input->potentialVector->len;
    if (!applyElementWise(prodDeltaGain, 
        flux, 
        output->potentialVector, 
        input->potentialVector, 
        gain))
    {
        return false;
    }

    return true;
}

bool observeFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* input, GenericNode_T* output, bool drivesOutput)
{
    size_t n = input->potentialVector->len;

    if (drivesOutput)
    {
        if (!applyElementWise(vectorSum, 
            output->potentialVector, 
            input->potentialVector, 
            NO_ELEMENT, 
            gain))
        {
            return false;
        }

        if (!fluxDiscrepancy(flux, output))
        {
            return false;
        }
    }
    else
    {
        if (!applyElementWise(vectorSum, 
            input->potentialVector, 
            output->potentialVector, 
            NO_ELEMENT, 
            gain))
        {
            return false;
        }

        if (!fluxDiscrepancy(flux, input))
        {
            return false;
        }
    }

    return true;
}

bool forceFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* _dnu1, GenericNode_T* _dnu2, bool _dnu3)
{
    // FIXME: is this correct usage?
    (void)memcpy(flux, gain, sizeof *gain);
    return true;
}
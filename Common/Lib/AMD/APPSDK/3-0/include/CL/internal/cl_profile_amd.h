//=====================================================================
// Copyright 2009-2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file 
/// 
//=====================================================================

#ifndef __CL_PROFILE_AMD_H
#define __CL_PROFILE_AMD_H

#include "CL/cl_platform.h"

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

typedef struct _cl_perfcounter_amd * cl_perfcounter_amd;
typedef cl_ulong cl_perfcounter_property;
typedef cl_uint cl_perfcounter_info;

/* cl_perfcounter_info */
enum PerfcounterInfo
{
    CL_PERFCOUNTER_NONE                 = 0x0,
    CL_PERFCOUNTER_REFERENCE_COUNT      = 0x1,
    CL_PERFCOUNTER_DATA                 = 0x2,
    CL_PERFCOUNTER_GPU_BLOCK_INDEX      = 0x3,
    CL_PERFCOUNTER_GPU_COUNTER_INDEX    = 0x4,
    CL_PERFCOUNTER_GPU_EVENT_INDEX      = 0x5,
    CL_PERFCOUNTER_LAST
};

/*! \brief Creates a new HW performance counter
 *   for the specified OpenCL context.
 *
 *  \param device must be a valid OpenCL device.
 *
 *  \param properties the list of properties of the hardware counter
 *
 *  \param errcode_ret  A non zero value if OpenCL failed to create PerfCounter
 *  - CL_SUCCESS if the function is executed successfully.
 *  - CL_INVALID_CONTEXT if the specified context is invalid.
 *  - CL_OUT_OF_RESOURCES if we couldn't create the object
 *
 *  \return the created perfcounter object
 */
extern CL_API_ENTRY cl_perfcounter_amd CL_API_CALL
clCreatePerfCounterAMD(
    cl_device_id                /* device */,
    cl_perfcounter_property*    /* properties */,
    cl_int*                     /* errcode_ret */
) CL_API_SUFFIX__VERSION_1_0;

/*! \brief Destroy a performance counter object.
 *
 *  \param perf_counter the perfcounter object for release
 *
 *  \return A non zero value if OpenCL failed to release PerfCounter
 *  - CL_SUCCESS if the function is executed successfully.
 *  - CL_INVALID_OPERATION if we failed to release the object
 */
extern CL_API_ENTRY cl_int CL_API_CALL
clReleasePerfCounterAMD(
    cl_perfcounter_amd  /* perf_counter */
) CL_API_SUFFIX__VERSION_1_0;

/*! \brief Increments the perfcounter object reference count.
 *
 *  \param perf_counter the perfcounter object for retain
 *
 *  \return A non zero value if OpenCL failed to retain PerfCounter
 *  - CL_SUCCESS if the function is executed successfully.
 *  - CL_INVALID_OPERATION if we failed to release the object
 */
extern CL_API_ENTRY cl_int CL_API_CALL
clRetainPerfCounterAMD(
    cl_perfcounter_amd  /* perf_counter */
) CL_API_SUFFIX__VERSION_1_0;

/*! \brief Enqueues the begin command for the specified counters.
 *
 *  \param command_queue must be a valid OpenCL command queue.
 *
 *  \param num_perf_counters the number of perfcounter objects in the array.
 *
 *  \param perf_counters specifies an array of perfcounter objects.
 *
 *  \return A non zero value if OpenCL failed to release PerfCounter
 *  - CL_SUCCESS if the function is executed successfully.
 *  - CL_INVALID_OPERATION if we failed to enqueue the begin operation
 */
extern CL_API_ENTRY cl_int CL_API_CALL
clEnqueueBeginPerfCounterAMD(
    cl_command_queue    /* command_queue */,
    cl_uint             /* num_perf_counters */,
    cl_perfcounter_amd* /* perf_counters */,
    cl_uint             /* num_events_in_wait_list */,
    const cl_event*     /* event_wait_list */,
    cl_event*           /* event */
) CL_API_SUFFIX__VERSION_1_0;

/*! \brief Enqueues the end command for the specified counters.
 *
 *  \param command_queue must be a valid OpenCL command queue.
 *
 *  \param num_perf_counters the number of perfcounter objects in the array.
 *
 *  \param perf_counters specifies an array of perfcounter objects.
 *
 *  \param event the event object associated with the end operation.
 *
 *  \return A non zero value if OpenCL failed to release PerfCounter
 *  - CL_SUCCESS if the function is executed successfully.
 *  - CL_INVALID_OPERATION if we failed to enqueue the end operation
 */
extern CL_API_ENTRY cl_int CL_API_CALL
clEnqueueEndPerfCounterAMD(
    cl_command_queue    /* command_queue */,
    cl_uint             /* num_perf_counters */,
    cl_perfcounter_amd* /* perf_counters */,
    cl_uint             /* num_events_in_wait_list */,
    const cl_event*     /* event_wait_list */,
    cl_event*           /* event */
) CL_API_SUFFIX__VERSION_1_0;

/*! \brief Retrieves the results from the counter objects.
 *
 *  \param perf_counter specifies a perfcounter objects for query.
 *
 *  \param param_name specifies the information to query.
 *
 *  \param param_value is a pointer to memory where the appropriate result
 *  being queried is returned. If \a param_value is NULL, it is ignored.
 *
 *  \param param_value_size is used to specify the size in bytes of memory
 *  pointed to by \a param_value. This size must be >= size of return type.
 *
 *  \param param_value_size_ret returns the actual size in bytes of data copied
 *  to \a param_value. If \a param_value_size_ret is NULL, it is ignored.
 *
 *  \param values must be a valid pointer to an array of 64-bit values
 *  and the array size must be equal to num_perf_counters.
 *
 *  \return
 *  - CL_SUCCESS if the function is executed successfully.
 *  - CL_PROFILING_INFO_NOT_AVAILABLE if event isn't finished.
 *  - CL_INVALID_OPERATION if we failed to get the data
 */
extern CL_API_ENTRY cl_int CL_API_CALL
clGetPerfCounterInfoAMD(
    cl_perfcounter_amd  /* perf_counter */,
    cl_perfcounter_info /* param_name */,
    size_t              /* param_value_size */,
    void*               /* param_value */,
    size_t*             /* param_value_size_ret */
) CL_API_SUFFIX__VERSION_1_0;

#ifdef __cplusplus
} /*extern "C"*/
#endif /*__cplusplus*/

#endif  /*__CL_PROFILE_AMD_H*/

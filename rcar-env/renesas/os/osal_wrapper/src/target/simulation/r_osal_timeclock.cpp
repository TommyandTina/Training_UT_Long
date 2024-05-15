/**
 * \addtogroup osal_wrapper
 * @{
 */

// project includes
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/utils/r_cast_saturation.hpp"

#include "target/simulation/r_osal_common.hpp"
#include "target/simulation/soc_abstraction/soc_init.hpp"

// system includes
#include <chrono>

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ClockTimeGetTimeStamp")
#endif
e_osal_return_t R_OSAL_ClockTimeGetTimeStamp(e_osal_clock_type_t clock_type, st_osal_time_t * const p_time_stamp)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    using namespace renesas::rcarxos::utils;
    if ((OSAL_CLOCK_TYPE_HIGH_RESOLUTION != clock_type) && (OSAL_CLOCK_TYPE_HIGH_PRECISION != clock_type))
    {
        return OSAL_RETURN_PAR;
    }

    if (NULL == p_time_stamp)
    {
        return OSAL_RETURN_PAR;
    }

    using Clock = std::chrono::high_resolution_clock;
    using Ns    = std::chrono::nanoseconds;
    using Sec   = std::chrono::seconds;

    const auto factor = std::chrono::duration_cast<Ns>(Sec(1)).count();

    std::chrono::time_point<Clock> timeNow = Clock::now();
    Ns                             nsec    = std::chrono::duration_cast<Ns>(timeNow.time_since_epoch());

    p_time_stamp->tv_nsec = narrow_check_cast<int32_t>(nsec.count() % factor);
    p_time_stamp->tv_sec  = narrow_check_cast<int32_t>(nsec.count() / factor);

    return OSAL_RETURN_OK;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ClockTimeCalculateTimeDifference")
#endif
e_osal_return_t R_OSAL_ClockTimeCalculateTimeDifference(const st_osal_time_t * const p_time2,
                                                        const st_osal_time_t * const p_time1,
                                                        osal_nano_sec_t * const      p_time_difference)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == p_time2 || NULL == p_time1 || NULL == p_time_difference)
    {
        return OSAL_RETURN_PAR;
    }
    
    if ((0 > p_time1->tv_sec) || (0 > p_time1->tv_nsec) || (OSAL_MAX_TV_NSEC < p_time1->tv_nsec))
    {
        return OSAL_RETURN_PAR;
    }
    else if ((0 > p_time2->tv_sec) || (0 > p_time2->tv_nsec) || (OSAL_MAX_TV_NSEC < p_time2->tv_nsec))
    {
        return OSAL_RETURN_PAR;
    }
    else
    {
        /*Do nothing*/
    }

    const auto factor  = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)).count();
    *p_time_difference = (factor * (p_time2->tv_sec - p_time1->tv_sec)) + (p_time2->tv_nsec - p_time1->tv_nsec);

    return OSAL_RETURN_OK;
}
/**
 * @}
 */
/**
 * \defgroup osal_wrapper osal_wrapper 
 * @brief implementation for simulation environment
 */


#ifndef R_SOC_INIT_HPP
#define R_SOC_INIT_HPP
/**
 * @brief Checks if socUnit is properly initialized using R_OSAL_Initialize()
 * 
 * @return true when socUnit is properly initilized
 * @return false when socUnit is not proprrly initialized
 * @ingroup soc_abstraction
 */
bool r_soc_initialized();

/**
 * @brief initializes the soc unit and nesecarry device configuations
 * 
 * @return true when sucssefull
 * @return false when failure
 * @ingroup soc_abstraction
 */
bool r_soc_init();

/**
 * @brief initializes the soc unit and performs nesecarry cleanup
 * 
 * @return true when sucssefull
 * @return false when failure
 * @ingroup soc_abstraction
 */
bool r_soc_deinit();

#endif // R_SOC_UNIT_INIT_HPP

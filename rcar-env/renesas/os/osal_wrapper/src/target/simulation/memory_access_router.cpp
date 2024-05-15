/** \addtogroup soc_abstraction
 *  @{
 */

#include "target/simulation/memory_access_router.hpp"

MemoryAccessRouter & MemoryAccessRouter::getInstance()
{
    static MemoryAccessRouter instance;
    return instance;
}
/** @}*/
#include "rcar-xos/generic_sim_handler/generic_sim_handler_ext.h"
#include "rcar-xos/utils/r_logging.hpp"
#include "rcar-xos/utils/r_utils.hpp"
#include "target/simulation/soc_abstraction/dev_cfg.hpp"
#include "target/simulation/memory_access_router.hpp"
#include "target/simulation/soc_abstraction/generic_soc.hpp"
#include "target/simulation/soc_abstraction/generic_simulator.hpp"
#include "target/simulation/impc_memory_access_router.hpp"
#include "target/simulation/sysram_memory_access_router.hpp"
#include "target/simulation/soc_abstraction/register_interface.hpp"
#include "target/simulation/soc_abstraction/soc_init.hpp"

#include <iostream>
#include <memory>
#include <vector>

#define __STDC_WANT_LIB_EXT1__ // needed to get getenv_s for some toolchains. see
                               // https://en.cppreference.com/w/c/program/getenv
#include <stdlib.h>

using namespace renesas::rcarxos::utils::literals;

#define ISP_SIMULATOR_DLL_NAME "libISP_SIM"
#define IMR_SIMULATOR_DLL_NAME "libIMR_SIM"
#define IMP_SUBSYSTEM_DLL_NAME "libIMPX_SIM"
#define DOF_SIMULATOR_DLL_NAME "libDOF_SIM"
#define ACF_SIMULATOR_DLL_NAME "libACF_SIM"
#define STV_SIMULATOR_DLL_NAME "libSTV_SIM"
#define SMDPS_SIMULATOR_DLL_NAME "libSMDPS_SIM"
#define SMDPOST_SIMULATOR_DLL_NAME "libSMDPOST_SIM"
#define DSP_SIMULATOR_DLL_NAME "libDSP_SIM"

#if defined(RCAR_V3M2)
#define IMP_SUBSYSTEM_SIMULATOR_NAME "IMPX_V3M2_SIM"
#define IMR_SIMULATOR_NAME "IMR_V3M2_SIM"
#elif defined(RCAR_V3H1)
#define IMP_SUBSYSTEM_SIMULATOR_NAME "IMPX_V3H1_SIM"
#define IMR_SIMULATOR_NAME "IMR_V3H1_SIM"
#define CISP_SIMULATOR_NAME "CISP_V3H1_SIM"
#define TISP_SIMULATOR_NAME "TISP_V3H1_SIM"
#elif defined(RCAR_V3H2)
#define IMP_SUBSYSTEM_SIMULATOR_NAME "IMPX_V3H2_SIM"
#define IMR_SIMULATOR_NAME "IMR_V3H2_SIM"
#define CISP_SIMULATOR_NAME "CISP_V3H2_SIM"
#define TISP_SIMULATOR_NAME "TISP_V3H2_SIM"
#define DOF_SIMULATOR_NAME "DOF_V3H2_SIM"
#define ACF_SIMULATOR_NAME "ACF_V3H2_SIM"
#define STV_SIMULATOR_NAME "STV_V3H2_SIM"
#elif defined(RCAR_V3U)
#define IMP_SUBSYSTEM_SIMULATOR_NAME "IMPX_V3U1_SIM"
#define IMR_SIMULATOR_NAME "IMR_V3U1_SIM"
#define CISP_SIMULATOR_NAME "CISP_V3U1_SIM"
#define TISP_SIMULATOR_NAME "TISP_V3U1_SIM"
#define DOF_SIMULATOR_NAME "DOF_V3U1_SIM"
#define ACF_SIMULATOR_NAME "ACF_V3U1_SIM"
#define STV_SIMULATOR_NAME "STV_V3U1_SIM"
#elif defined(RCAR_V4H1)
#define IMP_SUBSYSTEM_SIMULATOR_NAME "IMPX_V4H1_SIM"
#define IMR_SIMULATOR_NAME "IMR_V4H1_SIM"
#define CISP_SIMULATOR_NAME "CISP_V4H1_SIM"
#define TISP_SIMULATOR_NAME "TISP_V4H1_SIM"
#define DOF_SIMULATOR_NAME "DOF_V4H1_SIM"
// DOF8bit add OTFDCR
#define VIP_SIMULATOR_NAME "VIP_V4H1_SIM"
#define SMDPS_SIMULATOR_NAME "SMDPS_V4H1_SIM"
#define SMDPOST_SIMULATOR_NAME "SMDPOST_V4H1_SIM"
#define DSP_SIMULATOR_NAME "DSP_V4H1_SIM"
#elif defined(RCAR_V4H2)
#define IMP_SUBSYSTEM_SIMULATOR_NAME "IMPX_V4H2_SIM"
#define IMR_SIMULATOR_NAME "IMR_V4H2_SIM"
#define CISP_SIMULATOR_NAME "CISP_V4H2_SIM"
#define TISP_SIMULATOR_NAME "TISP_V4H2_SIM"
#define DOF_SIMULATOR_NAME "DOF_V4H2_SIM"
// DOF8bit add OTFDCR
#define VIP_SIMULATOR_NAME "VIP_V4H2_SIM"
#define SMDPS_SIMULATOR_NAME "SMDPS_V4H2_SIM"
#define SMDPOST_SIMULATOR_NAME "SMDPOST_V4H2_SIM"
#define DSP_SIMULATOR_NAME "DSP_V4H2_SIM"
#elif defined(RCAR_V4M)
#define IMP_SUBSYSTEM_SIMULATOR_NAME "IMPX_V4M1_SIM"
#define IMR_SIMULATOR_NAME "IMR_V4M1_SIM"
#define CISP_SIMULATOR_NAME "CISP_V4M1_SIM"
#define TISP_SIMULATOR_NAME "TISP_V4M1_SIM"
#define DOF_SIMULATOR_NAME "DOF_V4M1_SIM"
// DOF8bit add OTFDCR
#define VIP_SIMULATOR_NAME "VIP_V4M1_SIM"
#define SMDPS_SIMULATOR_NAME "SMDPS_V4M1_SIM"
#define SMDPOST_SIMULATOR_NAME "SMDPOST_V4M1_SIM"
#define DSP_SIMULATOR_NAME "DSP_V4M1_SIM"
#else
#error "unknown SoC. Only V3M2, V3H1, V3H2, V3U, V4H1, V4H2 and V4M1 are supported."
#endif

using osal_peripherals = std::vector<std::shared_ptr<RegisterInterfaceAxi>>;
std::unique_ptr<osal_peripherals> g_OsalPeripherals = nullptr;

static std::mutex g_OsalInitMutex;

bool r_soc_initialized()
{
    if (nullptr == g_OsalPeripherals)
    {
        return false;
    }
    return true;
}

bool r_soc_init()
{
    std::lock_guard<std::mutex> lk(g_OsalInitMutex);
    bool             ret = true;
    if (r_soc_initialized())
    {
        return false; // already initialized
    }

    /***  Initialize peripherals ***/
    g_OsalPeripherals = std::unique_ptr<osal_peripherals>(new osal_peripherals);

    if (nullptr == g_OsalPeripherals)
    {
        return false; // couldn't initialize
    }

    static auto socDeleter = [](RegisterInterfaceAxi * obj) {
         // Make sure the object is unregistered before deletion
        if(!GenericSoc::getInstance().unregisterPeripheralAddressSpace(*obj)) 
        {
            ERROR(std::cerr) << "Could not register peripheral" << std::endl;
            throw std::runtime_error("Could not register peripheral");
        }
        delete obj;
    };
    dev_cfg devcfg;
    /* Initialize all Simulation Devices */

    // [SIM_V060_CD_OSL001_03_002_001_001]
    g_OsalPeripherals->push_back(
        std::shared_ptr<RegisterInterfaceAxi>(new SysRamMemoryAccessRouter(), socDeleter));
    if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
    {
        ERROR(std::cerr) << "Could not register SystemRAM" << std::endl;
        ret = false;
    }
    // [Covers:SIM_V060_UD_OSL001_03_002_001]

    // [SIM_V060_CD_OSL001_03_002_001_002]
    if (ret)
    {
    // [Covers:SIM_V060_UD_OSL001_03_002_001]
        if (!genericSimHandlerExt_registerSharedLib(IMP_SUBSYSTEM_DLL_NAME))
        {
            ERROR(std::cerr) << "Could not load " << IMP_SUBSYSTEM_DLL_NAME << " shared library." << std::endl;
            ret = false;
        }
    // [SIM_V060_CD_OSL001_03_002_001_003]
    }
    // [Covers:SIM_V060_UD_OSL001_03_002_001]

    if (ret)
    {
        Osal_Device osal_dev, osal_dev_imp_00;     //need more than one device handle to comapre base address of simulator
        if (!dev_cfg::find_device("imp_top_00", osal_dev) || !dev_cfg::find_device("imp_00", osal_dev_imp_00))
        {
            ERROR(std::cerr) << "Could not load find imp_top_00 and/or imp_00 to setup simulator correctly." << std::endl;
            ret = false;
        }
        osal_axi_bus_id_t axi_id = devcfg.axi_id(osal_dev.get_busdomain());
        if (OSAL_AXI_BUS_ID_INVALID == axi_id)
        {
            ERROR(std::cerr) << "Could not get axi bus id for imp_top_00 to setup simulator correctly." << std::endl;
            ret = false;
        }
        if (ret)
        {  
            std::uintptr_t base_addr =std::min(osal_dev.get_base(), osal_dev_imp_00.get_base());
            /** @todo Is it the same ID for all subsystem peripherals? */
            auto           simulator = new GenericSimulator(IMP_SUBSYSTEM_SIMULATOR_NAME,
                                                  axi_id, 
                                                  base_addr, 0xFFBD4000 - base_addr, osal_dev.get_irq_num_list(),
                                                  &MemoryAccessRouter::getInstance());
            g_OsalPeripherals->push_back(std::shared_ptr<GenericSimulator>(simulator, socDeleter));
            if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
            {
                ERROR(std::cerr) << "Could not register IMP-Subsystem Simulators" << std::endl;
                ret = false;
            }

        }
        if (ret)
        {
            g_OsalPeripherals->push_back(
                std::shared_ptr<RegisterInterfaceAxi>(new ImpcMemoryAccessRouter(), socDeleter));
            if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
            {
                ERROR(std::cerr) << "Could not register IMPC" << std::endl;
                ret = false;
            }

        }
#if defined(RCAR_V3U)
        if (ret)
        {
            if (!dev_cfg::find_device("imp_ldmac_master_00", osal_dev))
            {
                ERROR(std::cerr) << "Could not load find imp_ldmac_master_00 to setup simulator correctly."
                                 << std::endl;
                ret = false;
            }
            osal_axi_bus_id_t axi_id = devcfg.axi_id(osal_dev.get_busdomain());
            if (OSAL_AXI_BUS_ID_INVALID == axi_id)
            {
                ERROR(std::cerr) << "Could not get axi bus id for imp_ldmac_master_00 to setup simulator correctly."
                                 << std::endl;
                ret = false;
            }
            /** @todo Is it the same ID for all subsystem peripherals? */
            auto simulator = new GenericSimulator("ILDMA_V3U1_SIM",
                                                  axi_id,
                                                  osal_dev.get_base(), osal_dev.get_range(),
                                                  osal_dev.get_irq_num_list(), &MemoryAccessRouter::getInstance());
            g_OsalPeripherals->push_back(std::shared_ptr<GenericSimulator>(simulator, socDeleter));
            if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
            {
                ERROR(std::cerr) << "Could not register imp_ldmac_master_00 simulator" << std::endl;
                ret = false;
            }

        }

        if (ret)
        {
            if (!dev_cfg::find_device("imp_ldmac_checker_00", osal_dev))
            {
                ERROR(std::cerr) << "Could not load find imp_ldmac_checker_00 to setup simulator correctly."
                                 << std::endl;
                ret = false;
            }
            osal_axi_bus_id_t axi_id = devcfg.axi_id(osal_dev.get_busdomain());
            if (OSAL_AXI_BUS_ID_INVALID == axi_id)
            {
                ERROR(std::cerr) << "Could not get axi bus id for imp_ldmac_checker_00 to setup simulator correctly."
                                 << std::endl;
                ret = false;
            }
            /** @todo Is it the same ID for all subsystem peripherals? */
            auto simulator = new GenericSimulator("ILDMA_V3U1_SIM",
                                                  axi_id,
                                                  osal_dev.get_base(), osal_dev.get_range(),
                                                  osal_dev.get_irq_num_list(), &MemoryAccessRouter::getInstance());
            g_OsalPeripherals->push_back(std::shared_ptr<GenericSimulator>(simulator, socDeleter));
            if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
            {
                ERROR(std::cerr) << "Could not register imp_ldmac_checker_00 simulator" << std::endl;
                ret = false;  
            }
        }
#endif
    }

    /* Initialize IMR */
    if (ret)
    {
        if (!genericSimHandlerExt_registerSharedLib(IMR_SIMULATOR_DLL_NAME))
        {
            ERROR(std::cerr) << "Could not load " << IMR_SIMULATOR_DLL_NAME << " shared library." << std::endl;
            ret = false;
        }
        else
        {
            std::vector<Osal_Device> osal_devs, osal_devs_imr;

            /* ims devices */
            osal_devs     = dev_cfg::getListofDevices("ims");
            osal_devs_imr = dev_cfg::getListofDevices(
                "imr"); /* simulators do not differenciate between IMR without rotator scaller extensions and with */
            osal_devs.insert(osal_devs.end(), osal_devs_imr.begin(), osal_devs_imr.end());
            for (auto dev : osal_devs)
            {
                osal_axi_bus_id_t     axi_id   = devcfg.axi_id(dev.get_busdomain());
                std::uintptr_t        baseAddr = dev.get_base();
                std::size_t           size     = dev.get_range();
                std::vector<uint32_t> spi      = dev.get_irq_num_list();
                g_OsalPeripherals->push_back(
                    std::shared_ptr<GenericSimulator>(new GenericSimulator(IMR_SIMULATOR_NAME, axi_id, baseAddr, size,
                                                                              spi, &MemoryAccessRouter::getInstance()),
                                                         socDeleter));
                if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
                {
                    ERROR(std::cerr) << "Could not register simulator peripheral [" << dev.get_id() << "]" << std::endl;
                    ret = false;
                }
            }
        }
    }

    /* ISP simulator */
#if defined(RCAR_V4H) || defined(RCAR_V3U) || defined(RCAR_V3H2) || defined(RCAR_V4M)
    if (ret)
    {
        if (!genericSimHandlerExt_registerSharedLib(ISP_SIMULATOR_DLL_NAME))
        {
            ERROR(std::cerr) << "Could not load " << ISP_SIMULATOR_DLL_NAME << " shared library." << std::endl;
            ret = false; 
        }
        else
        {
            std::vector<Osal_Device> osal_devs_cisp, osal_devs_tisp;

            /* cisp/tisp devices */
            osal_devs_cisp = dev_cfg::getListofDevices("cisp");
            osal_devs_tisp = dev_cfg::getListofDevices("tisp");

            for (auto dev : osal_devs_cisp)
            {
                osal_axi_bus_id_t     axi_id   = devcfg.axi_id(dev.get_busdomain());
                std::uintptr_t        baseAddr = dev.get_base();
                std::size_t           size     = dev.get_range();
                std::vector<uint32_t> spi      = dev.get_irq_num_list();
                g_OsalPeripherals->push_back(std::shared_ptr<GenericSimulator>(
                    new GenericSimulator(CISP_SIMULATOR_NAME, axi_id, baseAddr, size, spi,
                                         &MemoryAccessRouter::getInstance()),
                    socDeleter));
                if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(),
                                                                              true))
                {
                    ERROR(std::cerr) << "Could not register simulator peripheral [" << dev.get_id() << "]" << std::endl;
                    ret = false;
                }
            }

            for (auto dev : osal_devs_tisp)
            {
                osal_axi_bus_id_t     axi_id   = devcfg.axi_id(dev.get_busdomain());
                std::uintptr_t        baseAddr = dev.get_base();
                std::size_t           size     = dev.get_range();
                std::vector<uint32_t> spi      = dev.get_irq_num_list();
                g_OsalPeripherals->push_back(std::shared_ptr<GenericSimulator>(
                    new GenericSimulator(TISP_SIMULATOR_NAME, axi_id, baseAddr, size, spi,
                                         &MemoryAccessRouter::getInstance()),
                    socDeleter));
                if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(),
                                                                              true))
                {
                    ERROR(std::cerr) << "Could not register simulator peripheral [" << dev.get_id() << "]" << std::endl;
                    ret = false;
                }
            }
        }
    }
#endif

    /* DOF simulator */
#if defined(RCAR_V4H) || defined(RCAR_V3U) || defined(RCAR_V3H2) || defined(RCAR_V4M)
    if (ret)
    {
        if (!genericSimHandlerExt_registerSharedLib(DOF_SIMULATOR_DLL_NAME))
        {
            ERROR(std::cerr) << "Could not load " << DOF_SIMULATOR_DLL_NAME << " shared library." << std::endl;
            ret = false;
        }
        else
        {
            // DOF8bit add OTFDCR
            std::vector<Osal_Device> osal_devs, osal_devs_vip;

            /* dof devices */
            osal_devs     = dev_cfg::getListofDevices("dof");
            for (auto dev : osal_devs)
            {
                osal_axi_bus_id_t     axi_id   = devcfg.axi_id(dev.get_busdomain());
                std::uintptr_t        baseAddr = dev.get_base();
                std::size_t           size     = dev.get_range();
                std::vector<uint32_t> spi      = dev.get_irq_num_list();
                g_OsalPeripherals->push_back(
                    std::shared_ptr<GenericSimulator>(new GenericSimulator(DOF_SIMULATOR_NAME, axi_id, baseAddr, size,
                                                                              spi, &MemoryAccessRouter::getInstance()),
                                                         socDeleter));
                if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
                {
                    ERROR(std::cerr) << "Could not register simulator peripheral [" << dev.get_id() << "]" << std::endl;
                    ret = false;
                }
            }
// DOF8bit add OTFDCR
#if defined(RCAR_V4H) || defined(RCAR_V4M)
            osal_devs_vip = dev_cfg::getListofDevices("vip_wrap");
            for (auto dev : osal_devs_vip)
            {
                osal_axi_bus_id_t     axi_id   = devcfg.axi_id(dev.get_busdomain());
                std::uintptr_t        baseAddr = dev.get_base();
                std::size_t           size     = dev.get_range();
                std::vector<uint32_t> spi      = dev.get_irq_num_list();
                g_OsalPeripherals->push_back(
                    std::shared_ptr<GenericSimulator>(new GenericSimulator(VIP_SIMULATOR_NAME, axi_id, baseAddr, size,
                                                                              spi, &MemoryAccessRouter::getInstance()),
                                                         socDeleter));
                if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
                {
                    ERROR(std::cerr) << "Could not register simulator peripheral [" << dev.get_id() << "]" << std::endl;
                    ret = false;
                }
            }
#endif // RCAR_V4H
        }
    }
#endif

    /* ACF simulator */
#if defined(RCAR_V3U) || defined(RCAR_V3H2)
    if (ret)
    {
        if (!genericSimHandlerExt_registerSharedLib(ACF_SIMULATOR_DLL_NAME))
        {
            ERROR(std::cerr) << "Could not load " << ACF_SIMULATOR_DLL_NAME << " shared library." << std::endl;
            ret = false;
        }
        else
        {
            std::vector<Osal_Device> osal_devs;

            /* acf devices */
            osal_devs     = dev_cfg::getListofDevices("acf");
            for (auto dev : osal_devs)
            {
                osal_axi_bus_id_t     axi_id   = devcfg.axi_id(dev.get_busdomain());
                std::uintptr_t        baseAddr = dev.get_base();
                std::size_t           size     = dev.get_range();
                std::vector<uint32_t> spi      = dev.get_irq_num_list();
                g_OsalPeripherals->push_back(
                    std::shared_ptr<GenericSimulator>(new GenericSimulator(ACF_SIMULATOR_NAME, axi_id, baseAddr, size,
                                                                              spi, &MemoryAccessRouter::getInstance()),
                                                         socDeleter));
                if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
                {
                    ERROR(std::cerr) << "Could not register simulator peripheral [" << dev.get_id() << "]" << std::endl;
                    ret = false;
                }
            }
        }
    }
#endif

    /* STV simulator */
#if defined(RCAR_V3U) || defined(RCAR_V3H2)
    if (ret)
    {
        if (!genericSimHandlerExt_registerSharedLib(STV_SIMULATOR_DLL_NAME))
        {
            ERROR(std::cerr) << "Could not load " << STV_SIMULATOR_DLL_NAME << " shared library." << std::endl;
            ret = false;
        }
        else
        {
            std::vector<Osal_Device> osal_devs;

            /* stv devices */
            osal_devs     = dev_cfg::getListofDevices("stv");
            for (auto dev : osal_devs)
            {
                osal_axi_bus_id_t     axi_id   = devcfg.axi_id(dev.get_busdomain());
                std::uintptr_t        baseAddr = dev.get_base();
                std::size_t           size     = dev.get_range();
                std::vector<uint32_t> spi      = dev.get_irq_num_list();
                g_OsalPeripherals->push_back(
                    std::shared_ptr<GenericSimulator>(new GenericSimulator(STV_SIMULATOR_NAME, axi_id, baseAddr, size,
                                                                              spi, &MemoryAccessRouter::getInstance()),
                                                         socDeleter));
                if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
                {
                    ERROR(std::cerr) << "Could not register simulator peripheral [" << dev.get_id() << "]" << std::endl;
                    ret = false;
                }
            }
        }
    }
#endif

    /* SMDPS simulator */
#if defined(RCAR_V4H) || defined(RCAR_V4M)
    if (ret)
    {
        if (!genericSimHandlerExt_registerSharedLib(SMDPS_SIMULATOR_DLL_NAME))
        {
            ERROR(std::cerr) << "Could not load " << SMDPS_SIMULATOR_DLL_NAME << " shared library." << std::endl;
            ret = false;
        }
        else
        {
            std::vector<Osal_Device> osal_devs;

            /* smdps devices */
            osal_devs     = dev_cfg::getListofDevices("smd_ps");
            for (auto dev : osal_devs)
            {
                osal_axi_bus_id_t     axi_id   = devcfg.axi_id(dev.get_busdomain());
                std::uintptr_t        baseAddr = dev.get_base();
                std::size_t           size     = dev.get_range();
                std::vector<uint32_t> spi      = dev.get_irq_num_list();
                g_OsalPeripherals->push_back(
                    std::shared_ptr<GenericSimulator>(new GenericSimulator(SMDPS_SIMULATOR_NAME, axi_id, baseAddr, size,
                                                                              spi, &MemoryAccessRouter::getInstance()),
                                                         socDeleter));
                if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
                {
                    ERROR(std::cerr) << "Could not register simulator peripheral [" << dev.get_id() << "]" << std::endl;
                    ret = false;
                }
            }
        }
    }
#endif

    /* SMDPOST simulator */
#if defined(RCAR_V4H) || defined(RCAR_V4M)
    if (ret)
    {
        if (!genericSimHandlerExt_registerSharedLib(SMDPOST_SIMULATOR_DLL_NAME))
        {
            ERROR(std::cerr) << "Could not load " << SMDPOST_SIMULATOR_DLL_NAME << " shared library." << std::endl;
            ret = false;
        }
        else
        {
            std::vector<Osal_Device> osal_devs;

            /* smdpost devices */
            osal_devs     = dev_cfg::getListofDevices("smd_post");
            for (auto dev : osal_devs)
            {
                osal_axi_bus_id_t     axi_id   = devcfg.axi_id(dev.get_busdomain());
                std::uintptr_t        baseAddr = dev.get_base();
                std::size_t           size     = dev.get_range();
                std::vector<uint32_t> spi      = dev.get_irq_num_list();
                g_OsalPeripherals->push_back(
                    std::shared_ptr<GenericSimulator>(new GenericSimulator(SMDPOST_SIMULATOR_NAME, axi_id, baseAddr, size,
                                                                              spi, &MemoryAccessRouter::getInstance()),
                                                         socDeleter));
                if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
                {
                    ERROR(std::cerr) << "Could not register simulator peripheral [" << dev.get_id() << "]" << std::endl;
                    ret = false;
                }
            }
        }
    }
#endif

    /* DSP simulator */
#if  defined(RCAR_V4H) || defined(RCAR_V4M)
    if (ret)
    {
        if (!genericSimHandlerExt_registerSharedLib(DSP_SIMULATOR_DLL_NAME))
        {
            ERROR(std::cerr) << "Could not load " << DSP_SIMULATOR_DLL_NAME << " shared library." << std::endl;
            ret = false;
        }
        else
        {
            std::vector<Osal_Device> osal_dev0, osal_dev1, osal_dev2, osal_dev3;

            /* dsp devices */
            osal_dev0     = dev_cfg::getListofDevices("vdsp00");
            osal_dev1     = dev_cfg::getListofDevices("vdsp01");
#if defined(RCAR_V4H)
            osal_dev2     = dev_cfg::getListofDevices("vdsp02");
            osal_dev3     = dev_cfg::getListofDevices("vdsp03");
#endif

            osal_dev0.insert(osal_dev0.end(), osal_dev1.begin(), osal_dev1.end());
#if defined(RCAR_V4H)
            osal_dev0.insert(osal_dev0.end(), osal_dev2.begin(), osal_dev2.end());
            osal_dev0.insert(osal_dev0.end(), osal_dev3.begin(), osal_dev3.end());
#endif

            for (auto dev : osal_dev0)
            {
                osal_axi_bus_id_t     axi_id   = devcfg.axi_id(dev.get_busdomain());
                std::uintptr_t        baseAddr = dev.get_base();
                std::size_t           size     = dev.get_range();
                std::vector<uint32_t> spi      = dev.get_irq_num_list();
                g_OsalPeripherals->push_back(
                    std::shared_ptr<GenericSimulator>(new GenericSimulator(DSP_SIMULATOR_NAME, axi_id, baseAddr, size,
                                                                              spi, &MemoryAccessRouter::getInstance()),
                                                         socDeleter));
                if (!GenericSoc::getInstance().registerPeripheralAddressSpace(*g_OsalPeripherals->back(), true))
                {
                    ERROR(std::cerr) << "Could not register simulator peripheral [" << dev.get_id() << "]" << std::endl;
                    ret = false;
                }
            }
        }
    }
#endif



    if (!ret)
    {
        g_OsalPeripherals.reset();
        genericSimHandlerExt_unregisterSharedLib();
    }

    /* Initialize Manager Instances */

    return ret;
}

bool r_soc_deinit()
{
    std::lock_guard<std::mutex> lk(g_OsalInitMutex);
    if (nullptr == g_OsalPeripherals)
    {
        return false;
    }
    // release resources
    g_OsalPeripherals.reset();

    if (!genericSimHandlerExt_unregisterSharedLib())
    {
        return false; // couldn't unregister shareb lib correctly
    }
    return true;
}

/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/** \addtogroup soc_abstraction
 *  @{
 */

#ifndef IRQINTERFACE_H
#define IRQINTERFACE_H

#include "peripheral_interface.hpp"
#include <functional>
#include <iostream>
#include <map>
#include <stdint.h>
#include <utility>
#include <vector>
#include <thread>
#include <mutex>

#ifdef HAVE_EASY_PROFILER
#    include <easy/arbitrary_value.h>
#    include <easy/profiler.h>
#endif

class IrqControllerInterface {
protected:
    std::map<uint32_t, std::function<void(uint32_t)>> m_IrqHandler;
    std::map<uint32_t, bool>                          m_EnabledSPI;
    std::mutex                                        m_mtx;

public:
    IrqControllerInterface() {}
    virtual ~IrqControllerInterface() {}

    virtual bool registerIrqSource(PeripheralInterface& irqSource, std::vector<uint32_t> spi)
    {
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        std::lock_guard<std::mutex> lock(m_mtx);
        for (auto spiNo : spi)
        {
            irqSource.registerIrqObserver(spiNo, std::bind(&IrqControllerInterface::signalIrq, this, std::placeholders::_1));
            m_EnabledSPI[spiNo] = false;
        }
        return true;
    }

    virtual bool registerIrqSource(PeripheralInterface& irqSource)
    {
        return registerIrqSource(irqSource, irqSource.irqSpiList());
    }

    template<typename IrqHandler>
    void registerIrqHandler(uint32_t spi, IrqHandler&& irqHandler)
    {
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        //std::clog << __PRETTY_FUNCTION__ << " Register spi=" << spi << std::endl;
        std::lock_guard<std::mutex> lock(m_mtx);
        m_IrqHandler[spi] = std::forward<IrqHandler>(irqHandler);
    }

    void removeIrqHandler(uint32_t spi)
    {
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        std::lock_guard<std::mutex> lock(m_mtx);
        if (m_IrqHandler.find(spi) == m_IrqHandler.end())
        {
            // std::clog << __PRETTY_FUNCTION__ << " Not in list spi=" << spi << std::endl;
            return;
        }
        //std::clog << __PRETTY_FUNCTION__ << " Remove Handler for spi=" << spi << std::endl;
        m_IrqHandler.erase(spi);
        m_EnabledSPI[spi] = false;
    }

    virtual bool enableIrq(uint32_t spi)
    {
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        //std::clog << __PRETTY_FUNCTION__ << " Enable spi=" << spi << std::endl;
        std::lock_guard<std::mutex> lock(m_mtx);
        m_EnabledSPI[spi] = true;
        return true;
    }

    virtual bool disableIrq(uint32_t spi)
    {
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        //std::clog << __PRETTY_FUNCTION__ << " Disable spi=" << spi << std::endl;
        std::lock_guard<std::mutex> lock(m_mtx);
        m_EnabledSPI[spi] = false;
        return true;
    }

    virtual void signalIrq(uint32_t spi)
    {
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        std::vector<std::thread> th_vec;
        m_mtx.lock();
        if (m_EnabledSPI.find(spi) == m_EnabledSPI.end())
        {
            //std::clog << __PRETTY_FUNCTION__ << " not in list spi=" << spi << std::endl;
            m_mtx.unlock();
            return;
        }
        if (m_EnabledSPI[spi] == false)
        {
            //std::clog << __PRETTY_FUNCTION__ << " Not Enabled spi=" << spi << std::endl;
            m_mtx.unlock();
            return;
        }
        if (m_IrqHandler.find(spi) != m_IrqHandler.end())
        {
#ifdef HAVE_EASY_PROFILER
            std::string tmp = "Execute IRQ Handler" + std::to_string(spi);
            EASY_BLOCK(tmp.c_str(), profiler::colors::Blue800);
#endif
            //std::clog << __PRETTY_FUNCTION__ << " Call Handler spi=" << spi << std::endl;
            //m_IrqHandler[spi](spi);
            th_vec.push_back(std::thread(m_IrqHandler[spi], spi));
        }
        m_mtx.unlock();

        for(std::thread& th: th_vec)
        {
           th.join();
        }
    }
};

/** @}*/

#endif   // IRQINTERFACE_H

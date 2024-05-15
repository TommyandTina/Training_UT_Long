
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

#ifndef PERIPHERALINTERFACE_H
#define PERIPHERALINTERFACE_H

#include "register_interface.hpp"
#include <functional>
#include <initializer_list>
#include <iostream>
#include <map>
#include <stddef.h>
#include <stdint.h>
#include <utility>
#include <vector>
#include <thread>
#include <mutex>

#ifdef HAVE_EASY_PROFILER
#    include <easy/arbitrary_value.h>
#    include <easy/profiler.h>
#endif

class PeripheralInterface : public RegisterInterfaceAxi {
    std::vector<uint32_t> m_Spi;

protected:
    std::map<uint32_t, std::vector<std::function<void(uint32_t)>>> m_IrqObservers;
    std::map<uint32_t, std::vector<std::function<void(uint32_t)>>> m_IrqClearedObservers;
    std::mutex                                                     m_mtx;
    
public:
    explicit PeripheralInterface(uint32_t spi)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_Spi.push_back(spi);
    }
    explicit PeripheralInterface(std::initializer_list<uint32_t> spi_list)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_Spi.insert(m_Spi.end(), spi_list.begin(), spi_list.end());
    }
    explicit PeripheralInterface(std::vector<uint32_t> spi_list)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_Spi = spi_list;
    }

    template<typename Observer>
    void registerIrqObserver(uint32_t spi, Observer&& observer)
    {
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        std::lock_guard<std::mutex> lock(m_mtx);
        m_IrqObservers[spi].push_back(std::forward<Observer>(observer));
    }

    template<typename Observer>
    void registerIrqClearedObserver(uint32_t spi, Observer&& observer)
    {
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        std::lock_guard<std::mutex> lock(m_mtx);
        m_IrqClearedObservers[spi].push_back(std::forward<Observer>(observer));
    }

    virtual bool hasIrqClearedSignal() const
    {
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        return false;
    }

    void signalIrq(uint32_t spi)
    {
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        std::vector<std::thread> th_vec;
        m_mtx.lock();
        if (m_IrqObservers.find(spi) == m_IrqObservers.end())
        {
            //std::clog << __PRETTY_FUNCTION__ << " no observer for spi=" << spi << std::endl;
            m_mtx.unlock();
            return;   //no observers assigned
        }
        for (auto& obs : m_IrqObservers[spi])
        {
            //std::clog << __PRETTY_FUNCTION__ << " Call Observer for spi=" << spi << std::endl;
            //obs(spi);
            th_vec.push_back(std::thread(obs, spi));
        }
        m_mtx.unlock();
        for(std::thread& th: th_vec)
        {
            th.join();
        }
    }

    void signalIrqCleared(uint32_t spi)
    {
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        std::vector<std::thread> th_vec;
        m_mtx.lock();
        if (m_IrqClearedObservers.find(spi) == m_IrqClearedObservers.end())
        {
            //std::clog << __PRETTY_FUNCTION__ << " No observer spi=" << spi << std::endl;
            m_mtx.unlock();
            return;   //no observers assigned
        }
        for (auto& obs : m_IrqClearedObservers[spi])
        {
            // std::clog << __PRETTY_FUNCTION__ << " Call spi=" << spi << std::endl;
            //obs(spi);
            th_vec.push_back(std::thread(obs, spi));
        }
        m_mtx.unlock();
        for(std::thread& th: th_vec)
        {
            th.join();
        }
    }

    virtual std::vector<uint32_t> irqSpiList() const
    {
        return m_Spi;
    }
};

/** @}*/

#endif   // PERIPHERALINTERFACE_H

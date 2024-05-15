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

#ifndef TLB_HPP
#define TLB_HPP

#include "rcar-xos/vmsav8/r_vmsav8.hpp"
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

/**
 * @brief Provides 2-way address translation to an object of Type Tref done using configurable IA-32e paging scheme (4
 * Layer)
 *
 * @see https://www.cs.rutgers.edu/~pxk/416/notes/10-paging.html
 *
 * @tparam Tref Type of the object which is referenced to by the page tables
 */
template <typename Tref> class Tlb
{
public:
    typedef Tref RefValueType; //!< Type of the object which is referenced to by the page tables

    using TranslationTable = renesas::rcarxos::utils::vmsav8_64::TranslationTableSim<Tref *>;
    TranslationTable m_virt2physStage;
    TranslationTable m_phys2virtStage;

    mutable std::recursive_mutex m_mutex;

protected:
public:
    /**
     * @brief Construct a new Tlb object
     *
     * @note The bits used the Page Table Directory Ptr Tags are derived from the remaining bits available in the
     * architecture type (e.g. 64Bit or 32Bit)
     *
     * @param offsetWidth Number of Bits used for offset
     * @param pageTableBits Number of Bits used for Page Table Index
     * @param directoryBits Number of Bits used for Page Table Directory Index
     * @param directoryPtrBits Number of Bits used for Page Table Directory Pointer Index
     */
    Tlb()
    {
    }

    /**
     * @brief Adds entries to the Phys2Virt and Virt2Phys translation tables for the given memory block
     *
     * @param physAddr Physical Address of the memory block
     * @param virtPtr Virtual Address of the memory block
     * @param size of the memory block
     * @param ref object reference the address shall refer to
     * @return true when the memory block could be added to the page tables
     * @return false when the memory block could not be added to the page tables. E.g. when the address range is already
     * assigned
     */
    bool addEntry(uintptr_t physAddr, void * virtPtr, size_t size, Tref * ref)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        if (virtPtr != nullptr)
        {
            if (!m_virt2physStage.addEntry((uintptr_t)virtPtr, size, ref))
            {
                return false;
            }
        }
        if (!m_phys2virtStage.addEntry((uintptr_t)physAddr, size, ref))
        {
            if (virtPtr != nullptr)
            {
                m_virt2physStage.removeEntry((uintptr_t)virtPtr, size);
            }
            return false;
        }
        return true;
    }

    /**
     * @brief Removes entries to the Phys2Virt and Virt2Phys translation tables for the given memory block
     *
     * @param physAddr Physical Address of the memory block
     * @param virtPtr Virtual Address of the memory block
     * @param size of the memory block
     * @return true
     * @return false
     */
    bool removeEntry(uintptr_t physAddr, const void * virtPtr, size_t size)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        if (virtPtr != nullptr)
        {
            if (!m_virt2physStage.removeEntry((uintptr_t)virtPtr, size))
            {
                return false;
            }
        }
        if (!m_phys2virtStage.removeEntry(physAddr, size))
        {
            return false;
        }
        return true;
    }

    /**
     * @brief Get the Ref From Virtual address
     *
     * @param virtPtr virtual address/ptr the reference object is referenced from
     * @return Tref* object reference assigned to the address
     * @return nullptr when no object is references to by the given address
     */
    inline Tref * getRefFromVirtPtr(const void * virtPtr)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        Tref *                                value = nullptr;
        m_virt2physStage.getValueFromAddr((uintptr_t)virtPtr, value);
        return value;
    }

    /**
     * @brief Get the Ref From physical address
     *
     * @param physAddr physicla address the reference object is referenced from
     * @return Tref* object reference assigned to the address
     * @return nullptr when no object is references to by the given address
     */
    inline Tref * getRefFromPhysAddr(uintptr_t physAddr)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        Tref *                                value = nullptr;
        m_phys2virtStage.getValueFromAddr((uintptr_t)physAddr, value);
        return value;
    }

    /**
     * @brief returns the alignment/size and pagetable entry block
     *
     * @return size_t required alignment/size of an entry block
     */
    inline size_t alignment() const
    {
        return m_phys2virtStage.alignment();
    }

    /**
     * @brief Get the Virtual Address Range For Ref object
     *
     * @param ref Reference entry in tables
     * @return std::pair<uintptr_t, uintptr_t> first entry is start address and second end address. When no entry found
     * the address is set to 0
     */
    std::pair<uintptr_t, uintptr_t> getVirtRangeForRef(Tref * ref)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        return getRangeForRef(ref, m_virt2physStage);
    }

    /**
     * @brief Get the Physical Address Range For Ref object
     *
     * @param ref Reference entry in tables
     * @return std::pair<uintptr_t, uintptr_t> first entry is start address and second end address. When no entry found
     * the address is set to 0
     */
    std::pair<uintptr_t, uintptr_t> getPhysRangeForRef(Tref * ref)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        return getRangeForRef(ref, m_phys2virtStage);
    }

    /**
     * @brief Prints current configuration and entries of the tlb
     *
     * @param os stream to write to
     */
    void print(std::ostream & os)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        os << "print for tlb.hpp not implemented yet" << std::endl;
        //    size_t                                tableGranule[3] = {0};
        //    tableGranule[2]                                       = ((size_t)0x1 << m_OffsetBits);
        //    tableGranule[1]                                       = tableGranule[2] * ((size_t)0x1 <<
        //    m_PageTableBits); tableGranule[0]                                       = tableGranule[1] * ((size_t)0x1
        //    << m_PageDirectoryBits);

        //    os << "TLB platform is " << sizeof(void*) * 8 << "Bit" << std::endl;
        //    os << "TLB uses configurable IA-32e paging scheme (4 Layer) " << std::endl;

        //    os << "   PageTable uses " << m_PageTableBits << " Bits aka Entries: " << m_PageTableSize << std::endl;
        //    os << "   PageDirectory uses " << m_PageDirectoryBits << " Bits aka Entries: " << m_PageDirectorySize <<
        //    std::endl; os << "   PageDirectoryPtr uses " << m_PageDirectoryPtrBits << " Bits aka Entries: " <<
        //    m_PageDirectoryPtrSize << std::endl; os << "   PageDirectoryPtrTags use " << m_PageDirectoryPtrTagBits <<
        //    " Bits aka Entries: " << (0x1U << m_PageDirectoryPtrTagBits) << std::endl;

        //    os << "   Entry Block Size = " << ((size_t)0x1 << m_OffsetBits) << std::endl;
        //    os << "   PageTable Granule = " << tableGranule[2] / 1024 << "KB" << std::endl;
        //    os << "   PageDirectory Granule = " << tableGranule[1] / 1024 << "KB" << std::endl;
        //    os << "   PageDirectoryPtr Granule = " << tableGranule[0] / 1024 / 1024 << "MB" << std::endl;
        //    os << "   PageDirectoryPtrTag granule = " << ((size_t)0x1 << m_PageDirectoryPtrTagBits) * tableGranule[0]
        //    / 1024 / 1024 / 1024 << "GB" << std::endl;

        //    os << "  Current entries [phys-range][virtual-range] --> ref pointer:" << std::endl;
        //    Tref*     refEntry   = nullptr;
        //    uintptr_t entryStart = 0;
        //    uintptr_t entryEnd   = 0;
        //    for (auto tag : m_phys2virtStage)
        //    {
        //        uintptr_t pageDirPrtAddr = tag.first << (m_PageDirectoryPtrBits + m_PageDirectoryBits +
        //        m_PageTableBits + m_OffsetBits); for (auto pageDirPtr : *tag.second)
        //        {
        //            uintptr_t pageDirAddr = pageDirPrtAddr;
        //            if (pageDirPtr == nullptr)
        //            {
        //                pageDirPrtAddr += static_cast<decltype(pageDirPrtAddr)>(0x1) << (m_PageDirectoryBits +
        //                m_PageTableBits + m_OffsetBits); continue;
        //            }
        //            for (auto pageDir : *pageDirPtr)
        //            {
        //                uintptr_t addr = pageDirAddr;
        //                if (pageDir == nullptr)
        //                {
        //                    pageDirAddr += 0x1 << (m_PageTableBits + m_OffsetBits);
        //                    continue;
        //                }
        //                for (auto pageEl : *pageDir)
        //                {
        //                    if (refEntry != pageEl)
        //                    {
        //                        if (refEntry != nullptr)
        //                        {
        //                            //print here:
        //                            os << "   [0x" << std::hex << entryStart << "-0x" << entryEnd << "]";
        //                            auto virtRange = getVirtRangeForRef(refEntry);
        //                            os << "[0x" << std::hex << virtRange.first << "-0x" << virtRange.second << "]";
        //                            os << " --> Ref = 0x" << refEntry << std::endl;
        //                        }
        //                        refEntry   = pageEl;
        //                        entryStart = addr;
        //                        entryEnd   = addr + (0x1 << m_OffsetBits) - 1;
        //                    }
        //                    else
        //                    {
        //                        entryEnd = addr + (0x1 << m_OffsetBits) - 1;
        //                    }
        //                    addr += 0x1 << m_OffsetBits;
        //                }
        //                pageDirAddr += 0x1 << (m_PageTableBits + m_OffsetBits);
        //            }
        //            pageDirPrtAddr += 0x1 << (m_PageDirectoryBits + m_PageTableBits + m_OffsetBits);
        //        }
        //    }
        //    std::cout << std::dec;
    }
};

#endif // TLB_HPP
/** @}*/
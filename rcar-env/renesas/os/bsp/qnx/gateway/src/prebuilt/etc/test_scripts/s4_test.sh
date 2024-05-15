#!/bin/sh

set -m

test_opt=$@
test_num=0
test_idx=0
test_skip=""
test_multi=""
test_table=""

RED='\033[1;31m'
BLU='\033[1;34m'
DEF='\033[0m'

print_help()
{
    echo "\nUsage: $1 <options>\n"
    echo "Available options:\n"
    echo "For test specific driver"
    echo "  # $1 <module>\n"
    echo "For test multiple drivers"
    echo "  # $1 <module1> <module2> ... <moduleN>\n"
    echo "For test all drivers"
    echo "  # $1 all\n"
    echo "For test all, but except some drivers"
    echo "  # $1 all <-module1> <-module2> ... <-moduleN>\n"
    echo "List supported modules:"

    test_idx=0

    while [ $test_idx -lt ${#test_table[@]} ]; do
        eval name=\${${test_table[$test_idx]}[0]}
        test_idx=$((test_idx+1))
        echo "  $name"
    done
}

log_err()
{
    echo "${RED}$1${DEF}"
}

log_inf()
{
    echo "${BLU}$1${DEF}"
}

log_ask()
{
    echo -n "${BLU}$1${DEF}"
}

run_cmd()
{
    echo "# $1"
    eval $1
}

check_instance()
{
    pattern=$(echo $1 | sed 's/./[\0]/')

    if ! pidin ar | grep -q "${pattern}"; then
        return 1
    fi

    return 0
}

check_file()
{
    for file in "$@"; do
        if [ ! -e ${file} ]; then
            return 1
        fi
    done

    return 0
}

check_util()
{
    for util in "$@"; do
        if ! type ${util} > /dev/null; then
            log_err "${util} is not found"
            return 1
        fi
    done

    return 0
}

set_cpg()
{
    mstpcr5=$(io 32 0xE6152D14 | grep 0x | awk {'print$2'})
    mstpcr7=$(io 32 0xE6152D1C | grep 0x | awk {'print$2'})

    case $1 in
        scif0)
            mstpcr7=`printf 0x%X $((mstpcr7 & 0xfffffffb))`
            ;;
        scif3)
            mstpcr7=`printf 0x%X $((mstpcr7 & 0xffffffef))`
            ;;
        hscif0)
            mstpcr5=`printf 0x%X $((mstpcr5 & 0xffffbfff))`
            ;;
        hscif1)
            mstpcr5=`printf 0x%X $((mstpcr5 & 0xffff7fff))`
            ;;
        *)
            log_err "Unsupport module."
            return 0
    esac

    cpgwpr=$(echo 0x$(printf "%X" $((~mstpcr5)) | grep '........$'))

    run_cmd "io 32 0xE6150000 $cpgwpr > /dev/null"
    run_cmd "io 32 0xE6152D14 $mstpcr5 > /dev/null"

    cpgwpr=$(echo 0x$(printf "%X" $((~mstpcr7)) | grep '........$'))

    run_cmd "io 32 0xE6150000 $cpgwpr > /dev/null"
    run_cmd "io 32 0xE6152D1C $mstpcr7 > /dev/null"
}

set_pfc()
{
    ip0sr=$(io 32 0xE6050060 | grep 0x | awk {'print$2'})
    ip1sr=$(io 32 0xE6050064 | grep 0x | awk {'print$2'})

    case $1 in
        scif0)
            ip0sr=`printf 0x%X $((ip0sr & 0x00fffff0))`
            ip1sr=`printf 0x%X $((ip1sr & 0xfffff000))`
            ;;
        scif3)
            ip0sr=`printf 0x%X $((ip0sr & 0xff000000))` 
            ip0sr=`printf 0x%X $((ip0sr | 0x00111110))`
            ;;
        hscif0)
            ip0sr=`printf 0x%X $((ip0sr & 0xff000000))`
            ;;
        hscif1)
            ip0sr=`printf 0x%X $((ip0sr & 0x00fffff0))`
            ip1sr=`printf 0x%X $((ip1sr & 0xfffff000))`
            ip0sr=`printf 0x%X $((ip0sr | 0x11000000))`
            ip1sr=`printf 0x%X $((ip1sr | 0x00000111))`
            ;;
        *)
            log_err "Unsupport module."
            return 0
    esac

    pmmr=$(echo 0x$(printf "%X" $((~ip0sr)) | grep '........$'))

    run_cmd "io 32 0xE6050000 $pmmr > /dev/null"
    run_cmd "io 32 0xE6050060 $ip0sr > /dev/null"

    pmmr=$(echo 0x$(printf "%X" $((~ip1sr)) | grep '........$'))

    run_cmd "io 32 0xE6050000 $pmmr > /dev/null"
    run_cmd "io 32 0xE6050064 $ip1sr > /dev/null"
}

run_smp_test()
{
    echo "#########################################################################"
    echo "# SMP Test                                                              #"
    echo "#########################################################################"

    TEXT="SMP Test"

    for idx in 0 1 2 3 4 5 6 7; do
        case $idx in
            0 ) mask="0x01" ;;
            1 ) mask="0x02" ;;
            2 ) mask="0x04" ;;
            3 ) mask="0x08" ;;
            4 ) mask="0x10" ;;
            5 ) mask="0x20" ;;
            6 ) mask="0x40" ;;
            7 ) mask="0x80" ;;
        esac
        run_cmd "on -R$mask echo \"$TEXT\" > /tmp/smp_cpu${idx} &"
    done

    run_cmd "sleep 1"

    for idx in 0 1 2 3 4 5 6 7; do
        if [[ "$TEXT" != $(cat /tmp/smp_cpu${idx}) ]]; then
            log_err "CPU${idx} did not run yet."
        else
            log_inf "CPU${idx} is runnable."
        fi
    done
}

run_network_test()
{
    echo "#########################################################################"
    echo "# Network Test                                                          #"
    echo "#########################################################################"

    CASES="PING SPEED STRESS10M STRESS100M STRESS1000M"

    IOPKT="io-pkt-v6-hc"
    IFACE="tsn0"

    if ! check_util ifconfig ping iperf3; then
        return 1
    fi

    if ! check_instance ${IOPKT}; then
        log_err "${IOPKT} is not running."
        return 1
    fi

    ipaddr=$(ifconfig ${IFACE} | awk '{ print $2}' | grep -E "([0-9]{1,3}[\.]){3}[0-9]{1,3}")

    if [ -z "${ipaddr}" ]; then
        log_err "IP address is not available."
        return 1
    else
        run_cmd "ifconfig"
    fi

    log_inf "This test require iperf3 to perform various test cases."
    log_inf "Please start iperf3 server on any machine before going to next step."
    log_ask "Hit ENTER key when complete."

    while read answer; do
        if [ -z "$answer" ]; then
          break
        fi
    done

    log_ask "Please enter IP address of iperf3 running server: "

    while read destip; do
        ping -c 1 -w 1 $destip > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            break
        fi 
        log_err "Invalid IP address."
    done

    if echo "$CASES" | grep -q "PING"; then
        log_inf "Starting connectivity test."
        run_cmd "ping -c 100 $destip"
    fi

    if echo "$CASES" | grep -q "SPEED"; then
        log_inf "Starting bandwidth test."
        run_cmd "iperf3 -c $destip -t 30"
    fi

    if echo "$CASES" | grep -q "STRESS10M"; then
        log_inf "Starting stress test, try sending 10 Mbps for 1 minutes."
        run_cmd "iperf3 -c $destip -u -t 60 -b 10M"
    fi

    if echo "$CASES" | grep -q "STRESS100M"; then
        log_inf "Starting stress test, try sending 100 Mbps for 1 minutes."
        run_cmd "iperf3 -c $destip -u -t 60 -b 100M"
    fi

    if echo "$CASES" | grep -q "STRESS1000M"; then
        log_inf "Starting stress test, try sending 1000 Mbps for 1 minutes."
        run_cmd "iperf3 -c $destip -u -t 60 -b 1000M"
    fi
}

run_scif_test()
{
    echo "#########################################################################"
    echo "# SCIF Test                                                             #"
    echo "#########################################################################"

    CASES="PCK SCKi EXTERNAL"

    DRV="devc-serscif"
    DEV="/dev/ser1"

    if ! check_util ${DRV} tty stty; then
        return 1
    fi

    if ! tty | grep -q "ttyp"; then
        log_err "Please running this test from telnet session."
        return 1
    fi

    for chan in 0 3; 
    do
        log_inf "Starting test for SCIF${chan} device."
        run_cmd "slay -Q ${DRV}"
        set_pfc "scif${chan}"
        set_cpg "scif${chan}"
        log_inf "Please connect serial cable between board and PC, then open any terminal program, such as TeraTerm." 
        log_inf "Hit ENTER to begin the test."

        while read answer; do
            if [ -z "$answer" ]; then
                break
            fi
        done

        if echo "$CASES" | grep -q "PCK";
        then
            log_inf "Set clock source to PCK internal clock."
            run_cmd "slay -Q ${DRV}"
            run_cmd "${DRV} -e -F -b115200 -c66660000/16 -t 14 scif${chan}"
            run_cmd "waitfor ${DEV}" || { return 1; }

            for baud in 9600 19200 38400 57600 115200 230400;
            do
                log_inf "Set baud rate of ${DEV} to $baud."
                run_cmd "stty baud=${baud} < ${DEV}" || { return 1; }
                log_ask "Please set baud rate of PC to ${baud}. Hit ENTER key when complete"

                while read answer; do
                    if [ -z "$answer" ]; then
                      break
                    fi
                done

                run_cmd "echo Hello over ${baud} baud > ${DEV}"
                log_ask "Could you see Hello log printed out on PC (y/n) "

                while read answer; do
                    if [[ "$answer" != "Y" ]] && [[ "$answer" != "y" ]] ; then
                      log_err "Test case (SCIF${chan}, PCK, $baud) failed."
                    fi
                    break
                done
            done
        fi

        if echo "$CASES" | grep -q "SCKi";
        then
            log_inf "Set clock source to SCKi internal clock."
            run_cmd "slay -Q ${DRV}"
            run_cmd "${DRV} -e -F -b115200 -c266660000/16 -t 14 scif${chan}"
            run_cmd "waitfor ${DEV}" || { return 1; }

            for baud in 9600 19200 38400 57600 115200 230400;
            do
                log_inf "Set baud rate of ${DEV} to $baud."
                run_cmd "stty baud=${baud} < ${DEV}" || { return 1; }
                log_ask "Please set baud rate of PC to ${baud}. Hit ENTER key when complete"

                while read answer; do
                    if [ -z "$answer" ]; then
                      break
                    fi
                done

                run_cmd "echo Hello over ${baud} baud > ${DEV}"
                log_ask "Could you see Hello log printed out on PC (y/n) "

                while read answer; do
                    if [[ "$answer" != "Y" ]] && [[ "$answer" != "y" ]] ; then
                      log_err "Test case (SCIF${chan}, SCKi, $baud) failed."
                    fi
                    break
                done
            done
        fi

        if echo "$CASES" | grep -q "EXTERNAL";
        then
            log_inf "Set clock source to SCIF_CLK external clock."
            run_cmd "slay -Q ${DRV}"
            run_cmd "${DRV} -e -F -b115200 -x -c24000000/16 -t 14 scif${chan}"
            run_cmd "waitfor ${DEV}" || { return 1; }

            for baud in 9600 19200 38400 57600 115200;
            do
                log_inf "Set baud rate of ${DEV} to $baud."
                run_cmd "stty baud=${baud} < ${DEV}" || { return 1; }
                log_ask "Please set baud rate of PC to ${baud}. Hit ENTER key when complete"

                while read answer; do
                    if [ -z "$answer" ]; then
                      break
                    fi
                done

                run_cmd "echo Hello over ${baud} baud > ${DEV}"
                log_ask "Could you see Hello log printed out on PC (y/n) "

                while read answer; do
                    if [[ "$answer" != "Y" ]] && [[ "$answer" != "y" ]] ; then
                      log_err "Test case (SCIF${chan}, SCIF_CLK, $baud) failed."
                    fi
                    break
                done
            done
        fi
    done
}

run_hscif_test()
{
    echo "#########################################################################"
    echo "# HSCIF Test                                                            #"
    echo "#########################################################################"

    CASES="SCKi EXTERNAL"

    DRV="devc-serscif"
    DEV="/dev/ser1"

    if ! check_util ${DRV} tty stty; then
        return 1
    fi

    if ! tty | grep -q "ttyp"; then
        log_err "Please running this test from telnet session."
        return 1
    fi

    for chan in 0 1; 
    do
        log_inf "Starting test for HSCIF${chan} device."
        run_cmd "slay -Q ${DRV}"
        set_pfc "hscif${chan}"
        set_cpg "hscif${chan}"
        log_inf "Please connect serial cable between board and PC, then open any terminal program, such as TeraTerm." 
        log_inf "Hit ENTER to begin the test."

        while read answer; do
            if [ -z "$answer" ]; then
              break
            fi
        done

        if echo "$CASES" | grep -q "SCKi";
        then
            log_inf "Set clock source to SCKi internal clock."
            run_cmd "slay -Q ${DRV}"
            run_cmd "${DRV} -e -F -b115200 -c266660000/16 -t 14 hscif${chan}"
            run_cmd "waitfor ${DEV}" || { return 1; }

            for baud in 9600 19200 38400 57600 115200 230400;
            do
                log_inf "Set baud rate of ${DEV} to $baud."
                run_cmd "stty baud=${baud} < ${DEV}" || { return 1; }
                log_ask "Please set baud rate of PC to ${baud}. Hit ENTER key when complete"

                while read answer; do
                    if [ -z "$answer" ]; then
                      break
                    fi
                done

                run_cmd "echo Hello over ${baud} baud > ${DEV}"
                log_ask "Could you see Hello log printed out on PC (y/n) "

                while read answer; do
                    if [[ "$answer" != "Y" ]] && [[ "$answer" != "y" ]] ; then
                      log_err "Test case (HSCIF${chan}, SCKi, $baud) failed."
                    fi
                    break
                done
            done
        fi

        if echo "$CASES" | grep -q "EXTERNAL";
        then
            log_inf "Set clock source to SCIF_CLK external clock."
            run_cmd "slay -Q ${DRV}"
            run_cmd "${DRV} -e -F -b115200 -x -c24000000/16 -t 14 hscif${chan}"
            run_cmd "waitfor ${DEV}" || { return 1; }

            for baud in 9600 19200 38400 57600 115200 230400;
            do
                log_inf "Set baud rate of ${DEV} to $baud."
                run_cmd "stty baud=${baud} < ${DEV}" || { return 1; }
                log_ask "Please set baud rate of PC to ${baud}. Hit ENTER key when complete"

                while read answer; do
                    if [ -z "$answer" ]; then
                      break
                    fi
                done

                run_cmd "echo Hello over ${baud} baud > ${DEV}"
                log_ask "Could you see Hello log printed out on PC (y/n) "

                while read answer; do
                    if [[ "$answer" != "Y" ]] && [[ "$answer" != "y" ]] ; then
                      log_err "Test case (HSCIF${chan}, SCIF_CLK, $baud) failed."
                    fi
                    break
                done
            done
        fi
    done
}

run_flash_test()
{
    echo "#########################################################################"
    echo "# QSPI Test                                                             #"
    echo "#########################################################################"

    DRV="devf-rcar_qspi"
    DEV="/dev/fs0"
    PAR0="${DEV}p0"
    PAR1="${DEV}p1"

    if ! check_util ${DRV} flashctl dd diff; then
        return 1
    fi

    if ! check_instance ${DRV}; then
        log_err "${DRV} is not running"
        return 1
    fi

    if ! check_file ${DEV}; then
        log_err "${DEV} is not found"
        return 1
    fi

    if check_file ${PAR1}; then
        run_cmd "flashctl -p ${PAR1} -ev" || { return 1; }
    fi

    run_cmd "slay -Q ${DRV}"
    run_cmd "${DRV} -r -b5"
    run_cmd "waitfor ${DEV}" || { return 1; }

    run_cmd "flashctl -p ${DEV} -o 32M -l 32M -ev" || { return 1; }
    run_cmd "flashctl -p ${PAR0} -o 32M -l 32M -fv" || { return 1; }

    run_cmd "slay -Q ${DRV}"
    run_cmd "${DRV} -r -b5"
    run_cmd "waitfor ${DEV}" || { return 1; }

    run_cmd "dd if=/dev/random of=/tmp/test_flash bs=1024k count=30" || { return 1; }
    run_cmd "cp -V /tmp/test_flash /fs0p1" || { return 1; }
    run_cmd "diff /fs0p1/test_flash /tmp/test_flash"
    run_cmd "cp -V /fs0p1/test_flash /tmp/" || { return 1; }
    run_cmd "diff /fs0p1/test_flash /tmp/test_flash"
}

run_emmc_test()
{
    echo "#########################################################################"
    echo "# eMMC Test                                                             #"
    echo "#########################################################################"

    CASES="qnx6 fat32"

    DRV="devb-sdmmc-rcar"
    DEV="/dev/mmc0"

    if ! check_util ${DRV} fdisk mkqnx6fs mkdosfs diff dd; then
        return 1
    fi

    if ! check_instance ${DRV}; then
        log_err "${DRV} is not running"
        return 1
    fi

    if ! check_file ${DEV}; then
        log_err "${DEV} is not found"
        return 1
    fi

    if echo "$CASES" | grep -q "qnx6"; then
        log_inf "Create partition with QNX6 filesystem"
        run_cmd "fdisk ${DEV} delete -a" || { return 1; }
        run_cmd "fdisk ${DEV} add -t 178 -p 50" || { return 1; }
        run_cmd "mount -e ${DEV}" || { return 1; }
        run_cmd "mkqnx6fs -q /dev/mmc0t178" || { return 1; }
        run_cmd "mount -w -tqnx6 /dev/mmc0t178 /mmc" || { return 1; }
        run_cmd "dd if=/dev/random of=/mmc/testfile bs=1024k count=10" || { return 1; }
        run_cmd "cp -V /mmc/testfile /tmp/" || { return 1; }
        run_cmd "diff /mmc/testfile /tmp/testfile"
        run_cmd "cp -V /tmp/testfile /mmc/" || { return 1; }
        run_cmd "diff /mmc/testfile /tmp/testfile"
    fi

    if echo "$CASES" | grep -q "fat32"; then
        log_inf "Create partition with FAT32 filesystem"
        run_cmd "fdisk ${DEV} delete -a" || { return 1; }
        run_cmd "fdisk ${DEV} add -t11" || { return 1; }
        run_cmd "mount -e ${DEV}" || { return 1; }
        run_cmd "mkdosfs /dev/mmc0t11" || { return 1; }
        run_cmd "mount -w -tdos /dev/mmc0t11 /mmc" || { return 1; }
        run_cmd "dd if=/dev/random of=/mmc/testfile bs=1024k count=10" || { return 1; }
        run_cmd "cp -V /mmc/testfile /tmp/" || { return 1; }
        run_cmd "diff /mmc/testfile /tmp/testfile"
        run_cmd "cp -V /tmp/testfile /mmc/" || { return 1; }
        run_cmd "diff /mmc/testfile /tmp/testfile"
    fi
}

run_thermal_test()
{
    echo "#########################################################################"
    echo "# Thermal Test                                                          #"
    echo "#########################################################################"

    DRV="rcar-thermal"
    APP="thermal_test"
    DEV="/dev/thermal"

    if ! check_util ${DRV} ${APP}; then
        return 1
    fi

    if ! check_file ${DEV}; then
        if ! check_instance ${DRV}; then
            run_cmd "${DRV} -v -u50:20 &"
            run_cmd "waitfor ${DEV}" || { return 1; }
        else
            log_err "${DEV} is not found"
            return 1
        fi
    fi

    run_cmd "${APP}"
}

run_i2c_test()
{
    echo "#########################################################################"
    echo "# I2C Test                                                              #"
    echo "#########################################################################"

    DRV="i2c-rcar-A"
    DEV="/dev/i2c0"

    if ! check_util ${DRV} isend isendrecv; then
        return 1
    fi

    if ! check_instance ${DRV}; then
        log_err "${DRV} is not running"
        return 1
    fi

    if ! check_file ${DEV}; then
        log_err "${DEV} is not found"
        return 1
    fi

    log_inf "I/O Expander (U25, TCA9554PWR)"

    run_cmd "isend -a 0x20 -n ${DEV} 0x03 0x00"
    run_cmd "isendrecv -a 0x20 -n ${DEV} -l1 0x03"
    run_cmd "isendrecv -a 0x20 -n ${DEV} 0x03 0xFF"
    run_cmd "isendrecv -a 0x20 -n ${DEV} -l1 0x03"

    log_inf "Clock Generator (U36, RC21212dddGNA)"

    run_cmd "isend -a 0x6c -n ${DEV} 0x60 0x00 0x01 0x02"
    run_cmd "isend -a 0x6c -n ${DEV} 0x60 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08"
    run_cmd "isendrecv -a 0x6c -n ${DEV} 0x60 0x00 0x01"
    run_cmd "isendrecv -a 0x6c -n ${DEV} 0x60 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08"
    run_cmd "isendrecv -a 0x6c -n ${DEV} -l 2 0x60"
    run_cmd "isendrecv -a 0x6c -n ${DEV} -l 9 0x60"
}

run_cpg_test()
{
    echo "#########################################################################"
    echo "# CPG Test                                                              #"
    echo "#########################################################################"

    DRV="rcar-cpg-mgr"
    APP="cpg_ctrl"
    DEV="/dev/cpg"

    if ! check_util ${DRV} ${APP}; then
        return 1
    fi

    if ! check_file ${DEV}; then
        if ! check_instance ${DRV}; then
            run_cmd "${DRV} -d rcar-cpg &"
            run_cmd "waitfor ${DEV}" || { return 1; }
        else
            log_err "${DEV} is not found."
            return 1
        fi
    fi

    run_cmd "${APP} mod=post idx=0"
    run_cmd "${APP} mod=post idx=0 -e"
    run_cmd "${APP} mod=post idx=0"
    run_cmd "${APP} mod=post idx=0 -d"
    run_cmd "${APP} mod=post idx=0"
    run_cmd "${APP} mod=post idx=0 clk=57142857"
    run_cmd "${APP} mod=post idx=0"

}

run_sysc_test()
{
    echo "#########################################################################"
    echo "# SYSC Test                                                             #"
    echo "#########################################################################"

    DRV="rcar-sysc-mgr"
    APP="sysc_ctrl"
    DEV="/dev/sysc"

    if ! check_util ${DRV} ${APP}; then
        return 1
    fi

    if ! check_file ${DEV}; then
        if ! check_instance ${DRV}; then
            run_cmd "${DRV} -d rcar-sysc"
            run_cmd "waitfor ${DEV}" || { return 1; }
        else
            log_err "${DEV} is not found."
            return 1
        fi
    fi

    run_cmd "${APP} dom=a3e0"
    run_cmd "${APP} dom=a3e1"
    run_cmd "${APP} dom=a2e0d0"
    run_cmd "${APP} dom=a2e0d1"
    run_cmd "${APP} dom=a2e1d0"
    run_cmd "${APP} dom=a2e1d1"
}

run_spi_test()
{
    echo "#########################################################################"
    echo "# SPI Test                                                              #"
    echo "#########################################################################"

    CASES="LOOPBACK"

    if ! check_util spi-master spi-control spi-slave-rcar spi-slave-control; then
        return 1
    fi

    if echo "$CASES" | grep -q "LOOPBACK";
    then
        log_inf "Starting master mode loopback test."
        log_ask "Please connect CN37 Pin 5 to Pin 6. Hit ENTER key when complete."

        while read answer; do
            if [ -z "$answer" ]; then
              break
            fi
        done

        run_cmd "slay -Q spi-master"
        run_cmd "slay -Q spi-slave-rcar"

        run_cmd "spi-master -u0 -d /proc/boot/spi-rcar.so channel=0,blksize=64,dma"
        run_cmd "waitfor /dev/spi0" || { return 1; }
        run_cmd "spi-control /dev/spi0 -i"
        run_cmd "spi-control /dev/spi0 -s0 -w 74:65:73:74:69:6e:67 -v"
        run_cmd "spi-control /dev/spi0 -c size=32"
        run_cmd "spi-control /dev/spi0 -i"
        run_cmd "spi-control /dev/spi0 -s0 -d 74736574:74736574:74736574:74736574:74736574:74736574:74736574:74736574 -v"
    fi
}

run_dma_test()
{
    echo "#########################################################################"
    echo "# DMA Test                                                              #"
    echo "#########################################################################"

    APP="dma-memtomem-test"

    if ! check_util ${APP}; then
        return 1
    fi

    run_cmd "dma-memtomem-test dma=sys,chan=7,desc=2,extdesc"
    run_cmd "dma-memtomem-test dma=sys"
    run_cmd "dma-memtomem-test dma=sys,chan=7,desc=2,event,extdesc"

    run_cmd "dma-memtomem-test dma=rt,chan=0,desc=2,extdesc"
    run_cmd "dma-memtomem-test dma=rt"
    run_cmd "dma-memtomem-test dma=rt,chan=0,desc=2,event,extdesc"
}

add_test()
{
    entry="run_$1_test"

    set -A $1_test $1 ${entry}
    set -A test_table ${test_table[*]} $1_test

    if ! typeset -f ${entry} > /dev/null; then
        log_err "Please define function ${entry}."
        exit 1
    fi
}

init_test()
{
    add_test "smp"
    add_test "network"
    add_test "scif"
    add_test "hscif"
    add_test "flash"
    add_test "emmc"
    add_test "thermal"
    add_test "i2c"
    add_test "cpg"
    add_test "sysc"
    add_test "spi"
    add_test "dma"
}

select_test()
{
    for opt in $test_opt; do
        if [ $opt = "all" ]; then
            test_num=0
        elif [[ $opt == -* ]]; then
            set -A test_skip ${test_skip[*]} run_${opt#?}_test
        else
            test_idx=0
            while [ $test_idx -lt ${#test_table[@]} ]; do
                eval name=\${${test_table[$test_idx]}[0]}
                eval func=\${${test_table[$test_idx]}[1]}
                if [ $name = $opt ]; then
                    test_num=$((test_num+1))
                    set -A test_multi ${test_multi[*]} $func
                    break
                fi
                test_idx=$((test_idx+1))
            done
            if [ $test_idx -eq ${#test_table[@]} ]; then
                echo "Unknown module: $opt"
                exit 1
            fi
        fi
    done
}

start_test()
{
    if ! check_util print printf sed grep awk io; then
        return 1
    fi

    if [ $test_num -ne 0 ]; then 
        for func in ${test_multi[@]}; do
            eval ${func}
        done
        return 0
    fi

    for name in ${test_table[@]}; do
        eval func=\${${name}[1]}
        if [ ${#test_skip[@]} -ne 0 ]; then
            skip=0
            for temp in ${test_skip[@]}; do
                if [ $temp = $func ]; then
                    skip=1
                    break
                fi
            done
            if [ ${skip} -eq 0 ]; then
                eval ${func}
            fi
        else
            eval ${func}
        fi
    done
}

init_test

if [ $# -eq 0 ]; then
    print_help ${0##*/}
    exit 0
fi

select_test
start_test

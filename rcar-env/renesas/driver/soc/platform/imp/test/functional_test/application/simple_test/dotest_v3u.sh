#!/bin/bash

#Usage :
#  This script is used in the build directory.
#  For the Package version, change "VERNUM" or specify it with the first argument.
#  (E.g. >dotest_v3u.sh v2.5.0)

if [ "$1" == "" ]; then
    VERNUM=v2.0.0
else
    VERNUM=$1
fi
if [ "$(expr substr $(uname -s) 1 5)" == 'Linux' ]; then
    LD_LIBRARY_PATH=rcar-xos/${VERNUM}/sw/x86_64-gnu-linux/lib:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH
    EXENAME=x86_64-gnu-linux/bin/imp_simple_test
elif [ "$(expr substr $(uname -s) 1 10)" == 'MINGW32_NT' ]; then                                                                                           
    EXENAME=amd64-gnu-windows/bin/imp_simple_test.exe
else
  echo "Your platform ($(uname -a)) is not supported."
  exit 1
fi

#/* test case 1: imp core0 test. */
./rcar-xos/${VERNUM}/sw/${EXENAME} 1 IMP

#/* test case 2: - */
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 IMP 0
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 IMP 1
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 IMP 2
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 IMP 3
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 CVe 0
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 CVe 1
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 CVe 2
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 CVe 3
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 CVe 4
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 CVe 5
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 CVe 6
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 CVe 7
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 DMA 0
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 DMA 2
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 PSC 0
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 PSC 1
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 CNN 0
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 CNN 1
./rcar-xos/${VERNUM}/sw/${EXENAME} 2 CNN 2

#/* test case 3: int-trap test */
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 IMP 0
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 IMP 1
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 IMP 2
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 IMP 3
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 CVe 0
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 CVe 1
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 CVe 2
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 CVe 3
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 CVe 4
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 CVe 5
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 CVe 6
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 CVe 7
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 DMA 0
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 DMA 2
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 PSC 0
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 PSC 1
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 CNN 0
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 CNN 1
./rcar-xos/${VERNUM}/sw/${EXENAME} 3 CNN 2

#/* test case 4: imp core test */
./rcar-xos/${VERNUM}/sw/${EXENAME} 4 IMP 0
./rcar-xos/${VERNUM}/sw/${EXENAME} 4 IMP 1
./rcar-xos/${VERNUM}/sw/${EXENAME} 4 IMP 2
./rcar-xos/${VERNUM}/sw/${EXENAME} 4 IMP 3

#/* test case 5: cnn core test */
./rcar-xos/${VERNUM}/sw/${EXENAME} 5 CNN 0
./rcar-xos/${VERNUM}/sw/${EXENAME} 5 CNN 1
./rcar-xos/${VERNUM}/sw/${EXENAME} 5 CNN 2

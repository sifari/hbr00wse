#!/bin/bash

GXXFLAGS="-g -std=gnu++17"
JTHREADINC=~/opt/jthread/include
NANADIR=~/opt/nana-1.7.4-cf00
export NANADIR

INCINFO="-I ${JTHREADINC} -I ${NANADIR}/include"
LINKINFO="${LINKINFO} -L ${NANADIR}/lib -lnana -lasound"
LINKINFO="${LINKINFO} -L /usr/lib64 -Xlinker -rpath -Xlinker /usr/lib64 -lX11 -lXft -lfontconfig -lpthread -lrt -lpng -lasound"
for pgm in hbr00wse
do
    rm ./$pgm
    echo "========= doing $pgm"
    echo "g++ ${GXXFLAGS} ${INCINFO} -o $pgm $pgm.cpp ${LINKINFO}"
          g++ ${GXXFLAGS} ${INCINFO} -o $pgm $pgm.cpp ${LINKINFO}
    echo ./$pgm
	  ./$pgm
done


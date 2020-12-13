#!/bin/bash

CUR_PATH=`pwd`
DPDK_DIR=${CUR_PATH}/DPDK
MICA_DIR=${CUR_PATH}/mica/build
HUGE_DIR=/dev/hugepages
BENCH=microbench
CMODE=EREW #Concurrency mode(e.g., EREW, CREW, CRCWS)
SKEWNESS=0.5
DPDK_COMPILE=0

file_num=`ls -la ${HUGE_DIR} | wc -l`
if [ ${file_num} -gt 0 ];
then
	rm ${HUGE_DIR}/*
	echo "[Delete hugepage files ${file_num}]"
else
	echo "[Hugepage is empty folder]"
fi

if [ ${DPDK_COMPILE} -eq 1 ];
then
	cd $DPDK_DIR
	make install T=x86_64-native-linuxapp-gcc DEST_DIR=/home/heejin/Lighthouse/DPDK/x86_64-native-linuxapp-gcc
	cd x86_64-native-linuxapp-gcc
	make -j8
fi

cd $MICA_DIR
rm CMakeCache.txt
rm -rf CMakeFiles
../configure_all.sh
make -j8
./$BENCH $CMODE SKEWNESS $SKEWNESS

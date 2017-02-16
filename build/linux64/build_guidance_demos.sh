#!/bin/bash

output=guidance_demos
project_root=../../
project_root=`readlink -f $project_root`
CMAKE=$project_root/tools/cmake/3.3.0/linux64/bin/cmake

CMAKE_BUILD_TYPE="-DCMAKE_BUILD_TYPE=Release"
btype="$1"
if [ "$#" -eq 1 ] && [ "${btype,,}" == "debug" ]; then
    CMAKE_BUILD_TYPE="-DCMAKE_BUILD_TYPE=Debug"
fi

CMAKE_OPTIONS="$CMAKE_BUILD_TYPE"

mkdir -p $output
pushd $output

$CMAKE "$project_root/guidance-sdk/demo/"  "$CMAKE_OPTIONS"

numprocs=`cat /proc/cpuinfo | grep "processor" | wc -l`

echo -e "\033[1;32mBuilding linux64: make -j$numprocs\033[0m"
make -j$numprocs

if [ "$?" -ne "0" ]; then
	echo -e "\033[1;31mBuilding linux64 failed.\033[0m"
	popd && exit 1
fi

echo -e "\033[1;32mBuilding linux64 completed!\033[0m"

popd


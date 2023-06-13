#! /bin/zsh

cd yaml-cpp
mkdir build
cd build
 
cmake .. -G Xcode -DCMAKE_OSX_DEPLOYMENT_TARGET=10.9 -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"

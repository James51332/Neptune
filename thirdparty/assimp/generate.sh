#! /bin/zsh

cd assimp
mkdir build
cd build
 
cmake .. -G Xcode -DBUILD_SHARED_LIBS=OFF -DASSIMP_BUILD_ZLIB=ON

#!/usr/bin/bash4bug
rm -rf *.o libs/ /bin/bipac
g++ -fpic -c getPackages.cpp parsePackages.cpp comparePackages.cpp
g++ -fpic -o libbipac.so.1.0.0 *.o -shared -Wl,-soname,libbipac.so.1
mkdir libs
mv libbipac.so.1.0.0 ./libs/ && cd ./libs/
ln -s libbipac.so.1.0.0 libbipac.so.1 && ln -s libbipac.so.1  libbipac.so
LD_LIBRARY_PATH=$(pwd)/libs && export LD_LIBRARY_PATH
cd ../
g++ bipac.cpp -o bipac -I include/ -L libs/ -lbipac -lcurl
cp bipac /bin/
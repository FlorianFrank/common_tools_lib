git clone https://github.com/google/googletest
cd googletest
mkdir build
cd build
cmake .. -GNinja -DCMAKE_CXX_COMPILER=C:/ProgramData/chocolatey/bin/g++.exe -DCMAKE_C_COMPILER=C:/ProgramData/chocolatey/bin/gcc.exe
ninja
cd ../..
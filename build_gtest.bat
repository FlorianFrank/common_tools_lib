git clone https://github.com/google/googletest
cd googletest
mkdir build
cd build
cmake .. -GNinja -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc
ninja
cd ../..
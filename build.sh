# create libs folder and checkout dependencies
mkdir libs
cd libs
# clone magnum
git clone https://github.com/mosra/magnum.git
cd magnum && git reset --hard 286aaf2541f171bb7f90c80c38eec65b1d39b130 && cd ..
# clone magnum-plugins
git clone https://github.com/mosra/magnum-plugins.git
cd magnum-plugins \
  && git reset --hard eaf4791d54dc4c8356c1b09cb47ee5ab1c01cb3e && cd ..
# clone corrade
git clone https://github.com/mosra/corrade.git
cd corrade && git reset --hard 98b619faf8acef9abd2b70015eaed2018d98ac01 && cd ..
cd ..
# create build folder and build with cmake
cd ..
mkdir magnum-flycapture-build
cd magnum-flycapture-build
cmake ../magnum-flycapture \
  -DWITH_SDL2APPLICATION=ON \
  -DWITH_TGAIMPORTER=ON \
  -DWITH_FREETYPEFONT=ON \
  -DCMAKE_CXX_COMPILER=clang++
# make and run
cmake --build . --target Run -- -j4

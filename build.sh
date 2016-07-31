cd ..
mkdir magnum-flycapture-build
cd magnum-flycapture-build
cmake ../magnum-flycapture \
  -DWITH_SDL2APPLICATION=ON \
  -DWITH_TGAIMPORTER=ON \
  -DWITH_FREETYPEFONT=ON \
  -DCMAKE_CXX_COMPILER=clang++
cmake --build . --target Run -- -j4

echo 
echo Build CMake projects.
echo 

pwd_p=`pwd -P`

if [ ! -d Intermediate ];then
  mkdir Intermediate
fi

pushd Intermediate
  if [ ! -d CMake ];then
    mkdir CMake
  fi

  pushd CMake
    cmake ../..
    make -j20
  popd
popd
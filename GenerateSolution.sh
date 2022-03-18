echo 
echo Generate Solution and Project Files...
echo 

pwd_p=`pwd -P`

function buildProject() {
  dotnet build $BUILDTOOL_SOLUTION -c Release
}

pushd $pwd_p
  pushd ThirdParty/BuildTool
    BUILDTOOL_SOLUTION=$PWD/BuildTool.sln

    if [ ! -f "Build/Release/net6.0/BuildTool.dll" ];then
      buildProject
    fi

    pushd Build/Release/net6.0
      BUILDTOOL=$PWD/BuildTool.dll
      echo BUILDTOOL = $BUILDTOOL
    popd
  popd

  dotnet $BUILDTOOL --Solution Engine.Solution.cs
popd
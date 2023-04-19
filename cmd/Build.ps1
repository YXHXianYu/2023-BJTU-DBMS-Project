Remove-Item "./build" -Recurse
New-Item -Path "./build" -ItemType Directory
Set-Location "./build"
# cmake ..
cmake -DCMAKE_CXX_COMPILER="C:/Program Files/mingw-w64/mingw64/bin/g++.exe" -DCMAKE_C_COMPILER="C:/Program Files/mingw-w64/mingw64/bin/gcc.exe" -G "MinGW Makefiles" ..
mingw32-make
Set-Location ".."
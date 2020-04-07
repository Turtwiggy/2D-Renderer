How to compile with GCC
1) Download MSYS2
2) Download packages listed in deps_msys2_x64-windows
3) Generate make file by running build-make.bat
4) Use make file to compile project

How to compile with MSVC
1) Download VCPKG (make sure it's x64 - https://github.com/microsoft/vcpkg/issues/1254)
2) run "vcpkg integrate install"
3) run "vcpkg install deps_vcpkg_x64-{your platform}.txt" (the file in the base of the repo)
4) Generate visual studio files by running build-vs2019.bat
5) Either compile with vs2019, or use msbuild to compile project from command line

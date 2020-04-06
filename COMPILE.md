- dump libbacktrace.la libbacktrace.a in lib folder in your compiler

HOW TO MSVC 
1) Download VCPKG
2) run "vcpkg integrate install"
3) make sure you x64 vcpkg https://github.com/microsoft/vcpkg/issues/1254
4) Download required packages (check vcpkg.txt), 
or use the command "vcpkg install @vcpkg_x64-{your platform}.txt"
4) Run premake script
5) profit
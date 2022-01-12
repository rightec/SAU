@set GCC_EXEC_PREFIX=c:/gcc850/lib/gcc-lib/
@set COMPILER_PATH=c:/gcc850/bin/
@set LIBRARY_PATH=c:/gcc850/lib/
@set GCC_COLLECT=c:/gcc850/lib/
@set C_INCLUDE_PATH=c:/gcc850/include; c:/gcc850/include/g++;c:/gcc850/include/sys;
@set CPLUS_INCLUDE_PATH=c:/gcc850/include; c:/gcc850/include/sys; c:/gcc850/include/g++; c:/gcc850/include/g++/std;c:/gcc850/include/v850-elf
@set TMPDIR=c:/gcc850
@set PATH=c:\gcc850\bin;c:\gcc850\lib;E:\source\gcc850\SAU7000
make.exe -s -f SAU7000.mak
copy sau7000.out sau7000.elf
V850checksum.exe
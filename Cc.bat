@set GCC_EXEC_PREFIX=c:/gcc850/lib/gcc-lib/
@set COMPILER_PATH=c:/gcc850/bin/
@set LIBRARY_PATH=c:/gcc850/lib/
@set GCC_COLLECT=c:/gcc850/lib/
@set C_INCLUDE_PATH=c:/gcc850/include; c:/gcc850/include/g++;c:/gcc850/include/sys;
@set CPLUS_INCLUDE_PATH=c:/gcc850/include; c:/gcc850/include/sys; c:/gcc850/include/g++; c:/gcc850/include/g++/std;c:/gcc850/include/v850-elf
@set TMPDIR=c:/gcc850
@set PATH=c:\gcc850\bin;c:\gcc850\lib;%PATH%
@rem per vedere la versione e i vari passaggi usare -version e/o -verbose
@rem per non vedere tutti i tempi di compilazione usare -quiet

@rem @cpp.exe -g -lang-c++ -nostdinc -I c:\gcc850\include -iprefix c:\gcc850\include -iwithprefix \g++ -iwithprefix \g++\std -iwithprefix \g++\v850-elf  -undef %1.cpp ppc\%1.ii
@rem @cc1plus.exe  -quiet -O3 -version -fshort-enums -fverbose-asm -freg-struct-return -fallow-single-precision -fshort-double -o ./src/%1.s  ./ppc/%1.ii
@rem @as.exe ./src/%1.s -o ./obj/%1.o -a > .\lst\%1.lst
rem @cc1plus.exe -g -O1 -mno-prolog-function -version -quiet -fshort-enums -freg-struct-return -fallow-single-precision -fshort-double -fverbose-asm -o ./src/%1.s  ./ppc/%1.ii


@cpp.exe -g -lang-c++ -nostdinc -I c:\gcc850\include -iprefix c:\gcc850\include -iwithprefix \g++ -iwithprefix \g++\std -iwithprefix \g++\v850-elf  -undef %1.cpp ppc\%1.ii
@cc1plus.exe -g -O1 -mv850 -mno-prolog-function -fno-exceptions -fno-rtti -fno-enforce-eh-specs -Wreturn-type -mdisable-callt -version -quiet -fshort-enums -freg-struct-return -fshort-double -fverbose-asm -o ./src/%1.s  ./ppc/%1.ii
@as.exe -mv850e ./src/%1.s -o ./obj/%1.o -a > .\lst\%1.lst

@rem @cc1plus.exe -mv850 --version --help

@set GCC_EXEC_PREFIX=c:\gcc850\bin
@set GCC_COLLECT=c:\gcc850\bin\specs
@set CPLUS_INCLUDE_PATH=c:\gcc850\include; c:\gcc850\include\g++
@set LIBRARY_PATH=c:\gcc850\lib
@set TMPDIR=c:\gcc850
@set PATH=c:\gcc850\bin;%PATH%
@rem as -MRI -l -v %1.s --bitwise-or --statistics -o ./obj/%1.o -mcpu32 -a > .\lst\%1.lst
@as.exe -L -v %1.s -o ./obj/%1.o -a > .\lst\%1.lst

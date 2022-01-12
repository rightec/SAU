@set GCC_EXEC_PREFIX=c:\gcc850\bin
@set GCC_COLLECT=c:\gcc850\bin\specs
@set CPLUS_INCLUDE_PATH=c:\gcc850\include; c:\gcc850\include\g++
@set LIBRARY_PATH=c:\gcc850\lib
@set TMPDIR=c:\gcc850
@PATH=c:\gcc850\bin;%PATH%

@ld.exe -v -T multi.lk -Map multi.map -L c:\gcc850\lib



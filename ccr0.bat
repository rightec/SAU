@rem FILE BATCH PER COMPILAZIONE SENZA SIMBOLI MA CON OTTIMIZZAZIONE
@rem PER CARICAMENTO IN FLASH-ROM SUL TARGET
@rem cc1plus.exe -mv850e -version -O1 -quiet -fshort-enums -fverbose-asm -freg-struct-return -fshort-double -o ./src/%1.s  ./ppc/%1.ii
@rem as.exe -mv850e ./src/%1.s -o ./obj/%1.o -a > .\lst\%1.lst

@rem cc1plus.exe -mv850e -g -O0 -mno-prolog-function -fno-exceptions -fno-rtti -fno-enforce-eh-specs -Wreturn-type -mdisable-callt -version -Wall -quiet -mtda=0 -msda=0 -mzda=0 -fshort-enums -freg-struct-return -fshort-double -nostdinc -I c:\gcc850\include -iprefix c:\gcc850\include -iwithprefix \g++ -iwithprefix \g++\std -iwithprefix \g++\v850-elf  %1.cpp -o ./src/%1.s
@rem as.exe -mv850e -gstabs ./src/%1.s -o ./obj/%1.o -a > .\lst\%1.lst

@cc1plus.exe -g -O0 -mv850 -Wall -mUS-bit-set -mno-prolog-function -fno-exceptions -fno-rtti -fno-enforce-eh-specs -Wreturn-type -mdisable-callt -fshort-enums -freg-struct-return -fshort-double -fverbose-asm -I c:\gcc850\include -iprefix c:\gcc850\include -iwithprefix \g++ -iwithprefix \g++\std -iwithprefix \g++\v850-elf  -undef -o ./src/%1.s  %1.cpp
@as.exe -mv850 ./src/%1.s -o ./obj/%1.o -ahls > .\lst\%1.lst
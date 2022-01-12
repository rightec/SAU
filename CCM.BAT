@rem FILE BATCH PER COMPILAZIONE CON SIMBOLI E SENZA OTTIMIZZAZIONE
@rem PER DEBUG CON SIMULATORE/ROM-MONITOR

@rem versione per 3.3.2 cc1plus.exe -g3 -quiet -O1 -mv850 -Wall -mUS-bit-set -mno-prolog-function -fno-exceptions -fno-rtti -fno-enforce-eh-specs -Wreturn-type -mdisable-callt -fshort-enums -freg-struct-return -fshort-double -I c:\gcc850\include -iprefix c:\gcc850\include -iwithprefix \g++ -iwithprefix \g++\std -iwithprefix \g++\v850-elf  -undef -o ./src/%1.s  ./%1.cpp
@rem versione per 3.3.2 as.exe -mv850e --gstabs ./src/%1.s -o ./obj/%1.o -ahls > .\lst\%1.lst

@set TMPDIR=c:/gcc850

@rem versione senza verbose-asm per avere - simbolico
@rem cc1plus.exe -quiet -O2 -mv850e -Wall -mUS-bit-set -mno-prolog-function -fno-exceptions -fno-rtti -fno-enforce-eh-specs -Wreturn-type -mdisable-callt -fshort-enums -freg-struct-return -fshort-double  -I c:\gcc850\include -iprefix c:\gcc850\include -iwithprefix \g++ -iwithprefix \g++\std -iwithprefix \g++\v850-elf  -undef -o ./src/%1.s  %1.cpp
@rem as.exe -mv850e ./src/%1.s -o ./obj/%1.o -ahls > .\lst\%1.lst


@rem queste opzioni usano una mov con immediato a 16 bit ATTENZIONE!!!!
@rem cc1plus.exe -mv850e -g -O1 -mno-prolog-function -fno-exceptions -fno-rtti -fno-enforce-eh-specs -Wreturn-type -mdisable-callt -version -Wall -quiet -mtda=0 -msda=0 -mzda=0 -fshort-enums -freg-struct-return -fshort-double -nostdinc -I c:\gcc850\include -iprefix c:\gcc850\include -iwithprefix \g++ -iwithprefix \g++\std -iwithprefix \g++\v850-elf  %1.cpp -o ./src/%1.s
@rem as.exe -mv850e -gstabs ./src/%1.s -o ./obj/%1.o -a > .\lst\%1.lst
@rem --------------------------------------------------------------

@cc1plus.exe -g -quiet -O2 -mv850 -Wall -mUS-bit-set -mno-prolog-function -fno-exceptions -fno-rtti -fno-enforce-eh-specs -Wreturn-type -mdisable-callt -fshort-enums -freg-struct-return -fshort-double -fverbose-asm -I c:\gcc850\include -iprefix c:\gcc850\include -iwithprefix \g++ -iwithprefix \g++\std -iwithprefix \g++\v850-elf  -undef -o ./src/%1.s  %1.cpp
@as.exe -mv850 ./src/%1.s -o ./obj/%1.o -ahls > .\lst\%1.lst



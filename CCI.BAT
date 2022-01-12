@rem FILE BATCH PER COMPILAZIONE CON SIMBOLI E CON OTTIMIZZAZIONE DEDICATO ALLLA GESTIONE IRQ
@rem PER DEBUG CON SIMULATORE/ROM-MONITOR
@cc1plus.exe -mv850 -g -O2 -mprolog-function -fno-exceptions -fno-rtti -fno-enforce-eh-specs -Wreturn-type -mdisable-callt -version -Wall -quiet -mtda=0 -msda=0 -mzda=0 -fshort-enums -freg-struct-return -fshort-double -nostdinc -I c:\gcc850\include -iprefix c:\gcc850\include -iwithprefix \g++ -iwithprefix \g++\std -iwithprefix \g++\v850-elf  %1.cpp -o ./src/%1.s
@as.exe -mv850 -gstabs ./src/%1.s -o ./obj/%1.o -a > .\lst\%1.lst

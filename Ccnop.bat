@rem FILE BATCH PER COMPILAZIONE CON SIMBOLI E SENZA OTTIMIZZAZIONE
@rem PER EVITARE CHE NEL FILE DEI MESSAGGI VENGANO "SEGATE" LE STRINGHE NON USATE
@cc1plus.exe -mv850 -g -O0 -mno-prolog-function -fno-exceptions -fno-rtti -fno-enforce-eh-specs -Wreturn-type -mdisable-callt -version -Wall -quiet -mtda=0 -msda=0 -mzda=0 -fshort-enums -freg-struct-return -fshort-double -nostdinc -I c:\gcc850\include -iprefix c:\gcc850\include -iwithprefix \g++ -iwithprefix \g++\std -iwithprefix \g++\v850-elf  %1.cpp -o ./src/%1.s
@as.exe -mv850 ./src/%1.s -o ./obj/%1.o -a > .\lst\%1.lst

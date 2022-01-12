@rem File Batch per compilazione con simboli e senza ottimizzazione
@rem per debug con simulatore
@as.exe -L -v %1.s -o ./obj/%1.o -a > .\lst\%1.lst
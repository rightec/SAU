@rem File Batch per compilazione con simboli e senza ottimizzazione
@rem per debug con simulatore
@rem ld -v -T .\%1 -verbose -warn-common -verbose -Map .\%2 -L c:\gcc850\lib
ld -v -T .\%1 -warn-common -Map .\%2 -L c:\gcc850\lib



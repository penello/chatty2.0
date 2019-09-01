#!/bin/bash

nclient=$(grep "Test" testout.log | wc -l)


echo "numero client:"$nclient


succesop=$(grep "operazioni con successo:" testout.log | cut -d ":" -f 2 | paste -sd+ |bc)
noperazioni=$(grep "operazioni tentate:" testout.log | cut -d ":" -f 2| paste -sd+ |bc)

echo "Operazioni totali:"$noperazioni
echo "Operazioni svolte con successo:"$succesop
nfallimenti=$(( $succesop - $noperazioni ))
echo "Operazioni fallite:"$nfallimenti


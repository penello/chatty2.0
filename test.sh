#!/bin/bash

killall -SIGUSR1 main

for (( i=0; i<50; i++ ))
do
    ./mainclient $i 1 >>testout.log &
done

wait

killall -SIGUSR1 main

for (( i=0; i<30; i++ ))
do
    ./mainclient $i 2 >>testout.log &
done


for (( i=30; i<50; i++ ))
do
    ./mainclient $i 3 >>testout.log &
done
 
wait

killall -SIGUSR1 main

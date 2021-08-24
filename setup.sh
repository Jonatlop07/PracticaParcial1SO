#!/usr/bin/env bash
tamanios=('1024' '10240' '102400' '1048576' '10485760' '104857600')

make -f Makefile.mk

echo $'\n\n<---ARCHIVOS--->:'
for i in "${tamanios[@]}"
do
  ./arch_prod $i & ./arch_cons $i
  wait
done

rm data.txt

echo $'\n\n<---MEMORIA COMPARTIDA--->:'
for i in "${tamanios[@]}"
do
  ./shm_prod $i
  wait
done

echo $'\n\n<---TUBERIAS--->:'
./pipe
wait

echo $'\n\n<---SOCKETS--->'
./socket_prod & ./socket_cons 127.0.0.1
wait

echo $'\n\n<---MKFIFO--->:'
mkfifo mkfifo1 -m666
mkfifo mkfifo2 -m666
./mkfifo_prod & ./mkfifo_cons
wait
rm mkfifo1
rm mkfifo2



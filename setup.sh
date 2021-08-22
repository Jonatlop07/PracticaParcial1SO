#!/usr/bin/env bash
tamanios=('1024' '10240' '102400' '1048576' '10485760' '104857600')

gcc archivo_prod.c -o ./arch_prod
gcc archivo_cons.c -o ./arch_cons
gcc shm_prod.c -o ./shm_prod
gcc shm_cons.c -o ./shm_cons
gcc pipe.c -o ./pipe
gcc mkfifo_prod.c -o ./mkfifo_prod
gcc mkfifo_cons.c -o ./mkfifo_cons
gcc socket_prod.c -o ./socket_prod
gcc socket_cons.c -o ./socket_cons

rm ./mkfifo

for i in "${tamanios[@]}"
do
  ./arch_prod $i & ./arch_cons $i
  ./shm_prod $i & ./shm_cons $i
  ./pipe $i
  ./mkfifo_prod $i & ./mkfifo_cons $i
  ./socket_prod $i & ./socket_cons 127.0.0.1 $i
done

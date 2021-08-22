#!/usr/bin/env bash
tamanios=('1024' '10240' '102400' '1048576' '10485760' '104857600')

gcc archivo_prod.c -o ./arch_prod
gcc archivo_cons.c -o ./arch_cons
gcc shm_prod.c -o ./shm_prod
gcc shm_cons.c -o ./shm_cons

for i in "${tamanios[@]}"
do
  ./arch_prod $i & ./arch_cons $i
  ./shm_prod $i & ./shm_cons $i
done

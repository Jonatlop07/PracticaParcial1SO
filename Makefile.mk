compile:
	gcc archivo_prod.c -o ./arch_prod
	gcc archivo_cons.c -o ./arch_cons
	gcc shm_prod.c -o ./shm_prod
	gcc shm_cons.c -o ./shm_cons
	gcc pipe.c -o ./pipe
	gcc socket_prod.c -o ./socket_prod 
	gcc socket_cons.c -o ./socket_cons
	gcc mkfifo_prod.c -o ./mkfifo_prod
	gcc mkfifo_cons.c -o ./mkfifo_cons


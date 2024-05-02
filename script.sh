make clean
gcc -g -c -fPIC *.c
gcc -shared -o libhmm.so *.o
LD_PRELOAD=/home/mohamedrefat/Desktop/HMM/HMM/libhmm.so bash

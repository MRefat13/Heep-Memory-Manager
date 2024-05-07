make clean
sudo rm -rf /var/crash
sudo rm -rf ./vim_dump
ulimit -c unlimited
sudo systemctl restart apport
gcc -c -g -fPIC *.c
gcc -shared -o libhmm.so *.o
LD_PRELOAD=/home/mohamedrefat/Desktop/HMM/HMM/libhmm.so vim
apport-unpack /var/crash/_usr_bin_vim.basic.1000.crash ./vim_dump
gdb vim ./vim_dump/CoreDump

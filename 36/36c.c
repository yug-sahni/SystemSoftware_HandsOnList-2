/*
 *sudo apt install strace

strace -c mknod fifo3 p
strace -c mkfifo fifo4

strace -e trace=mknod,mknodat mknod fifo5 p
strace -e trace=mknod,mknodat mkfifo fifo6

rm -f fifo1 fifo2 fifo3 fifo4 fifo5 fifo6
 */

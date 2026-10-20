./init_ticket          # run once
./ticket & ./ticket & ./ticket &   # simulate concurrent customers
wait

./sharedsem
./counting
./rem_sem               # cleanup, if counting.c's own IPC_RMID wasn't reached
Ticket semaphore initialized to 1.
[1] 17164
[2] 17165
[3] 17166
New ticket number: 104
New ticket number: 105
New ticket number: 106
[1]   Done                       ./ticket
Child 1 wrote to shared memory.
Child 2 wrote to shared memory.
Final shared data: Written by child 2
Process 1 acquired a resource.
Process 2 acquired a resource.
Process 1 released a resource.
Process 3 acquired a resource.
Process 2 released a resource.
Process 4 acquired a resource.
Process 4 released a resource.
Process 3 released a resource.
Counting semaphore removed.
semget: No such file or directory


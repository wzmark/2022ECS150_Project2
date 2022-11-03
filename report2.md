# project 2 report 
## User-leverl thread library
### Summary
We accomplish this project by developing different phases in order. 
We first implement queue APU, and then thread API, and then implement the 
proper `semaphore()` mechanism to uthread, and at the end implement 
preemption.

### Queue API
In order to implement our queue, we create a struct structure called **queue**,
which contains several members. We use the void pointer `data` to encapsulate
each element in the queue as a node, and then we use `queueSize` to ensure 
there are enough space for data storage. Whenever we have a new node queued, 
we increase the length by queueSize+1 to make sure it doesn't run out of 
space. Moreover, in order to maintain that operations are O(1), we create the 
`front` and `next` struct member to help track the position of the front node 
and the next code of the queue. 

#### Queue Functions Implementation
We first implement the `queue_create` function. This function will allocate an
empty queue and helps create a new object of type 'struct queue' and return its
address. Then we implement the `queue_destroy` function, which will deallocate 
a queue. The function will return -1 when the queue is empty or not full, and 
willreturn 0 if the queue has been successfully destroyed. We also `free` our 
queuealong this process. Moreover, we implement `queue_enqueue` and 
`queue_dequeue`those two functions. Because we have already created the 
`front` node, so wecan make sure that we can directly access to the front-most 
node of the array. While we enqueue, queueSize +1 since we have one more 
node and make the front node become the next node. While we dequeue, the 
queueSize -1 because we dequeue the oldest node.After we successfully implement
enqueue and dequeue,we now try to execute `queue_delete`. This function helps
to find a specificdata and delete it, and the size of the queue will be 
decremented and return 0.Furthermore, we implement `queue_iterate`, which 
iterates through the queue, from the oldest to the newest, calls the callback 
function and applies a function pointer to each pointer in the queue. We also
need to make sure that if the data member has been deleted, the pointer will 
still continue to iterate to the end. The last function we apply is the 
`queue_length`, which return the length of the queue. In our queue, it will 
just return the number we get in `queueSize`, and will return -1 if the queue
 is full. 


### Uthread API
We provide applications that would like to use a thread library as an interface
that manages the scheduling of different threads and implement each thread 
function and make them successfully perform their tasks. In this phase, we 
create a struct object called `uthread_tcb`, which stands for thread control
block. The struct object including members`tid`, which stands for the thread
ID, `thread_state` will help us keep track if which state are we currently 
working on, and `stack`, which is used for allocating the thread's stack and 
will store the return value of it. Besides the struct object, we also create 
`queue_t` global variables which contains `queueofBlocked`, `queueofZombie`,
and `queueofZombie` to help us represent the blocked state, zombie state and 
zombie state. We create a function called `ThreadInitialize`, which will
initialize one thread, and will check `threadIdCount`. If the count equal to
0, we will put two NULL into the function `uthread_ctx_init` as main thread.
If the count doesn't equal to 0, we will put two regular values as the main
thread.

#### Uthread Functions Implementation
The first function we implement is called `uthread_run()`. This function will
run the multithreading library and will become the `idle` thread after 
successfully starts the scheduling library. The idle threads guide the initial 
threads and continue to yield until there are no more threads in the queue.
Then we implement `thread_create` function, which will help to create a new 
thread and enqueue each new thread. We first allocate memory and initialize the
appropriate properties for the thread and its context. The function will return
0 if success and -1 if failed. Furthermore, we work on `uthread_yield`. This
function builds to be called from the currently active and running thread
and yield for others threads to execute. We first `queue_enqueue` to enqueue
the current running thread into ready state, and then using `queue_dequeue` to
get the next ready thread, and then using `uthread_ctx_switch` to switch 
thread between previous thread and current running thread. The last function 
we implement is `uthread_exit`, which will be called from the currently active 
and running thread iin order to finish its execution. We change the current 
running thread's state to zombie state and then yield to the next thread.


### Semaphore API
We provide the application of semaphore in order to control the access to 
common resources by multiple threads. On top of the existing struct object,
we create another semaphore sturct, including a queue and variable `maxCount`,
`currentNumberofThread` and `lock`. The queue is built for handle threads
that are in the wating list, the maxCount is used for count the maximum 
number of threads that able to share the common resource, the 
currentNumberOfThread is used for track the number of current thread, and the
lock is used for controlling the access to common resources. 

#### Semaphore API Function Implementation
The first function we implement is called `sem_create`, which allocate and 
initialize a semaphore of internal count. We first allocate the momory and 
then initialize a new queue to pass in counts. Then we implement `sem_destroy`,
which used for deallocate semaphore. We reset `currentNumberofThread` and 
`maxCount` both to 0 in order to destroy the queue. Furthermore, we implement
`sem_down`, which will help block the caller thread until the semaphore
becomes avalible. We first let the `currentNumberofThread` minus 1, and then
will check whether the `currentNumberofThread` is lessthan 0, and if it does, 
we get the current thread and use enque function to put it back to the queue, 
and then block the current thread. The last function we implement, in the
opposite of sem_down, we utilize `sum_up`. This function will release a 
resource to semaphore if the waiting list associated to semaphore is not 
empty, and then unlock the first thread in the waiting list. We let the 
`currentNumberofThread` add 1 and then then get the block thread and dequeue
the queue and unblock the thread. 


### Preemption 
We implement necessary functions in order to force a thread force into 
yeilding after having been running for some amount of time. We also assign
a fixed time to each thread so that no particular thread takes up CPU time. 
Before implementing functions, we set up some variables that help us better 
perform preemption. We first define a Macro `HZ` equal to 100, which 
represents the frequency of preemption and 100HZ stands for 100 times per 
second. Then we creates two stucts, `new, old` and `Sigaction`. `new, old` is 
a type of itimerval, which is built for time controller, and `Sigaction` is 
a type of sigaction which is built for signal handling. After that, we 
create an alarm function which contains an `alarmtype` that will help us
intialize and remove the alarm. We also create an  handler function,which calls
 `uthread_yield` in order to block the thread which need to interupt. 

#### Preemtion Function Implemntation
The function we implement is `preempt_start`, as we have already initialzed the
timer and sigaction struct, all we need to do is to set up the timer and 
configure it to a frequency of 100hz. Then, we implement `preempt_stop`, which
restore previous timer configuration. All we need to do is to remove the alram
and sigaction. Furthermore, we implement `preempt_enable` and `preempt_diable`,
which stands for enable preemtion and disable preemtion. We use `sigprocmask`,
and set `SIG_UNBLOCK` to disable the preemption. After we successfully build
these functions about preemption, we need to add those functions in `uthread.c`
file in order to make it compatible. We call `preempt_start` in `uthread_run`
function when preempt is equal to 1. In function `uthread_yield`, we add 
`preempt_disable` because it is the critical section and we don't want the
preemption to interrupt the program. 


## Testing development
### Queue API Testing
In order to test our queue, we add some features in the `queue_tester.c`, which
tests different queue functions. We first create 3 data and 2 pointer. We will 
first seperatly enqueue data 0, data 1 and data 2 into the queue, and then we 
dequeue our queue, and check whether the return value is equal to data 0. 
Then we enqueue data 0 back to our queue, and to delete data 1. After we delete
the data 1, we continue to dequeue our queue, to see whether the return value
equal to data 2. 

### Uthread API Testing
To test our uthread, we use the test file `uthread_hello.c` to test the basic 
printing thread features and use the test file `uthread_yield.c` to test our
`uthread_yield` function, and the results we get is the same as the expected
output.

### Semaphore API Testing
To test the semaphore API, we use the test file `sem_simple.c` to test simple 
synchronization of three threads. The return results we get is thread 3, 
thread 2 and thread 1, which is the same as expected output, then we use
`sem_count` to alternate counting with two threads and two semaphores. The 
results we get is thread2 and thread1 alternately print all numbers from 0-19.
Then we use `sem_buffer.c` to test the producer and consumer test, that they
 exchange data in a buffer, and the last we use is `sem_prime`, to test the 
prime sieve implemented with a growing pipeline of threads, and the results
we get is a list of words that printing a given prime number is prime. 


### Preemption Testing
To test the preemption, we use the test file `test_preempt.c`. In the given 
file, we will first have the thread 1 printed, and then print thread 2. 
'Finish thread' will not be printed because the while loop will always be 1.
The results we return perfectly match what we expected to see.


## Sources
[GNU Library](https://www.gnu.org/software/libc/manual/html_mono/libc.html) 

[Abraham Silberschatz, Peter B. Galvin, Greg Gagne - Operating System 
Concepts-Wiley (2012).pdf] 






















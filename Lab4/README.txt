
Our lab implementations are split into two directories: sketch_part1 and sketch_part2.

Each of these directories contains a large commented block at the top that describes our implementation and approach to the problem. For a description of our concurrency model, see sketch_part1.ino. For a description of our lock model, see sketch_part2.ino. Additionally, functions are annotated as necessary within these files. 

Regardless, a simple explanation of each is below:

Part 1, Concurrency:
The main idea for our implementation here is to store a global queue of processes waiting to execute. The state associated with each process keeps track of their current stack pointer, if they've started execution, and the process that follows them. Insertion into this linked list is simple. The real meat of this part was getting `process_select` correct, as this handles all context switches. In this function, we handle all cases of execution: termination, initialization, switches. As long as no access to a shared resource ocurrs by processes, this can support variable number of concurrent processes. With our queue, we essentially have a round-robin schedule, where each process in the queue will get the same amount of execution time.

Part 2, Locks:
Here, our lock did not need to guarantee fairness, only mutual exclusion, so implementing it was easy. With standard locks, atomic operations such as test-and-set or compare-and-swap are enough to implement a lock. However, we don't have access to such operations, but we can provide the illusion of this by disabling interrupts. When interrupts are disabled, we are guaranteed that a context switch cannot occur, so anything that a process does while interrupts are disabled can be considered as atomic operations. With this in mind, our lock acquisition and release was easy. To acquire a lock, a process will disable interrupts, read the value of the lock, and if possible, set it as held. If someone else holds the lock, simply reactivate interrupts, and try again until success. To release a lock, disable interrupts, and set the `held` flag as 0, this will allow the next acquisition of the lock to succeed. It must be noted that this does not provide any level of fairness; if we wished to implement fairness, we could implement a queue functionality to our locks to ensure that access to a lock is given in some sort of order (first-come, first-served). This may also require some changes to our process_select, but is entirely dependent on how the fair lock is implemented.


For a deeper explanation of these implementations, please see the code for each part, specifically the comment block at the top of each file, which describes each function in greater detail.


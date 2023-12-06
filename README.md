# Light-duty-bridge-simulation-using-Multithreading

In this project, I used the C library, pthread, to create and simulate traffic over a narrow light-duty bridge. Upon running the program, 20 threads are created with each one representing one vehicle. The vehicles must be synchronized to cross the bridge.
The bridge can be crossed bi-directionally, although, vehicles going in opposite directions cannot cross it simultaneously. Furthermore, the bridge cannot support more than three vehicles.

Pthreads: Pthreads is a standardized model for dividing a program into subtasks whose execution can be interleaved or run in parallel. The "P" in Pthreads comes from POSIX (Portable Operating System Interface), the family of IEEE operating system interface standards in which Pthreads is defined (POSIX Section 1003.1c).

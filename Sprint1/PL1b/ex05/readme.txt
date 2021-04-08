5. 
a) the terminal, when receiving the interrupt command Ctrl-C, sends an interrupt signal 
to the process that is running in selected window (terminal) and the process is then 
terminated.

c) the terminal, when receiving the quit command Ctrl-\, sends a quit signal 
to the process that is running in selected window (terminal) and the process is then 
quitted.

e) on another terminal the same behaviour is experienced, nor SIGINT nor SIGQUIT
interrupts the process

f) after suspending with Ctrl-Z, and killing with kill -KILL %d, the process now 
is identified as defunct.
After using kill %1, the process is then terminated and no longer shows up on the
process table.
By executing jobs, before kill %1, the command line prints the processes that are
stopped, their number in order of stopping priority and the command that created them
The kill %1 command, terminated the process that was stopped first and therefore
is the first one in the list of stopped processes. kill %2 the second, and so forth.

e) write is a simple kernel function that only prints the string to be printed.
printf is a costlier function that it is outside of the kernel and only calls for 
the write function after it receives all the parameter and format the string to be printed.
So there is a chance that while the printf is gathering all the info to be printed, it 
also gathers information that do not belong to the print string an when it calls write,
it writes more information than it should do.
3. 
a) sigaction é mais correto de utilizar do que o signal em programas que 
utilizem mais do que um processo. O signal foi criado com o escalonamento do 
Linux em mente, logo a interpretação de um determinado sinal se o programas for
executado noutro sistema operativo (WinOS, OSX,...) pode ter resultados que não
os desejados.

b)
struct sigaction {
    void     (*sa_handler)(int); (sa_handler specifies the action to be associated with signum and may be SIG_DFL for
   the default action, SIG_IGN to ignore this signal, or a pointer to a signal handling
   function. This function receives the signal number as its only argument.)
   
    void     (*sa_sigaction)(int, siginfo_t *, void *); (If SA_SIGINFO is specified in sa_flags, then sa_sigaction (instead of sa_handler)
   specifies the signal-handling function for signum. This function receives three
   arguments, as described below.)
	
    sigset_t   sa_mask; (sa_mask specifies a mask of signals which should be blocked (i.e., added to the
   signal mask of the thread in which the signal handler is invoked) during execution of
   the signal handler. In addition, the signal which triggered the handler will be
   blocked, unless the SA_NODEFER flag is used.)
   
    int        sa_flags; (sa_flags specifies a set of flags which modify the behavior of the signal. It is
   formed by the bitwise OR of zero or more of the following:
   SA_NOCLDSTOP)
	
    void     (*sa_restorer)(void); (The sa_restorer field is not intended for application use. (POSIX does not specify a
   sa_restorer field.) Some further details of the purpose of this field can be found in
   sigreturn(2).)
	
};

sigaction() returns 0 on success; on error, -1 is returned, and errno is set to
   indicate the error.
   
   
   
   
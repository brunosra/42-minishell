Minishell Functions Documentation
=================================

1\. `readline` (GNU Readline library)
-------------------------------------

### Description:

Reads a line of input from the user, providing line-editing features and history support.

### Returns:

A dynamically allocated string containing the user input, or `NULL` on EOF (e.g., `Ctrl+D`).

### Usage:

`char *readline(const char *prompt);`

2\. `rl_clear_history` (GNU Readline library)
---------------------------------------------

### Description:

Clears the history of commands saved by `add_history`.

### Returns:

`void`

### Usage:

`void rl_clear_history(void);`

3\. `rl_on_new_line` (GNU Readline library)
-------------------------------------------

### Description:

Informs Readline that a new line has started, often used after a signal interrupt.

### Returns:

`0` if successful, `-1` on failure.

### Usage:

`int rl_on_new_line(void);`

4\. `rl_replace_line` (GNU Readline library)
--------------------------------------------

### Description:

Replaces the current contents of the input line with a new string.

### Returns:

`void`

### Usage:

`void rl_replace_line(const char *text, int clear_undo);`

5\. `rl_redisplay` (GNU Readline library)
-----------------------------------------

### Description:

Redisplays the current line after modifications (e.g., after calling `rl_replace_line`).

### Returns:

`void`

### Usage:

`void rl_redisplay(void);`

6\. `add_history` (GNU Readline library)
----------------------------------------

### Description:

Adds a line of input to the command history for future recall with arrow keys.

### Returns:

`void`

### Usage:

`void add_history(const char *line);`

7\. `printf` (Standard Library)
-------------------------------

### Description:

Formats and prints a string to the standard output.

### Returns:

The number of characters printed (excluding the null byte), or a negative value on error.

### Usage:

`int printf(const char *format, ...);`

8\. `malloc` (Standard Library)
-------------------------------

### Description:

Allocates a specified number of bytes of memory.

### Returns:

A pointer to the allocated memory, or `NULL` if allocation fails.

### Usage:

`void *malloc(size_t size);`

9\. `free` (Standard Library)
-----------------------------

### Description:

Frees previously allocated memory.

### Returns:

`void`

### Usage:

`void free(void *ptr);`

10\. `write` (System Call)
--------------------------

### Description:

Writes data to a file descriptor (e.g., stdout, a file, or a pipe).

### Returns:

The number of bytes written, or `-1` on error.

### Usage:

`ssize_t write(int fd, const void *buf, size_t count);`

11\. `access` (System Call)
---------------------------

### Description:

Checks the calling process's ability to access a file. It can verify the existence, read, write, or execute permissions.

### Returns:

Returns `0` on success, or `-1` on error (with `errno` set to indicate the error).

### Usage:

`int access(const char *pathname, int mode);`

12\. `open` (System Call)
-------------------------

### Description:

Opens a file and returns a file descriptor. It can also create a new file.

### Returns:

Returns the file descriptor on success, or `-1` on error.

### Usage:

`int open(const char *pathname, int flags, mode_t mode);`

13\. `read` (System Call)
-------------------------

### Description:

Reads data from a file descriptor into a buffer.

### Returns:

The number of bytes read, or `-1` on error.

### Usage:

`ssize_t read(int fd, void *buf, size_t count);`

14\. `close` (System Call)
--------------------------

### Description:

Closes a file descriptor, so that it no longer refers to any file and may be reused.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int close(int fd);`

15\. `fork` (System Call)
-------------------------

### Description:

Creates a new process by duplicating the calling process. The new process is referred to as the child process.

### Returns:

The process ID (PID) of the child process to the parent process, `0` to the child process, or `-1 on error.`

``

### Usage:

`pid_t fork(void);`

``

``

16. `wait` (System Call)
------------------------

### Description:

Suspends execution of the calling process until one of its children terminates.

### Returns:

The PID of the child that terminated, or `-1` on error.

### Usage:

`pid_t wait(int *status);`

17. `waitpid` (System Call)
---------------------------

### Description:

Waits for a specific child process to change state.

### Returns:

The PID of the child that terminated, `0` if `WNOHANG` is specified and no child has exited, or `-1` on error.

### Usage:

`pid_t waitpid(pid_t pid, int *status, int options);`

18. `wait3` (System Call)
-------------------------

### Description:

Similar to `wait`, but also returns resource usage information for the child process.

### Returns:

The PID of the child that terminated, or `-1` on error.

### Usage:

`pid_t wait3(int *status, int options, struct rusage *rusage);`

19. `wait4` (System Call)
-------------------------

### Description:

Similar to `waitpid`, but also returns resource usage information for the child process.

### Returns:

The PID of the child that terminated, or `-1` on error.

### Usage:

`pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage);`

20. `signal` (System Call)
--------------------------

### Description:

Sets a signal handler for the specified signal.

### Returns:

The previous value of the signal handler, or `SIG_ERR` on error.

### Usage:

`void (*signal(int signum, void (*handler)(int)))(int);`

21. `sigaction` (Signal Handling)
---------------------------------

### Description:

Examines and changes the action taken by a process on receipt of a specific signal.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);`

22. `sigemptyset` (Signal Handling)
-----------------------------------

### Description:

Initializes the signal set to exclude all signals.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int sigemptyset(sigset_t *set);`

23. `sigaddset` (Signal Handling)
---------------------------------

### Description:

Adds a specific signal to a signal set.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int sigaddset(sigset_t *set, int signum);`

24. `kill` (Signal Handling)
----------------------------

### Description:

Sends a specified signal to a specified process.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int kill(pid_t pid, int sig);`

25. `exit` (Standard Library)
-----------------------------

### Description:

Terminates the calling process and returns a status code to the operating system.

### Returns:

This function does not return; the process is terminated.

### Usage:

`void exit(int status);`

26. `getcwd` (Standard Library)
-------------------------------

### Description:

Gets the current working directory.

### Returns:

A pointer to the current working directory string on success, or `NULL` on error.

### Usage:

`char *getcwd(char *buf, size_t size);`

27. `chdir` (Standard Library)
------------------------------

### Description:

Changes the current working directory of the calling process to the specified path.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int chdir(const char *path);`

28. `stat` (File Information)
-----------------------------

### Description:

Retrieves information about a specified file.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int stat(const char *path, struct stat *buf);`

29. `lstat` (File Information)
------------------------------

### Description:

Retrieves information about a specified file, without following symbolic links.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int lstat(const char *path, struct stat *buf);`

30. `fstat` (File Information)
------------------------------

### Description:

Retrieves information about an open file referenced by the file descriptor.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int fstat(int fd, struct stat *buf);`

31. `unlink` (File Operations)
------------------------------

### Description:

Removes a link to a file, effectively deleting the file if it has no more links.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int unlink(const char *pathname);`

32. `execve` (Process Execution)
--------------------------------

### Description:

Executes the program pointed to by a file descriptor, replacing the current process image.

### Returns:

This function does not return on success; on error, it returns `-1`.

### Usage:

`int execve(const char *pathname, char *const argv[], char *const envp[]);`

33. `dup` (File Operations)
---------------------------

### Description:

Duplicates a file descriptor, returning a new file descriptor that refers to the same open file description.

### Returns:

Returns the new file descriptor on success, or `-1` on error.

### Usage:

`int dup(int oldfd);`

34. `dup2` (File Operations)
----------------------------

### Description:

Duplicates a file descriptor to a specified value.

### Returns:

Returns the new file descriptor on success, or `-1` on error.

### Usage:

`int dup2(int oldfd, int newfd);`

35. `pipe` (Interprocess Communication)
---------------------------------------

### Description:

Creates a pair of connected file descriptors for interprocess communication.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int pipe(int pipefd[2]);`

36. `opendir` (Directory Operations)
------------------------------------

### Description:

Opens a directory stream corresponding to the directory name.

### Returns:

Returns a pointer to the directory stream on success, or `NULL` on error.

### Usage:

`DIR *opendir(const char *name);`

37. `readdir` (Directory Operations)
------------------------------------

### Description:

Reads the next entry from a directory stream.

### Returns:

Returns a pointer to the next `struct dirent` on success, or `NULL` on error or end of directory.

### Usage:

`struct dirent *readdir(DIR *dirp);`

38. `closedir` (Directory Operations)
-------------------------------------

### Description:

Closes a directory stream.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int closedir(DIR *dirp);`

39. `strerror` (String Operations)
----------------------------------

### Description:

Returns a pointer to the textual representation of the current errno value.

### Returns:

Returns a pointer to the error string.

### Usage:

`char *strerror(int errnum);`

40. `perror` (Error Handling)
-----------------------------

### Description:

Prints a description for the last error that occurred, as indicated by `errno`.

### Returns:

This function does not return a value.

### Usage:

`void perror(const char *s);`

41. `isatty` (Terminal Operations)
----------------------------------

### Description:

Tests whether a file descriptor refers to a terminal.

### Returns:

Returns `1` if the file descriptor refers to a terminal, `0` otherwise, or `-1` on error.

### Usage:

`int isatty(int fd);`

42. `ttyname` (Terminal Operations)
-----------------------------------

### Description:

Returns the name of the terminal associated with the specified file descriptor.

### Returns:

Returns a pointer to the terminal name on success, or `NULL` on error.

### Usage:

`char *ttyname(int fd);`

43. `ttyslot` (Terminal Operations)
-----------------------------------

### Description:

Returns the slot number of the terminal associated with the calling process.

### Returns:

Returns the terminal slot number or `-1` on error.

### Usage:

`int ttyslot(void);`

44. `ioctl` (Device Control)
----------------------------

### Description:

Performs device-specific input/output operations on file descriptors.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int ioctl(int fd, unsigned long request, ...);`

45. `getenv` (Environment Variables)
------------------------------------

### Description:

Retrieves the value of an environment variable.

### Returns:

Returns a pointer to the value string, or `NULL` if the variable is not found.

### Usage:

`char *getenv(const char *name);`

46. `tcsetattr` (Terminal Control)
----------------------------------

### Description:

Sets the parameters associated with the terminal referenced by the file descriptor.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int tcsetattr(int fd, int optional_actions, const struct termios *termios_p);`

47. `tcgetattr` (Terminal Control)
----------------------------------

### Description:

Gets the parameters associated with the terminal referenced by the file descriptor.

### Returns:

Returns `0` on success, or `-1` on error.

### Usage:

`int tcgetattr(int fd, struct termios *termios_p);`

48. `tgetent` (Terminal Capabilities)
-------------------------------------

### Description:

Initializes the terminal capability data structures from the terminfo database.

### Returns:

Returns `1` if the terminal exists, `0` if it does not, or `-1` on error.

### Usage:

`int tgetent(char *bp, const char *name);`

49. `tgetflag` (Terminal Capabilities)
--------------------------------------

### Description:

Checks if a specified boolean capability is present.

### Returns:

Returns `1` if the capability is present, `0` if it is not, or `-1` on error.

### Usage:

`int tgetflag(const char *id);`

50. `tgetnum` (Terminal Capabilities)
-------------------------------------

### Description:

Retrieves the value of a specified numeric capability.

### Returns:

Returns the value of the capability, or `-1` if it does not exist.

### Usage:

`int tgetnum(const char *id);`

51. `tgetstr` (Terminal Capabilities)
-------------------------------------

### Description:

Retrieves the value of a specified string capability.

### Returns:

Returns a pointer to the capability string, or `NULL` if it does not exist.

### Usage:

`char *tgetstr(const char *id, char **area);`

52. `tgoto` (Terminal Capabilities)
-----------------------------------

### Description:

Generates a string to move the cursor to a specific position on the terminal.

### Returns:

Returns a pointer to the cursor movement string.

### Usage:

`char *tgoto(const char *cap, int col, int row);`

53. `tputs` (Terminal Capabilities)
-----------------------------------

### Description:

Outputs a string, applying padding to ensure correct timing.

### Returns:

Returns `OK` on success, or `ERR` on error.

### Usage:

`int tputs(const char *str, int affcnt, int (*outc)(int));`





``
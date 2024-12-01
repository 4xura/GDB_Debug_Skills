[toc]

# GDB | Init

## Exec Template

```bash
#!/bin/sh

gdb -q \
    -ex "" \
    -ex "" \
    -ex "" \
    ./pwn
```

## Set PATH

Customize `LD_LIBRARY_PATH`:

```bash
gdb -q \
	--args ./ld-2.35.so \
    --library-path ./libc-2.35.so.6 \
    ./pwn
```



# GDB | Config

## LD & Glibc

```sh
set exec-wrapper ./ld-linux-x86-64.so.2 --library-path ./
# set environment LD_LIBRARY_PATH=./
```

## Configuration

Check current configurations:

```sh
gdb --configuration
```

Example:

```sh
$ gdb --configuration
This GDB was configured as follows:
   configure --host=x86_64-pc-linux-gnu --target=x86_64-pc-linux-gnu
             --with-auto-load-dir=$debugdir:$datadir/auto-load
             --with-auto-load-safe-path=$debugdir:$datadir/auto-load
             --with-expat
             --with-gdb-datadir=/usr/share/gdb (relocatable)
             --with-jit-reader-dir=/usr/lib/gdb (relocatable)
             --without-libunwind-ia64
             --with-lzma
             --without-babeltrace
             --without-intel-pt
             --with-xxhash
             --with-python=/usr (relocatable)
             --with-python-libdir=/usr/lib (relocatable)
             --with-debuginfod
             --with-curses
             --with-guile
             --without-amd-dbgapi
             --enable-source-highlight
             --enable-threading
             --enable-tui
             --with-system-readline
             --with-separate-debug-dir=/usr/lib/debug (relocatable)
             --with-system-gdbinit=/etc/gdb/gdbinit
```



# GDB | Debug Symbols

```sh
show debug-file-directory
set debug-file-directory /path/to/lib/.debug/
info share 
```



# GDB | Attach

Run the binary by specifying `LD_LIBRARY_PATH`:

```sh
LD_LIBRARY_PATH=/path/to/custom/libc /path/to/ld --library-path /path/to/libc ./binary
```

`CTRL Z` to background the process, Get `PID`, and attach it to GDB:

```sh
ps aux | grep <binary_file_name>
gdb -p <PID>
```



# GDB | X

## X | Format

```sh
x /[n][f][u] <address>
```

- **`/`: Format specifiers**
- **`[n]`: Number of units to display**
- **`[f]` : Format**
  - `x`: Hexadecimal
  - `d`: Decimal
  - `u`: Unsigned Decimal
  - `o`: Octal
  - `t`: Binary
  - `a`: Address
  - `i`: Instruction (disassembly)
  - `c`: Character
  - `s`: String
- **`[u]`: Unit size**
  - `b`: Byte
  - `h`: Hafword
  - `w`: Word
  - `g`: Giant word

## X | Examples

Codes: [link](https://github.com/4xura/GDB_Debug_Skills/blob/main/Codes/gdb_x.c)

### Examine a Global Variable

```c
pwndbg> x &global_var
0x555555558018 <global_var>:    0x12345678
```

If any of `[n]`, `[f]`, or `[u]` are omitted, GDB uses defaults:

- Default format: `x` (hexadecimal)
- Default unit size: `w` (word)
- Default count: `1`

### Inspect Multiple Words

```c
pwndbg> x/4x &global_var
0x555555558018 <global_var>:    0x0000000012345678      0x47202c6f6c6c6548
0x555555558028 <global_str+8>:  0x4048f5c300214244      0x0000000000000000
```

### Examine a String

Display strings until hitting null terminator:

```c
pwndbg> x/s &global_str
0x555555558020 <global_str>:    "Hello, GDB!"
```

### Examine a Float

```c
pwndbg> x/f &global_float
0x55555555802c <global_float>:  3.1400001
```

### Disassemble Instructions

Disassembles 10 instructions starting from the `main` function:

```c
pwndbg> x/10i main
   0x555555555139 <main>:       push   rbp
   0x55555555513a <main+1>:     mov    rbp,rsp
   0x55555555513d <main+4>:     sub    rsp,0x10
=> 0x555555555141 <main+8>:     mov    DWORD PTR [rbp-0x4],0xdeadbeef
   0x555555555148 <main+15>:    lea    rax,[rip+0xeb5]        # 0x555555556004
   0x55555555514f <main+22>:    mov    rdi,rax
   0x555555555152 <main+25>:    call   0x555555555030 <puts@plt>
   0x555555555157 <main+30>:    mov    eax,0x0
   0x55555555515c <main+35>:    leave
   0x55555555515d <main+36>:    ret
```

### Examine a Local Variable

After setting a breakpoint after `test_function`, display the value of `local_var` in hexadecimal format, which is on the stack and only exists while `test_function()` is executing

```c
pwndbg> x/x &local_var
0x7fffffffd7fc: 0xdeadbeef
```

Display a local string:

```c
pwndbg> x/s &local_str
0x7fffffffd80b: "Local String"
```

Display a dynamically set variable after `malloc`:

```c
pwndbg> x/x dynamic_var
0x5555555596b0: 0x00000000feedface
```

### Explore Stack Memory

Display 16 words of memory starting at the current stack pointer (`$rsp`):

```c
pwndbg> x/16x $rsp
0x7fffffffd7f0: 0x00000018      0x00000000      0x00000000      0xdeadbeef
0x7fffffffd800: 0xff000000      0x00000000      0x00000000      0x00000000
0x7fffffffd810: 0x2f2f2f2f      0x2f2f2f2f      0xdc7b3300      0x491ba903
0x7fffffffd820: 0xffffd830      0x00007fff      0x555552d2      0x00005555
```

### Examine Memory Byte by Byte

Display the first 8 bytes of `global_var` in hexadecimal format, one byte at a time:

```c
pwndbg> x/8b &global_var
0x555555558038 <global_var>:    0x78    0x56    0x34    0x12    0x00    0x00    0x00    0x00
```

### Examine Memory as Characters

Display `global_str` character by character, including the null terminator:

```c
pwndbg> x/12c &global_str
0x555555558040 <global_str>:    72 'H'  101 'e' 108 'l' 108 'l' 111 'o' 44 ','  32 ' '  71 'G'
0x555555558048 <global_str+8>:  68 'D'  66 'B'  33 '!'  0 '\000'
```

### Examine Memory in Binary Format

Display `global_var` in binary format, showing its bit-level representation:

```c
pwndbg> x/4t &global_var
0x555555558038 <global_var>:    0000000000000000000000000000000000010010001101000101011001111000        0100011100100000001011000110111101101100011011000110010101001000
0x555555558048 <global_str+8>:  0100000001001000111101011100001100000000001000010100001001000100        0000000000000000000000000000000000000000000000000000000000000000
```

- Displays `global_var` in binary format, showing its bit-level representation.

### Explore Memory Addresses

Display the memory address of `global_var`.

```c
pwndbg> x/a &global_var
0x555555558038 <global_var>:    0x12345678
```



# GDB | Set

Code: [link](https://github.com/4xura/GDB_Debug_Skills/blob/main/Codes/gdb_set.c)

Set a breakpoint before return and compare modified value with the original ones.

## Set | Syntax

```c
set {<type>} <address> = <value>
```

- **`<type>`**: Specifies the data type of the memory content to modify (e.g., `int`, `char`, `float`).
- **`<address>`**: The memory address to modify.
- **`<value>`**: The new value to set at the specified address.

##  Set | Memory Address

The syntax for modifying memory is:

```c
pwndbg> set {int} &global_var = 0xdeadbeef
pwndbg> x &global_var
0x555555558038 <global_var>:    0xdeadbeef
```

## Set | Registers

Inspect registers with command `info registers` or `regs`. 

```c
pwndbg> regs
*RAX  0
...
pwndbg> set $rax = 0x1234
pwndbg> regs
*RAX  0x1234
...
```

## Set | String

Set a global string to a new value:

```c
pwndbg> x &global_str
0x555555558040 <global_str>:    0x6c6c6548
pwndbg> x/s &global_str
0x555555558040 <global_str>:    "Hello, GDB!"
pwndbg> set global_str[0] = 'X'
pwndbg> x/s &global_str
0x555555558040 <global_str>:    "Xello, GDB!"
```

## Set | Variable

```c
set variable <variable_name> = <value>
```



# GDB | Break

## Conditional Breakpoints

```c
break <file>:<line> if <condition>
```

or

```c
break <address> if <condition>
```



# GDB | Threads

Code: [link](https://github.com/4xura/GDB_Debug_Skills/blob/main/Codes/gdb_thread.c)

## Thread | List

Syntax:

```c
info threads
```

Example:

```c
pwndbg> i threads
  Id   Target Id                                      Frame 
* 1    Thread 0x7ffff7d92740 (LWP 5714) "gdb_threads" 0x00007ffff7e25a19 in ?? () from /usr/lib/libc.so.6
  2    Thread 0x7ffff7d916c0 (LWP 5715) "gdb_threads" 0x00007ffff7e76733 in clock_nanosleep ()
   from /usr/lib/libc.so.6
  3    Thread 0x7ffff75906c0 (LWP 5716) "gdb_threads" 0x00007ffff7e76733 in clock_nanosleep ()
   from /usr/lib/libc.so.6
  4    Thread 0x7ffff6d8f6c0 (LWP 5717) "gdb_threads" 0x00007ffff7e76733 in clock_nanosleep ()
   from /usr/lib/libc.so.6
```

## Thread | Switch

Syntax:

```c
thread <thread-id>
backtrace
```

Example:

```c
pwndbg> thread 2
[Switching to thread 2 (Thread 0x7ffff7d916c0 (LWP 5963))]
#0  0x00007ffff7e76733 in clock_nanosleep () from /usr/lib/libc.so.6
pwndbg> bt
#0  0x00007ffff7e76733 in clock_nanosleep () from /usr/lib/libc.so.6
#1  0x00007ffff7e82827 in nanosleep () from /usr/lib/libc.so.6
#2  0x00007ffff7e94f41 in sleep () from /usr/lib/libc.so.6
#3  0x000055555555523d in worker_function (arg=0x7fffffffd7f4) at gdb_threads.c:21
#4  0x00007ffff7e2939d in ?? () from /usr/lib/libc.so.6
#5  0x00007ffff7eae49c in ?? () from /usr/lib/libc.so.6
```

## Thread | Break

Syntax:

```c
break <address> thread <thread-id>
```

## Thread | Watch

Syntax:

```c
watch <variable_name> thread <thread-id>
```

Example:

```c
pwndbg> watch counter thread 3
Hardware watchpoint 1: counter
```

## Thread | Detach

Detach a thread and let it run independently:

```c
detach <thread_id>
```

Example:

```c
pwndbg> t 4
[Switching to thread 4 (Thread 0x7ffff6d8f6c0 (LWP 6120))]
Downloading 3.14 K source file /usr/src/debug/glibc/glibc/time/../sysdeps/unix/sysv/linux/clock_nanosleep.c
#0  0x00007ffff7e76733 in __GI___clock_nanosleep (clock_id=clock_id@entry=0, flags=flags@entry=0,                    
    req=req@entry=0x7ffff6d8ee60, rem=rem@entry=0x7ffff6d8ee60) at ../sysdeps/unix/sysv/linux/clock_nanosleep.c:48
48        r = INTERNAL_SYSCALL_CANCEL (clock_nanosleep_time64, clock_id, flags, req,
pwndbg> detach
Thread 3 incremented counter to 7
Detaching from program: /home/Axura/pwn/gdb_debug_skills/gdb_threads, process 6115
Thread 2 incremented counter to 8
Thread 1 incremented counter to 9
[Inferior 1 (process 6115) detached]
pwndbg> Thread 1 incremented counter to 10
Thread 2 incremented counter to 11
Thread 3 incremented counter to 12
Thread 2 incremented counter to 13
Thread 1 incremented counter to 14
Thread 3 incremented counter to 15
Final counter value: 15
```

## Thread | Scheduler-locking

The `set scheduler-locking` command in GDB controls how threads are scheduled during debugging.

1. **`off` (Default)**
   - All threads are allowed to run freely during program execution.
   - When we issue a stepping command (`step`, `next`), the program may switch between threads based on the operating system's thread scheduling.
   - Use this when we want the program to run as naturally as possible.
2. **`on`**
   - Only the current thread executes, while all other threads remain paused.
   - Use this to focus exclusively on a single thread without interference from other threads.
3. **`step`**
   - Similar to `on`, but with one key difference:
     When we issue stepping commands like `step` or `next`, only the current thread runs. However, if we use a continuation command like `continue`, all threads will resume execution.
   - This option strikes a balance between isolating a single thread for stepping and letting the program continue naturally when needed.

**Usage**

Switch to the thread we want to debug:

```c
thread 2
```

Set `scheduler-locking` to `step`:

```c
set scheduler-locking step
```

Debug with:

```c
// Step current thread
s
// Run all threads
c
```

## Thread | Bt

Backtrace for all threads:

```c
thread apply all bt 
```

Example:

```c
pwndbg> t apply all bt 

Thread 4 (Thread 0x7ffff6d8f6c0 (LWP 2988) "gdb_threads"):
#0  0x00007ffff7e76733 in __GI___clock_nanosleep (clock_id=clock_id@entry=0, flags=flags@entry=0, req=req@entry=0x7ffff6d8ee60, rem=rem@entry=0x7ffff6d8ee60) at ../sysdeps/unix/sysv/linux/clock_nanosleep.c:48
#1  0x00007ffff7e82827 in __GI___nanosleep (req=req@entry=0x7ffff6d8ee60, rem=rem@entry=0x7ffff6d8ee60) at ../sysdeps/unix/sysv/linux/nanosleep.c:25
#2  0x00007ffff7e94f41 in __sleep (seconds=0) at ../sysdeps/posix/sleep.c:55
#3  0x000055555555523d in worker_function (arg=0x7fffffffd7bc) at gdb_threads.c:21
#4  0x00007ffff7e2939d in start_thread (arg=<optimized out>) at pthread_create.c:447
#5  0x00007ffff7eae49c in __GI___clone3 () at ../sysdeps/unix/sysv/linux/x86_64/clone3.S:78

Thread 3 (Thread 0x7ffff75906c0 (LWP 2987) "gdb_threads"):
#0  0x00007ffff7e76733 in __GI___clock_nanosleep (clock_id=clock_id@entry=0, flags=flags@entry=0, req=req@entry=0x7ffff758fe60, rem=rem@entry=0x7ffff758fe60) at ../sysdeps/unix/sysv/linux/clock_nanosleep.c:48
#1  0x00007ffff7e82827 in __GI___nanosleep (req=req@entry=0x7ffff758fe60, rem=rem@entry=0x7ffff758fe60) at ../sysdeps/unix/sysv/linux/nanosleep.c:25
#2  0x00007ffff7e94f41 in __sleep (seconds=0) at ../sysdeps/posix/sleep.c:55
#3  0x000055555555523d in worker_function (arg=0x7fffffffd7b8) at gdb_threads.c:21
#4  0x00007ffff7e2939d in start_thread (arg=<optimized out>) at pthread_create.c:447
#5  0x00007ffff7eae49c in __GI___clone3 () at ../sysdeps/unix/sysv/linux/x86_64/clone3.S:78

Thread 2 (Thread 0x7ffff7d916c0 (LWP 2986) "gdb_threads"):
#0  0x00007ffff7e76733 in __GI___clock_nanosleep (clock_id=clock_id@entry=0, flags=flags@entry=0, req=req@entry=0x7ffff7d90e60, rem=rem@entry=0x7ffff7d90e60) at ../sysdeps/unix/sysv/linux/clock_nanosleep.c:48
#1  0x00007ffff7e82827 in __GI___nanosleep (req=req@entry=0x7ffff7d90e60, rem=rem@entry=0x7ffff7d90e60) at ../sysdeps/unix/sysv/linux/nanosleep.c:25
#2  0x00007ffff7e94f41 in __sleep (seconds=0) at ../sysdeps/posix/sleep.c:55
#3  0x000055555555523d in worker_function (arg=0x7fffffffd7b4) at gdb_threads.c:21
#4  0x00007ffff7e2939d in start_thread (arg=<optimized out>) at pthread_create.c:447
#5  0x00007ffff7eae49c in __GI___clone3 () at ../sysdeps/unix/sysv/linux/x86_64/clone3.S:78

Thread 1 (Thread 0x7ffff7d92740 (LWP 2982) "gdb_threads"):
#0  0x00007ffff7e25a19 in __futex_abstimed_wait_common64 (private=128, futex_word=0x7ffff7d91990, expected=2986, op=265, abstime=0x0, cancel=true) at futex-internal.c:57
#1  __futex_abstimed_wait_common (futex_word=futex_word@entry=0x7ffff7d91990, expected=2986, clockid=clockid@entry=0, abstime=abstime@entry=0x0, private=private@entry=128, cancel=cancel@entry=true) at futex-internal.c:87
#2  0x00007ffff7e25a9f in __GI___futex_abstimed_wait_cancelable64 (futex_word=futex_word@entry=0x7ffff7d91990, expected=<optimized out>, clockid=clockid@entry=0, abstime=abstime@entry=0x0, private=private@entry=128) at futex-internal.c:139
#3  0x00007ffff7e2b1a3 in __pthread_clockjoin_ex (threadid=140737351587520, thread_return=0x0, clockid=0, abstime=0x0, block=<optimized out>) at pthread_join_common.c:102
#4  0x0000555555555303 in main () at gdb_threads.c:44
#5  0x00007ffff7dbae08 in __libc_start_call_main (main=main@entry=0x55555555524e <main>, argc=argc@entry=1, argv=argv@entry=0x7fffffffd908) at ../sysdeps/nptl/libc_start_call_main.h:58
#6  0x00007ffff7dbaecc in __libc_start_main_impl (main=0x55555555524e <main>, argc=1, argv=0x7fffffffd908, init=<optimized out>, fini=<optimized out>, rtld_fini=<optimized out>, stack_end=0x7fffffffd8f8) at ../csu/libc-start.c:360
#7  0x00005555555550f5 in _start ()
```



# GDB | Processes

## Process | Switch

List all inferiors (processes GDB is debugging):

```c
info inferiors 
```

Switch between parent and child processes using:

```c
inferior <id>
```

## Process | Catch

Breakpoints set in the parent process before the `fork()` are copied into the child process. We can continue debugging both processes by switching between them.

- **`catch fork`**: Stops execution when a `fork()` is called.
- **`catch vfork`**: Stops execution when a `vfork()` is called.

```c
catch fork
// or
catch vfork
```

## Process | Detach-on-fork

### On|Off

**`set detach-on-fork on|off`**

- **`on`**: GDB detaches from the child process after a `fork()`. It continues to debug only the parent process (default behavior).
- **`off`**: GDB does not detach from the child process. Both parent and child processes remain under GDB control, and we can switch between them using `inferior` commands.

Example:

```c
set detach-on-fork off
```

### Parent | Child

**`set follow-fork-mode parent|child`**

- **`parent`**: GDB continues debugging the parent process after a `fork()`.
- **`child`**: GDB switches to debug the child process after a `fork()`.

Example:

```c
set follow-fork-mode child
```

### Debug Code

Code: [link](https://github.com/4xura/GDB_Debug_Skills/blob/main/Codes/gdb_processes.c)

Configures GDB to switch focus to the child process after `fork()`:

```c
pwndbg> set detach-on-fork off
pwndbg> set follow-fork-mode child
pwndbg> r
Starting program: /home/Axura/pwn/gdb_debug_skills/gdb_processes 
Downloading separate debug info for /lib64/ld-linux-x86-64.so.2
[Thread debugging using libthread_db enabled]                                                                        
Using host libthread_db library "/usr/lib/libthread_db.so.1".
Parent process: PID = 2948
[Attaching after Thread 0x7ffff7d92740 (LWP 2948) fork to child process 2952]
[New inferior 2 (process 2952)]
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib/libthread_db.so.1".
Child process: PID = 2952, Parent PID = 2948
Child process: global_var = 15
Process PID = 2952 exiting.
[Inferior 2 (process 2952) exited normally]
```

- The parent process starts (PID = 2948).
- GDB detects the `fork()` and attaches to the child process (PID = 2952) while retaining information about the parent.
- The child process executes its specific code:
  - Prints its `PID` and parent `PID`.
  - Updates the `global_var` to 15 and prints it.
- The child process exits normally, and GDB remains ready for further actions.

To debug the parent process as well, you can switch back to it using:

```c
pwndbg> inferior 1
[Switching to inferior 1 [process 2948] (/home/Axura/pwn/gdb_debug_skills/gdb_processes)]
[Switching to thread 1.1 (Thread 0x7ffff7d92740 (LWP 2948))]
```

Add `catch fork` to pause execution right before the `fork()`:

```c
pwndbg> catch fork
Catchpoint 1 (fork)
```

Use `info inferiors` to see all processes GDB is attached to:

```c
pwndbg> info inferiors 
  Num  Description       Connection           Executable        
* 1    process 2948      1 (native)           /home/Axura/pwn/gdb_debug_skills/gdb_processes 
  2    <null>                                 /home/Axura/pwn/gdb_debug_skills/gdb_processes
```



# GDB | Watch

Monitor value change on variable `myVar`:

```sh
watch myVar
watch (myVar > 10)
```

**Read watch:**

```c
rwatch <expression>
```

**Access watch (read/write)**:

```c
awatch <expression>
```

## Example

Code: [link](https://github.com/4xura/GDB_Debug_Skills/blob/main/Codes/gdb_watch.c)

Set a **read watchpoint** to break when `global_var` is accessed:

```c
pwndbg> rwatch global_var
Hardware read watchpoint 1: global_var
```

Set an **access watchpoint** to break when `global_var` is read or written:

```c
pwndbg> awatch global_var
Hardware access (read/write) watchpoint 2: global_var
```

Run and inspect breakpoints:

```c
pwndbg> r
pwndbg> i b
Num     Type            Disp Enb Address            What
1       read watchpoint keep y                      global_var
        breakpoint already hit 1 time
2       acc watchpoint  keep y                      global_var
        breakpoint already hit 1 time
```

View initial value of the global variable when the first breakpoints hit:

```c
pwndbg> p global_var
$1 = 0
```

Continue the program as breakpoints are hit, inspect the values of the global variable for example:

```c
pwndbg> i b
Num     Type            Disp Enb Address            What
1       read watchpoint keep y                      global_var
        breakpoint already hit 2 times
2       acc watchpoint  keep y                      global_var
        breakpoint already hit 2 times
pwndbg> p global_var
$7 = 10
```



# GDB | Docker

Install `gdbserver` on the docker container, and start it where the binary is running:

```sh
gdbserver 127.0.0.1:port ./pwn
```

Run the `gdb` command on the local machine:

```bash
gdb -q \
	-ex 'target remote 127.0.0.1:port'
	./pwn
```



# GDB | Assembly

Debugging inline Assembly.

## Example

Code: [link](https://github.com/4xura/GDB_Debug_Skills/blob/main/Codes/gdb_asm.c)

**Disassemble the function:**

```c
pwndbg> disassemble add_numbers
Dump of assembler code for function add_numbers:
   0x0000555555555139 <+0>:       push   rbp
   0x000055555555513a <+1>:       mov    rbp,rsp
   0x000055555555513d <+4>:       mov    DWORD PTR [rbp-0x14],edi
   0x0000555555555140 <+7>:       mov    DWORD PTR [rbp-0x18],esi
   0x0000555555555143 <+10>:      mov    edx,DWORD PTR [rbp-0x14]
   0x0000555555555146 <+13>:      mov    ecx,DWORD PTR [rbp-0x18]
   0x0000555555555149 <+16>:      mov    eax,edx
   0x000055555555514b <+18>:      add    eax,ecx
   0x000055555555514d <+20>:      mov    edx,eax
   0x000055555555514f <+22>:      mov    DWORD PTR [rbp-0x4],edx
   0x0000555555555152 <+25>:      mov    eax,DWORD PTR [rbp-0x4]
   0x0000555555555155 <+28>:      pop    rbp
   0x0000555555555156 <+29>:      ret
End of assembler dump.
```

**Switch to TUI mode for assembly view:**

```sh
layout asm
# To exit window: ctrl+x + a
```

**Set a breakpoint** in the function and **step through the assembly**:

```c
break add_numbers
run
stepi   # Step one assembly instruction
nexti   # Skip over function calls
```

**Observe register values** after each instruction:

```sh
info registers
# or in pwndbg
regs
```



# GDB | TUI

## TUI

The `layout` command in GDB is used to display different text-based layouts in the TUI (Text User Interface) mode. This mode enhances the debugging experience by showing various aspects of the program's state, such as source code, assembly, registers, or memory, alongside the GDB command line.

**Enter/Exit TUI Mode:**

```c
tui enable
tui disable
```

**Syntax:**

```c
layout <type>
```

**Switch Windows:**
Press `Ctrl-x` followed by `o` to cycle through different windows (source, assembly, registers, etc.).

While in TUI mode, press `Ctrl-x` followed by `1` to close all additional TUI windows and return to a single command-line window.

**Scroll Source Code:**
Use the arrow keys or `Ctrl-P` (up), `Ctrl-N` (down) to navigate the source code window.

**Scroll Assembly Instructions:**
Use the `Page Up` and `Page Down` keys or `Ctrl-U`/`Ctrl-D`.

## Layout

Displays the source code window. This is the default layout when entering TUI mode:

```c
layout src
```

Displays the disassembled instructions for the current function or address. Useful for debugging at the assembly level:

```c
layout asm
```

Displays both the source code and assembly windows side by side, allowing us to correlate source lines with assembly instructions:

```c
layout split
```

Displays a window with the current values of registers alongside the source code or assembly. This is especially helpful for low-level debugging:

```c
layout regs
```



# GDB | Core Dump

Debugging a **core dump** file is a way to analyze a program that has crashed. The core dump contains a snapshot of the program's memory and execution state at the moment of the crash. Using GDB, we can inspect this snapshot to identify the cause of the crash.

## Ulimit

By default, many Linux systems disable core dumps. We can enable them in the shell:

```sh
ulimit -c unlimited
```

This allows the system to generate core dump files when a program crashes.

We can make this setting permanent by adding it to our shell's configuration file (e.g., `.zshrc`) or by modifying `/etc/security/limits.conf`.

## Dump

Run the program as usual. If it crashes, a core dump file will be generated, typically named `core` or `core.<pid>`.

For example:

```sh
./myprogram
Segmentation fault (core dumped)
```

## Coredumpctl

Modern Linux systems use `core_pattern` to define where core dumps are stored. Check the current pattern with:

```sh
cat /proc/sys/kernel/core_pattern
```

**Check the Core Dump List** managed by `systemd-coredump`:

```sj
coredumpctl list
```

**View Core Dump Details**:

```
coredumpctl info <PID>
```

**Access or Save the Core Dump**:

```sh
coredumpctl dump <PID> > core
```

This saves the core dump to a file named `core` in the current directory.

## Debug

Run GDB, specifying both the executable and the core dump file:

```sh
gdb myprogram core
```

GDB will load the program and the core dump, showing information about the crash.

## Example

Code: [link](https://github.com/4xura/GDB_Debug_Skills/blob/main/Codes/gdb_crash.c)

**Crash with core dumped:**

```sh
ulimit -c unlimited
./gdb_crash
Program is about to crash...
[1]    8416 segmentation fault (core dumped)  ./gdb_crash
```

**List core dumps:**

```sh
$ coredumpctl list
TIME                           PID  UID  GID SIG     COREFILE EXE                                                   >
Thu 2024-11-14 23:01:05 PST   1739 1000 1000 SIGABRT missing  /usr/bin/dolphin                                      >
Thu 2024-11-14 23:02:49 PST   1929 1000 1000 SIGABRT missing  /usr/bin/dolphin                                      >
Fri 2024-11-15 18:26:06 PST   1022 1000 1000 SIGABRT missing  /usr/lib/kf6/baloo_file                               >
Sat 2024-11-16 00:15:10 PST   1174 1000 1000 SIGSEGV missing  /usr/bin/plasmashell                                  >
Sat 2024-11-16 00:29:45 PST   4287 1000 1000 SIGSEGV missing  /usr/bin/plasmashell                                  >
Sat 2024-11-16 00:39:10 PST   4929 1000 1000 SIGSEGV missing  /usr/bin/plasmashell                                  >
Sat 2024-11-16 01:09:53 PST   5284 1000 1000 SIGSEGV missing  /usr/bin/plasmashell                                  >
Sat 2024-11-16 17:45:41 PST   2707 1000 1000 SIGTRAP missing  /usr/lib/electron32/electron                          >
Sat 2024-11-16 17:49:58 PST   2975 1000 1000 SIGTRAP missing  /usr/lib/electron32/electron                          >
Sat 2024-11-16 17:50:07 PST   3462 1000 1000 SIGTRAP missing  /usr/lib/electron32/electron                          >
Sat 2024-11-16 17:50:19 PST   3648 1000 1000 SIGTRAP missing  /usr/lib/electron32/electron                          >
Sat 2024-11-16 17:50:37 PST   3829 1000 1000 SIGTRAP missing  /usr/lib/electron32/electron                          >
Sat 2024-11-16 18:49:36 PST  12106    0    0 SIGTRAP none     /home/Axura/hacktools/BloodHound-linux-x64/BloodHound >
Sat 2024-11-16 18:49:48 PST  12431    0    0 SIGTRAP none     /home/Axura/hacktools/BloodHound-linux-x64/BloodHound >
Sat 2024-11-16 18:58:50 PST   1026 1000 1000 SIGSEGV present  /usr/lib/kf6/baloo_file                               >
Sat 2024-11-16 23:21:16 PST   1036 1000 1000 SIGSEGV present  /usr/lib/kf6/baloo_file                               >
Fri 2024-11-22 00:20:41 PST 117392 1000 1000 SIGTRAP present  /usr/lib/electron32/electron                          >
Fri 2024-11-22 00:21:00 PST 117641 1000 1000 SIGTRAP present  /usr/lib/electron32/electron                          >
Sat 2024-11-23 20:26:56 PST  10388 1000 1000 SIGSEGV present  /usr/bin/wireshark                                    >
Sat 2024-11-30 18:48:05 PST   8416 1000 1000 SIGSEGV present  /home/Axura/pwn/gdb_debug_skills/gdb_crash
```

**View core dump detais:**

```sh
$ coredumpctl info 8416
           PID: 8416 (gdb_crash)
           UID: 1000 (Axura)
           GID: 1000 (Axura)
        Signal: 11 (SEGV)
     Timestamp: Sat 2024-11-30 18:48:05 PST (13min ago)
  Command Line: ./gdb_crash
    Executable: /home/Axura/pwn/gdb_debug_skills/gdb_crash
 Control Group: /user.slice/user-1000.slice/user@1000.service/app.slice/app-org.kde.konsole\x20\x281\x29@ea52d423206>
          Unit: user@1000.service
     User Unit: app-org.kde.konsole\x20\x281\x29@ea52d423206244b9aee4dd8c874f35e2.service
         Slice: user-1000.slice
     Owner UID: 1000 (Axura)
       Boot ID: 169ecd2ee4ee4f8aa9e8bfaff8ff244a
    Machine ID: 2a180b57a01b47d2b908871416ec7367
      Hostname: arLinuxA
       Storage: /var/lib/systemd/coredump/core.gdb_crash.1000.169ecd2ee4ee4f8aa9e8bfaff8ff244a.8416.1733021285000000>
  Size on Disk: 19.1K
       Message: Process 8416 (gdb_crash) of user 1000 dumped core.
                
                Stack trace of thread 8416:
                #0  0x000060111f49c149 n/a (/home/Axura/pwn/gdb_debug_skills/gdb_crash + 0x1149)
                #1  0x000060111f49c16f n/a (/home/Axura/pwn/gdb_debug_skills/gdb_crash + 0x116f)
                #2  0x00007a8e18618e08 n/a (libc.so.6 + 0x25e08)
                #3  0x00007a8e18618ecc __libc_start_main (libc.so.6 + 0x25ecc)
                #4  0x000060111f49c065 n/a (/home/Axura/pwn/gdb_debug_skills/gdb_crash + 0x1065)
                ELF object binary architecture: AMD x86-64
```

**Save core dump in current directory: **

```sh
$ coredumpctl dump 8416 > core
```

**Debug with core dump:**

```sh
gdb gdb_crash core
```

**Backtrace to see the function calls:**

```c
pwndbg> bt
#0  0x000060111f49c149 in faulty_function () at gdb_crash.c:6
#1  0x000060111f49c16f in main () at gdb_crash.c:11
#2  0x00007a8e18618e08 in __libc_start_call_main (main=main@entry=0x60111f49c152 <main>, argc=argc@entry=1, 
    argv=argv@entry=0x7fff1db153c8) at ../sysdeps/nptl/libc_start_call_main.h:58
#3  0x00007a8e18618ecc in __libc_start_main_impl (main=0x60111f49c152 <main>, argc=1, argv=0x7fff1db153c8, 
    init=<optimized out>, fini=<optimized out>, rtld_fini=<optimized out>, stack_end=0x7fff1db153b8)
    at ../csu/libc-start.c:360
#4  0x000060111f49c065 in _start ()
```

**Switch to the faulting frame:**

```c
pwndbg> frame 0
#0  0x000060111f49c149 in faulty_function () at gdb_crash.c:6
6           *ptr = 42; // This will cause a segmentation fault
```

**Check:**

```c
pwndbg> l
1       #include <stdio.h>
2       #include <stdlib.h>
3       
4       void faulty_function() {
5           int *ptr = NULL;
6           *ptr = 42; // This will cause a segmentation fault
7       }
8       
9       int main() {
10          printf("Program is about to crash...\n");
pwndbg> p ptr
$1 = (int *) 0x0
```

Or **Inspect local variables**:

```c
pwndbg> info locals
ptr = 0x0
```

This confirms the crash was caused by dereferencing a null pointer.



# GDB | Define

Define our own commands in GDB.

## Define | Hook-stop

Automatically run `bt` and `info registers` every time execution stops:

```c
define hook-stop
bt
info registers
end
```



# Compile | Gcc

Compile C code with specific LD and LIBC:

```bash
gcc -o kxheap kxheap.c -L/home/axura/pwn/kxheap/2.35 -Wl,--dynamic-linker=/home/axura/pwn/kxheap/2.35/ld-2.35.so -Wl,-rpath=/home/axura/pwn/kxheap/2.35 -lc -g
```








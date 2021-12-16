#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

int main() {

    printf("PID: %d by getpid()\n", getpid());
    printf("PID: %d by syscall(39)\n", (int)syscall(39));
   
    return 0;
}

// https://unix.stackexchange.com/questions/421750/where-do-you-find-the-syscall-table-for-linux

// x86_64 : https://github.com/torvalds/linux/blob/v4.17/arch/x86/entry/syscalls/syscall_64.tbl
// x86    : https://github.com/torvalds/linux/blob/v4.17/arch/x86/entry/syscalls/syscall_32.tbl
// arm    : https://github.com/torvalds/linux/blob/v4.17/arch/arm/tools/syscall.tbl

/*
some x86_64 syscalls

    0     common    read             __x64_sys_read
    1     common    write            __x64_sys_write
    2     common    open             __x64_sys_open
    3     common    close            __x64_sys_close
    4     common    stat             __x64_sys_newstat
    5     common    fstat            __x64_sys_newfstat
    6     common    lstat            __x64_sys_newlstat
    7     common    poll             __x64_sys_poll
    8     common    lseek            __x64_sys_lseek
    9     common    mmap             __x64_sys_mmap
    10    common    mprotect         __x64_sys_mprotect
    11    common    munmap           __x64_sys_munmap
    12    common    brk              __x64_sys_brk
    13    64        rt_sigaction     __x64_sys_rt_sigaction
    14    common    rt_sigprocmask   __x64_sys_rt_sigprocmask
    15    64        rt_sigreturn     __x64_sys_rt_sigreturn/ptregs
    16    64        ioctl            __x64_sys_ioctl
    17    common    pread64          __x64_sys_pread64
    18    common    pwrite64         __x64_sys_pwrite64
    19    64        readv            __x64_sys_readv
    20    64        writev           __x64_sys_writev
    21    common    access           __x64_sys_access
    22    common    pipe             __x64_sys_pipe
    23    common    select           __x64_sys_select
    24    common    sched_yield      __x64_sys_sched_yield
    25    common    mremap           __x64_sys_mremap
    26    common    msync            __x64_sys_msync
    27    common    mincore          __x64_sys_mincore
    28    common    madvise          __x64_sys_madvise
    29    common    shmget           __x64_sys_shmget
    30    common    shmat            __x64_sys_shmat
    31    common    shmctl           __x64_sys_shmctl
    32    common    dup              __x64_sys_dup
    33    common    dup2             __x64_sys_dup2
    34    common    pause            __x64_sys_pause
    35    common    nanosleep        __x64_sys_nanosleep
    36    common    getitimer        __x64_sys_getitimer
    37    common    alarm            __x64_sys_alarm
    38    common    setitimer        __x64_sys_setitimer
    39    common    getpid           __x64_sys_getpid
    40    common    sendfile         __x64_sys_sendfile64
    41    common    socket           __x64_sys_socket
    42    common    connect          __x64_sys_connect
    43    common    accept           __x64_sys_accept
    44    common    sendto           __x64_sys_sendto
    45    64        recvfrom         __x64_sys_recvfrom
    46    64        sendmsg          __x64_sys_sendmsg
    47    64        recvmsg          __x64_sys_recvmsg
    48    common    shutdown         __x64_sys_shutdown
    49    common    bind             __x64_sys_bind
    50    common    listen           __x64_sys_listen
    ...
*/

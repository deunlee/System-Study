## Man (Manual Pager)
`man (section) <name>`
- Section #1 : User command (executable programs or shell commands)
- Section #2 : System calls (functions provided by the kernel)
- Section #3 : Library calls (functions within program libraries)
- Section #4 : Special files (usually found in /dev)
- Section #5 : File formats and conventions eg /etc/passwd
- Section #6 : Games
- Section #7 : Miscellaneous (including macro packages and conventions),
- Section #8 : System administration commands (usually only for root)
- Section #9 : Kernel routines [Non standard]

```
$ man clear
clear(1)                   General Commands Manual                   clear(1)

NAME
       clear - clear the terminal screen

SYNOPSIS
       clear [-Ttype] [-V] [-x]

DESCRIPTION
       clear clears your screen if this is possible, including its scrollback
       buffer (if the extended "E3" capability is defined).  clear  looks  in
       the  environment  for the terminal type given by the environment vari‐
       able TERM, and then in the terminfo database to determine how to clear
       the screen.
...
```

```
$ man 1 passwd
PASSWD(1)                       User utilities                      PASSWD(1)

NAME
       passwd - update user's authentication tokens

SYNOPSIS
       passwd  [-k]  [-l]  [-u  [-f]] [-d] [-e] [-n mindays] [-x maxdays] [-w
       warndays] [-i inactivedays] [-S] [--stdin] [-?] [--usage] [username]

DESCRIPTION
       The passwd utility is used to update user's authentication token(s).
...
```

```
$ man 5 passwd
PASSWD(5)                 Linux Programmer's Manual                 PASSWD(5)

NAME
       passwd - password file

DESCRIPTION
       The /etc/passwd file is a text file that describes user login accounts
       for the system.  It should have read permission allowed for all  users
       (many  utilities, like ls(1) use it to map user IDs to usernames), but
       write access only for the superuser.
```

```
$ sudo mandb
$ man -f printf
printf (1)           - format and print data
printf (1p)          - write formatted output
printf (3)           - formatted output conversion
printf (3p)          - print formatted output
```

```
$ man -k printf
asprintf (3)         - print to allocated string
dprintf (3)          - formatted output conversion
dprintf (3p)         - print formatted output
tpm2_print (1)       - Prints TPM data structures
fprintf (3)          - formatted output conversion
fprintf (3p)         - print formatted output
fwprintf (3)         - formatted wide-character output conversion
fwprintf (3p)        - print formatted wide-character output
printf (1)           - format and print data
printf (1p)          - write formatted output
printf (3)           - formatted output conversion
printf (3p)          - print formatted output
snprintf (3)         - formatted output conversion
snprintf (3p)        - print formatted output
sprintf (3)          - formatted output conversion
sprintf (3p)         - print formatted output
swprintf (3)         - formatted wide-character output conversion
swprintf (3p)        - print formatted wide-character output
vasprintf (3)        - print to allocated string
vdprintf (3)         - formatted output conversion
vfprintf (3p)        - format output of a stdarg argument list
vfprintf (3)         - formatted output conversion
vfwprintf (3)        - formatted wide-character output conversion
vfwprintf (3p)       - wide-character formatted output of a stdarg argument list
vprintf (3)          - formatted output conversion
vprintf (3p)         - format the output of a stdarg argument list
vsnprintf (3)        - formatted output conversion
vsnprintf (3p)       - format output of a stdarg argument list
vsprintf (3)         - formatted output conversion
vswprintf (3)        - formatted wide-character output conversion
vswprintf (3p)       - wide-character formatted output of a stdarg argument list
vwprintf (3)         - formatted wide-character output conversion
vwprintf (3p)        - wide-character formatted output of a stdarg argument list
wprintf (3)          - formatted wide-character output conversion
wprintf (3p)         - print formatted wide-character output
```

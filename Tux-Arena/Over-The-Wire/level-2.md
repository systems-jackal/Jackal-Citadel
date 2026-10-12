## Level 2 - Spaces in filename.
**Goal**

Read contents of a file that has spaces in its filename `-- spaces in filename--`.
```Command Output
bandit2@bandit:~$ ls
--spaces in this filename--
bandit2@bandit:~$
```
 **Solution** 
 
The spaces in the file name makes the terminal interpret this as diffrent command arguements.

**Command Output**
```Command Output
bandit2@bandit:~$ cat --spaces in this filename--
error: unexpected argument '--spaces' found

  tip: to pass '--spaces' as a value, use '-- --spaces'

Usage: cat [OPTION]... [FILE]...

For more information, try '--help'.
bandit2@bandit:~$
```
This can be solved by wrapping the filename with quotes ("filename") also single quotes can be applied.

**Command Output**
```Command Output
A combination of the path prefix `./` and wrapping it in quotes

bandit2@bandit:~$ cat ./'--spaces in this filename--'
7ZZ2LFrykP2zEyvBl4m3clcL7tGYJPME
bandit2@bandit:~$

```

Password to the next level : `7ZZ2LFrykP2zEyvBl4m3clcL7tGYJPME`.

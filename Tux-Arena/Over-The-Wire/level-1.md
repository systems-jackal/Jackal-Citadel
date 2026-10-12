## Level 1 -Dashed filename
**Goal** : Navigation to a file named `-` in the home directory.

**Commands** 

The command needed to perfom the task are `ls , cd , cat , file , du , find`.
For this level, a combination of various commands lead to locating the file.

**Solution**

It is noted that the file name is a `dashed filename`.
This is is trcky since commands like `cat,rm and mv` treat like an invitation to execute a command rather than a file name.
It is read by prefixing it with a relative path.The prefix will be like `./` as its path method.

**Command Output**

```Command Output
bandit1@bandit:~$ ls
-
bandit1@bandit:~$
bandit1@bandit:~$ cat -
^C
bandit1@bandit:~$ cat ./-
PK8fYLZg2hnHSz83plBL1iEPKdD3QToB
bandit1@bandit:~$
```

Password for the next level : `PK8fYLZg2hnHSz83plBL1iEPKdD3QToB`

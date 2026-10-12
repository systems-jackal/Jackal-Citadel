# Level 0
**Goal** : Login to the game using ssh and also understanding the requirement in oredr to make the connection successfull.
In this case the ssh login credentials were `bandit0` as the username and   `bandit.labs.overthewire.org` as the host address.
It is noted that the connection port is specified to port 2220.

**Command** 
ssh -p 2220 `bandit0@bandit.labs.overthewire.org` where the `-p` is used to point to the port specified.

**Outcome** 
`ssh` is a protocol that ssecurely connects hosts that are remotely connected by exchanging cryptographic keys and tunneling the communication to be secure

**Task** 
The level Requires on to accesss the terminal of the environment and access a README file in the home directory.It is in this file that the password to the next level is located.Specific commands need to be used in oredr to read the content of the file.They include `ls , cd , cat , file , du , find`

**Command Output**
```Command Output
bandit0@bandit:~$ ls
readme
bandit0@bandit:~$ find . -type f -name "*md"
bandit0@bandit:~$ find . -type f -name "readme"
./readme
bandit0@bandit:~$ cat readme
Congratulations on your first steps into the bandit game!!
Please make sure you have read the rules at https://overthewire.org/rules/
If you are following a course, workshop, walkthrough or other educational activity,
please inform the instructor about the rules as well and encourage them to
contribute to the OverTheWire community so we can keep these games free!

The password you are looking for is: 6y2kwnwK6grgvwvpvLaa2T1cpFEKOhNR
```

 Password to this level: `bandit0`.
 
 Password to the next level: `6y2kwnwK6grgvwvpvLaa2T1cpFEKOhNR`

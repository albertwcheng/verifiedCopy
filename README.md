# verifiedCopy
Verify-as-it-copies file copying utility

to make
```
g++ -O3 -o vcp main.cpp
g++ -O3 -o vcp2 main2.cpp
```
then move to one directories in your $PATH

Copy each 1024-byte blocks of data from src to dst (full file name), and checking the just copied block. Try again if copied data do not match original, up to 3 attempts.
```
vcp src dst
```


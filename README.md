# verifiedCopy
Verify-as-it-copies file copying utility

to make
```
g++ -O3 -o vcp main.cpp
g++ -O3 -o vcp2 main2.cpp
```
then, optionally, move to one directories in your $PATH

Copy each 1024-byte blocks of data from src to dst (full file name), and checking the just copied block. Try again if copied data do not match original, up to 3 attempts.
```
vcp src dst
```
or
```
vcp2 copy src dst
```

Verify first numBytesToCheck bytes between file1 and file2, and print verified if same, notverified if different.
```
vcp2 verify src dst numBytesToCheck
```

Verify first numBytesToCheck bytes between file1 and file2, if not the same, do a verified copy of src to dst.
```
vcp2 copyIfNotVerified src dst numBytesToCheck
```


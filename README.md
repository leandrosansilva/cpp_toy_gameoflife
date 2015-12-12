# Conway's Game of Life in C++

Yep, so I decided to implement it in C++ as well, but my focus here
wasn't in the algorithm itself (which is terrible and slow), but on using 
a modern unit test framework (<a href="http://banditcpp.org/">Bandit</a>)
and learn a little bit more about test support in CMake and suport for
submodules in Git.

The day is beautiful outside but yeah, I decided to stay all the day
long in my room. Not the best Saturday ever, but I'm still alive at least :-)

Environment: Debian testing 64-bit with GCC 5.2 and Clang 3.7.

IDE? Vim, of course!

Usage:
```
$ mkdir build
$ cd build
$ cmake ../
$ make test
```

Or use the "pretty" Bandit output, inside the build directory:
```
./gof_tests --reporter=spec
```

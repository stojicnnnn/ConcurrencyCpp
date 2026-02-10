# Concurrency Test

Build and run:
```bash
mkdir build
cd build
cmake ..
make
setarch `uname -m` -R ./src/assignment1_test
```

# Vector Test

Compile and run:
```bash
g++ -std=c++11 test.cpp -o test && ./test
```
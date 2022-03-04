# Assignment 2: A Little Slice of Pi
Assignment 2 is a math library which approximates euler's number, square root, and pi using various formulas. The purpose of this assignment was to understand how the C __math.h__ library and ultimately all libraries work. 

## Build and run
Use make all to compile and link the library files with the test harness __mathlib-test__.  This executable can be executed with the following commands:

- -a: run all tests
- -e: runs e approximation test
- -b: runs Bailey-Borwein-Plouffe pi approximation test
- -m: runs Madhava pi approximation test
- -r: runs Euler sequence pi approximation test
- -v: runs Viete pi approximation test
- -n: runs Newton-Raphson square root approximation tests
- -s: enable printing of statistics to see computed terms/factors for each tested function
- -h: display a help message detailing program usage

Each test compares the value computed with the assignment __mathlib.h__ library vs the __math.h__ library.

## Resources Used
In this assignment I referenced Eugene's recorded lab section on Yuja. I also implemented the C functionality of Professor Long's pseudocode.

## Scan
"scan-build make" produces no errors.

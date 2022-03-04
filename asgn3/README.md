# Assignment 3: Sorting
Assignment 3's challenge was to implement Heap Sort, Insert Sort, Shell Sort, and Quick Sort, then link them together with a test harness **sorting.c**. The purpose of this assignment was to understand and compare the most used sorting algorithms.

## Build and Run
Any of the following commands will build the program: **make**, **make all**, **make sorting**. Any of these commands will produce one executable **sorting** that can be run as such: ./sorting \[-haeisgn:p:r:\] \[-n length\] \[-p elements\] \[-r seed\]

## Command Line Options
* -a Enables all sorting algorithms
* -e Enables Heap Sort
* -i Enables Insertion Sort
* -s Enables Shell Sort
* -q Enables Quick Sort
* -r seed: Specifies random seed (default: 13371453)
* -n size: Specifies number of array elements (default: 100)
* -p elements: Specifies number of elements to print (default: 100)
* -h Prints out program usage

## Scan
scan-build make produces no errors or warnings.

## Resources used
I referenced Eugene's recorded lab section on Yuja. I also used Professor Long's python pseudocode in the assignment 3 pdf as a base for my C programs.

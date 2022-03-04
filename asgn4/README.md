# Assignment 4: Perambulations of Denver Long
Assignment 4's task was to solve the traveling salesman problem, but for Hamiltonian Paths(each vertex must be visited once and must end at the origin vertex). This assignment went in depth with stacks and levels of abstraction using structs.

## Build and Run 
Any of the following commands will build the program: **make**, **make all**, **make tsp**. Any of these commands will produce one executable **tsp** that can be run as such: ./tsp \[-u\] \[-v\] \[-h\] \[-i infile\] \[-o outfile\]

## Command Line Options
* -h:             Program usage and help.
* -i infile:      Input containing graph (default: stdin).
* -o outfile:     Output of computed path (default: stdout).
* -u:             Use undirected graph.
* -v:             Enable verbose printing.

## Scan
scan-build make produces no errors or warnings.

## Resources used
I made use of sessions and got help from: Eugene, Christian, and Omar. 

# Assignment 7: The Great Firewall of Santa Cruz
In assignment 7 a censorship program was implemented. This program takes in a text document and checks to make sure the author is not guilty of a thoughtcrime. If the author is guilty, they are given a list of the *badspeak* and the proper new translations if applicable.

## Build
The commands **make** or **make banhammer** build the **banhammer** executable. The command **make clean** deletes the **banhammer** executable and object files.

## Run banhammer
Execute the program with any or none of the following options: ./banhammer \[hs\] \[-t size\] \[-f size\]
* -h              Display program help and usage.
* -s              Display program statistics.
* -t size         Specify hash table size (default: 2^16).
* -f size         Specify bloom filter size (default: 2^20).

## Scan
**scan build make** produces no errors or warnings.

## Resources Used
I utilized both of Eugene's sections.


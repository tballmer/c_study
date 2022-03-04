# Assignment 5: Huffman Coding
Assignment 5 includes 2 executables. The first, **encode**, huffman encodes a given file. The second, **decode**, decodes a Huffman encoded file. The Huffman coding technique implemented is one of the most efficient lossless compressions algorithms ever created. 

## Build
The commands **make** or **make all** make both executables. The command **make encode** builds only **encode**. The command **make decode** builds only **decode**. And **make clean** cleans executables and object files.

## Run encode
Encode a file through the options: ./encode \[-i infile\] \[-outfile\] \[-h\] \[-v\]
* -h: Program usage and help.
* -v: Prints compression statistics.
* -i infile: Input file to compress.
* -o outfile: Output of compressed data.

## Run decode
Decode a file through the options: ./decode \[-i infile\] \[-outfile\] \[-h\] \[-v\]
* -h: Program usage and help.
* -v: Prints compression statistics.
* -i infile: Input file to decompress.
* -o outfile: Output of compressed data.

## Scan
scan-build make produces no errors or warnings.

## Resources used
I made use of the recorded sections of Omar, Eugene, and Christian. 

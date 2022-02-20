# Hamming Codes
Description:
	I will be implementing hamming codes, encoding and decoding them. It used to counteract noisy interference, because transferring data through a noisy communication channel is prone to errors. This is done by adding extra information to our data which allows us to perform error checking, and request that the sender retransmit any data that was incorrect. In addition, not only detect eros but we can also correct them. This technique is called forward error correction (FEC).
## How To Run Program
```
make
```
- Used to build all programs
### Encode
```
./encode [-flag]
```
- Used to run encode file using hamming code.
```
-h: prints out help message.
-i infile: Specify the input file path containing data to encode into Hamming codes. The default input should be set as stdin.
-o outfile: Specify the output file path to write the encoded data (the Hamming codes) to. If not specified, the default output should be set as stdout.
```
- Available flags.
### Decode
```
./decode [-flag]
```
- Used to run decode file encoded using hamming code.
```
-h: prints out help message.
-i infile: Specify the input file path containing data to encode into Hamming codes. The default input should be set as stdin.
-o outfile: Specify the output file path to write the encoded data (the Hamming codes) to. If not specified, the default output should be set as stdout.
-v verbose: Prints statistics of the decoding process to stderr. The statistics to print are the total bytes processed, uncorrected errors, corrected errors, and the error rate. The error rate is defined as (uncorrected errors/total bytes processed), the ratio of uncorrected errors to total bytes processed.
```
- Available flags.
---

## How to use Makefile
```
make
**or**
make all
```
- Used to build Program
```
make clean
```
- Used to removes all files that compiler generated.
```
make format
```
- Format all source code, including the header files.
```
make scan-build
```
- Tests binary file for any missed errors
```
make [flag]
```
- Used to build specific files.
```
encode: Builds just the encoder.
decode: Builds just the decoder.
error: Builds just the supplied error-injection program.
entropy: Builds just the supplied entropy-measure program.
```
- Flags used to only build specific programs.


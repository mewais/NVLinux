# Non Volatile Memory Linux
A modified Linux kernel based on the old Linux 2.6. This kernel was modified as part of the research work that resulted in the following publication: [A virtual memory architecture to enhance STT-RAM performance as main memory](https://ieeexplore.ieee.org/document/7726657). It is only meant to run on a modified gem5 simulator with STT-RAM main memories.

## Building
Unfortunately, compiling this with anything gcc5 or higher will fail, so you can use gcc-4.8 and build using `make vmlinux`

## Simulator
When we worked on this, 5 years ago, we had modified gem5 ourselves to use the NVMain simulator for STT-RAMs. However, tons of repositories provide the same capability now, any of which should suffice.

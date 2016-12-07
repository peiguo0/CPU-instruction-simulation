# CPU-instruction-simulation
- *imem.txt* file which is used to initialize the Instruction Memory. Each line of the file corresponds to a Byte stored in the Instruction Memory in binary format, with the first line at address 0, the next line at address 1 and so on. Four contiguous lines correspond to a whole instruction. The words stored in memory are in “Big-Endian” format.
- We have defined a “halt” instruction as 32’b1 (0xFFFFFFFF) which is the last instruction in every *imem.txt* file. As the name suggests, when this instruction is fetched, the simulation is terminated.
- The Data Memory is initialized using the *dmem.txt* file. The format of the stored words is the same as the Instruction Memory. As with the instruction memory, the data memory addresses also begin at 0 and increment by one in each line. 
- The instructions that the simulator supports are *addu, subu, addiu, and, or, nor, beq, k, lw, sw, halt.*
- The link of Green sheet:[https://inst.eecs.berkeley.edu/\~cs61c/resources/MIPS\_Green\_Sheet.pdf][1]

[1]:	https://inst.eecs.berkeley.edu/~cs61c/resources/MIPS_Green_Sheet.pdf

# How to run  
- Make sure *MIPS.cpp, imem.txt, dmem.txt and Makefile.txt* are under same folder.  
- execute: **g++ MIPS.cpp** to compile.
- execure: **./a.out** to run.
- Then *RFresult.txt* and *dmemresult.txt* are generated under the same folder.  
- execute: **vi RFresult.txt** to view the result of register file.
- execute: **vi dmemresult.txt** to view the result of data memory.  
- Delete these two files every time before re-run the code. Or the result would be added to the end of the original file.
  

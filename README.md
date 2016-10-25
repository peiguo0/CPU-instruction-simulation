# CPU-instruction-simulation
- *imem.txt* file which is used to initialize the Instruction Memory. Each line of the file corresponds to a Byte stored in the Instruction Memory in binary format, with the first line at address 0, the next line at address 1 and so on. Four contiguous lines correspond to a whole instruction. The words stored in memory are in “Big-Endian” format.
- We have defined a “halt” instruction as 32’b1 (0xFFFFFFFF) which is the last instruction in every *imem.txt* file. As the name suggests, when this instruction is fetched, the simulation is terminated.
- The Data Memory is initialized using the *dmem.txt* file. The format of the stored words is the same as the Instruction Memory. As with the instruction memory, the data memory addresses also begin at 0 and increment by one in each line. 
- The instructions that the simulator supports are *addu, subu, addiu, and, or, nor, beq, k, lw, sw, halt.*
<<<<<<< HEAD
- The link of Green sheet:[https://inst.eecs.berkeley.edu/\~cs61c/resources/MIPS\_Green\_Sheet.pdf]

=======
- The link of Green sheet:\[https://inst.eecs.berkeley.edu/\~cs61c/resources/MIPS\_Green\_Sheet.pdf]
> > > > > > > origin/master

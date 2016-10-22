#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
#include<math.h>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 //行 memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


class RF
{
public:
    bitset<32> ReadData1, ReadData2;
    RF()
    {
        Registers.resize(32);
        Registers[0] = bitset<32> (0);
    }
    
    void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
    {
        unsigned long int ad1;
        unsigned long int ad2;
        unsigned long int adwr;

        ad1 = RdReg1.to_ulong();
        ad2 = RdReg2.to_ulong();
        adwr = WrtReg.to_ulong();
        
        if(WrtEnable==1 and adwr != 0){
            Registers[adwr]=WrtData;
        }
        
        ReadData1=Registers[ad1];
        ReadData2=Registers[ad2];
        
        //done. implement the funciton by you.
    }
    
    void OutputRF()
    {
        ofstream rfout;
        rfout.open("RFresult.txt");
        if (rfout.is_open())
        {
            rfout<<"A state of RF:"<<endl;
            for (int j = 0; j<32; j++)
            {
                rfout << Registers[j]<<endl;
            }
            
        }
        else cout<<"Unable to open file";
        rfout.close();
        
    }
private:
    vector<bitset<32> >Registers;
    
};

class ALU
{
public:
    bitset<32> ALUresult;
    bitset<32> o1;
    bitset<32> o2;
    unsigned long int op1;
    unsigned long int op2;
    int index;
    
    bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
    {

        o1=oprand1;
        o2=oprand2;
        o1.set(31,0);                    //convert to it's real value
        o2.set(31,0);
        op1 = o1.to_ulong();
        op2 = o2.to_ulong();
        bitset<32> temResult;            //convert int to bitset<32>
        
        
        if(ALUOP==ADDU){
            
            if(oprand1[31]==1){
                if(oprand2[31]==1){
                    temResult = (-pow(2,31)+op1-pow(2,32) +op2);            //convert int to bitset<32>
                    
                }
                if(oprand2[31]==0){
                    temResult = (-pow(2,31)+op1 +op2);            //convert int to bitset<32>
                    
                                    }
            }
            if(oprand1[31]==0){
                if(oprand2[31]==0){
                    temResult = (op1 + op2);            //convert int to bitset<32>
                }
                if(oprand2[31]==1){
                    temResult = (op1 -pow(2,31)+op2);            //convert int to bitset<32>
                }
            }

            ALUresult = temResult;
        }
        if(ALUOP==SUBU){
            bitset<32> temResult;
            if(oprand1[31]==1){
                if(oprand2[31]==1){
                    temResult = (-pow(2,31)+op1 +pow(2,31)-op2);            //convert int to bitset<32>
                }
                if(oprand2[31]==0){
                    temResult = (-pow(2,31)+op1 - op2);            //convert int to bitset<32>
                }
            }
            if(oprand1[31]==0){
                if(oprand2[31]==0){
                    temResult = (op1 - op2);            //convert int to bitset<32>
                }
                if(oprand2[31]==1){
                    temResult = (op1 +pow(2,31)-op2);            //convert int to bitset<32>
                }
            }
            ALUresult = temResult;
        }
        if(ALUOP==AND){
            op1 = oprand1.to_ulong();                   //logic calculate
            op2 = oprand2.to_ulong();

            index=0;
            while(index != 32){
                ALUresult.set(index,oprand1[index] and oprand2[index]); //& every bit
                index+=1;
            }
        }
        if(ALUOP==OR){
            op1 = oprand1.to_ulong();                   //logic calculate
            op2 = oprand2.to_ulong();

            index=0;
            while(index != 32){
                ALUresult.set(index,oprand1[index] or oprand2[index]);  //or every bit
                index+=1;
            }
        }
        if(ALUOP==NOR){
            op1 = oprand1.to_ulong();                   //logic calculate
            op2 = oprand2.to_ulong();

            index=0;
            while(index!=32){
                ALUresult.set(index,not oprand1[index] and not oprand2[index]); //nor every bit
                index+=1;
            }
            
        }
        //done. implement the ALU operations by you.
        return ALUresult;
    }
};

class INSMem
{
public:
    bitset<32> Instruction;
    INSMem()
    {
        IMem.resize(MemSize);             //why that large size? 8bits addressable memory?
        ifstream imem;
        string line;
        int i=0;
        imem.open("imem.txt");
        if (imem.is_open())
        {
            while (getline(imem,line))
            {
                IMem[i] = bitset<8>(line);
                //IMem[i] = stoul(line, NULL, 2);
                i++;
            }
        }
        else cout<<"Unable to open file1";
        imem.close();
        
    }
    
    bitset<32> ReadMemory (bitset<32> ReadAddress)
    {
        unsigned long int ad;
        int count;
        int index;
        bitset<8> tem;
        
        ad = ReadAddress.to_ulong();        //i is int, so address should be convert to int
        count=0;                            //each time 8 bits, need 4 times to read a instruction
        
        while (count != 3){
            
            tem=IMem[ad];
            //std::cout<<tem;
            //std::cout<<"\n";
            
            index=0;                       //reset index to 0
            while(index != 8){             //use or to add four IMem together to form Instruction
                Instruction.set(index,tem[index]);
                index+=1;
            }
            //std::cout<<"i\n";
            //std::cout<<Instruction;
            //std::cout<<"i\n";
            Instruction=Instruction<<8;     //move left 8 bits
            count+=1;
            ad+=1;                          //turn to next line (8bits)
        }
        
        index=0;
        tem=IMem[ad];
        while(index != 8){             //use or to add four IMem together to form Instruction
            Instruction.set(index,tem[index]);
            index+=1;
        }
        
        

        //std::cout<<"thisi\n";
        //done. implement by you. (Read the byte at the ReadAddress and the following three byte).
        return Instruction;
    }
    
private:
    vector<bitset<8> > IMem;             //each IMem[i] contains 8 bits content
    
};

class DataMem
{
public:
    bitset<32> readdata;
    DataMem()
    {
        DMem.resize(MemSize);
        ifstream dmem;
        string line;
        int i=0;
        dmem.open("dmem.txt");
        if (dmem.is_open())
        {
            while (getline(dmem,line))
            {
                DMem[i] = bitset<8>(line);
                //DMem[i] = stoul(line, NULL, 2);
                i++;
            }
        }
        else cout<<"Unable to open file2";
        dmem.close();
        
    }
    bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
    {
        unsigned long int ad;
        int count;
        int index;
        bitset<8> tem;
        
        ad = Address.to_ulong();
        
        
        if(writemem==1){                            //write data to DMem[ad]~DMem[ad+3]
            
            index=31;                                   //set index as the MSB
            while(index != 23){
                tem.set(index-24,WriteData[index]);
                index-=1;
            }
            DMem[ad]=tem;
            while(index != 15){
                tem.set(index-16,WriteData[index]);
                index-=1;
            }
            DMem[ad+1]=tem;
            while(index != 7){
                tem.set(index-8,WriteData[index]);
                index-=1;
            }
            DMem[ad+2]=tem;
            while(index != -1){
                tem.set(index,WriteData[index]);
                index-=1;
            }
            DMem[ad+3]=tem;
        }
        
        
        count=0;
        while(count != 3){                      //fetch the readdata<32>
            tem=DMem[ad];
            index=0;
            while(index != 8){
                readdata.set(index,tem[index]);
                index+=1;
            }
            readdata=readdata<<8;
            count+=1;
            ad+=1;
        }
        
        index=0;
        tem=DMem[ad];
        while(index != 8){
            readdata.set(index,tem[index]);
            index+=1;
        }

        
        
        //done except readset<1>. implement by you.
        return readdata;
    }
    
    void OutputDataMem(){
        ofstream dmemout;
        dmemout.open("dmemresult.txt");
        if (dmemout.is_open())
        {
            for (int j = 0; j< 1000; j++)
            {
                dmemout << DMem[j]<<endl;
            }
        }
        else cout<<"Unable to open file";
        dmemout.close();
        
    }
    
private:
    vector<bitset<8> > DMem;
    
};



int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;
    
    bitset<32> PC;
    PC.reset();
    bitset<32> ins;             //instruction
    bitset<6> opcode;
    bitset<6> func;             //function of R type
    bitset<5> RsAddress;
    bitset<5> RtAddress;
    bitset<5> RdAddress;
    bitset<16> immediate;
    
    
    int index;
    
    
    while (1)
    {
        ins = myInsMem.ReadMemory(PC);
        // Fetch
        //std::cout<< ins <<endl;
        if(ins==0b11111111111111111111111111111111){
            break;
        }
        // If current insturciton is4294967295 "11111111111111111111111111111111", then break;
        
        index=0;                                //initialize index to 0
        while(index != 6){                      //fetch the opcode out
            opcode.set(index,ins[26+index]);       //set every bit of opcode with it's value
            index+=1;
        }
        
        if(opcode==0){
            //R type instructions
            index=0;
            while(index != 6){                 //fetch the function code out
                func.set(index,ins[index]);
                index+=1;
            }
            
            index=11;                       //the 1st bit of Rd
            while(index != 16){             //fetch RdAddress
                RdAddress.set(index-11,ins[index]);
                index+=1;
            }
            while(index != 21){             //fetch RtAddress
                RtAddress.set(index-16,ins[index]);
                index+=1;
            }
            while(index != 26){             //fetch RsAddress
                RsAddress.set(index-21,ins[index]);
                index+=1;
            }
            
            
            if(func==33){
                bitset<3> aluop(ADDU);
                //  R[rd] = R[rs] + R[rt]
                myRF.ReadWrite(RsAddress,RtAddress,0,0,0);
                //std::cout<<"\nD1";
                //std::cout<<myRF.ReadData1;
                
                //std::cout<<"\nD2";
                //std::cout<<myRF.ReadData2;
                //std::cout<<"\nD2";
                
                bitset<32> usum = myALU.ALUOperation(aluop, myRF.ReadData1, myRF.ReadData2);
                myRF.ReadWrite(0,0,RdAddress,usum,1);
                
                //std::cout<<"sum";
                //std::cout<<usum;
                

                //func=21  addu
            }
            if(func==35){
                bitset<3> aluop(SUBU);
                //  R[rd] = R[rs] - R[rt]
                myRF.ReadWrite(RsAddress,RtAddress,0,0,0);
                bitset<32> usub = myALU.ALUOperation(aluop, myRF.ReadData1, myRF.ReadData2);
                myRF.ReadWrite(0,0,RdAddress,usub,1);
                //func=23  subu
                
            }
            if(func==36){
                bitset<3> aluop(AND);
                // R[rd] = R[rs] & R[rt]
                myRF.ReadWrite(RsAddress,RtAddress,0,0,0);
                bitset<32> reand = myALU.ALUOperation(aluop, myRF.ReadData1, myRF.ReadData2);
                myRF.ReadWrite(0,0,RdAddress,reand,1);
                
                //func=24  and
            }
            if(func==37){
                bitset<3> aluop(OR);
                // R[rd] = R[rs] | R[rt]
                myRF.ReadWrite(RsAddress,RtAddress,0,0,0);
                bitset<32> reor = myALU.ALUOperation(aluop, myRF.ReadData1, myRF.ReadData2);
                myRF.ReadWrite(0,0,RdAddress,reor,1);
                
                //func=25  or
            }
            if(func==39){
                // R[rd] = ~ (R[rs] | R[rt])
                bitset<3> aluop(NOR);
                myRF.ReadWrite(RsAddress,RtAddress,0,0,0);
                bitset<32> renor = myALU.ALUOperation(aluop, myRF.ReadData1, myRF.ReadData2);
                myRF.ReadWrite(0,0,RdAddress,renor,1);
                
                //func=27  nor
            }
        bitset<32> pctem=PC.to_ulong() + 4 ;
        PC = pctem;
        
        }
        
        
        if(opcode==9 or opcode==4 or opcode==35 or opcode==43 or opcode==2){
            //I type
            index=0;
            while(index != 16){
                immediate.set(index,ins[index]);        //fetch immediate
                index+=1;
            }
            
            while(index != 21){
                RtAddress.set(index-16,ins[index]);     //fetch RtAddress
                index+=1;
            }
            while(index != 26){
                RsAddress.set(index-21,ins[index]);     //fetch RsAddress
                index+=1;
            }
            
            
            if(opcode==9)
            {
                bitset<3> aluop(ADDU);
                bitset<32> imm(immediate.to_ulong());
                // R[rt] = R[rs] + SignExtImm
                myRF.ReadWrite(RsAddress,0,0,0,0);
                
                index=16;                               //signExtend
                while(index != 32){
                    imm[index]=immediate[15];
                    index+=1;
                }

                //if(immediate[15]==1){
                //    immediate.set(15,0);                //set sign bit as 0 after reading
                //   imm=immediate.to_ulong();
                //   imm.set(31,1);
                //}
                //else{
                //    imm=immediate.to_ulong();
                //}
                
                bitset<32> Raddiu = myALU.ALUOperation(aluop, myRF.ReadData1, imm); //calculate

                myRF.ReadWrite(0,0,RtAddress,Raddiu,1);
                
                bitset<32> pctem=PC.to_ulong() + 4 ;        //PC+4
                PC = pctem;
                //09  I type - addiu
            }
            
            
            if(opcode==4){
                //  if(R[rs]==R[rt])  PC=PC+4+BranchAddr
                bitset<3> aluop(ADDU);
                myRF.ReadWrite(RsAddress,RtAddress,0,0,0);
                if(myRF.ReadData1 == myRF.ReadData2){
                    
                bitset<32> BranchInit;
                  
                    
                    index=18;                           //signExtend
                    while(index != 32){
                        BranchInit[index]=immediate[15];
                        index+=1;
                    }

                    //if(immediate[15]==1){                   //if immediate is negative
                    //    BranchInit.set(31,1);
                    //    immediate.set(15,0);
                    //}
                    
                    for(int i=0;i<16;i++){
                        BranchInit[i+2]=immediate[i];
                    }
                    
                    //for(int i=18;i<32;i++){
                    //    BranchInit[i]=immediate[15];
                    //}
                    
                    PC=myALU.ALUOperation(aluop, PC, BranchInit);   //calculate PC+BranchInit
                    
                    bitset<32> pctem=PC.to_ulong() + 4;
                    
                    PC = pctem;
                }
                //04  I type - beq
            }
            if(opcode==35){
                bitset<3> aluop(ADDU);
                //  R[rt] = M[R[rs]+SignExtImm]
                myRF.ReadWrite(RsAddress,0,0,0,0);
                bitset<32> readmem;                     //Data from data memory
                bitset<32> readreg (myRF.ReadData1) ;
                
                bitset<32> ReadMemAd;
                //bitset<32> ReadMemAd(readreg.to_ulong()+immediate.to_ulong());
                
                bitset<32> signimmi(immediate.to_ulong());
                
                index=16;                           //signExtend
                while(index != 32){
                    signimmi[index]=immediate[15];
                    index+=1;
                }
                
                //if(immediate[15]==1){                       //if imme is negative
                //    signimmi.set(31,1);                     //set MSB as 1
                //    signimmi.set(15,0);
                //}
                
                ReadMemAd=myALU.ALUOperation(aluop, readreg, signimmi);   //calculate
                
                readmem = myDataMem.MemoryAccess(ReadMemAd,0,1,0);
                myRF.ReadWrite(0,0,RtAddress,readmem,1);
                
                bitset<32> pctem=PC.to_ulong() + 4 ;        //PC+4
                PC = pctem;
                
                //23  I type - lw
            }
            
            
            if(opcode==43){
                bitset<3> aluop(ADDU);
                // M[R[rs]+SignExtImm] = R[rt]
                myRF.ReadWrite(RsAddress,RtAddress,0,0,0);
                bitset<32> signimmi(immediate.to_ulong());
                bitset<32> readmemad;
                
                index=16;                           //signExtend
                while(index != 32){
                    signimmi[index]=immediate[15];
                    index+=1;
                }
                
                //if(immediate[15]==1){
                //    signimmi.set(15,0);
                //    signimmi.set(31,1);
                //}
                
                readmemad=myALU.ALUOperation(aluop, myRF.ReadData1, signimmi);
                
                myDataMem.MemoryAccess(readmemad,myRF.ReadData2,0,1);       //Write
                
                bitset<32> pctem=PC.to_ulong() + 4 ;        //PC+4
                PC = pctem;
                //2B  I type - sw
            }
        }
        
        if(opcode==2){
            //02  J type - jump
            bitset<32> pctem=PC.to_ulong()+4;           //PC+4
            PC = pctem;
            
            index=0;
            while(index != 26){                         //(PC+4)[31:28],address,
                PC.set(index+2, ins[index]);
                index+=1;
            }
            PC[0]=0;                                    //(PC+4)[31:28],address,00
            PC[1]=0;
        }
        
        // decode(Read RF)
        
        // Execute
        
        // Read/Write Mem
        
        // Write back to RF
        
        myRF.OutputRF(); // dump RF;
        
    }
    myDataMem.OutputDataMem(); // dump data mem
    //std::cout<< PC;
    
    return 0;
    
}

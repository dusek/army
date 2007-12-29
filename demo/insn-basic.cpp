#include "instructions/Instruction.h"
#include "instructions/RegisterArgument.h"
#include "instructions/ImmediateArgument.h"

#include <iostream>

class TestInstruction: public Instruction {
public:
    TestInstruction() {
        args.push_back(new RegisterArgument(R11));
        args.push_back(new RegisterArgument(R4));
        args.push_back(new ImmediateArgument(3));
    }

    void decode(uint32_t insn_word){}

    bool executable(CPU &cpu) const {return true;}

    void execute(CPU &cpu) const {}

protected:
    std::string str_qualifiers() const { return ""; }

protected:
    uint32_t mask() const {return 0;}
    uint32_t bits() const {return 0;}
    std::string mnemonic() const {return "TST";}
};

int main(int argc, char *argv[]) {
    Instruction *insn = new TestInstruction();
    std::cout << insn->to_string(false) << std::endl;
    delete insn;
    return 0;
}

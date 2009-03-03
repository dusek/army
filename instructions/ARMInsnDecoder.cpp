#include <bitset>
#include <iostream>

#include "instructions/ARMInsnDecoder.h"
#include "instructions/InstructionSet.h"
#include "instructions/cond.h"
#include "instructions/ImmedArg.h"
#include "instructions/Arg.h"

arm::Instruction *ARMInsnDecoder::fetch_decode(addr_t addr, EndianMemory &mem)
{
    ARM_Word insn_word = mem.read_value(addr);
    std::bitset<32> word(insn_word);

    arm::Instruction *insn = 0;
    Cond cond_ = cond(insn_word);

    bool dpi = false;

    if (!word.test(27)) {
        // word[27] = 0
        if (cond_ == NV)
            return 0;

        if (!word.test(26)) {
            // word[26] = 0
            if (word.test(25)) {
                // word[25] = 1
                // word[27..25] = 001
                if (word.test(24) && !word.test(23) && !word.test(20)) {
                    if (!word.test(21)) {
                        std::cerr << "Undefined instruction: insn_word[27..20] = 00110?00" << std::endl;
                    } else {
                        // Move immediate to status register
                        std::cerr << "Not implemented instruction: Move immediate to status register (not usable in User mode)" << std::endl;
                    }
                } else {
                    dpi = true;
                }
            } else {
                // word[25] = 0
                // word[27..25] = 000
                if (word.test(4) && word.test(7)) {
                    // multiplies, extra load/stores
                    if (word.test(6)) {
                        std::cerr << "Not implemented instruction: extra load/store" << std::endl;
                    } else {
                        if (word.test(5)) {
                            //word[6..5] = 01
                            std::cerr << "Not implemented instruction: extra load/store" << std::endl;
                        } else {
                            //word[6..5] = 00
                            if (word.test(24)) {
                                std::cerr << "Not implemented instruction: \"Swap/swap byte\"" << std::endl;
                            } else {
                                bool S = word.test(20);
                                bool accumulate = word.test(21);
                                CPURegisters::Register op1 = RegisterFromWord(insn_word, 0);
                                CPURegisters::Register op2 = RegisterFromWord(insn_word, 8);

                                if (word.test(23)) {
                                    // multiply (accumulate) long
                                    CPURegisters::Register dest_hi = RegisterFromWord(insn_word, 16);
                                    CPURegisters::Register dest_lo = RegisterFromWord(insn_word, 12);

                                    EndianMemory::Signedness signedness = word.test(22) ? EndianMemory::Signed : EndianMemory::Unsigned;

                                    insn = arm::is::MULL(dest_lo, dest_hi, op1, op2, accumulate, signedness, S, cond_);
                                } else {
                                    // multiply (accumulate)
                                    CPURegisters::Register dest = RegisterFromWord(insn_word, 16);
                                    CPURegisters::Register acc = RegisterFromWord(insn_word, 12);

                                    insn = arm::is::MUL(dest, acc, op1, op2, accumulate, S, cond_);
                                }
                            }
                        }
                    }
                } else if (word.test(24) && !word.test(23) && !word.test(20)) {
                    if (!word.test(4)) {
                        // move status register to register, move register to status register
                        std::cerr << "Not implemented instruction: move between register and status register (not usable in User mode)" << std::endl;
                    } else { // since here holds !(word.test(4) && word.test(7) (above) and word.test(4), we have !word.test(7) here
                        // misc. insns
                        if (!word.test(5) && !word.test(6)) {
                            // BX
                            if (!word.test(22))
                                insn = arm::is::BX(RegisterFromWord(insn_word, 0), cond_);
                            else
                                insn = arm::is::CLZ(RegisterFromWord(insn_word, 12), RegisterFromWord(insn_word, 0));
                        } else {
                            std::cerr << "Not implemented instruction: BXL, BKPT, DSP" << std::endl;
                        }
                    }
                } else {
                    dpi = true;
                }
            }
        } else {
            // word[26] = 1
            // word[27..26] = 01
            if (word.test(25) && word.test(4)) {
                std::cerr << "Undefined instruction: word[27..25] = 011, word[4] = 1" << std::endl;
            } else {
                // load/store immediate + register offset
                if (word.test(20))
                    insn = arm::is::LDR(RegisterFromWord(insn_word, 12), RegisterFromWord(insn_word, 16), Arg_decode_ldst_index(insn_word), word.test(24), word.test(23), word.test(22), word.test(21), cond_);
                else
                    insn = arm::is::STR(RegisterFromWord(insn_word, 12), RegisterFromWord(insn_word, 16), Arg_decode_ldst_index(insn_word), word.test(24), word.test(23), word.test(22), word.test(21), cond_);
            }
        }
    } else {
        // word[27] = 1
        if (!word.test(26)) {
            // word[27..26] = 10
            if (!word.test(25)) {
                // word[27..25] = 100
                if (cond_ == NV)
                    std::cerr << "Undefined instruction: word[27..25] = 100, cond = 1111" << std::endl;
                else {
                    // load/store multiple
                    if (word.test(20))
                        insn = arm::is::LDM(!word.test(24), word.test(23), word.test(21), RegisterFromWord(insn_word, 16), insn_word, cond_);
                    else
                        insn = arm::is::STM(!word.test(24), word.test(23), word.test(21), RegisterFromWord(insn_word, 16), insn_word, cond_);
                }
            } else {
                // word[27..25] = 101
                if (cond_ != NV)
                    insn = arm::is::B(ImmedArg_ValFromWord(insn_word, 0, 24), word.test(24), cond_);
                else
                    std::cerr << "Not implemented: Branch with Thumb exchange" << std::endl;
            }
        } else {
            // word[27..26] = 11
            if (!word.test(25)) {
                // word[27..25] = 110
                std::cerr << "Not implemented: Coprocessor load/store and double register transfer" << std::endl;
            } else {
                // word[27..25] = 111
                if (!word.test(24)) {
                    // word[27..24] = 1110
                    std::cerr << "Not implemented: Coprocessor DP or register transfer" << std::endl;
                } else {
                    // word[27..24] = 1111
                    if (cond_ != NV)
                        insn = arm::is::SWI(insn_word & 0x00ffffff, cond_);
                    else
                        std::cerr << "Undefined instruction" << std::endl;
                }
            }
        }
    }

    if (dpi) {
        // decode data-processing instruction
        ARM_Word opcode = ImmedArg_ValFromWord(insn_word, 21, 4);
        bool S = word.test(20);
        Arg *sh_op = Arg_decode_shifter_operand(insn_word);
        CPURegisters::Register dest = RegisterFromWord(insn_word, 12);
        CPURegisters::Register op1  = RegisterFromWord(insn_word, 16);
        switch(opcode) {
            case  0: insn = arm::is::AND(dest, op1, sh_op, S, cond_); break;
            case  1: insn = arm::is::EOR(dest, op1, sh_op, S, cond_); break;
            case  2: insn = arm::is::SUB(dest, op1, sh_op, S, cond_); break;
            case  3: insn = arm::is::RSB(dest, op1, sh_op, S, cond_); break;
            case  4: insn = arm::is::ADD(dest, op1, sh_op, S, cond_); break;
            case  5: insn = arm::is::ADC(dest, op1, sh_op, S, cond_); break;
            case  6: insn = arm::is::SBC(dest, op1, sh_op, S, cond_); break;
            case  7: insn = arm::is::RSC(dest, op1, sh_op, S, cond_); break;
            case  8: insn = arm::is::TST(op1, sh_op, cond_); break;
            case  9: insn = arm::is::TEQ(op1, sh_op, cond_); break;
            case 10: insn = arm::is::CMP(op1, sh_op, cond_); break;
            case 11: insn = arm::is::CMN(op1, sh_op, cond_); break;
            case 12: insn = arm::is::ORR(dest, op1, sh_op, S, cond_); break;
            case 13: insn = arm::is::MOV(dest, sh_op, S, cond_); break;
            case 14: insn = arm::is::BIC(dest, op1, sh_op, S, cond_); break;
            case 15: insn = arm::is::MVN(dest, sh_op, S, cond_); break;
        }
    }

    return insn;
}

void ARMInsnDecoder::dispose(arm::Instruction *insn)
{
    delete insn;
}

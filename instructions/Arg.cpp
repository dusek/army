#include <bitset>

#include "instructions/Arg.h"

#include "instructions/ImmedArg.h"
#include "instructions/RegArg.h"
#include "instructions/ShOpRORArg.h"
#include "instructions/ShOpRRXArg.h"
#include "instructions/ShOpLSLArg.h"
#include "instructions/ShOpSRArg.h"

Arg::Arg()
{}

Arg::~Arg()
{}

std::ostream& operator<<(std::ostream& o, const Arg& arg)
{
    arg.fmt(o);
    return o;
}

Arg *Arg_decode_shifter_operand(ARM_Word insn_word)
{
    std::bitset<32> word(insn_word);

    if (word.test(25))
        return new ShOpRORArg(ImmedArg_decode(insn_word, 0, 8, false), ImmedArg_decode(insn_word, 8, 4, true));
    else {
        // non-Immediate sh op (i.e. this is register shifted or rotated by some amount)
        Arg *amount = 0;
        
        if (word.test(4)) {
            // shift (or rotate) amount is in register value
            if (word.test(7))
                return 0; // [25]0, [4]1, [7]1 is invalid
            else
                amount = RegArg_decode(insn_word, 8);
        }
        else {
            // shift (or rotate) amount is in an immediate encoded in instruction word
            ImmedArg *amount_immed = ImmedArg_decode(insn_word, 7, 5, false);
            if (amount_immed->val() == 0) {
                if ((!word.test(6) && word.test(5)) || (word.test(6) && !word.test(5))) {
                    amount = new ImmedArg(32);
                }
                if (word.test(5) || word.test(6))
                    delete amount_immed;
                else
                    amount = amount_immed;
            } else {
                amount = amount_immed;
            }
        }

        Arg *value = RegArg_decode(insn_word, 0);

        if (word.test(5)) {
            // ?1
            if (word.test(6)) {
                // 11 - ROR or RRX
                if (amount)
                    return new ShOpRORArg(value, amount);
                else return new ShOpRRXArg(value);
            } else {
                // 01 - LSR
                return new ShOpSRArg(value, amount, ShOpSRArg::ShiftTypeLogical);
            }
        } else {
            // ?0
            if (word.test(6)) {
                // 10 - ASR
                return new ShOpSRArg(value, amount, ShOpSRArg::ShiftTypeArithmetic);
            } else {
                // 00 - LSL
                if (amount)
                    return new ShOpLSLArg(value, amount);
                else {
                    // shift amount is zero -> this is "pure" register argument
                    delete amount;
                    return value;
                }
            }
        }
    }
}

Arg *Arg_decode_ldst_index(ARM_Word insn_word)
{
    std::bitset<32> word(insn_word);
    if (!word.test(25))
        return ImmedArg_decode(insn_word, 0, 12, false);
    else {
        word.set(25, false);
        return Arg_decode_shifter_operand(word.to_ulong());
    }
}

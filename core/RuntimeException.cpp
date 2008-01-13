#include <ostream>

#include "core/RuntimeException.h"

void RuntimeException::output(std::ostream &o) const {
    o << "Fatal error executing instruction at " << std::hex << offending_insn_addr_ << std::endl;
    output_details(o);
}
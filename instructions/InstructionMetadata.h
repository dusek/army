#ifndef ARMY_CORE_INSTRUCTION_METADATA_H_
#define ARMY_CORE_INSTRUCTION_METADATA_H_

#include <string>

class InstructionMetadata {
    virtual const std::string & mnemonic() const = 0;
    virtual const std::string & qualifiers() const = 0;
};

#endif

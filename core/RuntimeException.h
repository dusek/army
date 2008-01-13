#ifndef ARMY_CORE_RUNTIME_EXCEPTION_H_
#define ARMY_CORE_RUNTIME_EXCEPTION_H_

#include <ostream>

#include "core/Exception.h"

#include "armyconfig.h"

class RuntimeException : public Exception {
public:
    void output(std::ostream &o) const;
    void set_addr(addr_t addr) { offending_insn_addr_ = addr; };

protected:
    virtual void output_details(std::ostream &o) const = 0;

private:
    addr_t offending_insn_addr_;
};

#endif

/// \file 
/// Class InstrLike -- objects which can be converted to instructions.

#ifndef ILANG_ILA_INSTRLIKE_INSTRLIKE_H__
#define ILANG_ILA_INSTRLIKE_INSTRLIKE_H__

#include <variant>

/// \namespace ilang
namespace ilang {

// forward declarations
class Instr;  // basic form of an instruction
class SimdInstr;
// class SimdComposition;

typedef std::variant<SimdInstr> InstrLike;  // all things like an Instr

template<typename C>
inline void compile_with(C compiler, const InstrLike& instr) {
  return std::visit(compiler, instr);
}

}

#endif /* ILANG_ILA_INSTRLIKE_INSTRLIKE_H__ */

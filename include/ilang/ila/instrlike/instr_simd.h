/// \file 
/// Class SIMDInstr -- an object representing a SIMD instruction.

#ifndef ILANG_ILA_INSTRLIKE_INSTR_SIMD_H__
#define ILANG_ILA_INSTRLIKE_INSTR_SIMD_H__

#include <ilainstructionlike.h>

/// \namespace ilang
namespace ilang {

class SimdInstr : public IlaInstructionLike {

public:
  static SimdInstr New(const std::string& name, const InstrLvlAbsPtr& host);

  ~SimdInstr();

  // ------------------------- ACCESSORS/MUTATORS --------------------------- //

  inline const VarContainerPtr& data_atom() { return data_atom_; }

  /// \brief Returns the child ILA managing a single data-atom in order to add 
  /// data-atom instructions.
  inline const InstrLvlAbsPtr& da_manager() { return program(); }

  /// \brief Returns the (potentially) read or updated state 
  /// variables of this instruction
  /// Does not include variables which may be added during compilation
  StateNameSet used_states() const override;

  /// \brief Returns the (potentially) updated state variables of this 
  /// instruction
  /// Does not include variables which may be added during compilation
  StateNameSet updated_states() const override;

  /// \brief Returns an expression that is true when this instruction has
  /// completed.
  ExprPtr done_condition();  

  // ------------------------- METHODS --------------------------- //

  
protected:
  SimdInstr(
    const std::string& name, const VarContainerPtr& data_atom, 
    const ExprPtr& da_start_trigger, const ExprPtr& da_trigger_value, 
    const ExprPtr& da_finish_condition,
    const VarContainerPtr& vector
  );

private:
  VarContainerPtr data_atom_;

}

}

#endif /* ILANG_ILA_INSTRLIKE_INSTR_SIMD_H__ */

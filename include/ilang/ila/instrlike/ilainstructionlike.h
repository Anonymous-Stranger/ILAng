#ifndef ILANG_ILA_INSTRLIKE_ILAINSTRUCTIONLIKE_H__
#define ILANG_ILA_INSTRLIKE_ILAINSTRUCTIONLIKE_H__

#include <functional>
#include <set>

#include <ilang/ila/ast_hub.h>
#include "instrlike.h"

/// \namespace ilang
namespace ilang {

// Forward declaration for adding host ila.
class InstrLvlAbs;

/*
  Instructions of an ILA are defined by a decode 
  condition and a state update function.
  We say that an object is like an ILA instruction 
  if (1) there are some states and inputs that cause 
  it to be run and (2) it's execution can be interpreted 
  as a sequence of state updates.
  I.e. (1) it has a decode condition and (2) it can be 
  compiled into a set of instructions.

  Moreover, we require that if we need any chaining of
  instructions or 
*/

class IlaInstructionLike: public Object {

public:
  /// Pointer type for normal use of Instr.
  typedef std::shared_ptr<InstrLike> InstrLikePtr;
  /// Pointer type for read-only use of Instr.
  typedef std::shared_ptr<const InstrLike> InstrLikeCnstPtr;
  /// Pointer type for ILA.
  typedef std::shared_ptr<InstrLvlAbs> InstrLvlAbsPtr;
  /// Type for a set of state names
  typedef std::set<std::string> StateNameSet;

  /// Default destructor (constructor is protected).
  /// Child classes should create a static New() method.
  ~IlaInstructionLike()=default;

  // ------------------------- ACCESSORS/MUTATORS --------------------------- //

  /// Return true if Is type Instr.
  bool is_instr() const override { return true; }
  
  /// Return the host ILA.
  inline InstrLvlAbsPtr host() const { return host_; }

  /// \brief Set the decode condition if not yet assigned.
  /// \param[in] decode is the pointer to the decode condition (bool).
  virtual void set_decode(const ExprPtr& decode);

  /// \brief Return the decode condition.
  inline ExprPtr decode() const { return decode_; }

  /// \brief Set the child-program (as a child-ILA) of the instruction.
  /// \param[in] program the pointer to the child-ILA.
  virtual void set_program(const InstrLvlAbsPtr& program);

  /// \brief Return the child-ILA comprising the child-program.
  inline InstrLvlAbsPtr program() const { return prog_; }

  /// \brief Returns the (potentially) read or updated state 
  /// variables of this instruction
  /// Does not include variables which may be added during compilation
  virtual StateNameSet used_states() const=0;

  /// \brief Returns the (potentially) updated state variables of this 
  /// instruction
  /// Does not include variables which may be added during compilation
  virtual StateNameSet updated_states() const=0;

    /// \brief Returns the (potentially) read or updated state 
  /// variables of this instruction and the program it triggers
  /// Does not include variables which may be added during compilation
  virtual StateNameSet used_states_in_prog() const=0;

  /// \brief Returns the (potentially) updated state variables of this 
  /// instruction and the program it triggers
  /// Does not include variables which may be added during compilation
  virtual StateNameSet updated_states_in_prog() const=0;

  // ------------------------- METHODS --------------------------- //

  /// \brief Compiles this instruction into Instrs by updating host ILA.
  virtual void Compile()=0;
  /// [TODO] consider enforcing idempotency of Compile.

  /// \brief Set the decode function, overwriting existing.
  /// \param[in] decode is the pointer to the decode function (bool).
  virtual void ForceSetDecode(const ExprPtr& decode);

protected:
  /// Constructor
  IlaInstructionLike(const std::string& name, const InstrLvlAbsPtr& host);

  /// Simplify AST nodes with their representatives.
  ExprPtr Unify(const ExprPtr& e);

private:
  /// The ILA this instruction belongs to.
  InstrLvlAbsPtr host_ = nullptr;
  /// The child-program (child-ILA being triggered).
  InstrLvlAbsPtr prog_ = nullptr;
  /// The decode condition needed for this instruction to execute.
  ExprPtr decode_ = nullptr;
};

}

#endif /* ILANG_ILA_INSTRLIKE_ILAINSTRUCTION_H__ */
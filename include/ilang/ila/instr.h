/// \file
/// Class Instr - the object representing an instruction.

#ifndef ILANG_ILA_INSTR_H__
#define ILANG_ILA_INSTR_H__

#include <memory>
#include <string>

#include <ilang/ila/hash_ast.h>
#include <ilang/ila/object.h>
#include <ilang/ila/instrlike/ilainstructionlike.h>

/// \namespace ilang
namespace ilang {

// Forward declaration for adding host ila.
class InstrLvlAbs;

/// \brief The class for the Instruction. An Instr object contains:
/// - the decode function
/// - a set of update functions for the state variables
/// - several attributes, e.g. view
class Instr : public IlaInstructionLike {
public:
  /// Pointer type for normal use of Instr.
  typedef std::shared_ptr<Instr> InstrPtr;
  /// Pointer type for read-only use of Instr.
  typedef std::shared_ptr<const Instr> InstrCnstPtr;

private:
  /// Type for storing a set of Expr.
  typedef std::map<std::string, ExprPtr> ExprPtrMap;

public:
  // ------------------------- CONSTRUCTOR/DESTRUCTOR ----------------------- //
  /// Constructor with the ast simplifier.
  Instr(const std::string& name, const InstrLvlAbsPtr& host = nullptr);
  /// Default destructor.
  ~Instr();

  // ------------------------- HELPERS -------------------------------------- //
  /// \brief Create a new instruction (Instr) binded with the host. Used
  /// for hiding implementation specific type details.
  static InstrPtr New(const std::string& name, InstrLvlAbsPtr host = nullptr);

  // ------------------------- ACCESSORS/MUTATORS --------------------------- //
  /// \brief Set the update function for the state variable specified by name.
  /// \param[in] name the name of the state variable.
  /// \param[in] update the update function expression (same type as state).
  void set_update(const std::string& name, const ExprPtr& update);

  /// \brief Set the update function for the state variable specified by var
  /// pointer.
  /// \param[in] state the pointer to the state variable.
  /// \param[in] update the update function expression (same type as state).
  void set_update(const ExprPtr& state, const ExprPtr& update);

  /// \brief Return the update function for the state specified by name.
  /// \param[in] name the name of the state variable.
  /// \return the state update function.
  ExprPtr update(const std::string& name) const;

  /// \brief Return the update function for the state specified by var pointer.
  /// \param[in] state the pointer to the state variable.
  ExprPtr update(const ExprPtr& state) const;

  /// \brief Returns the (potentially) read or updated state 
  /// variables of this instruction
  /// Does not include variables which may be added during compilation
  StateNameSet used_states() const override;

  /// \brief return the (potentially) updated state of this function
  StateNameSet updated_states() const override;

  // ------------------------- METHODS -------------------------------------- //
  
  /// \brief Compiles this instruction into Instrs by updating host ILA.
  void Compile() override;
  
  /// \brief Overwrite update function for the state variable specified by name.
  /// \param[in] name the name of the state variable.
  /// \param[in] update the update function expression (same type as state).
  void ForceAddUpdate(const std::string& name, const ExprPtr& update);

  /// Output function.
  std::ostream& Print(std::ostream& out) const override;

  /// Overload output stream operator.
  friend std::ostream& operator<<(std::ostream& out, InstrPtr i);
  /// Overload output stream operator for const object.
  friend std::ostream& operator<<(std::ostream& out, InstrCnstPtr i);

private:
  // ------------------------- MEMBERS -------------------------------------- //
  /// The set of update functions, mapped by name.
  ExprPtrMap updates_;
}; // class Instr

/// Pointer type for normal use of Instr.
typedef Instr::InstrPtr InstrPtr;
/// Pointer type for read-only use of Instr.
typedef Instr::InstrCnstPtr InstrCnstPtr;
/// Type for storing a set of Instr.
typedef std::vector<InstrPtr> InstrVec;

} // namespace ilang

#endif // ILNAG_ILA_INSTR_H__

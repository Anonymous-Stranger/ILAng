/// \file
/// Header for a collection of ILA helpers.

#ifndef ABS_KNOB_H__
#define ABS_KNOB_H__

#include "ila/instr_lvl_abs.h"

namespace ila {

class AbsKnob {
public:
  /// Add all vars (including child) to the set.
  static void GetVarOfIla(const InstrLvlAbsPtr top, std::set<ExprPtr>& vars);
  /// Add all state vars (including child) to the set.
  static void GetStVarOfIla(const InstrLvlAbsPtr top, std::set<ExprPtr>& vars);
  /// Add all input vars (including child) to the set.
  static void GetInVarOfIla(const InstrLvlAbsPtr top, std::set<ExprPtr>& vars);
  /// Add all state vars (including child) to the set.
  template <class InstrCntr>
  static void GetStVarOfInstr(const InstrCntr& instrs, std::set<ExprPtr>& vars);

  /// Get the set of all vars (including child).
  static std::set<ExprPtr> GetVarOfIla(const InstrLvlAbsPtr top);
  /// Get the set of all state vars (including child).
  static std::set<ExprPtr> GetStVarOfIla(const InstrLvlAbsPtr top);
  /// Get the set of all input vars (including child).
  static std::set<ExprPtr> GetInVarOfIla(const InstrLvlAbsPtr top);
  /// Get the set of all state vars.
  template <class InstrCntr>
  static std::set<ExprPtr> GetStVarOfInstr(const InstrCntr& instrs);

  /// Add dependant vars (either state or input) to the set.
  static void GetVarOfExpr(const ExprPtr e, std::set<ExprPtr>& vars);
  /// Get the set of dependant vars (either state or input).
  static std::set<ExprPtr> GetVarOfExpr(const ExprPtr e);

  /// Add all instructions (including child) to the std::set/std::vector.
  template <class InstrCntr>
  static void GetInstrOfIla(const InstrLvlAbsPtr top, InstrCntr& instrs);
  /// Get the set of instructions (including child).
  static std::set<InstrPtr> GetInstrOfIla(const InstrLvlAbsPtr top);

  /// \brief Return a new ILA that contains the dependant instructions and
  /// child-ILAs of an instruction (defined by sub-programs).
  static InstrLvlAbsPtr ExtrDeptModl(const InstrPtr instr);

  /// Copy all state and input var (not including child) from A to B.
  static void CopyVar(const InstrLvlAbsCnstPtr src, const InstrLvlAbsPtr dst);

  /// \brief Rewrite an expression by replacing based on the rule.
  /// - If leaves contain non-var nodes, will replace with no further traverse.
  static ExprPtr Rewrite(const ExprPtr e, const ExprMap& rule);

  /// Verbose tag for logging system.
  static const std::string k_verbose_tag;

}; // class AbsKnob

} // namespace ila

#endif // ABS_KNOB_H__


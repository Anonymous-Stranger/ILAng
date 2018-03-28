/// \file
/// Header for generating verification condition for equivalence checking.

#ifndef EQ_CHECK_CRR_H__
#define EQ_CHECK_CRR_H__

#include "ila/instr_lvl_abs.h"
#include "refinement/comp_ref_rel.h"
#include "z3++.h"

/// \namespace ila
namespace ila {

/// \brief Generator for commutating diagram-based equivalence checking.
class CommDiag {
public:
  // ------------------------- CONSTRUCTOR/DESTRUCTOR ----------------------- //
  /// Default constructor.
  CommDiag(z3::context& ctx, const CrrPtr crr);
  /// Default destructor.
  ~CommDiag();

  // ------------------------- METHODS -------------------------------------- //
  /// \brief Check equivalence between two models based on the refinement
  /// relation provided up to the given unroll bound.
  /// \param[in] max unroll bound (0 for auto-search).
  bool EqCheck(const int& max = 0);

  /// \brief Generate verification condition up to the given maximum bound. This
  /// equals to tran && !prop
  z3::expr GenVerCond(const int& max);

  /// \brief Generate verification condition for the transition path.
  z3::expr GenVerCondTran(const int& max);
  /// \brief Generate verification condition for the property.
  z3::expr GenVerCondProp();

private:
  // ------------------------- MEMBERS -------------------------------------- //
  /// The underlying z3 context.
  z3::context& ctx_;
  /// The refinement relation.
  CrrPtr crr_;
  /// The z3 expr adapter used.
  Z3ExprAdapter g_ = Z3ExprAdapter(ctx_);
  /// Upper bound for unrolling.
  int max_ = 0;

  /// \brief Check the refinement mapping is valid.
  /// - F: flushing function (states + inputs)
  /// - A: apply function (states + inputs)
  /// - Eq: equivalence of all state variables (not including inputs)
  /// - Property: !(F ^ A) is unsat
  /// - Property: A(s0) & !(F(s1) & A(s0) & Eq(s0, s1)) is unsat
  bool CheckRefinement(const RefPtr ref);

  /// \brief Generate verification condition for one part of the commutating
  /// diagram up to the given bound.
  /// - (so_0 = sn_0) for all state variables
  /// - unroll, starting from so_0 for k steps, F(so_i) for all i >= 0
  /// - unroll, starting from sn_0 for 1 step, A(sn_0)
  /// - unroll, starting from sn_1 for k steps, F(sn_i) for all i > 0
  /// - if #step in CRR < max, k = #step, otherwise k = max
  /// - if #step not known, OR all cmpl in i-th steps for i > 0 and return
  z3::expr GenVerCondRefine(const RefPtr ref, const int& max);

  int DetBndOld(const RefPtr ref);
  int DetBndNew(const RefPtr ref);

}; // class CommDiag

} // namespace ila

#endif // EQ_CHECK_CRR_H__


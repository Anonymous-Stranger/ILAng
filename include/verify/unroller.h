/// \file
/// Header for unrolling ILA execution.

#ifndef UNROLLER_H__
#define UNROLLER_H__

#include "ila/instr_lvl_abs.h"
#include "ila/z3_expr_adapter.h"
#include "z3++.h"
#include <set>
#include <vector>

namespace ila {

/// \brief Class for unrolling ILA execution in different settings.
class Unroller {
public:
  typedef z3::expr ZExpr;
  typedef z3::expr_vector ZExprVec;
  typedef std::set<ExprPtr> IExprSet;
  typedef ExprPtrVec IExprVec;

  Unroller(z3::context& ctx);
  virtual ~Unroller();

  inline void AddGlobPred(const ExprPtr p);
  inline void AddInitPred(const ExprPtr p);
  inline void ClearGlobPred();
  inline void ClearInitPred();

  ZExpr InstrSeq(const std::vector<InstrPtr>& seq, const int& pos = 0);

private:
  z3::context& ctx_;
  Z3ExprAdapter gen_;

protected:
  IExprSet vars_;
  IExprVec k_pred_;
  IExprVec k_next_;

private:
  IExprVec g_pred_;
  IExprVec i_pred_;
  ZExprVec k_prev_;
  ZExprVec k_curr_z3_;
  ZExprVec k_next_z3_;

  ZExprVec cstr_;

  inline z3::context& ctx() const { return ctx_; }
  inline Z3ExprAdapter& gen() { return gen_; }

protected:
  ZExpr UnrollSubs(const size_t& len, const int& pos = 0);
  virtual void CollectVar() = 0;
  virtual void Transition(const size_t& idx) = 0;

  ExprPtr StateUpdCmpl(const InstrPtr instr, const ExprPtr var);

private:
  void BootStrap(const int& pos);

  inline ZExpr Substitute(ZExpr expr, const ZExprVec& src_vec,
                          const ZExprVec& dst_vec) const;

  void AssertPredSubs(const IExprVec& pred_vec, const std::string& suffix,
                      const ZExprVec& src_vec, const ZExprVec& dst_vec);

  void UpdateNextSubs(ZExprVec& next_z, const IExprVec& next_i,
                      const std::string& suffix, const ZExprVec& src,
                      const ZExprVec& dst);

  // <a> == <b>_suffix
  void AssertVarEqual(const ZExprVec& a, const IExprSet& b,
                      const std::string& b_suffix);

  ZExpr ConjPred(const ZExprVec& vec);

  // legacy
  z3::expr InstrUpdDflt(const InstrLvlAbsPtr ila, const std::string& prev,
                        const std::string& next);

  z3::expr InstrUpdCmpl(const InstrPtr instr, const std::string& prev,
                        const std::string& next);

  z3::expr StateUpdCmpl(const InstrPtr instr, const ExprPtr var,
                        const std::string& prev);

  std::set<ExprPtr> GetAllVar(const std::vector<InstrPtr>& seq) const;

}; // class Unroller

class ListUnroll : public Unroller {
public:
  typedef std::vector<InstrPtr> InstrVec;

  ListUnroll(z3::context& ctx, const InstrVec& seq = {});
  ~ListUnroll();

  ZExpr InstrSeqAssn(const std::vector<InstrPtr>& seq, const int& pos = 0);

  ZExpr InstrSeqSubs(const std::vector<InstrPtr>& seq, const int& pos = 0);

private:
  InstrVec seq_;

  void CollectVar();
  void Transition(const size_t& idx);
};

#if 0
class BulkUnroll : public Unroller {
public:
  BulkUnroll(z3::context& ctx, const InstrLvlAbsPtr top);
  ~BulkUnroll();

  ZExpr IlaBulkAssn(const InstrLvlAbsPtr top, const int& length,
                    const int& pos = 0);

  ZExpr IlaBulkSubs(const InstrLvlAbsPtr top, const int& length,
                    const int& pos = 0);

private:
  InstrLvlAbsPtr top_;

  void BootStrap(const InstrLvlAbsPtr top);
  void CollectVar();
  void VisitHierCollectVar(const InstrLvlAbsPtr m);
};
#endif

} // namespace ila

#endif // UNROLLER_H__


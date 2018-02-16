/// \file
/// Source for the op expression

#include "ila/ast/expr_op.h"

namespace ila {

// ------------------------- Class ExprOp ----------------------------------- //
ExprOp::ExprOp() { ILA_ERROR << "Undefined ExprOp constructor.\n"; }

ExprOp::ExprOp(const ExprPtr arg) {
  // arg
  ExprPtrVec args = {arg};
  set_args(args);
  // host
  set_host(arg->host());
}

ExprOp::ExprOp(const ExprPtr arg0, const ExprPtr arg1) {
  // args
  ExprPtrVec args = {arg0, arg1};
  set_args(args);
  // set host
  auto host0 = arg0->host();
  auto host1 = arg1->host();
  if (host0 == host1) {
    set_host(host0);
  } else if (host0 == NULL) {
    set_host(host1);
  } else if (host1 == NULL) {
    set_host(host0);
  }
}

ExprOp::ExprOp(const ExprPtr arg0, const ExprPtr arg1, const ExprPtr arg2) {
  // args
  ExprPtrVec args = {arg0, arg1, arg2};
  set_args(args);
  // set host
  auto host0 = arg0->host();
  auto host1 = arg1->host();
  auto host2 = arg2->host();
  if (host0 == host1 && host1 == host2) {
    set_host(host0);
  } else if (host1 == NULL && host2 == NULL) {
    set_host(host0);
  } else if (host0 == NULL && host2 == NULL) {
    set_host(host1);
  } else if (host0 == NULL && host1 == NULL) {
    set_host(host2);
  }
}

ExprOp::ExprOp(const ExprPtr arg0, const int& param1) {
  // args
  set_args({arg0});
  // params
  set_params({param1});
  // set host
  set_host(arg0->host());
}

ExprOp::ExprOp(const ExprPtr arg0, const int& param1, const int& param2) {
  // args
  set_args({arg0});
  // params
  set_params({param1, param2});
  // set hsot
  set_host(arg0->host());
}

ExprOp::~ExprOp() {}

std::ostream& ExprOp::Print(std::ostream& out) const {
  return out << name().format_str(op_name(), "");
}

Sort ExprOp::GetSortBinaryOperation(const Sort& s0, const Sort& s1) {
  ILA_ASSERT(s0 == s1) << "Undefined sorts " << s0 << " and " << s1
                       << " for binary operations.";
  // return the smae sort as input arguments.
  return s0;
}

Sort ExprOp::GetSortBinaryComparison(const Sort& s0, const Sort& s1) {
  ILA_ASSERT(s0 == s1) << "Undefined sorts " << s0 << " and " << s1
                       << " for binary comparison.";
  // return boolean sort.
  return Sort::MakeBoolSort();
}

// ------------------------- Class ExprOpNeg -------------------------------- //
ExprOpNeg::ExprOpNeg(const ExprPtr arg) : ExprOp(arg) {
  ILA_ASSERT(arg->is_bv()) << "Negate can only be applied to bitvector.";
  set_sort(arg->sort());
}

z3::expr ExprOpNeg::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                              const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 1) << "Negate is a unary function.";
  return -expr_vec[0];
}

// ------------------------- Class ExprOpNot -------------------------------- //
ExprOpNot::ExprOpNot(const ExprPtr arg) : ExprOp(arg) {
  ILA_ASSERT(arg->is_bool()) << "Not can only be applied to bool.";
  set_sort(arg->sort());
}

z3::expr ExprOpNot::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                              const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 1) << "Not is a unary function.";
  return !expr_vec[0];
}

// ------------------------- Class ExprOpCompl ------------------------------ //
ExprOpCompl::ExprOpCompl(const ExprPtr arg) : ExprOp(arg) {
  ILA_ASSERT(arg->is_bv()) << "Complement can only be applied to bitvector.";
  set_sort(arg->sort());
}

z3::expr ExprOpCompl::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                                const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 1) << "Negate is a unary function.";
  return ~expr_vec[0];
}

// ------------------------- Class ExprOpAnd -------------------------------- //
ExprOpAnd::ExprOpAnd(const ExprPtr arg0, const ExprPtr arg1)
    : ExprOp(arg0, arg1) {
  set_sort(GetSortBinaryOperation(arg0->sort(), arg1->sort()));
}

z3::expr ExprOpAnd::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                              const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 2);
  ILA_ASSERT(is_bool() || is_bv()) << "AND can only be either bool or bv.";
  if (is_bool())
    return expr_vec[0] && expr_vec[1];
  else
    return expr_vec[0] & expr_vec[1];
}

// ------------------------- Class ExprOpOr --------------------------------- //
ExprOpOr::ExprOpOr(const ExprPtr arg0, const ExprPtr arg1)
    : ExprOp(arg0, arg1) {
  set_sort(GetSortBinaryOperation(arg0->sort(), arg1->sort()));
}

z3::expr ExprOpOr::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                             const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 2);
  ILA_ASSERT(is_bool() || is_bv()) << "OR can only be either bool or bv.";
  if (is_bool())
    return expr_vec[0] || expr_vec[1];
  else
    return expr_vec[0] | expr_vec[1];
}

// ------------------------- Class ExprOpXor -------------------------------- //
ExprOpXor::ExprOpXor(const ExprPtr arg0, const ExprPtr arg1)
    : ExprOp(arg0, arg1) {
  set_sort(GetSortBinaryOperation(arg0->sort(), arg1->sort()));
}

z3::expr ExprOpXor::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                              const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 2);
  ILA_ASSERT(is_bool() || is_bv()) << "XOR can only be either bool or bv.";
  if (is_bv()) {
    // auto ast_xor = Z3_mk_bvxor(ctx, expr_vec[0], expr_vec[1]);
    return expr_vec[0] ^ expr_vec[1];
    // return z3::to_expr(ctx, ast_xor);
  } else {
    auto ast_xor = Z3_mk_xor(ctx, expr_vec[0], expr_vec[1]);
    return z3::to_expr(ctx, ast_xor);
  }
}

// ------------------------- Class ExprOpAdd -------------------------------- //
ExprOpAdd::ExprOpAdd(const ExprPtr arg0, const ExprPtr arg1)
    : ExprOp(arg0, arg1) {
  set_sort(GetSortBinaryOperation(arg0->sort(), arg1->sort()));
}

z3::expr ExprOpAdd::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                              const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 2) << "Add is binary operation.";
  ILA_ASSERT(is_bv()) << "ADD can only be applied to bv.";
  return expr_vec[0] + expr_vec[1];
}

// ------------------------- Class ExprOpSub -------------------------------- //
ExprOpSub::ExprOpSub(const ExprPtr arg0, const ExprPtr arg1)
    : ExprOp(arg0, arg1) {
  set_sort(GetSortBinaryOperation(arg0->sort(), arg1->sort()));
}

z3::expr ExprOpSub::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                              const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 2) << "Sub is binary operation.";
  ILA_ASSERT(is_bv()) << "Sub can only be applied to bv.";
  return expr_vec[0] - expr_vec[1];
}

// ------------------------- Class ExprOpEq --------------------------------- //
ExprOpEq::ExprOpEq(const ExprPtr arg0, const ExprPtr arg1)
    : ExprOp(arg0, arg1) {
  set_sort(GetSortBinaryComparison(arg0->sort(), arg1->sort()));
}

z3::expr ExprOpEq::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                             const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 2) << "Eq is binary comparison.";
  return expr_vec[0] == expr_vec[1];
}

// ------------------------- Class ExprOpLt --------------------------------- //
ExprOpLt::ExprOpLt(const ExprPtr arg0, const ExprPtr arg1)
    : ExprOp(arg0, arg1) {
  set_sort(GetSortBinaryComparison(arg0->sort(), arg1->sort()));
}

z3::expr ExprOpLt::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                             const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 2) << "Lt is binary comparison.";
  ILA_ASSERT(is_bv()) << "Lt can only be applied to bv.";
  return expr_vec[0] < expr_vec[1];
}

// ------------------------- Class ExprOpGt --------------------------------- //
ExprOpGt::ExprOpGt(const ExprPtr arg0, const ExprPtr arg1)
    : ExprOp(arg0, arg1) {
  set_sort(GetSortBinaryComparison(arg0->sort(), arg1->sort()));
}

z3::expr ExprOpGt::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                             const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 2) << "Gt is binary comparison.";
  ILA_ASSERT(is_bv()) << "Gt can only be applied to bv.";
  return expr_vec[0] > expr_vec[1];
}

// ------------------------- Class ExprOpLoad ------------------------------- //
ExprOpLoad::ExprOpLoad(const ExprPtr mem, const ExprPtr addr)
    : ExprOp(mem, addr) {
  ILA_ASSERT(mem->sort().addr_width() == addr->sort().bit_width())
      << "Address width does not match with memory.";
  // sort should be the data sort of the mem
  auto data_sort = Sort::MakeBvSort(mem->sort().data_width());
  set_sort(data_sort);
}

z3::expr ExprOpLoad::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                               const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 2);
  return z3::select(expr_vec[0], expr_vec[1]);
}

// ------------------------- Class ExprOpLoad ------------------------------- //
ExprOpStore::ExprOpStore(const ExprPtr mem, const ExprPtr addr,
                         const ExprPtr data)
    : ExprOp(mem, addr, data) {
  ILA_ASSERT(mem->sort().addr_width() == addr->sort().bit_width())
      << "Address width does not match with memory.";
  ILA_ASSERT(mem->sort().data_width() == data->sort().bit_width())
      << "Data width does not match with memory.";
  set_sort(mem->sort());
}

z3::expr ExprOpStore::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                                const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 3) << "Store takes 3 arguments.";
  auto mem = expr_vec[0];
  auto addr = expr_vec[1];
  auto data = expr_vec[2];
  return z3::store(mem, addr, data);
}

// ------------------------- Class ExprOpConcat ----------------------------- //
ExprOpConcat::ExprOpConcat(const ExprPtr hi, const ExprPtr lo)
    : ExprOp(hi, lo) {
  ILA_ASSERT(hi->is_bv()) << "Concat non-bv var " << hi;
  ILA_ASSERT(lo->is_bv()) << "Concat non-bv var " << lo;
  set_sort(Sort::MakeBvSort(hi->sort().bit_width() + lo->sort().bit_width()));
}

z3::expr ExprOpConcat::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                                 const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 2) << "Concat takes 2 arguments.";
  auto hi = expr_vec[0];
  auto lo = expr_vec[1];
  return z3::concat(hi, lo);
}

// ------------------------- Class ExprOpExtract ---------------------------- //
ExprOpExtract::ExprOpExtract(const ExprPtr bv, const int& hi, const int& lo)
    : ExprOp(bv, hi, lo) {
  ILA_ASSERT(bv->is_bv()) << "Extract can only be applied to bitvector.";
  ILA_ASSERT(hi >= lo) << "Invalid boundary for extraction.";
  set_sort(Sort::MakeBvSort(hi - lo + 1));
}

z3::expr ExprOpExtract::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                                  const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 1) << "Extract take 1 argument.";
  ILA_ASSERT(param_num() == 2) << "Extract need two parameters.";
  auto bv = expr_vec[0];
  unsigned hi = static_cast<unsigned>(param(0));
  unsigned lo = static_cast<unsigned>(param(1));
  return bv.extract(hi, lo);
}

// ------------------------- Class ExprOpZeroExtend ------------------------- //
ExprOpZExt::ExprOpZExt(const ExprPtr bv, const int& bit_width)
    : ExprOp(bv, bit_width) {
  ILA_ASSERT(bv->is_bv()) << "Zero-extend can only be applied to bit-vector.";
  ILA_ASSERT(bit_width >= bv->sort().bit_width())
      << "Invalid target bit-width for extend.";
  set_sort(Sort::MakeBvSort(bit_width));
}

z3::expr ExprOpZExt::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                               const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 1) << "Extend take 1 argument.";
  ILA_ASSERT(param_num() == 1) << "Extend need one parameter.";
  auto bv = expr_vec[0];
  unsigned wid = static_cast<unsigned>(param(0));
  return z3::zext(bv, wid);
}

// ------------------------- Class ExprOpIte -------------------------------- //
ExprOpImply::ExprOpImply(const ExprPtr ante, const ExprPtr cons)
    : ExprOp(ante, cons) {
  ILA_ASSERT(ante->is_bool()) << "Antecedent must be Boolean.";
  ILA_ASSERT(cons->is_bool()) << "Consequent must be Boolean.";
  set_sort(Sort::MakeBoolSort());
}

z3::expr ExprOpImply::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                                const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 2) << "Imply takes two arguments.";
  auto ante = expr_vec[0];
  auto cons = expr_vec[1];
  return z3::implies(ante, cons);
}

// ------------------------- Class ExprOpIte -------------------------------- //
ExprOpIte::ExprOpIte(const ExprPtr cnd, const ExprPtr true_expr,
                     const ExprPtr false_expr)
    : ExprOp(cnd, true_expr, false_expr) {
  ILA_ASSERT(cnd->is_bool()) << "Condition must be Boolean.";
  ILA_ASSERT(true_expr->sort() == false_expr->sort())
      << "True/false branch sort mismatch.";
  set_sort(true_expr->sort());
}

z3::expr ExprOpIte::GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                              const std::string& suffix) const {
  ILA_ASSERT(expr_vec.size() == 3) << "Ite takes 3 arguments.";
  auto cnd = expr_vec[0];
  auto t_e = expr_vec[1];
  auto t_f = expr_vec[2];
  return z3::ite(cnd, t_e, t_f);
}

} // namespace ila


/// \file
/// Source for the var expression

#include "ila/ast/expr_var.h"
#include "util/log.h"

namespace ila {

const std::string ExprVar::k_prefix_var_ = "var";

ExprVar::ExprVar() { ILA_ERROR << "Undefined ExprVar constructor.\n"; }

ExprVar::ExprVar(const std::string& name) {
  set_arity(0);
  set_sort(NULL); // FIXME
}

ExprVar::ExprVar(const std::string& name, const int& bit_width) {
  set_arity(0);
  set_sort(NULL); // FIXME
}

ExprVar::ExprVar(const std::string& name, const int& addr_width,
                 const int& data_width) {
  set_arity(0);
  set_sort(NULL); // FIXME
}

ExprVar::~ExprVar() {}

z3::expr ExprVar::GetZ3Expr(z3::context& z3_ctx,
                            const Z3ExprVec& z3expr_vec) const {
  // TODO
  return z3_ctx.bool_const("dummy bool var");
}

std::ostream& ExprVar::Print(std::ostream& out) const {
  // TODO
  return out;
}

std::ostream& ExprVar::PrintBool(std::ostream& out) const {
  // TODO
  return out;
}

std::ostream& ExprVar::PrintBv(std::ostream& out) const {
  // TODO
  return out;
}

std::ostream& ExprVar::PrintMem(std::ostream& out) const {
  // TODO
  return out;
}

} // namespace ila


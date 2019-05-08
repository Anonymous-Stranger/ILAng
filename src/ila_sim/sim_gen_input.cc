#include <ilang/ila_sim/ila_sim.h>

namespace ilang {
void IlaSim::create_input(const ExprPtr &input_expr) {
  auto input_uid = GetUidExpr(input_expr);
  if (input_uid != AST_UID_EXPR::VAR) {
    ILA_ERROR << "Error: Input should be var.";
  }
  auto input_sort = input_expr->sort();
  auto input_sort_uid = GetUidSort(input_sort);
  if (input_sort_uid == AST_UID_SORT::MEM) {
    ILA_ERROR << "Error: mem is not supported as input.";
  }
  if (input_sort_uid == AST_UID_SORT::BOOL) {
    create_bool_input(input_expr);
  }
  if (input_sort_uid == AST_UID_SORT::BV) {
    create_bv_input(input_expr);
  }
  return;
}

void IlaSim::create_bool_input(const ExprPtr &expr) {
  auto input_name_str = expr->host()->name().str() + "_" + expr->name().str();
  header_ << header_indent_ << "sc_in<bool> " << input_name_str << "_in;"
          << endl;
  header_ << header_indent_ << "bool " << input_name_str << ";" << endl;
}

void IlaSim::create_bv_input(const ExprPtr &expr) {
  auto bit_width = expr->sort()->bit_width();
  auto input_name_str = expr->host()->name().str() + "_" + expr->name().str();
  auto input_type_str = "sc_biguint<" + to_string(bit_width) + "> ";
  header_ << header_indent_ << "sc_in< " << input_type_str << "> "
          << input_name_str << "_in;" << endl;
  header_ << header_indent_ << input_type_str << input_name_str << ";" << endl;
}
}; // namespace ilang

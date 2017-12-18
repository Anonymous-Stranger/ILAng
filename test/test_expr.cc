/// \file
/// Unit test for the class Expr.

#include "ila/ast/expr.h"
#include "ila/ast/expr_const.h"
#include "ila/ast/expr_op.h"
#include "ila/ast/expr_var.h"
#include "gtest/gtest.h"

namespace ila {

TEST(ExprVar, Construct) {
  // ExprPtr var_null = std::make_shared<ExprVar>();
  ExprPtr var_bool = std::make_shared<ExprVar>("var_bool");
  ExprPtr var_bv_0 = std::make_shared<ExprVar>("var_bv_0", 8);
  ExprPtr var_bv_1 = std::make_shared<ExprVar>("var_bv_1", 8);
  ExprPtr var_mem = std::make_shared<ExprVar>("var_mem", 8, 8);

  EXPECT_TRUE(var_bool->IsExpr());
  EXPECT_FALSE(var_bv_0->IsSort());
  EXPECT_FALSE(var_bv_0->IsFunc());

  EXPECT_TRUE(var_bool->IsVar());
  EXPECT_TRUE(var_bv_0->IsVar());
  EXPECT_FALSE(var_bv_1->IsOp());
  EXPECT_FALSE(var_mem->IsConst());
}

TEST(ExprConst, Construct) {
  BoolVal val_true(true);
  BoolVal val_false("False");
  ExprPtr const_true = std::make_shared<ExprConst>(val_true);
  ExprPtr const_false = std::make_shared<ExprConst>(val_false);

  EXPECT_TRUE(const_true->IsConst());
  EXPECT_TRUE(const_false->IsExpr());
  EXPECT_TRUE(const_true->IsBool());
  EXPECT_FALSE(const_false->IsBv());

  BvVal val_0(0);
  BvVal val_1("1");
  ExprPtr const_0 = std::make_shared<ExprConst>(val_0, 8);
  ExprPtr const_1 = std::make_shared<ExprConst>(val_1, 8);

  EXPECT_TRUE(const_0->IsBv());
  EXPECT_FALSE(const_1->IsBool());
}

TEST(ExprOp, Construct) {
  ExprPtr var_bool = std::make_shared<ExprVar>("var_bool");
  ExprPtr var_bv_0 = std::make_shared<ExprVar>("var_bv_0", 8);
  ExprPtr var_bv_1 = std::make_shared<ExprVar>("var_bv_1", 8);

  ExprPtr op_and = std::make_shared<ExprOpAnd>(var_bv_0, var_bv_1);
  EXPECT_EQ("AND", std::dynamic_pointer_cast<ExprOp>(op_and)->op_name());

  ExprPtr op_or = std::make_shared<ExprOpOr>(var_bv_0, op_and);
  EXPECT_EQ("OR", std::dynamic_pointer_cast<ExprOp>(op_or)->op_name());

  ExprPtr op_eq = std::make_shared<ExprOpEq>(var_bv_1, op_or);
  EXPECT_EQ("EQ", std::dynamic_pointer_cast<ExprOp>(op_eq)->op_name());
  EXPECT_TRUE(op_eq->IsBool());
}

} // namespace ila


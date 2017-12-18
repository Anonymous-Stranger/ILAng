/// \file
/// Unit test for the class Expr.

#include "ila/ast/expr.h"
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

TEST(ExprOp, Construct) {
  ExprPtr var_bool = std::make_shared<ExprVar>("var_bool");
  ExprPtr var_bv_0 = std::make_shared<ExprVar>("var_bv_0", 8);
  ExprPtr var_bv_1 = std::make_shared<ExprVar>("var_bv_1", 8);

  ExprPtr op_and = std::make_shared<ExprOpAnd>(var_bv_0, var_bv_1);
  EXPECT_EQ("AND", std::dynamic_pointer_cast<ExprOp>(op_and)->op_name());

  ExprPtr op_or = std::make_shared<ExprOpOr>(var_bv_0, op_and);
  EXPECT_EQ("OR", std::dynamic_pointer_cast<ExprOp>(op_or)->op_name());
}

} // namespace ila


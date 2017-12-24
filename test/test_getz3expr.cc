/// \file
/// Unit test for generating z3 express for ast.

#include "ila/expr_fuse.h"
#include "ila/z3_expr_adapter.h"
#include "gtest/gtest.h"

namespace ila {

class TestZ3Expr : public ::testing::Test {
public:
  TestZ3Expr() {
    s = new z3::solver(ctx);
    gen = new Z3ExprAdapter(ctx);

    bv_var_x = ExprFuse::NewBvVar("bv_var_x", 8);
    bv_var_y = ExprFuse::NewBvVar("bv_var_y", 8);
    bv_const_0 = ExprFuse::BvConst(0, 8);
    bv_const_1 = ExprFuse::BvConst(1, 8);
    bool_var_x = ExprFuse::NewBoolVar("bool_var_x");
    bool_var_y = ExprFuse::NewBoolVar("bool_var_y");
    bool_true = ExprFuse::BoolConst(true);
    bool_false = ExprFuse::BoolConst(false);
    mem_var_x = ExprFuse::NewMemVar("mem_var_x", 8, 8);
    mem_var_y = ExprFuse::NewMemVar("mem_var_y", 8, 8);
    mem_const = ExprFuse::MemConst(0, 8, 8);
  }

  ~TestZ3Expr() {
    delete s;
    delete gen;
  }

  void SetUp() {}

  void TearDown() {}

  z3::context ctx;
  z3::solver* s;
  Z3ExprAdapter* gen;

  ExprPtr bv_var_x;
  ExprPtr bv_var_y;
  ExprPtr bv_const_0;
  ExprPtr bv_const_1;
  ExprPtr bool_var_x;
  ExprPtr bool_var_y;
  ExprPtr bool_true;
  ExprPtr bool_false;
  ExprPtr mem_var_x;
  ExprPtr mem_var_y;
  ExprPtr mem_const;

}; // class TestZ3Expr

TEST_F(TestZ3Expr, NotNot) {
  auto ast_not = ExprFuse::Not(bool_var_x);
  auto ast_notnot = ExprFuse::Not(ast_not);
  auto ast_eq = ExprFuse::Eq(bool_var_x, ast_notnot);

  auto expr_eq = gen->GetZ3Expr(ast_eq);
  s->add(!expr_eq);
  EXPECT_EQ(z3::unsat, s->check());
}

TEST_F(TestZ3Expr, NegNeg) {
  auto ast_neg = ExprFuse::Negate(bv_var_x);
  auto ast_negneg = ExprFuse::Negate(ast_neg);
  auto ast_eq = ExprFuse::Eq(bv_var_x, ast_negneg);

  auto expr_eq = gen->GetZ3Expr(ast_eq);
  s->add(!expr_eq);
  EXPECT_EQ(z3::unsat, s->check());
}

TEST_F(TestZ3Expr, LoadStore) {
  auto ast_load = ExprFuse::Load(mem_var_x, bv_var_x);
  auto ast_store = ExprFuse::Store(mem_var_x, bv_var_x, ast_load);
  auto ast_eq = ExprFuse::Eq(mem_var_x, ast_store);

  auto expr_eq = gen->GetZ3Expr(ast_eq);
  s->add(!expr_eq);
  EXPECT_EQ(z3::unsat, s->check());
}

TEST_F(TestZ3Expr, StoreLoad) {
  auto ast_store = ExprFuse::Store(mem_var_x, bv_var_x, bv_var_y);
  auto ast_load = ExprFuse::Load(ast_store, bv_var_x);
  auto ast_eq = ExprFuse::Eq(bv_var_y, ast_load);

  auto expr_eq = gen->GetZ3Expr(ast_eq);
  s->add(!expr_eq);
  EXPECT_EQ(z3::unsat, s->check());
}

} // namespace ila


/// \file
/// Unit test for unrolling a sequence of instruction.

#include "unit-include/eq_ilas.h"
#include "unit-include/simple_cpu.h"
#include "unit-include/util.h"
#include "verify/unroller.h"
#include <iostream>

namespace ila {

using namespace ExprFuse;

class TestUnroll : public ::testing::Test {
public:
  TestUnroll() {}
  ~TestUnroll() {}
  void SetUp() { DebugLog::Enable("Unroller"); }
  void TearDown() {
    SetToStdErr(0);
    DebugLog::Disable("Unroller");
  }

  EqIlaGen ila_gen_;
  z3::context ctx_;

}; // class TestUnroll

TEST_F(TestUnroll, InsteSeqFlatSubs) {
  auto m = ila_gen_.GetIlaFlat1();

  std::vector<InstrPtr> seq;
  for (size_t i = 0; i != m->instr_num(); i++) {
    seq.push_back(m->instr(i));
  }

  ListUnroll* unroller = new ListUnroll(ctx_);
  auto cstr = unroller->InstrSeqSubs(seq);
}

TEST_F(TestUnroll, InsteSeqFlatAssn) {
  auto m = ila_gen_.GetIlaFlat1();

  std::vector<InstrPtr> seq;
  for (size_t i = 0; i != m->instr_num(); i++) {
    seq.push_back(m->instr(i));
  }

  ListUnroll* unroller = new ListUnroll(ctx_);
  auto cstr = unroller->InstrSeqAssn(seq);
}

TEST_F(TestUnroll, InstrSeqFlatNone) {
  auto m = ila_gen_.GetIlaFlat1();
  std::vector<InstrPtr> seq;
  for (size_t i = 0; i != m->instr_num(); i++) {
    seq.push_back(m->instr(i));
  }

  ListUnroll* unroller = new ListUnroll(ctx_);
  auto cstr = unroller->InstrSeqNone(seq);
}

TEST_F(TestUnroll, InstrSeqSolve) {
  SetToStdErr(1);
  auto m0 = SimpleCpu("m0");
  auto m1 = SimpleCpu("m1");

  /*
   * reg0 = Load 0
   * reg1 = Load 1
   * reg2 = reg0 + reg1
   * Store reg2 2
   */
  std::vector<InstrPtr> seq0 = {m0->instr("Load"), m0->instr("Load"),
                                m0->instr("Add"), m0->instr("Store")};
  std::vector<InstrPtr> seq1 = {m1->instr("Load"), m1->instr("Load"),
                                m1->instr("Add"), m1->instr("Store")};

  auto unroller = new ListUnroll(ctx_);

  // ILA init
  for (size_t i = 0; i != m0->init_num(); i++) {
    unroller->AddInitPred(m0->init(i));
  }
  { // BMC init
    auto mem = m0->state("mem");
    unroller->AddInitPred(Eq(Load(mem, 0), GenLoad(0, 0)));
    unroller->AddInitPred(Eq(Load(mem, 1), GenLoad(1, 1)));
    unroller->AddInitPred(Eq(Load(mem, 2), GenAdd(2, 0, 1)));
    unroller->AddInitPred(Eq(Load(mem, 3), GenStore(2, 2)));
  }
  // unroll
  auto cstr0 = unroller->InstrSeqSubs(seq0);

  // ILA init
  for (size_t i = 0; i != m1->init_num(); i++) {
    unroller->AddInitPred(m1->init(i));
  }
  { // BMC init
    auto mem = m1->state("mem");
    unroller->AddInitPred(Eq(Load(mem, 0), GenLoad(0, 0)));
    unroller->AddInitPred(Eq(Load(mem, 1), GenLoad(1, 1)));
    unroller->AddInitPred(Eq(Load(mem, 2), GenAdd(2, 0, 1)));
    unroller->AddInitPred(Eq(Load(mem, 3), GenStore(2, 2)));
  }
  // unroll
  auto cstr1 = unroller->InstrSeqAssn(seq1);

  z3::solver s(ctx_);
  s.add(cstr0);
  s.add(cstr1);

  EXPECT_EQ(z3::sat, s.check());

  s.reset();

  s.add(cstr0);
  s.add(cstr1);
  // connect initial value
  Z3ExprAdapter gen(ctx_);
  for (size_t i = 0; i != m0->state_num(); i++) {
    auto var0 = m0->state(i);
    auto var1 = m1->state(var0->name().str());
    auto eq = unroller->Equal(var0, 0, var1, 0);
    s.add(eq);
  }
  // assert end value equal (take mem as example)
  auto mem0 = m0->state("mem");
  auto mem1 = m1->state("mem");
  auto prop = unroller->Equal(mem0, 4, mem1, 4);
  s.add(!prop);
  // solve
  EXPECT_EQ(z3::unsat, s.check());
}

} // namespace ila


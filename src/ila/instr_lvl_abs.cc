/// \file
/// The source for the class InstrLvlAbs.

#include "ila/instr_lvl_abs.h"

namespace ila {

InstrLvlAbs::InstrLvlAbs(const std::string& name) : Object(name) {
  ILA_WARN_IF(name == "") << "ILA name not specified...\n";
  InitObject();
}

InstrLvlAbs::~InstrLvlAbs() {}

InstrLvlAbsPtr InstrLvlAbs::NewILA(const std::string& name) {
  return std::make_shared<InstrLvlAbs>(name);
}

bool InstrLvlAbs::is_spec() const { return is_spec_; }

bool InstrLvlAbs::to_simplify() const { return simplify_; }

const ExprMngrPtr InstrLvlAbs::expr_mngr() const { return expr_mngr_; }

void InstrLvlAbs::set_spec(bool spec) { is_spec_ = spec; }

void InstrLvlAbs::set_simplify(bool simplify) { simplify_ = simplify; }

void InstrLvlAbs::set_expr_mngr(const ExprMngrPtr expr_mngr) {
  expr_mngr_ = expr_mngr;
}

void InstrLvlAbs::AddInput(const ExprPtr input_var) {
  // sanity check
  ILA_NOT_NULL(input_var);
  ILA_ASSERT(input_var->is_var()) << "Register non-var to Inputs\n";
  // should be the first
  auto name = input_var->name();
  auto pos = inputs_.find(name);
  ILA_ASSERT(pos == inputs_.end()) << "Input variable " << input_var
                                   << " has been declared.\n";
  // register to the simplifier
  auto var = expr_mngr_->Simplify(input_var, simplify_);
  // register to Inputs
  inputs_.push_back(name, var);
}

void InstrLvlAbs::AddState(const ExprPtr state_var) {
  // sanity check
  ILA_NOT_NULL(state_var);
  ILA_ASSERT(state_var->is_var()) << "Register non-var to States\n";
  // should be the first
  auto name = state_var->name();
  auto pos = states_.find(name);
  ILA_ASSERT(pos == states_.end()) << "State variable " << state_var
                                   << "has been declared.\n";
  // register to the simplifier
  auto var = expr_mngr_->Simplify(state_var, simplify_);
  // register to States
  states_.push_back(name, var);
}

void InstrLvlAbs::AddInit(const ExprPtr cntr_expr) {
  // sanity check
  ILA_NOT_NULL(cntr_expr);
  ILA_ASSERT(cntr_expr->is_bool()) << "Initial condition must be Boolean.\n";
  // simplify
  auto cntr = expr_mngr_->Simplify(cntr_expr, simplify_);
  // register to Initial conditions
  inits_.push_back(cntr);
}

void InstrLvlAbs::SetFetch(const ExprPtr fetch_expr) {
  // sanity check
  ILA_NOT_NULL(fetch_expr);
  ILA_ASSERT(fetch_expr->is_bv()) << "Fetch function must be bit-vector.\n";
  // should be the first
  ILA_ASSERT(!fetch_) << "Fetch function has been assigned.\n";
  // simplify
  auto fetch = expr_mngr_->Simplify(fetch_expr, simplify_);
  // set as fetch function
  fetch_ = fetch;
}

void InstrLvlAbs::SetValid(const ExprPtr valid_expr) {
  // sanity check
  ILA_NOT_NULL(valid_expr);
  ILA_ASSERT(valid_expr->is_bool()) << "Valid function must be Boolean.\n";
  // should be the first
  ILA_ASSERT(!valid_) << "Valid function has been assigned.\n";
  // simplify
  auto valid = expr_mngr_->Simplify(valid_expr, simplify_);
  // set as valid function
  valid_ = valid;
}

void InstrLvlAbs::AddInstr(const InstrPtr instr) {
  ILA_NOT_NULL(instr);
  // set simplifier to the instruction
  instr->set_mngr(expr_mngr_);
  instr->set_simplify(simplify_);
  // register the instruction and idx
  auto name = instr->name();
  instrs_.push_back(name, instr);
}

void InstrLvlAbs::AddChild(const InstrLvlAbsPtr child) {
  ILA_NOT_NULL(child);
  /// register the child-ILA and idx
  auto name = child->name();
  childs_.push_back(name, child);
}

ExprPtr InstrLvlAbs::NewBoolInput(const std::string& name) {
  ExprPtr bool_input = ExprFuse::NewBoolVar(name);
  AddInput(bool_input);
  return bool_input;
}

ExprPtr InstrLvlAbs::NewBvInput(const std::string& name, const int& bit_width) {
  ExprPtr bv_input = ExprFuse::NewBvVar(name, bit_width);
  AddInput(bv_input);
  return bv_input;
}

ExprPtr InstrLvlAbs::NewBoolState(const std::string& name) {
  ExprPtr bool_state = ExprFuse::NewBoolVar(name);
  AddState(bool_state);
  return bool_state;
}

ExprPtr InstrLvlAbs::NewBvState(const std::string& name, const int& bit_width) {
  ExprPtr bv_state = ExprFuse::NewBvVar(name, bit_width);
  AddState(bv_state);
  return bv_state;
}

ExprPtr InstrLvlAbs::NewMemState(const std::string& name, const int& addr_width,
                                 const int& data_width) {
  ExprPtr mem_state = ExprFuse::NewMemVar(name, addr_width, data_width);
  AddState(mem_state);
  return mem_state;
}

InstrPtr InstrLvlAbs::NewInstr(const std::string& name) {
  InstrPtr instr = Instr::NewInstr(name);
  AddInstr(instr);
  return instr;
}

InstrLvlAbsPtr InstrLvlAbs::NewChild(const std::string& name) {
  InstrLvlAbsPtr child = NewILA(name);
  AddChild(child);
  return child;
}

ExprPtr InstrLvlAbs::GetState(const std::string& name) const {
  auto pos = states_.find(Symbol(name));
  if (pos == states_.end())
    return NULL;
  else
    return pos->second;
}

ExprPtr InstrLvlAbs::GetInput(const std::string& name) const {
  auto pos = inputs_.find(Symbol(name));
  if (pos == inputs_.end())
    return NULL;
  else
    return pos->second;
}

ExprPtr InstrLvlAbs::GetFetch() const { return fetch_; }

ExprPtr InstrLvlAbs::GetValid() const { return valid_; }

InstrPtr InstrLvlAbs::GetInstr(const std::string& name) const {
  auto pos = instrs_.find(Symbol(name));
  if (pos == instrs_.end()) {
    return NULL;
  } else {
    return pos->second;
  }
}

InstrLvlAbsPtr InstrLvlAbs::GetChild(const std::string& name) const {
  auto pos = childs_.find(Symbol(name));
  if (pos == childs_.end()) {
    return NULL;
  } else {
    return pos->second;
  }
}

bool InstrLvlAbs::Check() const {
  // TODO
  // check input
  // check state
  // check init
  // check fetch
  // check valid
  // check instr
  // check child-ILA?
  // check sequencing
  return true;
}

void InstrLvlAbs::Simplify() {
  // TODO
  // init
  // fetch
  // valid
  // instr
  // child-ILA?
}

void InstrLvlAbs::MergeChild() {
  // TODO
  // merge shared states
  // merge simplifier
}

std::ostream& InstrLvlAbs::Print(std::ostream& out) const {
  out << "ILA." << name();
  // TODO
  return out;
}

std::ostream& operator<<(std::ostream& out, InstrLvlAbs& ila) {
  return ila.Print(out);
}

void InstrLvlAbs::InitObject() {
  inputs_.clear();
  states_.clear();
  inits_.clear();
  fetch_ = NULL;
  valid_ = NULL;
  instrs_.clear();
  childs_.clear();

  is_spec_ = true;
  simplify_ = true;

  expr_mngr_ = std::make_shared<ExprMngr>();
  expr_mngr_->clear();
}

} // namespace ila


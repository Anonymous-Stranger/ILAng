/// \file
/// The implementation of the abstraction to ILA converter.

#include <ilang/synth-interface/synth_abs_converter.h>
#include <ilang/util/log.h>

namespace ilang {

SynthAbsConverter::SynthAbsConverter() {}

SynthAbsConverter::~SynthAbsConverter() {}

SynthAbsConverterPtr SynthAbsConverter::New() {
  return std::make_shared<SynthAbsConverter>();
}

InstrLvlAbsPtr SynthAbsConverter::Convert(const ilasynth::Abstraction& abs) {
  // destination ILA
  auto model_name = abs.getName();
  auto m = InstrLvlAbs::New(model_name);

  // port basic abstraction definition
  PortInputs(abs, m);
  PortStates(abs, m);
  PortFuncs(abs, m);

  PortValid(abs, m);
  PortFetch(abs, m);

  PortInits(abs, m);

  // TODO instruction

  // TODO child-ILAs

  return m;
}

ExprPtr
SynthAbsConverter::ConvertSynthNodeToIlangExpr(const ilasynth::nptr_t& node,
                                               const InstrLvlAbsPtr& ila) {
  ILA_CHECK(!node_expr_map_.empty()) << "Empty map -- missing leaf vars";
  ILA_NOT_NULL(node);
  auto n = node.get();

  // return if already visit
  auto pos = node_expr_map_.find(n);
  if (pos != node_expr_map_.end()) {
    return pos->second;
  }

  // create new Expr w.r.t the node
  auto CnvtNode2Expr = [this](const ilasynth::Node* nl) { CnvtNodeToExpr(nl); };

  node->depthFirstVisit(CnvtNode2Expr);

  // make sure Expr has been converted/generated
  pos = node_expr_map_.find(n);
  ILA_ASSERT(pos != node_expr_map_.end()) << "Fail converting " << n->getName();

  return pos->second;
}

void SynthAbsConverter::PortInputs(const ilasynth::Abstraction& abs,
                                   const InstrLvlAbsPtr& ila) {
  auto inps_synth = abs.getInps();
  for (auto it : inps_synth) {
    auto node = it.second.var;
    auto type = node->getType();
    auto name = node->getName();
    ILA_WARN_IF(name != it.first) << name << " != " << it.first;

    // create input var accordingly
    switch (type.type) {

    case ilasynth::NodeType::Type::BOOL:
      ila->NewBoolInput(name);
      break;

    case ilasynth::NodeType::Type::BITVECTOR:
      ila->NewBvInput(name, type.bitWidth);
      break;

    case ilasynth::NodeType::Type::MEM:
      ila->NewMemInput(name, type.addrWidth, type.dataWidth);
      break;

    default:
      ILA_ERROR << "Input of type other than Bool/Bv/Mem not supported.";
      break;
    };

#if 0
    if (type.isBool()) {
      ila->NewBoolInput(name);
    } else if (type.isBitvector()) {
      ila->NewBvInput(name, type.bitWidth);
    } else {
      ILA_ASSERT(type.isMem())
          << "Unknown type " << type << " for input " << name;
      ila->NewMemInput(name, type.addrWidth, type.dataWidth);
    }
#endif

    // update book keeping
    ILA_ASSERT(node_expr_map_.find(node.get()) == node_expr_map_.end());
    node_expr_map_[node.get()] = ila->input(name);

    ILA_DLOG("SynthImport") << "Input: " << ila->input(name);
  }
}

void SynthAbsConverter::PortStates(const ilasynth::Abstraction& abs,
                                   const InstrLvlAbsPtr& ila) {
  // bool
  auto bits_synth = abs.getBits();
  for (auto it : bits_synth) {
    auto node = it.second.var;
    auto type = node->getType();
    auto name = node->getName();

    ILA_ASSERT(type.isBool());
    ILA_WARN_IF(name != it.first) << name << " != " << it.first;

    auto var = ila->NewBoolState(name);

    ILA_ASSERT(node_expr_map_.find(node.get()) == node_expr_map_.end());
    node_expr_map_[node.get()] = var;

    ILA_DLOG("SynthImport") << "Bool Var: " << ila->state(name);
  }

  // bv
  auto regs_synth = abs.getRegs();
  for (auto it : regs_synth) {
    auto node = it.second.var;
    auto type = node->getType();
    auto name = node->getName();

    ILA_ASSERT(type.isBitvector());
    ILA_WARN_IF(name != it.first) << name << " != " << it.first;

    auto var = ila->NewBvState(name, type.bitWidth);

    ILA_ASSERT(node_expr_map_.find(node.get()) == node_expr_map_.end());
    node_expr_map_[node.get()] = var;

    ILA_DLOG("SynthImport") << "Bv Var: " << ila->state(name);
  }

  // mem
  auto mems_synth = abs.getMems();
  for (auto it : mems_synth) {
    auto node = it.second.var;
    auto type = node->getType();
    auto name = node->getName();

    ILA_ASSERT(type.isMem());
    ILA_WARN_IF(name != it.first) << name << " != " << it.first;

    auto var = ila->NewMemState(name, type.addrWidth, type.dataWidth);

    ILA_ASSERT(node_expr_map_.find(node.get()) == node_expr_map_.end());
    node_expr_map_[node.get()] = var;

    ILA_DLOG("SynthImport") << "Mem Var: " << ila->state(name);
  }
}

void SynthAbsConverter::PortValid(const ilasynth::Abstraction& abs,
                                  const InstrLvlAbsPtr& ila) {
  auto valid_synth = abs.getFetchValidNode();
  auto type = valid_synth->getType();
  ILA_WARN_IF(!type.isBool()) << "Non-Bool valid function " << valid_synth;

  auto valid_ilang = ConvertSynthNodeToIlangExpr(valid_synth, ila);

  return;
}

void SynthAbsConverter::PortFetch(const ilasynth::Abstraction& abs,
                                  const InstrLvlAbsPtr& ila) {
  // TODO
  return;
}

void SynthAbsConverter::PortInits(const ilasynth::Abstraction& abs,
                                  const InstrLvlAbsPtr& ila) {
  // TODO
  return;
}

void SynthAbsConverter::PortFuncs(const ilasynth::Abstraction& abs,
                                  const InstrLvlAbsPtr& ila) {
  auto funs_synth = abs.getFuns();
  for (auto it : funs_synth) {
    auto node = it.second.var;

    auto type = node->getType();
    ILA_CHECK(type.isFunc());

    auto name = node->getName();
    ILA_WARN_IF(name != it.first) << name << " != " << it.first;

    // output sort
    ILA_CHECK(type.bitWidth > 0) << "Invalid output width " << type.bitWidth;
    auto out_sort = Sort::MakeBvSort(type.bitWidth);

    // argument sort
    std::vector<decltype(Sort::MakeBvSort(1))> args_sort = {};
    auto& args_width = type.argsWidth;
    for (auto i = 0; i != args_width.size(); i++) {
      args_sort.push_back(Sort::MakeBvSort(args_width.at(i)));
    }

    // create the func object
    auto func = Func::New(name, out_sort, args_sort);
    ILA_DLOG("SynthImport") << "Fun: " << func;

    // add to the mapping
    ILA_ASSERT(node_func_map_.find(node.get()) == node_func_map_.end());
    node_func_map_[node.get()] = func;
  }

  return;
}

void SynthAbsConverter::PortDecodeFuncs(const ilasynth::Abstraction& abs,
                                        const InstrLvlAbsPtr& ila) {
  // TODO
  return;
}

void SynthAbsConverter::PortNextStateFuncs(const ilasynth::Abstraction& abs,
                                           const InstrLvlAbsPtr& ila) {
  // TODO
  return;
}

void SynthAbsConverter::CnvtNodeToExpr(const ilasynth::Node* n) {
  ILA_NOT_NULL(n);

  // return if already visited
  if (node_expr_map_.find(n) != node_expr_map_.end()) {
    return;
  }
  if (node_func_map_.find(n) != node_func_map_.end()) {
    return;
  }

  // Only convert consts and operators, since we need host info for vars.
  // vars should and must be already created.
  if (n->isConstant()) {
    CnvtNodeToExprConst(n);
  } else {
    auto type = n->getType();
    switch (type.type) {
    case ilasynth::NodeType::Type::BOOL:
      CnvtNodeToExprBoolOp(n);
      break;
    case ilasynth::NodeType::Type::BITVECTOR:
      CnvtNodeToExprBvOp(n);
      break;
    case ilasynth::NodeType::Type::MEM:
      CnvtNodeToExprMemOp(n);
      break;
    default:
      ILA_ERROR << "Op of type other than Bool/Bv/Mem not supported.";
      break;
    };
  }

  return;
}

void SynthAbsConverter::CnvtNodeToExprConst(const ilasynth::Node* n) {
  // place holder for the result
  decltype(ExprFuse::BoolConst(true)) expr = NULL;

  auto type = n->getType();
  if (type.isBool()) {
    auto val = true; // FIXME
    expr = ExprFuse::BoolConst(val);
  } else if (type.isBitvector()) {
    auto val = 0; // FIXME
    expr = ExprFuse::BvConst(val, type.bitWidth);
  } else {
    ILA_ASSERT(type.isMem());
    auto def_val = 0; // FIXME
    expr = ExprFuse::MemConst(def_val, type.addrWidth, type.dataWidth);
  }

  ILA_NOT_NULL(expr) << "Fail converting constant node " << n->getName();
  node_expr_map_[n] = expr;

  return;
}

void SynthAbsConverter::CnvtNodeToExprBoolOp(const ilasynth::Node* n) {
  // get input arguments
  ExprPtrVec expr_args = {};
  for (auto i = 0; i != n->nArgs(); i++) {
    auto node_arg_i = n->arg(i);
    auto pos = node_expr_map_.find(node_arg_i.get());
    ILA_ASSERT(pos != node_expr_map_.end()) << "Invalid DF-visit";
    // XXX func?

    auto expr_arg_i = pos->second;
    expr_args.push_back(expr_arg_i);
  }

  // construct Expr
  auto op_ptr = dynamic_cast<const ilasynth::BoolOp*>(n);
  ILA_NOT_NULL(op_ptr) << "Fail casting " << n->getName() << " to Bool Op";

  decltype(ExprFuse::BoolConst(true)) expr = NULL;

  switch (op_ptr->getOp()) {
  case ilasynth::BoolOp::Op::NOT:
    expr = ExprFuse::Not(expr_args.at(0));
    break;
  case ilasynth::BoolOp::Op::AND:
    expr = ExprFuse::And(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::OR:
    expr = ExprFuse::Or(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::XOR:
    expr = ExprFuse::Xor(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::XNOR: {
    auto tmp_xor = ExprFuse::Xor(expr_args.at(0), expr_args.at(1));
    expr = ExprFuse::Not(tmp_xor);
    break;
  }
  case ilasynth::BoolOp::Op::NAND: {
    auto tmp_and = ExprFuse::And(expr_args.at(0), expr_args.at(1));
    expr = ExprFuse::Not(tmp_and);
    break;
  }
  case ilasynth::BoolOp::Op::NOR: {
    auto tmp_or = ExprFuse::Or(expr_args.at(0), expr_args.at(1));
    expr = ExprFuse::Not(tmp_or);
    break;
  }
  case ilasynth::BoolOp::Op::IMPLY:
    expr = ExprFuse::Imply(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::SLT:
    expr = ExprFuse::Lt(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::SGT:
    expr = ExprFuse::Gt(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::SLE:
    expr = ExprFuse::Le(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::SGE:
    expr = ExprFuse::Ge(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::ULT:
    expr = ExprFuse::Ult(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::UGT:
    expr = ExprFuse::Ugt(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::ULE:
    expr = ExprFuse::Ule(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::UGE:
    expr = ExprFuse::Uge(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::EQUAL:
    expr = ExprFuse::Eq(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::DISTINCT:
    expr = ExprFuse::Ne(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BoolOp::Op::IF:
    expr = ExprFuse::Ite(expr_args.at(0), expr_args.at(1), expr_args.at(2));
    break;
  default:
    ILA_ERROR << "Cannot find corresponding Bool Op for " << n->getName();
    break;
  };

  ILA_NOT_NULL(expr) << "Fail converting Bool Op node " << n->getName();
  node_expr_map_[n] = expr;

  return;
}

void SynthAbsConverter::CnvtNodeToExprBvOp(const ilasynth::Node* n) {
  // get input arguments
  ExprPtrVec expr_args = {};
  for (auto i = 0; i != n->nArgs(); i++) {
    auto node_arg_i = n->arg(i);
    auto pos = node_expr_map_.find(node_arg_i.get());
    ILA_ASSERT(pos != node_expr_map_.end()) << "Invalid DF-visit";
    // XXX func?

    auto expr_arg_i = pos->second;
    expr_args.push_back(expr_arg_i);
  }

  // construct Expr
  auto op_ptr = dynamic_cast<const ilasynth::BitvectorOp*>(n);
  ILA_NOT_NULL(op_ptr) << "Fail casting " << n->getName() << " to Bv Op";

  decltype(ExprFuse::BvConst(1, 1)) expr = NULL;

  switch (op_ptr->getOp()) {
  case ilasynth::BitvectorOp::Op::NEGATE:
    expr = ExprFuse::Negate(expr_args.at(0));
    break;
  case ilasynth::BitvectorOp::Op::COMPLEMENT:
    expr = ExprFuse::Complement(expr_args.at(0));
    break;
  case ilasynth::BitvectorOp::Op::LROTATE:
    ILA_ERROR << "LROTATE not implemented.";
    break;
  case ilasynth::BitvectorOp::Op::RROTATE:
    ILA_ERROR << "RROTATE not implemented.";
    break;
  case ilasynth::BitvectorOp::Op::Z_EXT:
    expr = ExprFuse::ZExt(expr_args.at(0), op_ptr->param(0));
    break;
  case ilasynth::BitvectorOp::Op::S_EXT:
    expr = ExprFuse::SExt(expr_args.at(0), op_ptr->param(0));
    break;
  case ilasynth::BitvectorOp::Op::EXTRACT:
    expr =
        ExprFuse::Extract(expr_args.at(0), op_ptr->param(0), op_ptr->param(1));
    break;
  case ilasynth::BitvectorOp::Op::ADD:
    expr = ExprFuse::Add(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::SUB:
    expr = ExprFuse::Sub(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::AND:
    expr = ExprFuse::And(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::OR:
    expr = ExprFuse::Or(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::XOR:
    expr = ExprFuse::Xor(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::XNOR: {
    auto tmp_xor = ExprFuse::Xor(expr_args.at(0), expr_args.at(1));
    expr = ExprFuse::Not(tmp_xor);
    break;
  }
  case ilasynth::BitvectorOp::Op::NAND: {
    auto tmp_and = ExprFuse::And(expr_args.at(0), expr_args.at(1));
    expr = ExprFuse::Not(tmp_and);
    break;
  }
  case ilasynth::BitvectorOp::Op::NOR: {
    auto tmp_or = ExprFuse::Or(expr_args.at(0), expr_args.at(1));
    expr = ExprFuse::Not(tmp_or);
    break;
  }
  case ilasynth::BitvectorOp::Op::SDIV:
    ILA_ERROR << "SDIV not implemented.";
    break;
  case ilasynth::BitvectorOp::Op::UDIV:
    ILA_ERROR << "UDIV not implemented.";
    // expr = ExprFuse::Div(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::SREM:
    ILA_ERROR << "SREM not implemented.";
    break;
  case ilasynth::BitvectorOp::Op::UREM:
    ILA_ERROR << "UREM not implemented.";
    // expr = ExprFuse::Rem(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::SMOD:
    ILA_ERROR << "SMOD not implemented.";
    break;
  case ilasynth::BitvectorOp::Op::SHL:
    expr = ExprFuse::Shl(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::LSHR:
    expr = ExprFuse::Lshr(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::ASHR:
    expr = ExprFuse::Ashr(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::MUL:
    ILA_ERROR << "MUL not implemented.";
    // expr = ExprFuse::Mul(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::CONCAT:
    expr = ExprFuse::Concat(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::GET_BIT:
    expr =
        ExprFuse::Extract(expr_args.at(0), op_ptr->param(0), op_ptr->param(0));
    break;
  case ilasynth::BitvectorOp::Op::READMEM:
    expr = ExprFuse::Load(expr_args.at(0), expr_args.at(1));
    break;
  case ilasynth::BitvectorOp::Op::READMEMBLOCK:
    ILA_ERROR << "READMEMBLOCK not implemented.";
    break;
  case ilasynth::BitvectorOp::Op::IF:
    expr = ExprFuse::Ite(expr_args.at(0), expr_args.at(1), expr_args.at(2));
    break;
  case ilasynth::BitvectorOp::Op::APPLY_FUNC:
    ILA_ERROR << "APPLY_FUNC not implemented.";
    break;
  default:
    ILA_ERROR << "Cannot find corresponding Bv Op for " << n->getName();
    break;
  };

  ILA_NOT_NULL(expr) << "Fail converting Bv Op node " << n->getName();
  node_expr_map_[n] = expr;

  return;
}

void SynthAbsConverter::CnvtNodeToExprMemOp(const ilasynth::Node* n) {
  // TODO
  return;
}

} // namespace ilang


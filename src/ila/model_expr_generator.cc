/// \file
/// Source for the model z3 expression generator.

#include "ila/model_expr_generator.h"

namespace ila {

ModelExprGen::ModelExprGen(z3::context& ctx)
    : ctx_(ctx), gen_(Z3ExprAdapter(ctx)) {}

ModelExprGen::~ModelExprGen() {}

void ModelExprGen::set_z3_simplify(bool z3_smpl) { gen_.set_simplify(z3_smpl); }

z3::expr ModelExprGen::Node(const ExprPtr node, const std::string& prefix,
                            const std::string& suffix) {
  // TODO
  return ctx_.bool_val(false);
}

Z3ExprMapPtr ModelExprGen::Instr(const InstrPtr instr,
                                 const std::string& prefix,
                                 const std::string& suffix) {
  // TODO
  return NULL;
}

} // namespace ila


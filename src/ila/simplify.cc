/// \file
/// Source for the class ExprMngr

#include "ila/simplify.h"

namespace ila {

ExprMngr::ExprMngr() {}

ExprMngr::~ExprMngr() {}

void ExprMngr::Reset() { map_.clear(); }

ExprPtr ExprMngr::Simplify(const ExprPtr node) {
  // TODO
  return node;
}

} // namespace ila


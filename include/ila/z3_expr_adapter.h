/// \file
/// Header for the class Z3ExprAdapter

#ifndef __Z3_EXPR_ADAPTER_H__
#define __Z3_EXPR_ADAPTER_H__

#include "ila/expr_fuse.h"
#include "z3++.h"
#include <unordered_map>

/// \namespace ila
namespace ila {

/// \brief The class for generating z3 expression from an ILA.
class Z3ExprAdapter {
public:
  // ------------------------- CONSTRUCTOR/DESTRUCTOR ----------------------- //
  /// Constructor.
  Z3ExprAdapter(z3::context& ctx);
  /// ~Default destructor.
  ~Z3ExprAdapter();

  /// Type for caching the generated expressions.
  typedef std::unordered_map<const ExprPtrRaw, z3::expr, ExprHash> ExprMap;

  // ------------------------- METHODS -------------------------------------- //
  /// Get the z3 expression of the AST node.
  z3::expr GetExpr(const ExprPtr expr, const std::string& suffix = "");

  /// Function object for getting z3 expression.
  void operator()(const ExprPtrRaw expr);

private:
  // ------------------------- MEMBERS -------------------------------------- //
  /// The underlying z3 context.
  z3::context& ctx_;
  /// Container for cacheing intermediate expressions.
  ExprMap expr_map_;
  /// Name suffix for each expression generation (e.g. time frame)
  std::string suffix_ = "";

  // ------------------------- HELPERS -------------------------------------- //
  /// Insert the z3 expression of the given node into the map.
  void PopulateExprMap(const ExprPtrRaw expr);

}; // class Z3ExprAdapter

} // namespace ila

#endif // __Z3_EXPR_ADAPTER_H__


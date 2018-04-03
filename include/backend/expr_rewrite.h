/// \file
/// Header for function object for rewriting Expr.

#ifndef EXPR_REWRITE_H__
#define EXPR_REWRITE_H__

#include "ila/expr_fuse.h"

namespace ila {

class FuncObjRewrite {
public:
  FuncObjRewrite(const ExprMap& rule) : rule_(rule) {}

  inline ExprPtr get(const ExprPtr e) const {
    auto pos = rule_.find(e);
    ILA_ASSERT(pos != rule_.end()) << e << " not found";
    return pos->second;
  }

  bool pre(const ExprPtr e) const;

  void post(const ExprPtr e);

private:
  ExprMap rule_;

  ExprPtr Rewrite(const ExprPtr e) const;

  ExprPtr RewriteOp(const ExprPtr e) const;

}; // class DfsFuncObjRewrite

} // namespace ila

#endif // EXPR_REWRITE_H__


/// \file
/// Header for the class Expr

#ifndef __EXPR_H__
#define __EXPR_H__

#include "ila/ast/ast.h"
#include "ila/ast/sort.h"
#include "z3++.h"
#include <memory>
#include <ostream>
#include <string>
#include <vector>

/// \namespace ila
namespace ila {

/// \class Expr
/// The class for expression, which is the basic type for variables,
/// constraints, state update expressions, etc.
class Expr : public Ast {
public:
  // ------------------------- CONSTRUCTOR/DESTRUCTOR ----------------------- //
  /// Default constructor.
  Expr();
  /// Default destructor.
  virtual ~Expr();

  /// \def ExprPtr
  typedef std::shared_ptr<Expr> ExprPtr;
  /// \def ExprPtrVec
  typedef std::vector<ExprPtr> ExprPtrVec;
  /// \def Z3ExprVec Expr specific type for vector of z3 expr
  typedef std::vector<z3::expr> Z3ExprVec;

  // ------------------------- ACCESSORS/MUTATORS --------------------------- //
  /// Return the prefix of the expression (depends on type).
  virtual const std::string& prefix() const { return k_prefix_expr_; }

  /// Return the pointer of the sort.
  const Sort& sort() const;
  /// Return the arity.
  const size_t& arity() const;
  /// Return the i-th argument.
  ExprPtr arg(const size_t& i) const;
  /// Return the number of parameters.
  const size_t& num_param() const;
  /// Return the i-th paramter.
  const int& param(const size_t& i) const;

  /// Set the sort of the expression.
  void set_sort(const Sort& sort);
  /// Set the arity and reserve/resize the argument vector.
  void set_arity(const size_t& arity);
  /// Set the arguments.
  void set_args(const ExprPtrVec& args);
  /// Set the number of parameters.
  void set_num_param(const size_t& num_param);
  /// Set the parameters.
  void set_params(const std::vector<int> params);

  // ------------------------- METHODS -------------------------------------- //
  /// Is type expr (object).
  bool IsExpr() const { return true; }

  /// Return true if this is a Boolean expression.
  bool IsBool() const { return sort_.IsBool(); }
  /// Return true if this is a Bitvector expression.
  bool IsBv() const { return sort_.IsBv(); }
  /// Return true if this is an Array expression.
  bool IsMem() const { return sort_.IsMem(); }
  /// Return true if this is an Application expression.
  bool IsApp() const { return sort_.IsApp(); }

  /// Return true if this is a constant.
  virtual bool IsConst() const { return false; }
  /// Return true if this is a variable.
  virtual bool IsVar() const { return false; }
  /// Return true if this is an operation.
  virtual bool IsOp() const { return false; }

  /// Return the z3 expression for the node.
  virtual z3::expr GetZ3Expr(z3::context& z3_ctx,
                             const Z3ExprVec& z3expr_vec) const = 0;

  /// Output to stream.
  virtual std::ostream& Print(std::ostream& out) const = 0;

  /// Compare two expression with object.
  static bool Equal(const Expr& lhs, const Expr& rhs);
  /// Compare two expression with pointer.
  static bool Equal(const ExprPtr lhs, const ExprPtr rhs);
  /// Overload comparison.
  friend bool operator==(const Expr& lhs, const Expr& rhs);
  /// Overload output stream operator
  friend std::ostream& operator<<(std::ostream& out, const Expr& expr);

  /// Templated visitor: visit each node in a depth-first order and apply the
  /// function object F on it.
  template <class F> void DepthFirstVisit(F& func) const {
    size_t n = arity();
    for (size_t i = 0; i != n; i++) {
      const ExprPtr arg_i = this->arg(i);
      arg_i->DepthFirstVisit<F>(func);
    }
    func(this);
  }

private:
  // ------------------------- MEMBERS -------------------------------------- //
  /// The sort of the expr.
  Sort sort_;
  /// Number of arguments.
  size_t arity_;
  /// Vector of arguments.
  ExprPtrVec args_;
  /// Number of parameters.
  size_t num_param_;
  /// Vector of parameters.
  std::vector<int> params_;

  /// Static counter for expressions.
  static unsigned coutner_;
  /// Static prefix for intermediate expression name.
  static const std::string k_prefix_expr_;

  // ------------------------- HELPERS -------------------------------------- //
  /// Return true if the expression is well-sorted.
  bool IsWellSorted() const;

}; // class Expr

typedef Expr::ExprPtr ExprPtr;

} // namespace ila

#endif // __EXPR_H__


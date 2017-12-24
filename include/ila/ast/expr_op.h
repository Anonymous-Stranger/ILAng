/// \file
/// Header for the class ExprOp

#ifndef __EXPR_OP_H__
#define __EXPR_OP_H__

#include "ila/ast/expr.h"
#include "util/log.h"
#include <string>

/// \namespace ila
namespace ila {

/// \brief Expression for operations, e.g. AND, OR, ADD, etc. Operations are
/// non-terminating nodes in the AST.
class ExprOp : public Expr {
public:
  // ------------------------- CONSTRUCTOR/DESTRUCTOR ----------------------- //
  /// Default constructor. DO NOT USE.
  ExprOp();
  /// Constructor for unary operators.
  ExprOp(const ExprPtr arg);
  /// Constructor for binary operators.
  ExprOp(const ExprPtr arg0, const ExprPtr arg1);
  /// Default destructor.
  virtual ~ExprOp();

  // ------------------------- ACCESSORS/MUTATORS --------------------------- //
  /// Return the prefix of the variable expression.
  const std::string& prefix() const { return k_prefix_op_; }
  /// Return the name of the operation.
  virtual std::string op_name() const = 0;

  /// Return trus since this is an operation.
  bool is_op() const { return true; }

  // ------------------------- METHODS -------------------------------------- //
  /// Return the z3 expression for the node.
  virtual z3::expr GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                             const std::string& suffix = "") const = 0;

  /// Output to stream.
  virtual std::ostream& Print(std::ostream& out) const = 0;

protected:
  // ------------------------- HELPERS -------------------------------------- //
  /// Derived the sort for binary operations.
  Sort GetSortBinaryOperation(const Sort& s0, const Sort& s1);
  /// Derived the sort for binary comparisons.
  Sort GetSortBinaryComparison(const Sort& s0, const Sort& s1);

  /// Print unary operations.
  std::ostream& PrintUnaryOp(std::ostream& out,
                             const std::string& op_name) const;
  /// Print binary operations.
  std::ostream& PrintBinaryOp(std::ostream& out,
                              const std::string& op_name) const;
  /// Print n-ary operations.
  std::ostream& PrintNnaryOp(std::ostream& out, ExprPtr op);

private:
  // ------------------------- MEMBERS -------------------------------------- //
  /// Static prefix for the operation expressions.
  static const std::string k_prefix_op_;
  /// Static counter for un-named op expressions.
  static unsigned counter_;

}; // class ExprOp

/******************************************************************************/
// Unary
/******************************************************************************/

/// \brief The wrapper for unary negate operation "-".
class ExprOpNeg : public ExprOp {
public:
  /// Constructor for Negate operation.
  ExprOpNeg(const ExprPtr arg);
  std::string op_name() const { return "NEGATE"; }
  z3::expr GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                     const std::string& suffix) const;
  std::ostream& Print(std::ostream& out) const;
}; // class ExprOpNeg

/// \brief The wrapper for unary not operation "!". (bool only)
class ExprOpNot : public ExprOp {
public:
  /// Constructor for Not operation.
  ExprOpNot(const ExprPtr arg);
  std::string op_name() const { return "NOT"; }
  z3::expr GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                     const std::string& suffix) const;
  std::ostream& Print(std::ostream& out) const;
}; // class ExprOpNot

/// \brief The wrapper for unary bit-wise complement "~". (bv only)
class ExprOpCompl : public ExprOp {
public:
  /// Constructor for Complement operation.
  ExprOpCompl(const ExprPtr arg);
  std::string op_name() const { return "COMPLEMENT"; }
  z3::expr GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                     const std::string& suffix) const;
  std::ostream& Print(std::ostream& out) const;
}; // class ExprOpCompl

/******************************************************************************/
// Binary operation
/******************************************************************************/

/// \brief The wrapper for binary logical AND operation "&".
class ExprOpAnd : public ExprOp {
public:
  /// Constructor for AND operation.
  ExprOpAnd(const ExprPtr arg0, const ExprPtr arg1);
  std::string op_name() const { return "AND"; }
  z3::expr GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                     const std::string& suffix) const;
  std::ostream& Print(std::ostream& out) const;
}; // class ExprOpAnd

/// \brief The wrapper for binary logical OR operation "|".
class ExprOpOr : public ExprOp {
public:
  /// Constructor for OR operation.
  ExprOpOr(const ExprPtr arg0, const ExprPtr arg1);
  std::string op_name() const { return "OR"; }
  z3::expr GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                     const std::string& suffix) const;
  std::ostream& Print(std::ostream& out) const;
}; // class ExprOpOr

class ExprOpXor : public ExprOp {
public:
  /// Constructor for XOR operation.
  ExprOpXor(const ExprPtr arg0, const ExprPtr arg1);
  std::string op_name() const { return "XOR"; }
  z3::expr GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                     const std::string& suffix) const;
  std::ostream& Print(std::ostream& out) const;
}; // class ExprOpXor

// TODO ExprOpShl

// TODO ExprOpShr

// TODO ExprOpAdd

// TODO ExprOpSub

// TODO ExprOpDiv

// TODO ExprOpRem

// TODO ExprOpMod

// TODO ExprOpMul

/******************************************************************************/
// Binary comparison
/******************************************************************************/

/// \brief The class wrapper for binary comparison EQ "==".
class ExprOpEq : public ExprOp {
public:
  /// Constructor for Equal comparison.
  ExprOpEq(const ExprPtr arg0, const ExprPtr arg1);
  std::string op_name() const { return "EQ"; }
  z3::expr GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                     const std::string& suffix) const;
  std::ostream& Print(std::ostream& out) const;
}; // class ExprOpEq

// TODO ExprOpNe

// TODO ExprOpLt

// TODO ExprOpGt

// TODO ExprOpLe

// TODO ExprOpGe

/******************************************************************************/
// Memory
/******************************************************************************/

/// \brief The class wrapper for memory load.
class ExprOpLoad : public ExprOp {
public:
  /// Constructor for memory load.
  ExprOpLoad(const ExprPtr mem, const ExprPtr addr);
  std::string op_name() const { return "LOAD"; }
  z3::expr GetZ3Expr(z3::context& ctx, const Z3ExprVec& expr_vec,
                     const std::string& suffix) const;
  std::ostream& Print(std::ostream& out) const;
}; // class ExprOpLoad

// TODO ExprOpStore

/******************************************************************************/
// Bit-manipulation
/******************************************************************************/

// TODO ExprOpConcat

// TODO ExprOpExtract

// TODO ExprOpZeroExtend

/******************************************************************************/
// Function usage
/******************************************************************************/

// TODO ExprOpAppFunc

} // namespace ila

#endif // __EXPR_OP_H__


/// \file
/// Header of the class ExprRef

#ifndef __EXPR_REF_H__
#define __EXPR_REF_H__

#include "ila/ast/expr.h"
#include "ila/ast/expr_const.h"
#include "ila/ast/expr_op.h"
#include "ila/ast/expr_var.h"
#include "ila/ast/sort.h"
#include <string>

/// \namespace ila
namespace ila {

/// \class ExprRef
/// ExprRef is the uniform interface and wrapper for different types of
/// expressions, e.g. var, constant, and different operations.
class ExprRef {
public:
  // ------------------------- CONSTRUCTOR/DESTRUCTOR ----------------------- //
  /// Default constructor.
  ExprRef();
  /// Default destructor.
  ~ExprRef();

  // ------------------------- ACCESSORS/MUTATORS --------------------------- //

  // ------------------------- METHODS -------------------------------------- //
  // static functions for wrapping the hierarchy of AST.

  /// Create new Boolean variable.
  static ExprPtr NewBoolVar(const std::string& name);
  /// Create new bitvector variable.
  static ExprPtr NewBvVar(const std::string& name, const int& bit_width);
  /// Create new memory variable.
  static ExprPtr NewMemVar(const std::string& name, const int& addr_width,
                           const int& data_width);

  /// Create a Boolean constant.
  static ExprPtr NewBoolConst(const bool& val);
  /// Create a Boolean constant from BoolVal.
  static ExprPtr NewBoolConst(const BoolVal& val);
  /// Create a bitvector constant.
  static ExprPtr NewBvConst(const int& val, const int& bit_width);
  /// Create a bitvector constant from BvVal.
  static ExprPtr NewBvConst(const BvVal& val, const int& bit_width);
  /// Create a memory constant with only the defauly value.
  static ExprPtr NewMemConst(const int& def_val, const int& addr_width,
                             const int& data_width);
  /// Create a memory constant from MemVal.
  static ExprPtr NewMemConst(const MemVal& val, const int& addr_width,
                             const int& data_width);

  /// Logical AND
  static ExprPtr LogicalAnd(const ExprPtr l, const ExprPtr r);
  /// Logical OR
  static ExprPtr LogicalOr(const ExprPtr l, const ExprPtr r);

  /// Comparison - equal
  static ExprPtr CompEq(const ExprPtr l, const ExprPtr r);

private:
  // ------------------------- MEMBERS -------------------------------------- //

  // ------------------------- HELPERS -------------------------------------- //

}; // class ExprRef

} // namespace ila

#endif // __EXPR_REF_H__


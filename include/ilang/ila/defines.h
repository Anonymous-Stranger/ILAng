/// \file
/// Headers for macros, type definitions, etc.

#ifndef DEFINES_H__
#define DEFINES_H__

#include <ilang/ila/symbol.h>
#include <ilang/util/container.h>
#include <memory>
#include <vector>
#include "z3++.h"

/// \namespace ila
namespace ila {

/// Vector type for z3 expression.
typedef std::vector<z3::expr> Z3ExprVec;
/// Pointer for the z3 expression vector.
typedef std::shared_ptr<Z3ExprVec> Z3ExprVecPtr;
/// Map type for z3 expression.
typedef KeyVec<Symbol, z3::expr> Z3ExprMap;
/// Pointer for the z3 expression map.
typedef std::shared_ptr<Z3ExprMap> Z3ExprMapPtr;

}  // namespace ila

#endif  // DEFINES_H__

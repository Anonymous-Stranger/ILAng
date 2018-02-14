/// \file
/// Header for the class Sort

#ifndef SORT_H__
#define SORT_H__

#include "ila/ast/ast.h"
#include <ostream>
#include <vector>

/// \namespace ila
namespace ila {

/// SortType
typedef enum { SORT_BOOL, SORT_BV, SORT_MEM, SORT_APP } SortType;

/// \brief The class for sort (type for expr, and the range/domain of
/// functions).
class Sort : public Ast {
public:
  /// Pointer type for normal use of Sort.
  typedef std::shared_ptr<Sort> SortPtr;
  /// Type for storing a set of Sort.
  typedef std::vector<SortPtr> SortPtrVec;

  // ------------------------- CONSTRUCTOR/DESTRUCTOR ----------------------- //
  /// Constructor for Boolean type.
  Sort();
  /// Constructor for Bitvector type.
  Sort(const int& bit_width);
  /// Constructor for Memory (Array) type.
  Sort(const int& addr_width, const int& data_width);
  /// Constructor for Application type.
  Sort(const SortPtr range_sort, const SortPtrVec& args_sort);
  /// Default destructor.
  ~Sort();

  // ------------------------- ACCESSORS/MUTATORS --------------------------- //
  /// Return the bit width (bitvector).
  const int& bit_width() const;
  /// Return the address width (mem).
  const int& addr_width() const;
  /// Return the data width (mem).
  const int& data_width() const;
  /// Return the range sort (app).
  const SortPtr range() const;
  /// Return the number of domain argument (app).
  size_t num_arg() const;
  /// Return the i-th domain sort (app).
  const SortPtr arg(const size_t& i) const;

  /// Is type sort (object).
  bool is_sort() const { return true; }

  /// Return true if this is a Boolean expression.
  bool is_bool() const;
  /// Return true if this is a Bitvector expression.
  bool is_bv() const;
  /// Return true if this is an Memory expression.
  bool is_mem() const;
  /// Return true if this is an Application expression.
  bool is_app() const;

  // ------------------------- METHODS -------------------------------------- //
  /// Output to stream.
  std::ostream& Print(std::ostream& out) const;
  /// Compare two Sorts.
  static bool Equal(const Sort& lhs, const Sort& rhs);

  /// Overload output stream operator.
  friend std::ostream& operator<<(std::ostream& out, const Sort& s);
  /// Overlaod comparison.
  friend bool operator==(const Sort& lhs, const Sort& rhs);

  /// Create Bool sort
  static Sort MakeBoolSort();
  /// Create Bitvector sort.
  static Sort MakeBvSort(const int& bit_width);
  /// Create Memory sort.
  static Sort MakeMemSort(const int& addr_width, const int& data_width);
  /// Create Application sort.
  static Sort MakeAppSort(const SortPtr range, const SortPtrVec& args);
  /// Create Sort pointer.
  static SortPtr MakeSortPtr(const Sort& sort);

private:
  // ------------------------- MEMBERS -------------------------------------- //
  /// The type for the sort.
  SortType type_;
  /// Bit width of bitvector.
  int bit_width_;
  /// Address width of mem.
  int addr_width_;
  /// Data width of mem.
  int data_width_;
  /// Sort of the output data of application.
  SortPtr range_sort_;
  /// Sorts of the application arguments (domain).
  SortPtrVec args_sort_;

  // ------------------------- HELPERS -------------------------------------- //

  /// Print Boolean type sort.
  std::ostream& PrintBool(std::ostream& out) const;
  /// Print Bitvector type sort.
  std::ostream& PrintBv(std::ostream& out) const;
  /// Print Array type sort.
  std::ostream& PrintMem(std::ostream& out) const;
  /// Print Application type sort.
  std::ostream& PrintApp(std::ostream& out) const;

}; // class Sort

/// Pointer type for normal use of Sort.
typedef Sort::SortPtr SortPtr;
/// Type for storing a set of Sort.
typedef Sort::SortPtrVec SortPtrVec;

} // namespace ila

#endif // SORT_H__


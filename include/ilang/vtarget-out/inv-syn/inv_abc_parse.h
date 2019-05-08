/// \file Parsing ABC invariant synthesis result
/// what we need are : blif file, clauses file
/// dut name and whether to discourage use of outside variable
// ---Hongce Zhang

#ifndef INV_ABC_PARSE_H__
#define INV_ABC_PARSE_H__

#include <set>
#include <vector>
#include <string>

namespace ilang {

/// \brief Parser for ABC pdr result
class AbcInvariantParser {

protected:
  /// the parsing result
  std::string  parse_result;
  /// only 1-bit
  std::set<std::string> outside_reference;
  
  /// the real parsing parsing function
  void parse(
    const std::string & blif_name,
    const std::string & abc_result_fn);
  /// dut name
  const std::string dut_name;
  /// whether to warn names
  const bool discourage_outside_var_ref;
  /// whether to replace names
  const bool replace_outside_var_ref;
  /// whether parsing is successful
  bool parse_succeed;
  
public:
  AbcInvariantParser(
    const std::string & blif_name,
    const std::string & abc_result_fn,
    const std::string & dut_name,
    bool discourage_outside_variable_reference,
    bool replace_outside_variable_reference);
    
  std::string GetParseResult() const { return parse_result; }

  const std::set<std::string> & GetNewVarDefs() const { return outside_reference; }

  bool get_parse_status() const {return parse_succeed;}
}; // class AbcInvariantParser

}; // namespace ilang

#endif // INV_ABC_PARSE_H__

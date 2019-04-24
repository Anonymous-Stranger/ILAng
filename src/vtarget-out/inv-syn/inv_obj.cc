/// \file Invariant Object --- should be able to parse from
/// a smt-like-output
// Hongce Zhang

#include <ilang/util/str_util.h>
#include <ilang/util/container_shortcut.h>
#include <ilang/util/log.h>

#include <ilang/vtarget-out/inv-syn/inv_obj.h>
// parser for Z3/freqhorn output
#include <ilang/smt-inout/chc_inv_in.h>
// parser for cvc4 output
#include <ilang/vtarget-out/inv-syn/sygus/sygus_inv_parse.h>

#include <fstream>

namespace ilang {

/// empty invariants
InvariantObject::InvariantObject() {
  
}

void InvariantObject::set_dut_inst_name(const std::string & name) {
  dut_inst_name = name;
}

/// add invariants from smt-like output
void InvariantObject::AddInvariantFromChcResultFile(
    smt::YosysSmtParser & design_info, 
    const std::string & tag, const std::string & chc_result_fn,
    bool flatten_datatype, bool flatten_hierarchy ) {

  ILA_ASSERT(not dut_inst_name.empty()) 
    << "BUG: duv instance name unknown. "
    << "set_dut_inst_name should be called first!";
    
  smt::SmtlibInvariantParser parser(
    &design_info,
    flatten_datatype, flatten_hierarchy,
    {"INV"}, dut_inst_name);

  if (not parser.ParseInvResultFromFile(chc_result_fn) ) {
    ILA_ERROR << "No new invariant has been extracted!";
    return;
  }
  ILA_ASSERT(not parser.in_bad_state());
  inv_vlg_exprs.push_back( parser.GetFinalTranslateResult() );


  auto raw_smt = parser.GetRawSmtString();
  ILA_ERROR_IF(raw_smt.empty()) << "Parser failed to extract raw CHC string, got empty string!";
  smt_formula_vec.push_back( raw_smt );
 
  for (auto && name_vlg_pair : parser.GetLocalVarDefs()) {
    inv_extra_vlg_vars.push_back(std::make_tuple(
      name_vlg_pair.first,
      name_vlg_pair.second._translate,
      name_vlg_pair.second._type.GetBoolBvWidth()));
  }
  for (auto && name_w_pair : parser.GetFreeVarDefs()) {
    if( IN(name_w_pair.first, inv_extra_free_vars) )
      ILA_ASSERT(inv_extra_free_vars[name_w_pair.first] == name_w_pair.second)
        << "Overwriting free var:" << name_w_pair.first << " w. width: " << name_w_pair.second
        << " old width:" << inv_extra_free_vars[name_w_pair.first];
    inv_extra_free_vars.insert(name_w_pair);
  }
} // AddInvariantFromChcResultFile

/// add invariants from smt-like output
void InvariantObject::AddInvariantFromSygusResultFile(
    smt::YosysSmtParser & design_info, 
    const std::string & tag, const std::string & chc_result_fn,
    bool flatten_datatype, bool flatten_hierarchy ) {
  
  ILA_ASSERT(not dut_inst_name.empty()) << "BUG: duv instance name unknown."
    << "set_dut_inst_name should be called first!";

  smt::SyGuSInvariantParser parser(
    &design_info,
    flatten_datatype, flatten_hierarchy,
    {"INV"}, dut_inst_name);

  if (not parser.ParseInvResultFromFile(chc_result_fn) ) {
    ILA_ERROR << "Parser failed to extract invariant, no new invariant has been extracted!";
    return;
  }
  ILA_ASSERT(not parser.in_bad_state());
  auto inv_extracted = parser.GetFinalTranslateResult();
  ILA_ASSERT(not inv_extracted.empty()) << "Parser failed to extract invariant, got empty string!";
  inv_vlg_exprs.push_back( inv_extracted );

  auto raw_smt = parser.GetRawSmtString();
  ILA_ASSERT(not raw_smt.empty()) << "Parser failed to extract raw smt string, got empty string!";
  smt_formula_vec.push_back( raw_smt );

  for (auto && name_vlg_pair : parser.GetLocalVarDefs()) {
    inv_extra_vlg_vars.push_back(std::make_tuple(
      name_vlg_pair.first,
      name_vlg_pair.second._translate,
      name_vlg_pair.second._type.GetBoolBvWidth()));
  }
  for (auto && name_w_pair : parser.GetFreeVarDefs()) {
    if( IN(name_w_pair.first, inv_extra_free_vars) )
      ILA_ASSERT(inv_extra_free_vars[name_w_pair.first] == name_w_pair.second)
        << "Overwriting free var:" << name_w_pair.first << " w. width: " << name_w_pair.second
        << " old width:" << inv_extra_free_vars[name_w_pair.first];
    inv_extra_free_vars.insert(name_w_pair);
  }
} // AddInvariantFromSygusResultFile

/// add invariants from verilog-like output
void InvariantObject::AddInvariantFromVerilogExpr(const std::string & tag, const std::string & vlg_in) {
  inv_vlg_exprs.push_back(vlg_in);
  smt_formula_vec.push_back("");
}


const InvariantObject::smt_formula_vec_t & InvariantObject::GetSmtFormulae() const {
  return smt_formula_vec;
}

const InvariantObject::inv_vec_t & InvariantObject::GetVlgConstraints() const {
  return inv_vlg_exprs;
}
/// get the vars
const InvariantObject::extra_var_def_vec_t & InvariantObject::GetExtraVarDefs() const {
  return inv_extra_vlg_vars;
}

/// get the vars
const InvariantObject::extra_free_var_def_vec_t & InvariantObject::GetExtraFreeVarDefs() const {
  return inv_extra_free_vars;
}


/// this is to support making candidate invariant as confirmed
void InvariantObject::InsertFromAnotherInvObj(const InvariantObject & r) {
  auto error_msg = "Potentially strange behavior in invariant object, unaccounted situation: ";
  ILA_WARN_IF (not r.inv_extra_vlg_vars.empty()) << error_msg << "inv_extra_vlg_vars"; // should be okay I think
  ILA_WARN_IF (not r.inv_extra_free_vars.empty()) << error_msg << "inv_extra_free_vars";
  inv_extra_vlg_vars.insert(inv_extra_vlg_vars.end(), r.inv_extra_vlg_vars.begin(), r.inv_extra_vlg_vars.end());
  for(auto && p : r.inv_extra_free_vars) {
    ILA_ERROR_IF (IN(p.first,inv_extra_free_vars) and inv_extra_free_vars[p.first] != p.second )
      << "Conflict for free var:" << p.first;
    inv_extra_free_vars.insert(p);
  }
  inv_vlg_exprs.insert(inv_vlg_exprs.end(), r.inv_vlg_exprs.begin(), r.inv_vlg_exprs.end());
  smt_formula_vec.insert(smt_formula_vec.end(), r.smt_formula_vec.begin(), r.smt_formula_vec.end());
}

/// clear all stored invariants
void InvariantObject::ClearAllInvariants() {
  inv_vlg_exprs.clear();
  inv_extra_free_vars.clear();
  inv_extra_vlg_vars.clear();
  smt_formula_vec.clear();
}

size_t InvariantObject::NumInvariant() const {
  return inv_vlg_exprs.size();
}

/// export invariants to a file
void InvariantObject::ExportToFile(const std::string &fn) const {
  std::ofstream fout(fn);
  ILA_WARN << "Will not preserve the original smt formula";
  if (not fout.is_open()) {
    ILA_ERROR << "Failed to open:" << fn << " for write.";
    return;
  }
  fout << inv_extra_vlg_vars.size() << " ";
  fout << inv_extra_free_vars.size() << " ";
  fout << inv_vlg_exprs.size() << "\n";
  for (auto && vlg_var : inv_extra_vlg_vars) {
    ILA_ERROR_IF(S_IN(" ", std::get<0>(vlg_var)) || S_IN("\t", std::get<0>(vlg_var)))
      << "The inv var name contains white-space, cannot be handled correctly!";
    ILA_ERROR_IF(S_IN("\n", std::get<1>(vlg_var)) || S_IN("\r", std::get<1>(vlg_var)))
      << "The inv expression contains line-break, cannot be handled correctly!";
    
    fout<< std::get<0>(vlg_var) << " "
        << std::get<2>(vlg_var) << " "
        << std::get<1>(vlg_var) << "\n";
  }
  for (auto && vlg_var : inv_extra_free_vars) {
    ILA_ERROR_IF(S_IN(" ", vlg_var.first) || S_IN("\t", vlg_var.first))
      << "The free var name contains white-space, cannot be handled correctly!";
    fout<< vlg_var.first << " "
        << vlg_var.second << "\n";
  }
  for (auto && vlg_var : inv_vlg_exprs) {
    ILA_ERROR_IF(S_IN("\n", vlg_var) || S_IN("\r", vlg_var))
      << "The expression contains line-break, cannot be handled correctly!";
    fout<< vlg_var << "\n";
  }
}
/// import invariants that has been previous exported
void InvariantObject::ImportFromFile(const std::string &fn) {
  std::ifstream fin(fn);
  if(not fin.is_open()) {
    ILA_ERROR << "Failed to open:" << fn << " for read.";
    return;
  }
  unsigned num_vlg_var, num_free_var, num_expr;
  fin >> num_vlg_var >> num_free_var >> num_expr;
  for (unsigned idx = 0; idx < num_vlg_var; ++ idx) {
    std::string vname;
    int width;
    std::string expr;
    fin >> vname >> width;
    std::getline(fin,expr);
    inv_extra_vlg_vars.push_back(std::make_tuple(
      vname,expr, width ));
  }
  for (unsigned idx = 0; idx < num_free_var; ++ idx) {
    std::string vname;
    int width;
    fin >> vname >> width;
    inv_extra_free_vars.insert(std::make_pair(vname, width));
  }
  for (unsigned idx = 0; idx < num_expr; ++ idx) {
    std::string expr;
    std::getline(fin,expr);
    if (expr.empty()) {
      ILA_WARN << "Skipped an empty line in the input stream";
      std::getline(fin,expr);
    }
    inv_vlg_exprs.push_back(expr);
    smt_formula_vec.push_back(""); // empty
  }
}


void InvariantObject::RemoveInvByIdx(size_t idx) {
  ILA_ASSERT(idx < inv_vlg_exprs.size()) << "Index out-of-range";

  inv_vlg_exprs.erase(inv_vlg_exprs.begin()+idx);
  smt_formula_vec.erase(smt_formula_vec.begin()+idx);
}


}; // namespace ilang
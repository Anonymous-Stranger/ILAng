/// \file Source of generating CoSA accepted problem, vlg, mem, script
// --- Hongce Zhang

#include <algorithm>
#include <fstream>
#include <ilang/util/container_shortcut.h>
#include <ilang/util/fs.h>
#include <ilang/util/log.h>
#include <ilang/util/str_util.h>
#include <ilang/vtarget-out/absmem.h>
#include <ilang/vtarget-out/vtarget_gen_cosa.h>
#include <iostream>

namespace ilang {

#define VLG_TRUE "`true"
#define VLG_FALSE "`false"

VlgSglTgtGen_Cosa::VlgSglTgtGen_Cosa(
    const std::string&
        output_path, // will be a sub directory of the output_path of its parent
    const InstrPtr& instr_ptr, // which could be an empty pointer, and it will
                               // be used to verify invariants
    const InstrLvlAbsPtr& ila_ptr, const VerilogGenerator::VlgGenConfig& config,
    nlohmann::json& _rf_vmap, nlohmann::json& _rf_cond,
    VerilogInfo* _vlg_info_ptr, const std::string& vlg_mod_inst_name,
    const std::string& ila_mod_inst_name, const std::string& wrapper_name,
    const std::vector<std::string>& implementation_srcs,
    const std::vector<std::string>& implementation_include_path,
    const vtg_config_t& vtg_config, backend_selector backend)
    : VlgSglTgtGen(output_path, instr_ptr, ila_ptr, config, _rf_vmap, _rf_cond,
                   _vlg_info_ptr, vlg_mod_inst_name, ila_mod_inst_name,
                   wrapper_name, implementation_srcs,
                   implementation_include_path, vtg_config, backend) {}

std::string convert_expr_to_cosa(const std::string& in) {
  return ReplaceAll(
      ReplaceAll(ReplaceAll(ReplaceAll(ReplaceAll(ReplaceAll(in, "&&", "&"),
                                                  "||", "|"),
                                       "~", "!"),
                            "==", "="),
                 VLG_TRUE, "True"),
      VLG_FALSE, "False");
}
void VlgSglTgtGen_Cosa::add_wire_assign_assumption(
    const std::string& varname, const std::string& expression,
    const std::string& dspt) {
  //_problems.assumptions.push_back(varname + " = " +
  //                                convert_expr_to_cosa(expression));
  vlg_wrapper.add_assign_stmt(varname, expression);
  ILA_ERROR_IF(expression.find(".") != std::string::npos)
      << "expression:" << expression << " contains unfriendly dot.";
}

void VlgSglTgtGen_Cosa::add_reg_cassign_assumption(
    const std::string& varname, const std::string& expression,
    const std::string& cond, const std::string& dspt) {
  // vlg_wrapper.add_always_stmt(varname + " <= " + varname + ";");
  // _problems.assumptions.push_back("(!( " + convert_expr_to_cosa(cond) +
  //                                 " ) | (" + varname + " = " +
  //                                convert_expr_to_cosa(expression) + "))");
  ILA_ERROR_IF(expression.find(".") != std::string::npos)
      << "expression:" << expression << " contains unfriendly dot.";
  vlg_wrapper.add_always_stmt("if (" + cond + ") " + varname +
                              " <= " + expression + "; //" + dspt);
}

/// Add an assumption
void VlgSglTgtGen_Cosa::add_an_assumption(const std::string& aspt,
                                          const std::string& dspt) {
  auto assumption_wire_name = vlg_wrapper.sanitizeName(dspt) + new_mapping_id();
  vlg_wrapper.add_wire(assumption_wire_name, 1, true);
  vlg_wrapper.add_assign_stmt(assumption_wire_name, aspt);
  ILA_ERROR_IF(aspt.find(".") != std::string::npos)
      << "aspt:" << aspt << " contains unfriendly dot.";
  _problems.assumptions.push_back(assumption_wire_name + " = 1_1");
  //_problems.assumptions.push_back(convert_expr_to_cosa(aspt));
}
/// Add an assertion
void VlgSglTgtGen_Cosa::add_an_assertion(const std::string& asst,
                                         const std::string& dspt) {
  auto assrt_wire_name = vlg_wrapper.sanitizeName(dspt) + new_property_id();
  vlg_wrapper.add_wire(assrt_wire_name, 1, true);
  vlg_wrapper.add_assign_stmt(assrt_wire_name, asst);
  _problems.probitem[dspt].assertions.push_back(assrt_wire_name + " = 1_1");
  ILA_ERROR_IF(asst.find(".") != std::string::npos)
      << "asst:" << asst << " contains unfriendly dot.";
  //_problems.probitem[dspt].assertions.push_back(convert_expr_to_cosa(asst));
}

/// export the script to run the verification
void VlgSglTgtGen_Cosa::Export_script(const std::string& script_name) {
  auto fname = os_portable_append_dir(_output_path, script_name);
  std::ofstream fout(fname);
  if (!fout.is_open()) {
    ILA_ERROR << "Error writing to file:" << fname;
    return;
  }
  fout << "#!/bin/bash" << std::endl;
  if (cosa_prob_fname != "")
    fout << "CoSA --problem " << cosa_prob_fname << std::endl;
  else
    fout << "echo 'Nothing to check!'" << std::endl;
}
/// export extra things (problem)
void VlgSglTgtGen_Cosa::Export_problem(const std::string& extra_name) {
  if (_problems.probitem.size() == 0) {
    ILA_ERROR << "Nothing to prove, no assertions inserted!";
    return;
  }

  cosa_prob_fname = extra_name;

  std::ofstream rstf(os_portable_append_dir(_output_path, "rst.ets"));
  if (!rstf.is_open()) {
    ILA_ERROR << "Error writing file: "
              << os_portable_append_dir(_output_path, "rst.ets");
    return;
  }
  rstf << "I: rst = 0_1" << std::endl;
  rstf << "I: reset_done = False" << std::endl;
  rstf << "S1: rst = 1_1" << std::endl;
  rstf << "S1: reset_done = False" << std::endl;
  rstf << "S2: rst = 0_1" << std::endl;
  rstf << "S2: reset_done = True" << std::endl;
  rstf << "# TRANS" << std::endl;
  rstf << "I -> S1" << std::endl;
  rstf << "S1 -> S2" << std::endl;
  rstf << "S2 -> S2" << std::endl;

  std::ofstream fout(os_portable_append_dir(_output_path, extra_name));
  if (!fout.is_open()) {
    ILA_ERROR << "Error writing file: "
              << os_portable_append_dir(_output_path, extra_name);
    return;
  }

  fout << "[GENERAL]" << std::endl;
  fout << "model_file:"; //
  fout << top_file_name << "[" << top_mod_name << "],";
  // if(target_type != target_type_t::INVARIANTS )
  //	fout << ila_file_name<<","; // will be combined
  fout << "rst.ets";
  // for(auto && fn : vlg_design_files) // will be combined
  //  fout << "," << os_portable_file_name_from_path(fn);
  fout << std::endl;

  fout << "assume_if_true: True" << std::endl;
  fout << "abstract_clock: True" << std::endl;
  fout << "[DEFAULT]" << std::endl;
  fout << "bmc_length: " << std::to_string(max_bound) << std::endl;
  fout << "precondition: reset_done" << std::endl;
  fout << std::endl;

  std::string assmpt = "(" + Join(_problems.assumptions, ") & (") + ")";
  for (auto&& pbname_prob_pair : _problems.probitem) {
    const auto& prbname = pbname_prob_pair.first;
    const auto& prob = pbname_prob_pair.second;
    auto asst = "(" + Join(prob.assertions, ") & (") + ")";
    auto prob_name = vlg_wrapper.sanitizeName(prbname);
    fout << "[" << prob_name << "]" << std::endl;
    fout << "description:\"" << prbname << "\"" << std::endl;
    fout << "formula:" << asst << std::endl;
    if (assmpt != "()")
      fout << "assumptions:" << assmpt << std::endl;
    fout << "prove: True" << std::endl;
    fout << "verification: safety" << std::endl;
    fout << "strategy: ALL" << std::endl;
    fout << "expected: True" << std::endl;
  }

} // only for cosa

/// export the memory abstraction (implementation)
/// Yes, this is also implementation specific, (jasper may use a different one)
void VlgSglTgtGen_Cosa::Export_mem(const std::string& mem_name) {
  // we will ignore the mem_name

  auto outfn = os_portable_append_dir(_output_path, top_file_name);
  std::ofstream fout(outfn, std::ios_base::app); // append

  VlgAbsMem::OutputMemFile(fout);
}

/// For jasper, this means do nothing, for yosys, you need to add (*keep*)
void VlgSglTgtGen_Cosa::Export_modify_verilog() {
  // collect all locations: filename -> lineno
  // open, read, count and write
  // if it is a port name, we will ask user to specify its upper level
  // signal name
  VerilogModifier vlg_mod(vlg_info_ptr);

  for (auto&& refered_vlg_item : _all_referred_vlg_names) {
    auto idx = refered_vlg_item.first.find("[");
    auto removed_range_name = refered_vlg_item.first.substr(0, idx);
    vlg_mod.RecordKeepSignalName(removed_range_name);
    vlg_mod.RecordConnectSigName(removed_range_name,
                                 refered_vlg_item.second.range);
  }
  vlg_mod.FinishRecording();

  // now let's do the job
  for (auto&& fn : vlg_design_files) {
    auto outfn = os_portable_append_dir(_output_path, top_file_name);
    std::ifstream fin(fn);
    std::ofstream fout(outfn, std::ios_base::app); // append
    if (!fin.is_open()) {
      ILA_ERROR << "Cannot read file:" << fn;
      continue;
    }
    if (!fout.is_open()) {
      ILA_ERROR << "Cannot open file for write:" << outfn;
      continue;
    }
    vlg_mod.ReadModifyWrite(fn, fin, fout);
  } // for (auto && fn : vlg_design_files)
  // .. (copy all the verilog file in the folder), this has to be os dependent
  if (vlg_include_files_path.size() != 0)
    ILA_WARN << "Not copying includes.";

} // Export_modify_verilog

}; // namespace ilang
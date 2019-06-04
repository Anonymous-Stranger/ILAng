/// \file
/// The header for the class IlaSim

#ifndef ILANG_TARGET_SC_ILA_SIM_H__
#define ILANG_TARGET_SC_ILA_SIM_H__

#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <ilang/ila/instr_lvl_abs.h>

#define EXTERNAL_MEM 0

// FIXME do not use "using namespace std", especially in headers
using namespace std;

namespace ilang {

// TODO(yuex): change id to instruction name for decode&state_update function

/// \brief A class to generate SystemC simulator model from ILA model
/// Example Use:
/// - InstrLvlAbsPtr your_ila_model_ptr;
/// - IlaSim ila_sim;
/// - ila_sim.set_instr_lvl_abs(your_ila_model_ptr);
/// - ila_sim.sim_gen("./");
class IlaSim {
public:
  typedef struct {
    string mem_str;
    string addr_str;
    string dest_str;
  } ld_info;

  typedef struct {
    string mem_str;
    string mem_map;
  } st_info;

  IlaSim();
  IlaSim(const InstrLvlAbsPtr& model_ptr);
  void set_instr_lvl_abs(const InstrLvlAbsPtr& model_ptr);
  void set_systemc_path(string systemc_path);
  void sim_gen(string export_dir, bool external_mem = false,
               bool readable = false);

private:
  // Initialize all member variables for a new simulator generation pass.
  void sim_gen_init(string export_dir, bool external_mem, bool readable);
  // Create initial lines for the simulator's header file
  void sim_gen_init_header();
  void sim_gen_input();
  void sim_gen_state();
  void sim_gen_decode();
  void sim_gen_state_update();
  void sim_gen_execute_kernel();
  void sim_gen_execute_invoke();
  void sim_gen_export();

  void create_input(const ExprPtr& input_expr);
  void create_bool_input(const ExprPtr& expr);
  void create_bv_input(const ExprPtr& expr);

  void create_state(const ExprPtr& state_expr);
  void create_child_state(const ExprPtr& state_expr);
  void create_mem_state(const ExprPtr& expr);
  void create_bool_state(const ExprPtr& expr, bool child = false);
  void create_bv_state(const ExprPtr& expr, bool child = false);
  void create_external_mem_port(const ExprPtr& expr);

  void create_decode(const InstrPtr& instr_expr);
  void decode_decl(stringstream& decode_function, string& indent,
                   string& decode_func_name);
  void decode_check_valid(stringstream& decode_function, string& indent,
                          const ExprPtr& valid_expr,
                          const InstrPtr& instr_expr);
  void decode_return(stringstream& decode_function, string& indent,
                     const ExprPtr& decode_expr, const InstrPtr& instr_expr);
  void decode_export(stringstream& decode_function, string& decode_func_name);
  void decode_mk_file(string& decode_func_name);

  void create_state_update(const InstrPtr& instr_expr);
  void state_update_decl(stringstream& state_update_function, string& indent,
                         const ExprPtr& updated_state,
                         const ExprPtr& update_expr,
                         string& state_update_func_name);
  void state_update_return(stringstream& state_update_function, string& indent,
                           const ExprPtr& updated_state,
                           const ExprPtr& update_expr);
  void state_update_export(stringstream& state_update_function,
                           string& state_update_func_name);
  void state_update_mk_file(string& state_update_func_name);
  void mem_state_update_decl(stringstream& state_update_function,
                             string& indent, const ExprPtr& expr);

  void execute_parent_instructions(stringstream& execute_kernel,
                                   string& indent);
  void execute_child_instructions(stringstream& execute_kernel, string& indent);
  void execute_instruction(stringstream& execute_kernel, string& indent,
                           const InstrPtr& instr_expr, bool child = false);
  void execute_decode(stringstream& execute_kernel, string& indent,
                      const InstrPtr& instr_expr);
  void execute_state_update_func(stringstream& execute_kernel, string& indent,
                                 const InstrPtr& instr_expr,
                                 const ExprPtr& updated_state);
  void execute_update_state(stringstream& execute_kernel, string& indent,
                            const InstrPtr& instr_expr,
                            const ExprPtr& updated_state);
  void execute_external_mem_load_begin(stringstream& execute_kernel,
                                       string& indent,
                                       const InstrPtr& instr_expr);
  void execute_external_mem_load_end(stringstream& execute_kernel,
                                     string& indent);
  void execute_read_external_mem(stringstream& execute_kernel, string& indent);
  void execute_write_external_mem(stringstream& execute_kernel, string& indent);
  void execute_external_mem_before_input(stringstream& execute_kernel,
                                         string& indent);
  void execute_external_mem_after_output(stringstream& execute_kernel,
                                         string& indent);
  void execute_external_mem_return(stringstream& execute_kernel,
                                   string& indent);
  void execute_read_input(stringstream& execute_kernel, string& indent);
  void execute_write_output(stringstream& execute_kernel, string& indent);
  void execute_kernel_export(stringstream& execute_kernel);
  void execute_kernel_mk_file();
  void execute_kernel_header();

  void dfs_store_op(const ExprPtr& expr);
  void dfs_load_from_store(const ExprPtr& expr);
  void dfs_int_var_width(const ExprPtr& expr);
  void dfs_external_mem_load(const ExprPtr& expr);
  void dfs_kernel(stringstream& dfs_simulator, string& indent,
                  const ExprPtr& expr);
  void dfs_uninterpreted_func_decl(const FuncPtr& func);
  void dfs_const_node(stringstream& dfs_simulator, string& indent,
                      const ExprPtr& expr);
  void dfs_unary_op(stringstream& dfs_simulator, string& indent,
                    const ExprPtr& expr);
  void dfs_unary_op_check(const ExprPtr& expr);
  void dfs_binary_op_bool_out(stringstream& dfs_simulator, string& indent,
                              const ExprPtr& expr);
  void dfs_binary_op_bool_out_check(const ExprPtr& expr);
  void dfs_binary_op_non_mem(stringstream& dfs_simulator, string& indent,
                             const ExprPtr& expr);
  void dfs_binary_op_non_mem_check(const ExprPtr& expr);
  void dfs_binary_op_mem(stringstream& dfs_simulator, string& indent,
                         const ExprPtr& expr);
  void dfs_extract_op(stringstream& dfs_simulator, string& indent,
                      const ExprPtr& expr);
  void dfs_ext_op(stringstream& dfs_simulator, string& indent,
                  const ExprPtr& expr);
  void dfs_func_op(stringstream& dfs_simulator, string& indent,
                   const ExprPtr& expr);
  void dfs_func_op_check(const ExprPtr& expr);
  void dfs_ite_op(stringstream& dfs_simulator, string& indent,
                  const ExprPtr& expr);

  string get_arg_str(const ExprPtr& arg);
  void increase_indent(string& indent);
  void decrease_indent(string& indent);
  int get_update_state_num(const InstrPtr& instr_expr);
  bool load_from_store_analysis(const ExprPtr& expr);
  void declare_variable_with_id(size_t id, string v_type, string v_name);
  void int_var_width_scan();

  string export_dir_;
  string systemc_path_;

  stringstream header_;
  stringstream mk_script_;
  stringstream obj_list_;
  string header_indent_;

  set<size_t> searched_id_set_;
  set<size_t> store_ite_set_;
  set<size_t> const_mem_set_;
  set<size_t> declared_id_set_;
  set<string> func_set_;
  set<size_t> defined_store_ite_set_;
  set<size_t> defined_state_set_;
  set<int> dfs_ld_search_set_;
  vector<ld_info> external_ld_set_;
  vector<st_info> external_st_set_;
  int ld_st_counter_;
  bool EXTERNAL_MEM_;
  const int MEM_MAP_ARRAY_DIV = 16;
  set<int> int_var_width_set_;
  // Readable_ is used to control whether the generated function name is
  // huname-readable. When being set true, function will be named based on the
  // instruction name and the updated state name. However, there is a potential
  // same-name bug if setting true.
  bool readable_;
  bool qemu_device_;

  InstrLvlAbsPtr model_ptr_;
};

}; // namespace ilang

#endif // ILANG_TARGET_SC_ILA_SIM_H__

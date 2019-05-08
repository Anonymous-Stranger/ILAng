#include <ilang/ila_sim/ila_sim.h>

namespace ilang {
IlaSim::IlaSim() {}

IlaSim::IlaSim(const InstrLvlAbsPtr &model_ptr) { model_ptr_ = model_ptr; }

void IlaSim::set_instr_lvl_abs(const InstrLvlAbsPtr &model_ptr) {
  model_ptr_ = model_ptr;
}

void IlaSim::set_systemc_path(string systemc_path) {
  systemc_path_ = systemc_path;
}

void IlaSim::sim_gen(string export_dir, bool external_mem, bool readable) {
  sim_gen_init(export_dir, external_mem, readable);
  sim_gen_init_header();
  sim_gen_input();
  sim_gen_state();
  sim_gen_decode();
  sim_gen_state_update();
  sim_gen_execute_kernel();
  sim_gen_execute_invoke();
  sim_gen_export();
}

void IlaSim::sim_gen_init(string export_dir, bool external_mem, bool readable) {
  stringstream().swap(header_);
  stringstream().swap(mk_script_);
  stringstream().swap(obj_list_);
  header_indent_ = "";
  searched_id_set_.clear();
  store_ite_set_.clear();
  const_mem_set_.clear();
  declared_id_set_.clear();
  func_set_.clear();
  defined_store_ite_set_.clear();
  defined_state_set_.clear();
  dfs_ld_search_set_.clear();
  external_ld_set_.clear();
  external_st_set_.clear();
  ld_st_counter_ = 0;
  EXTERNAL_MEM_ = external_mem;
  export_dir_ = export_dir;
  readable_ = readable;
}

void IlaSim::sim_gen_init_header() {
  header_ << header_indent_ << "#include \"systemc.h\"" << endl;
  header_ << header_indent_ << "#include <map>" << endl;
  header_ << header_indent_ << "SC_MODULE(" << model_ptr_->name() << ") {"
          << std::endl;
  increase_indent(header_indent_);
}

void IlaSim::sim_gen_input() {
  for (int i = 0; i < model_ptr_->input_num(); i++) {
    create_input(model_ptr_->input(i));
  }
}

void IlaSim::sim_gen_state() {
  std::queue<InstrLvlAbsPtr> child_ila_queue;
  for (int i = 0; i < model_ptr_->state_num(); i++)
    create_state(model_ptr_->state(i));

  for (int i = 0; i < model_ptr_->child_num(); i++)
    child_ila_queue.push(model_ptr_->child(i));

  while (!child_ila_queue.empty()) {
    auto current_ila = child_ila_queue.front();
    child_ila_queue.pop();
    for (int i = 0; i < current_ila->state_num(); i++)
      create_child_state(current_ila->state(i));
    for (int i = 0; i < current_ila->child_num(); i++)
      child_ila_queue.push(current_ila->child(i));
  }
}

void IlaSim::sim_gen_decode() {
  std::queue<InstrLvlAbsPtr> ila_queue;
  ila_queue.push(model_ptr_);
  while (!ila_queue.empty()) {
    auto current_ila = ila_queue.front();
    ila_queue.pop();
    // TODO(yuex): cout -> ila_debug
    cout << "current_ila_name:" << current_ila->name() << endl;
    for (int i = 0; i < current_ila->child_num(); i++) {
      cout << "current_ila_child:" << current_ila->child(i)->name() << endl;
      ila_queue.push(current_ila->child(i));
    }
    for (int i = 0; i < current_ila->instr_num(); i++) {
      cout << "current_ila_instr:" << current_ila->instr(i)->name() << endl;
      create_decode(current_ila->instr(i));
    }
    cout << endl;
  }
}

void IlaSim::sim_gen_state_update() {
  std::queue<InstrLvlAbsPtr> ila_queue;
  ila_queue.push(model_ptr_);
  while (!ila_queue.empty()) {
    auto current_ila = ila_queue.front();
    ila_queue.pop();
    for (int i = 0; i < current_ila->child_num(); i++) {
      ila_queue.push(current_ila->child(i));
    }
    for (int i = 0; i < current_ila->instr_num(); i++) {
      create_state_update(current_ila->instr(i));
    }
  }
}

void IlaSim::sim_gen_execute_kernel() {
  stringstream execute_kernel;
  string indent = "";
  execute_kernel << indent << "#include \"systemc.h\"" << endl;
  execute_kernel << indent << "#include \"test.h\"" << endl;
  execute_kernel << indent << "void " << model_ptr_->name() << "::compute() {"
                 << endl;
  increase_indent(indent);
  if (EXTERNAL_MEM_) {
    execute_write_external_mem(execute_kernel, indent);
    execute_read_external_mem(execute_kernel, indent);
    execute_external_mem_before_input(execute_kernel, indent);
  }
  execute_read_input(execute_kernel, indent);
  if (EXTERNAL_MEM_)
    execute_external_mem_after_output(execute_kernel, indent);
  execute_parent_instructions(execute_kernel, indent);
  execute_child_instructions(execute_kernel, indent);
  if (EXTERNAL_MEM_)
    execute_external_mem_return(execute_kernel, indent);

  execute_write_output(execute_kernel, indent);
  decrease_indent(indent);
  execute_kernel << indent << "};" << endl;
  execute_kernel_export(execute_kernel);
  execute_kernel_mk_file();
  execute_kernel_header();
}

void IlaSim::sim_gen_execute_invoke() {
  header_ << header_indent_ << "SC_HAS_PROCESS(" << model_ptr_->name() << ");"
          << endl;
  header_ << header_indent_ << model_ptr_->name()
          << "(sc_module_name name_) : sc_module(name_) {" << endl;
  increase_indent(header_indent_);
  header_ << header_indent_ << "SC_METHOD(compute);" << endl;
  header_ << header_indent_ << "sensitive";
  for (int i = 0; i < model_ptr_->input_num(); i++) {
    header_ << " << " << model_ptr_->name() << "_"
            << model_ptr_->input(i)->name() << "_in";
  }
  header_ << ";" << endl;
  decrease_indent(header_indent_);
  header_ << header_indent_ << "}" << endl;

  decrease_indent(header_indent_);
  header_ << header_indent_ << "};" << endl;
}

void IlaSim::sim_gen_export() {
  ofstream outFile;
  outFile.open(export_dir_ + "test.h");
  outFile << header_.rdbuf();
  outFile.close();
  mk_script_ << "g++ -I. -I " << systemc_path_ << "/include/ "
             << "-L. -L " << systemc_path_ << "/lib-linux64/ "
             << "-Wl,-rpath=" << systemc_path_ << "/lib-linux64/ "
             << "-c -o "
             << "test_tb.o test_tb.cc "
             << "-lsystemc" << endl;

  mk_script_ << "g++ -I. -I " << systemc_path_ << "/include/ "
             << "-L. -L " << systemc_path_ << "/lib-linux64/ "
             << "-Wl,-rpath=" << systemc_path_ << "/lib-linux64/ "
             << "-o "
             << "test_tb test_tb.o " << obj_list_.rdbuf() << "-lsystemc"
             << endl;
  outFile.open(export_dir_ + "mk.sh");
  outFile << mk_script_.rdbuf();
  outFile.close();
}
}; // namespace ilang

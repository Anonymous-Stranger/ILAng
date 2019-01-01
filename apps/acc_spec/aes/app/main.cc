#include <aes_128.h>
#include <aes_ila.h>
#include <ilang/vtarget-out/vtarget_gen.h>

/// To verify the exact AES128 ILA
void verifyAES128(Ila& model) {
  VerilogGeneratorBase::VlgGenConfig vlg_cfg;
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;

  vlg_cfg.pass_node_name = true;

  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification/";

  VerilogVerificationTargetGenerator vg(
      {},                                                    // no include
      {VerilogPath + "aes_abs.v", VerilogPath + "one_round.v",
       VerilogPath + "final_round.v", VerilogPath + "expand_key_128.v",
       VerilogPath + "S.v", VerilogPath + "S4.v", VerilogPath + "xS.v",
       VerilogPath + "table_lookup.v", VerilogPath + "T.v"}, // designs
      "aes_128",                                             // top_module_name
      RefrelPath + "ref-rel-var-map-aes128.json",            // variable mapping
      RefrelPath + "ref-rel-inst-cond-aes128.json",          // conditions of start/ready
      OutputPath,                                            // output path
      model.get(),                                           // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.GenerateTargets();
}

/// To verify the IO ILA
void verifyIO(Ila& model) {
  VerilogGeneratorBase::VlgGenConfig vlg_cfg;
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;

  vtg_cfg.MemAbsReadAbstraction = true; // enable read abstraction
  vlg_cfg.pass_node_name = true;

  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification/";

  VerilogVerificationTargetGenerator vg(
      {}, // no include
      {VerilogPath + "aes_top.v",   VerilogPath + "reg2byte.v",
       VerilogPath + "reg16byte.v", VerilogPath + "reg32byte.v",
       VerilogPath + "reg256byte.v",
       VerilogPath + "aes_128_abs.v"},                // designs
      "aes_top",                                      // top_module_name
      RefrelPath + "ref-rel-var-map.json",            // variable mapping
      RefrelPath + "ref-rel-inst-cond.json",          // conditions of start/ready
      OutputPath,                                     // output path
      model.get(),                                    // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.GenerateTargets();
}

/// To verify the block level operation of ILA
void verifyBlockLevel(Ila& model) {

  VerilogGeneratorBase::VlgGenConfig vlg_cfg;
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;

  vtg_cfg.MemAbsReadAbstraction = true; // enable read abstraction
  vtg_cfg.MaxBound = 40;                // bound of BMC
  vlg_cfg.pass_node_name = true;	// whether to use node name in Verilog

  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification/";

  VerilogVerificationTargetGenerator vg(
      {}, // no include
      {VerilogPath + "aes_top.v",   VerilogPath + "reg2byte.v",
       VerilogPath + "reg16byte.v", VerilogPath + "reg32byte.v",
       VerilogPath + "reg256byte.v",
       VerilogPath + "aes_128_abs.v"},                // designs
      "aes_top",                                      // top_module_name
      RefrelPath + "ref-rel-var-map-uaes.json",       // variable mapping
      RefrelPath + "ref-rel-inst-cond-uaes.json",     // conditions of start/ready
      OutputPath,                                     // output path
      model.child(0).get(),                           // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.GenerateTargets();
}

/// Build the model
int main() {
  // set ilang option, operators like '<' will refer to unsigned arithmetics
  SetUnsignedComparation(true); 
  // build the aes model
  AES aes_ila_model;
  // build the aes128 model
  AES_128 aes128;
  // verify separately the hierarchical ILA
  // from IO level down to the AES 128 function
  verifyIO(aes_ila_model.model);
  verifyBlockLevel(aes_ila_model.model);
  verifyAES128(aes128.model);

  return 0;
}

/// \file
/// Unit test for generating Verilog verification target

#include <ilang/ila/instr_lvl_abs.h>
#include <ilang/ilang++.h>
#include <ilang/synth-interface/synth_engine_interface.h>
#include <ilang/vtarget-out/vtarget_gen.h>

#include "unit-include/config.h"
#include "unit-include/pipe_ila.h"
#include "unit-include/memswap.h"
#include "unit-include/util.h"

namespace ilang {

#define SET_GLOBAL_CONFIG (config.VerificationSettingAvoidIssueStage=true)

TEST(TestVlgTargetGen, AesIlaInfo) {

  auto aesFile = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/aes_v/all";
  auto aesuFile = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/aes_v/allu";
  auto aes = ImportSynthAbsFromFile(aesFile, "AES");
  auto aesu = ImportSynthAbsFromFile(aesuFile, "AES_U");

  // aes->AddChild(aesu);

  ILA_DLOG("TestVlgTargetGen.IlaInfo") << "No. instr:" << aes->instr_num();
  for (unsigned i = 0; i < aes->instr_num(); ++i) {
    ILA_DLOG("TestVlgTargetGen.IlaInfo") << "\t" << aes->instr(i)->name().str()
                                         << "\t" << aes->instr(i)->decode();
  }
  ILA_DLOG("TestVlgTargetGen.IlaInfo") << "No. state:" << aes->state_num();
  for (unsigned i = 0; i < aes->state_num(); ++i) {
    ILA_DLOG("TestVlgTargetGen.IlaInfo") << "\t" << aes->state(i)->name().str();
  }
  ILA_DLOG("TestVlgTargetGen.IlaInfo") << std::endl;

  ILA_DLOG("TestVlgTargetGen.IlaInfo") << "No. instr:" << aesu->instr_num();
  for (unsigned i = 0; i < aes->instr_num(); ++i) {
    ILA_DLOG("TestVlgTargetGen.IlaInfo") << "\t" << aesu->instr(i)->name().str()
                                         << "\t" << aesu->instr(i)->decode();
  }
  ILA_DLOG("TestVlgTargetGen.IlaInfo") << "No. state:" << aesu->state_num();
  for (unsigned i = 0; i < aesu->state_num(); ++i) {
    ILA_DLOG("TestVlgTargetGen.IlaInfo")
        << "\t" << aesu->state(i)->name().str();
  }
  ILA_DLOG("TestVlgTargetGen.IlaInfo") << std::endl;
}

TEST(TestVlgTargetGen, PipeExample) {
  auto config = VerilogVerificationTargetGenerator::vtg_config_t();
  SET_GLOBAL_CONFIG;

  config.CosaGenJgTesterScript = true; // generate a jg tester script

  auto ila_model = SimplePipe::BuildModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/";
  VerilogVerificationTargetGenerator vg(
      {},                          // no include
      {dirName + "simple_pipe.v"}, //
      "pipeline_v",                // top_module_name
      dirName + "rfmap/vmap.json", // variable mapping
      dirName + "rfmap/cond.json", dirName + "verify/", ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      config);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();

}

// test all kinds of rfmap issue
// test bad states

TEST(TestVlgTargetGen, PipeExampleJasperGold) {
  auto config = VerilogVerificationTargetGenerator::vtg_config_t();
  SET_GLOBAL_CONFIG;

  auto ila_model = SimplePipe::BuildModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/";
  VerilogVerificationTargetGenerator vg(
      {},                          // no include
      {dirName + "simple_pipe.v"}, //
      "pipeline_v",                // top_module_name
      dirName + "rfmap/vmap.json", // variable mapping
      dirName + "rfmap/cond.json", dirName + "verify_jg/", ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::JASPERGOLD,
      config);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();
}

TEST(TestVlgTargetGen, PipeExampleNotEqu) {
  auto config = VerilogVerificationTargetGenerator::vtg_config_t();
  SET_GLOBAL_CONFIG;

  auto ila_model = SimplePipe::BuildModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/";
  VerilogVerificationTargetGenerator vg(
      {},                          // no include
      {dirName + "simple_pipe_wrong.v"}, //
      "pipeline_v",                // top_module_name
      dirName + "rfmap/vmap.json", // variable mapping
      dirName + "rfmap/cond.json", dirName + "disprove/", ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      config);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();
}

TEST(TestVlgTargetGen, Memory) {
  auto config = VerilogVerificationTargetGenerator::vtg_config_t();
  SET_GLOBAL_CONFIG;

  auto ila_model = MemorySwap::BuildModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/vmem/";
  VerilogVerificationTargetGenerator vg(
      {},                    // no include
      {dirName + "swap.v"},  // vlog files
      "swap",                // top_module_name
      dirName + "vmap.json", // variable mapping
      dirName + "cond.json", // cond path
      dirName,               // output path
      ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      config);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();

}


TEST(TestVlgTargetGen, MemoryRead) {
  auto config = VerilogVerificationTargetGenerator::vtg_config_t();
  SET_GLOBAL_CONFIG;

  auto ila_model = MemorySwap::BuildRdModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/vmem/";
  VerilogVerificationTargetGenerator vg(
      {},                    // no include
      {dirName + "read.v"},  // vlog files
      "rdtop",                // top_module_name
      dirName + "vmap-rd.json", // variable mapping
      dirName + "cond-rd.json", // cond path
      dirName,               // output path
      ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      config);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();

}

TEST(TestVlgTargetGen, MemorySwapAbsRead) {
  VerilogVerificationTargetGenerator::vtg_config_t config;
  SET_GLOBAL_CONFIG;

  config.MemAbsReadAbstraction = true; // enable read abstraction

  auto ila_model = MemorySwap::BuildModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/vmem/";
  VerilogVerificationTargetGenerator vg(
      {},                    // no include
      {dirName + "swap.v"},  // vlog files
      "swap",                // top_module_name
      dirName + "vmap.json", // variable mapping
      dirName + "cond.json", // cond path
      dirName + "rdabs/",          // output path
      ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      config);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();

}


TEST(TestVlgTargetGen, MemoryReadAbsRead) {
  VerilogVerificationTargetGenerator::vtg_config_t config;
  SET_GLOBAL_CONFIG;

  config.MemAbsReadAbstraction = true; // enable read abstraction

  auto ila_model = MemorySwap::BuildRdModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/vmem/";
  VerilogVerificationTargetGenerator vg(
      {},                    // no include
      {dirName + "read.v"},  // vlog files
      "rdtop",                // top_module_name
      dirName + "vmap-rd.json", // variable mapping
      dirName + "cond-rd.json", // cond path
      dirName + "rdabs/",      // output path
      ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      config);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();

}


TEST(TestVlgTargetGen, MemoryReadAbsReadJasperGold) {
  VerilogVerificationTargetGenerator::vtg_config_t config;
  SET_GLOBAL_CONFIG;

  config.MemAbsReadAbstraction = true; // enable read abstraction

  auto ila_model = MemorySwap::BuildRdModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/vmem/";
  VerilogVerificationTargetGenerator vg(
      {},                    // no include
      {dirName + "read.v"},  // vlog files
      "rdtop",                // top_module_name
      dirName + "vmap-rd.json", // variable mapping
      dirName + "cond-rd.json", // cond path
      dirName + "rdabs_jg/",      // output path
      ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::JASPERGOLD,
      config);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();

}

TEST(TestVlgTargetGen, UndetValue) {
  VerilogVerificationTargetGenerator::vtg_config_t config;
  SET_GLOBAL_CONFIG;

  auto ila_model = UndetVal::BuildModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/undetf/";
  VerilogVerificationTargetGenerator vg(
      {},                        // no include
      {dirName + "val.v"},       // vlog files
      "undetval",                // top_module_name
      dirName + "vmap-val.json", // variable mapping
      dirName + "cond-val.json", // cond path
      dirName,                   // output path
      ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      config);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();

}



TEST(TestVlgTargetGen, UndetFunc) {
  VerilogVerificationTargetGenerator::vtg_config_t config;
  SET_GLOBAL_CONFIG;

  auto ila_model = UndetFunc::BuildModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/undetf/";
  VerilogVerificationTargetGenerator vg(
      {},                        // no include
      {dirName + "func.v"},       // vlog files
      "undetfunc",                // top_module_name
      dirName + "vmap-func.json", // variable mapping
      dirName + "cond-func.json", // cond path
      dirName,                   // output path
      ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      config);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();

}


TEST(TestVlgTargetGen, AesExample) {}

}; // namespace ilang

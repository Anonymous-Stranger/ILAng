/// \file
/// Unit test for generating Verilog verification target

#include <ilang/ila/instr_lvl_abs.h>
#include <ilang/ilang++.h>
#include <ilang/vtarget-out/vtarget_gen.h>

#include "unit-include/config.h"
#include "unit-include/memswap.h"
#include "unit-include/pipe_ila.h"
#include "unit-include/util.h"

namespace ilang {

TEST(TestVlgTargetGen, AesIlaInfo) {

  auto aesFile = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/aes_v/all";
  auto aesuFile = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/aes_v/allu";
  auto aes = ImportSynthAbstraction(aesFile, "AES").get();
  auto aesu = ImportSynthAbstraction(aesuFile, "AES_U").get();

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
  auto ila_model = SimplePipe::BuildModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/";
  VerilogVerificationTargetGenerator vg(
      {},                          // no include
      {dirName + "simple_pipe.v"}, //
      "pipeline_v",                // top_module_name
      dirName + "rfmap/vmap.json", // variable mapping
      dirName + "rfmap/cond.json", dirName + "verify/", ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();
}

// test all kinds of rfmap issue
// test bad states

TEST(TestVlgTargetGen, PipeExampleJasperGold) {
  auto ila_model = SimplePipe::BuildModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/";
  VerilogVerificationTargetGenerator vg(
      {},                          // no include
      {dirName + "simple_pipe.v"}, //
      "pipeline_v",                // top_module_name
      dirName + "rfmap/vmap.json", // variable mapping
      dirName + "rfmap/cond.json", dirName + "verify_jg/", ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::JASPERGOLD);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();
}

TEST(TestVlgTargetGen, PipeExampleNotEqu) {
  auto ila_model = SimplePipe::BuildModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/";
  VerilogVerificationTargetGenerator vg(
      {},                                // no include
      {dirName + "simple_pipe_wrong.v"}, //
      "pipeline_v",                      // top_module_name
      dirName + "rfmap/vmap.json",       // variable mapping
      dirName + "rfmap/cond.json", dirName + "disprove/", ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();
}

TEST(TestVlgTargetGen, Memory) {
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
      VerilogVerificationTargetGenerator::backend_selector::COSA);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();
}

TEST(TestVlgTargetGen, MemoryRead) {
  auto ila_model = MemorySwap::BuildRdModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/vmem/";
  VerilogVerificationTargetGenerator vg(
      {},                       // no include
      {dirName + "read.v"},     // vlog files
      "rdtop",                  // top_module_name
      dirName + "vmap-rd.json", // variable mapping
      dirName + "cond-rd.json", // cond path
      dirName,                  // output path
      ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();
}

TEST(TestVlgTargetGen, MemoryAbsRead) {
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;

  vtg_cfg.MemAbsReadAbstraction = true; // enable read abstraction

  auto ila_model = MemorySwap::BuildModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/vmem/";
  VerilogVerificationTargetGenerator vg(
      {},                    // no include
      {dirName + "swap.v"},  // vlog files
      "swap",                // top_module_name
      dirName + "vmap.json", // variable mapping
      dirName + "cond.json", // cond path
      dirName + "rdabs/",    // output path
      ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA, vtg_cfg);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();
}

TEST(TestVlgTargetGen, MemoryReadAbsRead) {
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;

  vtg_cfg.MemAbsReadAbstraction = true; // enable read abstraction

  auto ila_model = MemorySwap::BuildRdModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/vmem/";
  VerilogVerificationTargetGenerator vg(
      {},                       // no include
      {dirName + "read.v"},     // vlog files
      "rdtop",                  // top_module_name
      dirName + "vmap-rd.json", // variable mapping
      dirName + "cond-rd.json", // cond path
      dirName + "rdabs/",       // output path
      ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA, vtg_cfg);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();
}

TEST(TestVlgTargetGen, MemoryReadAbsReadJasperGold) {
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;

  vtg_cfg.MemAbsReadAbstraction = true; // enable read abstraction

  auto ila_model = MemorySwap::BuildRdModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/vmem/";
  VerilogVerificationTargetGenerator vg(
      {},                       // no include
      {dirName + "read.v"},     // vlog files
      "rdtop",                  // top_module_name
      dirName + "vmap-rd.json", // variable mapping
      dirName + "cond-rd.json", // cond path
      dirName + "rdabs_jg/",    // output path
      ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::JASPERGOLD,
      vtg_cfg);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();
}

TEST(TestVlgTargetGen, UndetValue) {
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
      VerilogVerificationTargetGenerator::backend_selector::COSA);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();
}

TEST(TestVlgTargetGen, UndetFunc) {
  auto ila_model = UndetFunc::BuildModel();

  auto dirName = std::string(ILANG_TEST_SRC_ROOT) + "/unit-data/vpipe/undetf/";
  VerilogVerificationTargetGenerator vg(
      {},                         // no include
      {dirName + "func.v"},       // vlog files
      "undetfunc",                // top_module_name
      dirName + "vmap-func.json", // variable mapping
      dirName + "cond-func.json", // cond path
      dirName,                    // output path
      ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateTargets();
}

TEST(TestVlgTargetGen, AesExample) {}

}; // namespace ilang

/// \file
/// Unit test for invariant synthesis

#include <ilang/util/fs.h>
#include <ilang/ila/instr_lvl_abs.h>
#include <ilang/ilang++.h>
#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>
#include <ilang/vtarget-out/inv-syn/sygus/sim_trace_extract.h>

#include "unit-include/config.h"
#include "unit-include/pipe_ila.h"
#include "unit-include/memswap.h"
#include "unit-include/util.h"

namespace ilang {


typedef std::vector<std::string> P;
// Z3, ABC, FREQHORN

// this is the only one with exec
// This test uses CEGAR loop
// This tests has no extra start cycle
TEST(TestVlgVerifInvSyn, SimpleCntCegar) {
  auto ila_model = CntTest::BuildModel();

  VerilogVerificationTargetGenerator::vtg_config_t cfg;
  cfg.InvariantSynthesisReachableCheckKeepOldInvariant = false;
  cfg.CosaAddKeep = false;
  cfg.VerificationSettingAvoidIssueStage = true;
  cfg.YosysSmtFlattenDatatype = false; // let's test flatten datatype also
  cfg.YosysSmtFlattenHierarchy = true;

  auto dirName = os_portable_append_dir( std::string(ILANG_TEST_SRC_ROOT) , 
    P({ "unit-data","inv_syn","cnt2"}) );
  auto outDir  = os_portable_append_dir( std::string(ILANG_TEST_SRC_ROOT) , 
    P({"unit-data","inv_syn","cnt2-cex"}) );

  InvariantSynthesizerCegar vg(
      {},                          // no include
      {os_portable_append_dir(dirName , P({"verilog", "opposite.v" }))} , //
      "opposite",                // top_module_name
      os_portable_append_dir(dirName , P({ "rfmap","vmap.json" })), // variable mapping
      os_portable_append_dir(dirName , P({ "rfmap","cond-noinv.json" })),
      outDir, ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      VerilogVerificationTargetGenerator::synthesis_backend_selector::Z3,
      cfg);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateVerificationTarget();
  EXPECT_FALSE(vg.RunVerifAuto("INC", "", true));
  vg.ExtractVerificationResult();
  vg.GenerateSynthesisTarget(); // you will need fp engine
  EXPECT_FALSE(vg.RunSynAuto(true));
  vg.ExtractSynthesisResult(); // very weired, it throw away something in arg
  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateInvariantVerificationTarget();
  auto design_stat = vg.GetDesignStatistics();
  design_stat.StoreToFile(os_portable_append_dir(outDir, "design_stat.txt"));
  ILA_INFO << "========== Design Info ==========" ;
  ILA_INFO << "#bits= " << design_stat.NumOfDesignStateBits;
  ILA_INFO << "#vars=" << design_stat.NumOfDesignStateVars;
  ILA_INFO << "#extra_bits= " << design_stat.NumOfExtraStateBits;
  ILA_INFO << "#extra_vars=" << design_stat.NumOfExtraStateVars;
  ILA_INFO << "t(eq)= " << design_stat.TimeOfEqCheck;
  ILA_INFO << "t(syn)=" << design_stat.TimeOfInvSyn;
  ILA_INFO << "t(proof)= " << design_stat.TimeOfInvProof;
  ILA_INFO << "t(validate)=" << design_stat.TimeOfInvValidate;
  design_stat.LoadFromFile(outDir+"design_stat.txt");
} // CegarPipelineExample



TEST(TestVlgVerifInvSyn, SimpleCntCegarPassed) {
  auto ila_model = CntTest::BuildModel();

  VerilogVerificationTargetGenerator::vtg_config_t cfg;
  cfg.InvariantSynthesisReachableCheckKeepOldInvariant = false;
  cfg.CosaAddKeep = false;
  cfg.VerificationSettingAvoidIssueStage = true;
  cfg.YosysSmtFlattenDatatype = false; // let's test flatten datatype also
  cfg.YosysSmtFlattenHierarchy = true;

  auto dirName = os_portable_append_dir( std::string(ILANG_TEST_SRC_ROOT) , 
    P({ "unit-data","inv_syn","cnt2"}) );
  auto outDir  = os_portable_append_dir( std::string(ILANG_TEST_SRC_ROOT) , 
    P({"unit-data","inv_syn","cnt2-pass"}) );

  InvariantSynthesizerCegar vg(
      {},                          // no include
      {os_portable_append_dir(dirName , P({"verilog", "opposite.v" }))} , //
      "opposite",                // top_module_name
      os_portable_append_dir(dirName , P({ "rfmap","vmap.json" })), // variable mapping
      os_portable_append_dir(dirName , P({ "rfmap","cond-noinv.json" })),
      outDir, ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      VerilogVerificationTargetGenerator::synthesis_backend_selector::Z3,
      cfg);

  EXPECT_FALSE(vg.in_bad_state());

  vg.GenerateVerificationTarget();
  EXPECT_TRUE(vg.RunVerifAuto("INC", "", true));
} // SimpleCntCegarPassed

}; // namespace ilang

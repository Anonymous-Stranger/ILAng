/// \file Header for wrapper for verilog verification target generation
/// This is used to avoid include json explicitly
// --- Hongce Zhang

#ifndef ILANG_VTARGET_OUT_VTARGET_GEN_H__
#define ILANG_VTARGET_OUT_VTARGET_GEN_H__

#include <ilang/config.h>
#include <ilang/ila/instr_lvl_abs.h>
#include <ilang/verilog-out/verilog_gen.h>

namespace ilang {

/// \brief VlgVerifTgtGenBase: do nothing, should not instantiate
class VlgVerifTgtGenBase {
  // ----------------------- Type Definition ----------------------- //
public:

  // ----------- Verification Settings -------------- //
  /// Type of the backend
  typedef enum { NONE = 0, COSA = 1, JASPERGOLD = 2 } backend_selector;
  /// Type of invariant synthesis backend
  typedef enum {Z3, GRAIN, ABC, ELDERICA } synthesis_backend_selector;
  /// Verilog Target Generation Configuration
  typedef struct _vtg_config {
  /// Set the targets: instructions/invariants/both
  enum { INST, INV, BOTH } target_select;
  /// If not an empty string, then only check for that instruction
  std::string CheckThisInstructionOnly;
  /// Ensure the instruction will not be reseted while
  /// in the whole execution of checking instruction
  /// from reseted --> to forever
  bool InstructionNoReset; // true
  /// Does not insert assertions of variable mapping
  /// if an instruction does not update that var
  bool OnlyCheckInstUpdatedVars; // true
  /// whether to remove the extra issue cycle and starts from reset
  bool VerificationSettingAvoidIssueStage;

  // ----------- Options for CoSA settings -------------- //
  /// Do we set separate problems for different var map (CoSA only)
  bool PerVariableProblemCosa; // true
  /// Whether to abstract the memory read
  bool MemAbsReadAbstraction; // false
  /// Whether to force the instruction check to start from reset state
  bool ForceInstCheckReset;
  /// For COSA target generator : whether to force NEW/OLD port declaration
  enum { AUTO = 0, NEW = 1, OLD = 2 } PortDeclStyle;
  /// Generate a jg script to help validate cosa?
  bool CosaGenJgTesterScript;
  /// generate the trace for all variables? or just the relevent variables
  bool CosaFullTrace;
  /// For CoSA backend: do we add (* keep *)? default true, however, it can be
  /// buggy, so you can disable it if you want
  bool CosaAddKeep;
  /// whether to force dot reference check in the generation
  /// if you expect to use cosa on the it, yes, you need to
  /// use the default setting :  NOTIFY_PANIC
  /// in some rare cases, you may want to use JasperGold after it
  /// in that case, it is okay to just ignore it
  enum CosaDotReferenceNotify_t { NOTIFY_PANIC = 0, NOTIFY_WARNING = 1, NOTIFY_IGNORE = 2 } 
    CosaDotReferenceNotify;
  // The bound of BMC, default 127
  unsigned MaxBound;
  /// Only enforce var eq on updated vars, should not be used
  bool OnlyAssumeUpdatedVarsEq; // should be false

  // ----------- Options for CoSA script -------------- //
  /// If not empty, the generated script will include the path of Cosa
  std::string CosaPath;
  /// If not empty, the generated script will include sourcing a script
  std::string CosaPyEnvironment;
  /// A choice of solver (in the script)
  std::string CosaSolver;
  /// Whether the Solver should generate vcd trace
  bool CosaGenTraceVcd;
  /// Assumption overly constrained check
  bool CosaAssumptionOverlyConstrainedCheck;
  /// other CoSA options
  std::string CosaOtherSolverOptions;

  // ----------- Options for Yosys SMT-LIB2 Generator -------------- //
  /// The path to yosys, if yosys is not in the PATH, default empty
  std::string YosysPath;
  /// Whether to flatten the module hierarchy
  bool YosysSmtFlattenHierarchy;
  /// Whether to flatten the datatypes
  bool YosysSmtFlattenDatatype;
  /// for invariant synthesis do we keep memory abstraction in Verilog
  /// you can keep it true, untill the invariant refers to some memory there
  bool InvariantSynthesisKeepMemory;
  /// for invariant check, do we keep memory abstraction in Verilog
  bool InvariantCheckKeepMemory;
  /// Whether to assume the old invariants when check for reachability
  /// It seems for Z3, setting this to be false is faster (I don't know why)
  /// For freqhorn-enhance, this will be (internally) overwritten to be true
  bool InvariantSynthesisReachableCheckKeepOldInvariant;

  // ----------- Options for Z3/FreqHorn/ABC Solver -------------- //
  /// The path to Z3, if "z3" is not in the PATH, default empty
  std::string Z3Path;
  /// The path to FreqHorn, if "freqhorn" is not in the PATH, default empty
  std::string GrainPath;
  /// FreqHorn Configuration Options
  std::vector<std::string> GrainOptions;
  /// The path to ABC, if "abc" is not in the PATH, default empty
  std::string AbcPath;

  /// The default constructor for default values
  _vtg_config()
      : target_select(BOTH), CheckThisInstructionOnly(""),
        InstructionNoReset(true), OnlyCheckInstUpdatedVars(true),
        VerificationSettingAvoidIssueStage(false),

        // ----------- Options for CoSA settings -------------- //
        PerVariableProblemCosa(false),
        MemAbsReadAbstraction(false),
        ForceInstCheckReset(false), PortDeclStyle(AUTO),
        CosaGenJgTesterScript(false), CosaFullTrace(false), CosaAddKeep(true), 
        CosaDotReferenceNotify(CosaDotReferenceNotify_t::NOTIFY_PANIC),
        MaxBound(127),
        OnlyAssumeUpdatedVarsEq(false),

        // ----------- Options for CoSA script -------------- //
        CosaPath(""), CosaPyEnvironment(""),
        CosaSolver(""), CosaGenTraceVcd(true), 
        CosaAssumptionOverlyConstrainedCheck(false),
        CosaOtherSolverOptions(""),

        // ----------- Options for Yosys SMT-LIB2 Generator -------------- //
        YosysSmtFlattenHierarchy(false),
        YosysSmtFlattenDatatype(false),
        InvariantSynthesisKeepMemory(true),
        InvariantCheckKeepMemory(true),
        InvariantSynthesisReachableCheckKeepOldInvariant(false)

        {}
} vtg_config_t;

public:
  // ----------------------- Constructor/Destructor ----------------------- //
  // constructor : do nothing
  VlgVerifTgtGenBase() {}
  // destructor : do nothing (make it virtual !!!)
  virtual ~VlgVerifTgtGenBase() {}

private:
  // avoid instantiation
  virtual void do_not_instantiate(void) = 0;
}; // class VlgVerifTgtGenBase

class VerilogVerificationTargetGenerator {
public:
  /// Type of the backend
  using backend_selector = VlgVerifTgtGenBase::backend_selector;
  /// Type of configuration
  using vtg_config_t = VlgVerifTgtGenBase::vtg_config_t;

public:
  // --------------------- CONSTRUCTOR ---------------------------- //
  ///
  /// \param[in] implementation's include path (if it uses `include)
  /// \param[in] verilog's path, currently we only handle situation where all in
  /// the same folder \param[in] name of the top module of the implementation,
  /// leave "" to allow auto analysis \param[in] where to get variable mapping
  /// \param[in] where to get refinement relation
  /// \param[in] output path (ila-verilog, wrapper-verilog, problem.txt,
  /// run-verify-by-???, modify-impl, it there is ) \param[in] pointer to the
  /// ila \param[in] the backend selector \param[in] (optional) the default
  /// configuration for outputing verilog
  VerilogVerificationTargetGenerator(
      const std::vector<std::string>& implementation_include_path,
      const std::vector<std::string>& implementation_srcs,
      const std::string& implementation_top_module,
      const std::string& refinement_variable_mapping,
      const std::string& refinement_conditions, const std::string& output_path,
      const InstrLvlAbsPtr& ila_ptr, backend_selector backend,
      const vtg_config_t& vtg_config = vtg_config_t(),
      const VerilogGenerator::VlgGenConfig& config =
          VerilogGenerator::VlgGenConfig());
  // --------------------- DECONSTRUCTOR ---------------------------- //
  virtual ~VerilogVerificationTargetGenerator();

  /// export all targets
  void GenerateTargets(void);
  /// return true if the generator's in a bad state and cannot proceed.
  bool in_bad_state(void) const;

private:
  /// will be casted to different generator inside the implementation
  VlgVerifTgtGenBase* _generator;

}; // VerilogVerificationTargetGenerator

}; // namespace ilang

#endif // ILANG_VTARGET_OUT_VTARGET_GEN_H__

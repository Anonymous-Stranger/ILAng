/// \file
/// Template-based synthesis engine interface.

#include <ilang/synth-interface/synth_engine_mngr.h>
#include <ilang/util/log.h>
#include <ilasynth/abstraction.hpp>
// XXX ILA_ASSERT macro redefined in both ilang and ilasynth. Use ILA_CHECK.

namespace ilang {

SynEngMngr::SynEngMngr() {}

SynEngMngr::~SynEngMngr() {}

SynEngMngrPtr SynEngMngr::New() { return std::make_shared<SynEngMngr>(); }

InstrLvlAbsPtr SynEngMngr::ImportIlaFromFile(const std::string& fileName) {
  ilasynth::Abstraction abs("");
  try {
    abs.importAllFromFile(fileName);
  } catch (...) {
    ILA_ERROR << "Fail importing ILAs from " << fileName << std::endl;
  }

  // create place holder for the ILA
  auto model_name = abs.getName();
  auto m = InstrLvlAbs::New(model_name);

  // inputs
  auto inps_synth = abs.getInps();
  for (auto it : inps_synth) {
    ILA_DLOG("SynthImport") << "Input: " << it.first << std::endl;
    auto node = it.second.var;
    auto type = node->getType();
    auto name = node->getName();
    ILA_WARN_IF(name != it.first) << "Name mismatch " << name << std::endl;

    if (type.isBool()) {
      m->NewBoolInput(name);
    } else if (type.isBitvector()) {
      m->NewBvInput(name, type.bitWidth);
    } else {
      ILA_CHECK(type.isMem()) << "Unknown node type " << type << std::endl;
      m->NewMemInput(name, type.addrWidth, type.dataWidth);
    }
  }

  // state vars -- bits
  auto bits_synth = abs.getBits();
  for (auto it : bits_synth) {
    ILA_DLOG("SynthImport") << "Bit: " << it.first << std::endl;
    auto node = it.second.var;
    auto type = node->getType();
    auto name = node->getName();
    ILA_WARN_IF(!type.isBool()) << "Type mismatch " << name << std::endl;
    ILA_WARN_IF(name != it.first) << "Name mismatch " << name << std::endl;

    m->NewBoolState(name);
  }

  // state vars -- regs
  auto regs_synth = abs.getRegs();
  for (auto it : regs_synth) {
    ILA_DLOG("SynthImport") << "Reg: " << it.first << std::endl;
    auto node = it.second.var;
    auto type = node->getType();
    auto name = node->getName();
    ILA_WARN_IF(!type.isBitvector()) << "Type mismatch " << name << std::endl;
    ILA_WARN_IF(name != it.first) << "Name mismatch " << name << std::endl;

    m->NewBvState(name, type.bitWidth);
  }

  // state vars -- mem
  auto mems_synth = abs.getMems();
  for (auto it : mems_synth) {
    ILA_DLOG("SynthImport") << "Mem: " << it.first << std::endl;
    auto node = it.second.var;
    auto type = node->getType();
    auto name = node->getName();
    ILA_WARN_IF(!type.isMem()) << "Type mismatch " << name << std::endl;
    ILA_WARN_IF(name != it.first) << "Name mismatch " << name << std::endl;

    m->NewMemState(name, type.addrWidth, type.dataWidth);
  }

  // functions
  auto funs_synth = abs.getFuns();
  for (auto it : funs_synth) {
    ILA_DLOG("SynthImport") << "Fun: " << it.first << std::endl;
    auto node = it.second.var;
    auto type = node->getType();
    auto name = node->getName();
    ILA_WARN_IF(!type.isFunc()) << "Type mismatch " << name << std::endl;
    ILA_WARN_IF(name != it.first) << "Name mismatch " << name << std::endl;

    // TODO double check node type in ilasynth
    decltype(Sort::MakeBoolSort()) out_sort = NULL;
    if (type.bitWidth == 1) {
      out_sort = Sort::MakeBoolSort();
    } else {
      out_sort = Sort::MakeBvSort(type.bitWidth);
    }
    ILA_NOT_NULL(out_sort);

    std::vector<decltype(Sort::MakeBoolSort())> arg_sort = {};
    auto synth_args_type = type.argsWidth;
    for (auto i = 0; i != synth_args_type.size(); i++) {
      // FIXME
      arg_sort.push_back(Sort::MakeBoolSort());
    }
  }

  return m;
}

} // namespace ilang



#include <ilang/ila/instrlike/ilainstructionlike.h>
#include <ilang/ila/instr_lvl_abs.h>

namespace ilang {

IlaInstructionLike::IlaInstructionLike(
  const std::string& name, const InstrLvlAbsPtr& host
) : Object(name), host_(host) {}

void IlaInstructionLike::set_decode(const ExprPtr& decode) {
  ILA_ERROR_IF(decode_)
      << "Decode for " << name()
      << "has been assigned. Use ForceSetDecode to overwrite.";

  if (!decode_) {
    ForceSetDecode(decode);
  }
}

void IlaInstructionLike::ForceSetDecode(const ExprPtr& decode) {
  ILA_NOT_NULL(decode); // setting NULL pointer to decode function
  ILA_CHECK(decode->is_bool()) << "Decode must have Boolean sort.";

  decode_ = Unify(decode);
}

ExprPtr IlaInstructionLike::Unify(const ExprPtr& e) {
  return host_ ? host_->Unify(e) : e;
}

}



/// \file
/// Header for TSO memory model

#ifndef TSO_MANUAL_H__
#define TSO_MANUAL_H__

namespace ila {

/// \brief Class of TSO trace step
class TsoTraceStep : public TraceStep {
public:
  /// Type of trace step pointer
  typedef std::shared_ptr<TraceStep> TraceStepPtr;
  // ------------------------- MEMBERS -------------------------------------- //
  TraceStepPtr wfe_global; // maybe we should get away from raw pointer?
  // ------------------------- CONSTRUCTOR/DESTRUCTOR ----------------------- //
  /// To create a trace step (for inst, we don't need it for facet/init)
  TsoTraceStep(const InstrPtr & inst , ZExprVec & cstr, z3::context& ctx , size_t pos );
}; // class TsoTraceStep

/// \brief Class of TSO
class Tso : public MemoryModel {
public:
  /// Type of trace step pointer
  typedef std::shared_ptr<TsoTraceStep> TsoTraceStepPtr;
  /// Type of trace steps, we need to collect the set of trace steps (WRITE)
  typedef std::set<TsoTraceStepPtr> TsoTraceStepPtrSet;
protected:
  TraceStepPtrSet WRITE_list;
  TraceStepPtrSet READ_list;
  TraceStepPtrSet FENCE_list;
  TraceStepPtrSet RMW_list;
  TraceStepPtrSet PureWrite_list;

public:
  /// To create more view operations associated with an instruction, and also to add them to the set
  void virtual RegisterSteps(size_t regIdx , const InstrVec & _inst_seq) override;
  /// To do some extra bookkeeping work when it is known that no more instruction steps are needed.
  void virtual FinishRegisterSteps() override;
  /// To apply the axioms, the complete program should be given
  void virtual ApplyAxioms() override;
  // HZ note: All the step should be registered through the first function: RegisterSteps
  /// Constructor
  Tso(z3::context& ctx, 
    ZExprVec & _cstrlist, 
    const StateNameSet & shared_states, 
    const ILANameStateNameSetMap & private_states, 
    const InstrLvlAbsPtr & global_ila_ptr) :
   MemoryModel(ctx, _cstrlist, shared_states, private_states, global_ila_ptr) { }

private:
  z3::expr RF( const TraceStepPtr &w,const TraceStepPtr &r);
  z3::expr FR( const TraceStepPtr &r,const TraceStepPtr &w);
  z3::expr CO( const TraceStepPtr &w1,const TraceStepPtr &w2);

}; // class Tso


// ------------------------- ACCESSOR FUNCTIONs -------------------------------------- //
/// \brief DOT wfe DOT global
MemoryModel::TraceStepPtr __wfe_global(const MemoryModel::TraceStepPtr & ts);

}

#endif // TSO_MANUAL_H__

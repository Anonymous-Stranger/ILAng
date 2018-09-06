/// \file
/// Source for the base class of memory model

#include <string>
#include "mcm/memory_model.h"
#include "mcm/set_op.h"

namespace ila {

/******************************************************************************/
// Helper Class: VarUseFinder
/******************************************************************************/

template <class T, typename C> 
void VarUseFinder::Traverse(const ExprPtr & expr, VarUseList & uses ) {
  size_t num = expr->arg_num();
  for (size_t i = 0; i != num; ++i) 
    Traverse( expr->arg(i) );
  if ( expr->is_var() )
    uses.push_back( C(expr) );
}

template <class T, typename C> 
void VarUseFinder::Traverse(const InstrPtr & i, VarUseList & uses ) {
  Traverse( i->GetDecode , uses);

  StateNameSet instr_writes_ = i->GetUpdatedStates(); // get the set of updated state names
  for (auto & state_name_ : instr_writes_) 
    Traverse( i->GetUpdate(state_name_) , uses ); // traverse the update function
}

template <class T, typename C> 
void VarUseFinder::Traverse(const InstrLvlAbsPtr & i, VarUseList & uses ) {
  Traverse(i->fetch() , uses);
  Traverse(i->valid() , uses);

  size_t num = i->instr_num();
  for (size_t idx = 0; idx != num ; ++ idx) 
    Traverse(i->instr(idx) , uses);
}

std::string ExprPtr2Name(const ExprPtr & e) {
  return e->name();
}

/******************************************************************************/
// TraceStep
/******************************************************************************/

  // counter init
  unsigned TraceStep::trace_step_seq_no = 0;

  std::string TraceStep::GetName()
  {
    ++ trace_step_seq_no; // not thread-safe, but we are not building multi-threaded program anyway
    return _inst->name() + "_ts" + std::to_string(trace_step_seq_no);
  }

  void TraceStep::InitReadWriteSet( const InstrPtr & inst  )  // this can be the inst/parent inst
  {
      VarUseFinder<std::string, ExprPtr2Name> use_finder_;
      use_finder_.Traverse( inst , _inst_read_set );
      inst->GetUpdatedStates(_inst_write_set);
  }

  TraceStep::TraceStep(const InstrPtr & inst , ZExprVec & cstr, z3::context& ctx , size_t pos , const Z3ExprAdapterPtr & z3a  ) :
    _type(TraceStepType::INST_EVT), _inst(inst), _cstr(cstr), _name( GetName() ), timestamp( ctx.int_const( _name.c_str() ) ),
    _pos_suffix(pos) , _expr2z3_ptr_(z3a) , _ctx_(ctx)
    {
      InitReadWriteSet(inst);
      _cstr.push_back( timestamp > 0 ); // if it is not overwritten, then we will set this constraint
    }

  TraceStep::TraceStep(const InstrPtr & inst , ZExprVec & cstr, z3::context& ctx , Z3Expr ts_overwrite ,  size_t pos , const Z3ExprAdapterPtr & z3a) :
    _type(TraceStepType::INST_EVT), _inst(inst), _cstr(cstr), _name( GetName() ), timestamp( ts_overwrite ) ,
    _pos_suffix(pos) , _expr2z3_ptr_(z3a) , _ctx_(ctx)
    { InitReadWriteSet(inst); }


  /// To create a facet event , the last parameter is actually not in use now
  TraceStep::TraceStep(const InstrPtr & ref_inst, ZExprVec & cstr , z3::context& ctx, const std::string & s,  size_t pos , const Z3ExprAdapterPtr & z3a) :
    _type(TraceStepType::FACET_EVT), _parent_inst(ref_inst), _cstr(cstr), _name( GetName() + "_fevt" ), timestamp( ctx.int_const( _name.c_str() ) ) ,
    _pos_suffix(pos) , _expr2z3_ptr_(z3a) , _ctx_(ctx)
    { 
      InitReadWriteSet(_parent_inst); 
      _cstr.push_back( timestamp > 0 ); // For inst event and facet events
    }

  /// To update the set for TraceStepType::FACET_EVT
  void TraceStep::AddStateAccess(const std::string & name, AccessType acc_type)
  {
    ILA_ASSERT( _type == TraceStepType::FACET_EVT ) << "Cannot change the state access info of instruction trace step: "<< _name ;
    switch(acc_type) {
      case AccessType::WRITE: 
        ILA_ASSERT( IN(name, _inst_write_set) ) << "Facet event cannot update state: " << name << " that is not written by the instruction";
        _write_state_set.insert(name);
        break;
      case AccessType::READ: 
        ILA_ASSERT( IN(name, _inst_read_set) ) << "Facet event cannot read state: " << name << " that is not used by the instruction";
        _read_state_set.insert(name);
        break;
      default: ILA_ASSERT( false ) << "Access type of state: " << name << " is neither READ nor WRITE";
    }
  }
  void TraceStep::AddStateAccess(const StateNameSet &s, AccessType acc_type)
  {
    ILA_ASSERT( _type == TraceStepType::FACET_EVT ) << "Cannot change the state access info of instruction trace step: "<< _name ;
    switch(acc_type) {
      case AccessType::WRITE: 
        for (auto & name : s) {
          ILA_ASSERT( IN(name, _inst_write_set) ) << "Facet event cannot update state: " << name << " that is not written by the instruction";
          _write_state_set.insert(name);
        }        
        break;
      case AccessType::READ: 
        for (auto & name : s) {
          ILA_ASSERT( IN(name, _inst_read_set) ) << "Facet event cannot read state: " << name << " that is not used by the instruction";
          _read_state_set.insert(name);
        }  
        break;
      default: ILA_ASSERT( false ) << "Access type of state: " << name << " is neither READ nor WRITE";
    }

  }

  bool TraceStep::Access( AccessType acc_type , const std::string & name) {
    ILA_ASSERT( _type == TraceStepType::INST_EVT ) << "Not implemented for facet event";
    ILA_ASSERT( acc_type == AccessType::READ || acc_type == AccessType::WRITE ) << "Access type must be READ or WRITE";

    StateNameSet & access_set ( acc_type == AccessType::READ ?  _inst_read_set : _inst_write_set);
    return IN(name, access_set) ;
  }


  bool TraceStep::Access( AccessType acc_type , SharedStatesSet * m_p_shared_states ) {
    ILA_ASSERT( m_p_shared_states ) << "Implementation bug: the shared state pointer is not set";
    ILA_ASSERT( _type == TraceStepType::INST_EVT ) << "Not implemented for facet event";
    ILA_ASSERT( acc_type == AccessType::READ || acc_type == AccessType::WRITE ) << "Access type must be READ or WRITE";
    
    StateNameSet & access_set ( acc_type == AccessType::READ ?  _inst_read_set : _inst_write_set);

    for(auto && _s_n : access_set) {
      if ( In_p(_s_n , m_p_shared_states) )
        return true;
    }
    return false;
  }

/******************************************************************************/
// MemoryModel
/******************************************************************************/

  bool MemoryModel::AccessShared ( const InstrPtr & ip, AccessType acc_type )
  {

    ILA_ASSERT(m_p_shared_states) << "Implementation bug: the shared state pointer is not set";

    StateNameSet access_set;
    if (acc_type == AccessType::READ) {
      VarUseFinder<std::string, ExprPtr2Name> use_finder_;
      use_finder_.Traverse( ip , access_set );
    } else if (acc_type == AccessType::WRITE) {
      ip->GetUpdatedStates(access_set);
    }
    else
      ILA_ASSERT(false) << "Access type of querying instruction: "<< ip->name() << " is neither READ or WRITE";

    for(auto && _s_n : access_set) {
      if ( In_p(_s_n , m_p_shared_states) )
        return true;
    }
    return false;
  }
  void MemoryModel::SetSharedStates(SharedStatesSet * p)
  { 
    ILA_ASSERT(m_p_shared_states == NULL) << "Implementation bug: overwriting the shared state pointer";
    m_p_shared_states = p;
    for( auto && n_l_pair : *m_p_shared_states ) {
      m_shared_state_names.insert(n_l_pair.first);
    }    
  }


/******************************************************************************/
// Helper Functions
/******************************************************************************/

/// This is to deal with forall (if does not exist, it should be true also)
z3::expr Z3ForallList(const std::vector<z3::expr> & l, z3::context& ctx_) {
  if ( l.size() == 0 )
    return ctx_.bool_val(true); // forall x, (if does not exist, it should be true also)
  if ( l.size() == 1 )
    return l[0];
  z3::expr ret = l[0];
  auto it = l.begin();   ++it;
  for(;it!= l.end(); ++it)
    ret = ret && (*it);
  return ret;
}

/// This is to apply to exists, (if does not exist, it should be false)
z3::expr Z3ExistsList(const std::vector<z3::expr> & l, z3::context& ctx_) {
  if ( l.size() == 0 )
    return ctx_.bool_val(false); // exists x, (if does not exist, it should be false)
  if ( l.size() == 1 )
    return l[0];
  z3::expr ret = l[0];
  auto it = l.begin();   ++it;
  for(;it!= l.end(); ++it)
    ret = ret || (*it);
  return ret;  
}


z3::expr Z3Implies(const z3::expr &a, const z3::expr &b) { return z3::implies(a,b); }
z3::expr Z3And(const z3::expr &a, const z3::expr &b) { return a&&b; }


/******************************************************************************/
// Axiom Functions
/******************************************************************************/

z3::expr HB( TraceStep & l, TraceStep & r )
{
  return l.timestamp < r.timestamp;
}
z3::expr PO( TraceStep & l, TraceStep & r )
{
  return SameCore(l,r) && HB(l,r);
}
z3::expr Decode( TraceStep & l) {
  return l.z3adapter()->GetExpr( l.inst()->GetDecode() , std::to_string( l.pos_suffix() ) ); 
  /// NOTE: in fact, there is also an extra_suffix
}

z3::expr SameAddress( TraceStep & l, TraceStep & r )
{
  // find the same 
  StateNameSet left;
  switch( l.type() ) {
    case TraceStepType::FACET_EVT: UNION(l._read_state_set , l._write_state_set, left );  break;
    case TraceStepType::INST_EVT : UNION(l._inst_read_set  , l._inst_write_set,  left );  break;
    default:
      ILA_ASSERT(false) << "TraceStepType is neither facet event or instruction event.";
  }

  StateNameSet right;
  switch( r.type() ) {
    case TraceStepType::FACET_EVT: UNION(r._read_state_set , r._write_state_set, right );  break;
    case TraceStepType::INST_EVT : UNION(r._inst_read_set  , r._inst_write_set,  right );  break;
    default:
      ILA_ASSERT(false) << "TraceStepType is neither facet event or instruction event.";
  }

  StateNameSet intersect;
  INTERSECT( left, right, intersect );
  if ( intersect.empty() )
    return l.ctx().bool_val(false);
  // so here we can be sure, there are some variables that are shared

  // but we need to make sure they have the same type
  StateNameSet same_type_intersect;
  for(auto & state_name : intersect) {

    /// Implementational bug: input 
    auto left_state_ptr  = l.inst()->host()->state(state_name);
    auto right_state_ptr = r.inst()->host()->state(state_name);

    if( left_state_ptr->sort() == right_state_ptr->sort()  ) 
      same_type_intersect.insert(state_name); // skip sort
  }
  if( same_type_intersect.empty() )
    return l.ctx().bool_val(false);

  // we need to know their types
  Z3Expr ret_expr = l.ctx().bool_val(true);

  for (auto & state_name : intersect) {
    auto mem_expr_left = l.inst()->host()->state(state_name);
    if( ! ( mem_expr_left->sort()->is_mem() ) )
      continue; // skip if not memory variable
    // Now: here is the most difficult thing 
    // we need to analyze the ast to get the address and data
    // and convert them to z3 expression
    auto mem_expr_right = r.inst()->host()->state(state_name);
    ret_expr = ret_expr && ( PeelAddr(mem_expr_left) == PeelAddr(mem_expr_right) );
  }
  return ret_expr;
}
z3::expr SameData( TraceStep & l, TraceStep & r);



bool SameCore( TraceStep & l, TraceStep & r) {
  // I hope this won't be a problem (it may require ctx to create constant)
  return SameCoreStatic(l,r) == SameAddress
}
StaticResult SameAddressStatic( TraceStep & l, TraceStep & r )
{
  // l.r \/ l.w \/ r.r \/ r.w =/= phi
  StateNameSet left;
  switch( l.type() ) {
    case TraceStepType::FACET_EVT: UNION(l._read_state_set , l._write_state_set, left );  break;
    case TraceStepType::INST_EVT : UNION(l._inst_read_set  , l._inst_write_set,  left );  break;
    default:
      ILA_ASSERT(false) << "TraceStepType is neither facet event or instruction event.";
  }

  StateNameSet right;
  switch( r.type() ) {
    case TraceStepType::FACET_EVT: UNION(r._read_state_set , r._write_state_set, right );  break;
    case TraceStepType::INST_EVT : UNION(r._inst_read_set  , r._inst_write_set,  right );  break;
    default:
      ILA_ASSERT(false) << "TraceStepType is neither facet event or instruction event.";
  }

  StateNameSet intersect;
  INTERSECT( left, right, intersect );

  // but we need to make sure they have the same type
  StateNameSet same_type_intersect;
  for(auto & state_name : intersect) {
    
    /// Implementational bug: input 
    auto left_state_ptr  = l.inst()->host()->state(state_name);
    auto right_state_ptr = r.inst()->host()->state(state_name);

    if( left_state_ptr->sort() == right_state_ptr->sort()  ) 
      same_type_intersect.insert(state_name); // skip sort
  }
  if( same_type_intersect.empty() )
    return StaticResult::STATIC_FALSE;
  return StaticResult::STATIC_UNKNOWN;

}

StaticResult DecodeStatic( TraceStep & l)
{
  return StaticResult::STATIC_UNKNOWN;
}
StaticResult SameCoreStatic( TraceStep & l, TraceStep & r)
{
  std::string & left =  l.inst()->host()->name().str();
  std::string & right = r.inst()->host()->name().str();
  if (left == right)
    return StaticResult::STATIC_TRUE;
  return STATIC_FALSE;
}

} // namespace ila
set ROOT [pwd]
set SRC $ROOT/verilog
set TCL $ROOT/scripts_impl_b
set EXP $ROOT/exps_impl_b

include $TCL/setup_w.tcl
include $TCL/eqcheck_w.tcl

set_prove_time_limit 0s; set_prove_per_property_time_limit 0s
set_max_trace_length 0; set_prove_per_property_max_time_limit 60m

assume -disable {::Subset - A}
assume -disable {::Subset - B}
assume -disable {::Subset - C1}
assume -disable {::Subset - C2}
assume -disable {::Subset - D}
assume -disable {::Subset - E}

assert -disable <embedded>::terminate
assume -disable {::arch equal - data}; 
assume -disable {::init - consistent RAM access}

set_max_trace_length 16; set_prove_per_property_max_time_limit 0s 

#exit

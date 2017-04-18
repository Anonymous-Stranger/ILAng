set ROOT [pwd]
set SRC $ROOT/verilog
set TCL $ROOT/scripts_impl_d
set EXP $ROOT/exps_impl_d

include $TCL/setup_w.tcl
include $TCL/eqcheck_w.tcl

set_prove_time_limit 0s; set_prove_per_property_time_limit 0s
set_max_trace_length 0; set_prove_per_property_max_time_limit 60m

assert -remove <embedded>::terminate; assert -remove {<embedded>::eq - arg_1_TREADY}; assert -remove {<embedded>::eq - arg_0_TVALID}; assert -remove {<embedded>::eq - LB1D_p_cnt}; assert -remove {<embedded>::eq - in_stream_empty}; assert -remove {<embedded>::eq - in_stream_full}; assert -remove {<embedded>::eq - LB2D_proc_x}; assert -remove {<embedded>::eq - LB2D_proc_y}; assert -remove {<embedded>::eq - LB2D_proc_w}; assert -remove {<embedded>::eq - slice_stream_empty}; assert -remove {<embedded>::eq - slice_stream_full}; assert -remove {<embedded>::eq - LB2D_shift_x}; assert -remove {<embedded>::eq - LB2D_shift_y}; assert -remove {<embedded>::eq - stencil_stream_empty}; assert -remove {<embedded>::eq - stencil_stream_full}; assert -remove {<embedded>::eq - pp_it_1}; assert -remove {<embedded>::eq - pp_it_2}; assert -remove {<embedded>::eq - pp_it_3}; assert -remove {<embedded>::eq - pp_it_4}; assert -remove {<embedded>::eq - pp_it_5}; assert -remove {<embedded>::eq - pp_it_6}; assert -remove {<embedded>::eq - pp_it_7}; assert -remove {<embedded>::eq - pp_it_8}; assert -remove {<embedded>::eq - pp_it_9}; assert -remove {<embedded>::eq - exit_it_1}; assert -remove {<embedded>::eq - exit_it_2}; assert -remove {<embedded>::eq - exit_it_3}; assert -remove {<embedded>::eq - exit_it_4}; assert -remove {<embedded>::eq - exit_it_5}; assert -remove {<embedded>::eq - exit_it_6}; assert -remove {<embedded>::eq - exit_it_7}; assert -remove {<embedded>::eq - LB1D_in}; assert -remove {<embedded>::eq - LB1D_buff}; assert -remove {<embedded>::eq - in_stream_buff_0}; assert -remove {<embedded>::eq - in_stream_buff_1}; assert -remove {<embedded>::eq - slice_stream_buff_0}; assert -remove {<embedded>::eq - slice_stream_buff_1}; assert -remove {<embedded>::eq - stencil_stream_buff_0}; assert -remove {<embedded>::eq - stencil_stream_buff_1}; assert -remove {<embedded>::eq - LB2D_shift_0}; assert -remove {<embedded>::eq - LB2D_shift_1}; assert -remove {<embedded>::eq - LB2D_shift_2}; assert -remove {<embedded>::eq - LB2D_shift_3}; assert -remove {<embedded>::eq - LB2D_shift_4}; assert -remove {<embedded>::eq - LB2D_shift_5}; assert -remove {<embedded>::eq - LB2D_shift_6}; assert -remove {<embedded>::eq - LB2D_shift_7}; assert -remove {<embedded>::eq - arg_0_TDATA}; assert -remove {<embedded>::eq - LB2D_proc_0}; assert -remove {<embedded>::eq - LB2D_proc_1}; assert -remove {<embedded>::eq - LB2D_proc_2}; assert -remove {<embedded>::eq - LB2D_proc_3}; assert -remove {<embedded>::eq - LB2D_proc_4}; assert -remove {<embedded>::eq - LB2D_proc_5}; assert -remove {<embedded>::eq - LB2D_proc_6}; assert -remove {<embedded>::eq - LB2D_proc_7}

assume -disable {::Subset - A}
assume -disable {::Subset - B}
assume -disable {::Subset - C1}
assume -disable {::Subset - C2}
assume -disable {::Subset - D}
assume -disable {::Subset - E}

assume -disable {::arch equal - data}; 
assume -disable {::init - consistent RAM access}

#assume -enable {::Subset - A}
#include $TCL/iterate_bound_w.tcl
#save -jdb $EXP/wri_bound_A.jdb -capture_setup -capture_session_data
#assume -disable {::Subset - A}

#assume -enable {::Subset - B}
#include $TCL/iterate_bound_w.tcl
#save -jdb $EXP/wri_bound_B.jdb -capture_setup -capture_session_data
#assume -disable {::Subset - B}

#assume -enable {::Subset - C1}
#include $TCL/iterate_bound_w.tcl
#save -jdb $EXP/wri_bound_C1.jdb -capture_setup -capture_session_data
#assume -disable {::Subset - C1}

#assume -enable {::Subset - C2}
#include $TCL/iterate_bound_w.tcl
#save -jdb $EXP/wri_bound_C2.jdb -capture_setup -capture_session_data
#assume -disable {::Subset - C2}

#assume -enable {::Subset - D}
#include $TCL/iterate_bound_w.tcl
#save -jdb $EXP/wri_bound_D.jdb -capture_setup -capture_session_data
#assume -disable {::Subset - D}

#assume -enable {::Subset - E}
#include $TCL/iterate_bound_w.tcl
#save -jdb $EXP/wri_bound_E.jdb -capture_setup -capture_session_data
#assume -disable {::Subset - E}

#exit


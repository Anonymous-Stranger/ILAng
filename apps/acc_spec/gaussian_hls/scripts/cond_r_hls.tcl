# Assumptions for proving read instruction -- HLS
#
# initial state for read instruction -- HLS.
assume -name {HLS start} -env \
{ (cnt_init == 0) |=> (hls_complete == 0) \
} -type {temporary} -update_db;

# terminating condition -- HLS
assume -name {HLS continue} -env \
{ (cnt_init == 1) |-> ( \
    ((hls_arg_0_TVALID == 1) & \
     (hls_target_U.hls_target_Loop_1_proc_U0.p_p2_in_bounded_stencil_stream_V_value_V_read == 0)) \
    |=> \
    (hls_complete == 1) \
)} -type {temporary} -update_db;

assume -name {HLS break} -env \
{ (cnt_init == 1) |-> ( \
   ((hls_arg_0_TVALID == 0) | \
    (hls_target_U.hls_target_Loop_1_proc_U0.p_p2_in_bounded_stencil_stream_V_value_V_read == 1)) \
   |=> \
   (hls_complete == 0) \
)} -type {temporary} -update_db;

# initialize micro-architecture states in HLS implementation.
# hls_target_Loop_1_proc_U0
assume -name {HLS init-micro gaussion} -env \
{ (cnt_init == 0) |=> ( \
    hls_target_U.hls_target_Loop_1_proc_U0.ap_start == 1 & \
    hls_target_U.hls_target_Loop_1_proc_U0.ap_CS_fsm == 1 & \
    # ap_ST_st1_fsm_0 == 1
    hls_target_U.hls_target_Loop_1_proc_U0.ap_done_reg == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it0 == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it1 == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it2 == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it3 == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it4 == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it5 == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it6 == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it7 == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it8 == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it9 == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.arg_0_V_last_V_1_mVld == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.arg_0_V_value_V_1_mVld == 0 & \
    hls_target_U.hls_target_Loop_1_proc_U0.p_p2_in_bounded_stencil_stream_V_value_V_read == 0 \
    # others
)} -update_db;

# p_p2_in_bounded_stencil_stream_s_U
assume -name {HLS init-micro stencil} -env \
{ (cnt_init == 0) |=> ( \
    hls_target_U.p_p2_in_bounded_stencil_stream_s_U.mOutPtr == 1 & \
    hls_target_U.p_p2_in_bounded_stencil_stream_s_U.if_read == 0 & \
    hls_target_U.p_p2_in_bounded_stencil_stream_s_U.if_write == 0 \
) } -update_db;

# testing
#assume -name {testing} -env \
{ ( \
    hls_target_U.p_p2_in_bounded_stencil_stream_s_U.if_read == 0 \
) } -update_db;

# hls_target_linebuffer_1_U0
assume -name {HLS init-micro linebuffer} -env \
{ (cnt_init == 0) |=> ( \
    #hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.slice_stream_V_value_V_U.internal_empty_n == 0 \ 
    #hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.ap_CS_fsm == 1 & \ 
    #hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.i_fu_181_p2 == 0 \ 
    #hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.ap_CS_fsm == 1 & \ 
    #hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.col_reg_349 == 0 & \ 
    #hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.row_reg_327 == 0 \  
    hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.slice_stream_V_value_V_dout == 0 & \
    hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.buffer_0_value_V_fu_100 == 2 & \
    hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.buffer_1_value_V_fu_104 == 3 & \
    hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.buffer_2_value_V_fu_108 == 4 & \
    hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.buffer_3_value_V_fu_112 == 5 & \
    hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.buffer_4_value_V_fu_116 == 6 & \
    hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.buffer_5_value_V_fu_120 == 7 & \
    hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.buffer_6_value_V_fu_124 == 8 & \
    hls_target_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.buffer_7_value_V_fu_96 == 9 \
)} -update_db;


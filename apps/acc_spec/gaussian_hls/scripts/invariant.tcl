# Assumptions for refining initial condition by blocking unreachable states
#
# alias
assume -name {inv - alias} -env \
{ ( \
    hls_LB1D_buff == hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_Loop_1_proc_U0.in_axi_stream_V_value_V_0_data_reg & \
    hls_in_stream_empty == ~hls_U.hls_target_linebuffer_1_U0.in_stream_V_value_V_U.internal_empty_n & \
    hls_in_stream_full == ~hls_U.hls_target_linebuffer_1_U0.in_stream_V_value_V_U.internal_full_n & \
    hls_LB2D_proc_x == hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.col_reg_349 & \
    hls_LB2D_proc_y == hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.row_reg_327 & \
    hls_LB2D_proc_w == \
        hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.write_idx_1_reg_315 & \
    hls_slice_stream_empty == ~hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.slice_stream_V_value_V_U.internal_empty_n & \
    hls_slice_stream_full == ~hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.slice_stream_V_value_V_U.internal_full_n & \
    hls_LB2D_shift_x == \
        hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.i_0_i_i_reg_152 & \
    hls_LB2D_shift_y == \
        hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.n1_reg_141 & \
    hls_stencil_stream_empty == ~hls_U.p_p2_in_bounded_stencil_stream_s_U.internal_empty_n & \
    hls_stencil_stream_full == ~hls_U.p_p2_in_bounded_stencil_stream_s_U.internal_full_n & \
    hls_p_cnt == hls_U.hls_target_Loop_1_proc_U0.indvar_flatten_reg_434 & \
    hls_gb_pp_it_1 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it1 & \
    hls_gb_pp_it_2 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it2 & \
    hls_gb_pp_it_3 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it3 & \
    hls_gb_pp_it_4 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it4 & \
    hls_gb_pp_it_5 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it5 & \
    hls_gb_pp_it_6 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it6 & \
    hls_gb_pp_it_7 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it7 & \
    hls_gb_pp_it_8 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it8 & \
    hls_gb_pp_it_9 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it9 & \
    hls_gb_exit_it_1 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppstg_exitcond_flatten_reg_2790_pp0_it1 & \
    hls_gb_exit_it_2 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppstg_exitcond_flatten_reg_2790_pp0_it2 & \
    hls_gb_exit_it_3 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppstg_exitcond_flatten_reg_2790_pp0_it3 & \
    hls_gb_exit_it_4 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppstg_exitcond_flatten_reg_2790_pp0_it4 & \
    hls_gb_exit_it_5 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppstg_exitcond_flatten_reg_2790_pp0_it5 & \
    hls_gb_exit_it_6 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppstg_exitcond_flatten_reg_2790_pp0_it6 & \
    hls_gb_exit_it_7 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppstg_exitcond_flatten_reg_2790_pp0_it7 & \
    hls_gb_exit_it_8 == hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppstg_exitcond_flatten_reg_2790_pp0_it8 & \
    hls_LB1D_fsm == hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_Loop_1_proc_U0.ap_CS_fsm & \
    hls_LB1D_fsm_nxt == hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_Loop_1_proc_U0.ap_NS_fsm & \
    hls_LB2D_proc_fsm == hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.ap_CS_fsm & \
    hls_LB2D_proc_fsm_nxt == hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.ap_NS_fsm & \
    hls_LB2D_shift_fsm == hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.ap_CS_fsm & \
    hls_LB2D_shift_fsm_nxt == hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.ap_NS_fsm & \
    hls_GB_fsm == hls_U.hls_target_Loop_1_proc_U0.ap_CS_fsm & \
    hls_GB_fsm_nxt == hls_U.hls_target_Loop_1_proc_U0.ap_NS_fsm & \
    hls_in_stream_mOutPtr == hls_U.hls_target_linebuffer_1_U0.in_stream_V_value_V_U.mOutPtr & \
    hls_slice_stream_mOutPtr == hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.slice_stream_V_value_V_U.mOutPtr & \
    hls_stencil_stream_mOutPtr == hls_U.p_p2_in_bounded_stencil_stream_s_U.mOutPtr & \
    hls_timeout_LB1D == hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_Loop_1_proc_U0.exitcond_flatten_reg_88 \
)} -type {temporary} -update_db;

# arch states
# buffer status: should not be full and empty at the same time
assume -name {inv - stream buffer status} -env \
{ ( \
    ~(ila_in_stream_full == 1 & ila_in_stream_empty == 1) & \
    ~(ila_slice_stream_full == 1 & ila_slice_stream_empty == 1) & \
    ~(ila_stencil_stream_full == 1 & ila_stencil_stream_empty == 1) & \
    ~(hls_in_stream_full == 1 & hls_in_stream_empty == 1) & \
    ~(hls_slice_stream_full == 1 & hls_slice_stream_empty == 1) & \
    ~(hls_stencil_stream_full == 1 & hls_stencil_stream_empty == 1) \
)} -type {temporary} -update_db;

# x/y/w index: should fall in valid range
assume -name {inv - index bound} -env \
{ ( \
    ila_LB2D_proc_x >= 0 & ila_LB2D_proc_x < 488 & \
    ila_LB2D_proc_y >= 0 & ila_LB2D_proc_y < 648 & \
    ila_LB2D_proc_w >= 0 & ila_LB2D_proc_w < 8 & \
    ila_LB2D_shift_x >= 0 & ila_LB2D_shift_x < 488 & \
    ila_LB2D_shift_y >= 0 & ila_LB2D_shift_y < 640 & \
    ila_p_cnt >= 0 & ila_p_cnt <= 307200 & \
    hls_LB2D_proc_x >= 0 & hls_LB2D_proc_x < 488 & \
    hls_LB2D_proc_y >= 0 & hls_LB2D_proc_y < 648 & \
    hls_LB2D_proc_w >= 0 & hls_LB2D_proc_w < 8 & \
    hls_LB2D_shift_x >= 0 & hls_LB2D_shift_x < 488 & \
    hls_LB2D_shift_y >= 0 & hls_LB2D_shift_y < 640 & \
    hls_p_cnt >= 0 & hls_p_cnt <= 307200 \
)} -type {temporary} -update_db;

# buffer status should match pixel position (idx)
assume -name {inv - proc vs slice - ila} -env \
{ ( \
    ila_LB2D_proc_y < 8 |-> ila_slice_stream_empty == 1 \
)} -type {temporary} -update_db;
#
assume -name {inv - proc vs slice - hls} -env \
{ ( \
    hls_LB2D_proc_y < 8 |-> hls_slice_stream_empty == 1 \
)} -type {temporary} -update_db;
#
assume -name {inv - shift vs stencil - ila} -env \
{ ( \
    (ila_LB2D_shift_x < 8 & ila_LB2D_shift_y == 0) |-> ila_stencil_stream_empty == 1 \
)} -type {temporary} -update_db;
#
assume -name {inv - shift vs stencil - hls} -env \
{ ( \
    (hls_LB2D_shift_x < 8 & hls_LB2D_shift_y == 0) |-> hls_stencil_stream_empty == 1 \
)} -type {temporary} -update_db;
#
assume -name {inv - stencil vs gb} -env \
{ ( \
    (ila_p_cnt == 307200) |-> ( \
        ila_stencil_stream_empty == 1 & \
        ila_slice_stream_empty == 1 & \
        ila_in_stream_empty == 1 & \
        ila_LB2D_proc_x == 487 & \
        ila_LB2D_proc_y == 687 & \
        ila_LB2D_shift_x == 487 & \
        ila_LB2D_shift_y == 679 ) \
)} -type {temporary} -update_db;

# pixel position (idx) should be consistent across process unit
assume -name {inv - proc vs shift} -env \
{ ( \
    ila_LB2D_proc_y >= ila_LB2D_shift_y + 8 & \
    hls_LB2D_proc_y >= hls_LB2D_shift_y + 8 \
)} -type {temporary} -update_db;
#
assume -name {inv - proc vs shift - ila} -env \
{ ( \
    ila_LB2D_proc_y == ila_LB2D_shift_y + 8 |-> ila_LB2D_proc_x >= ila_LB2D_shift_x \
)} -type {temporary} -update_db;
#
assume -name {inv - proc vs shift - hls} -env \
{ ( \
    hls_LB2D_proc_y == hls_LB2D_shift_y + 8 |-> hls_LB2D_proc_x >= hls_LB2D_shift_x \
)} -type {temporary} -update_db;
#
#assume -name {inv - shift vs gb} -env \
{ ( \
    ila_p_cnt <= ila_LB2D_shift_y * 480 + ila_LB2D_shift_x - 8 & \
    hls_p_cnt <= hls_LB2D_shift_y * 480 + hls_LB2D_shift_x - 8 \
)} -type {temporary} -update_db;
# 
assume -name {inv - shift vs gb} -env \
{ ( \
    (((ila_p_cnt <= ila_LB2D_shift_x - 8) & (ila_LB2D_shift_y == 0)) | (ila_LB2D_shift_y != 0)) & \
    (((hls_p_cnt <= hls_LB2D_shift_x - 8) & (hls_LB2D_shift_y == 0)) | (hls_LB2D_shift_y != 0)) \
)} -type {temporary} -update_db;

# micro-arch states
# internal fsm state encoding
assume -name {inv - valid fsm} -env \
{ ( \
    ((hls_LB1D_fsm == 1) | (hls_LB1D_fsm == 2) | (hls_LB1D_fsm == 4)) & \
    ((hls_LB2D_proc_fsm == 1) | (hls_LB2D_proc_fsm == 2) | (hls_LB2D_proc_fsm == 4) | (hls_LB2D_proc_fsm == 8)) & \
    ((hls_LB2D_shift_fsm == 1) | (hls_LB2D_shift_fsm == 2) | (hls_LB2D_shift_fsm == 4) | (hls_LB2D_shift_fsm == 8)) & \
    ((hls_GB_fsm == 1) | (hls_GB_fsm == 2) | (hls_GB_fsm == 4)) & \
    ((hls_LB1D_fsm_nxt == 1) | (hls_LB1D_fsm_nxt == 2) | (hls_LB1D_fsm_nxt == 4)) & \
    ((hls_LB2D_proc_fsm_nxt == 1) | (hls_LB2D_proc_fsm_nxt == 2) | (hls_LB2D_proc_fsm_nxt == 4) | (hls_LB2D_proc_fsm_nxt == 8)) & \
    ((hls_LB2D_shift_fsm_nxt == 1) | (hls_LB2D_shift_fsm_nxt == 2) | (hls_LB2D_shift_fsm_nxt == 4) | (hls_LB2D_shift_fsm_nxt == 8)) & \
    ((hls_GB_fsm_nxt == 1) | (hls_GB_fsm_nxt == 2) | (hls_GB_fsm_nxt == 4)) \
)} -type {temporary} -update_db;
 
# stream buffer pointer
assume -name {inv - valid in stream pointer} -env \
{ ( \
    (hls_in_stream_full == 0 & hls_in_stream_empty == 1 & hls_in_stream_mOutPtr == 3) | \
    (hls_in_stream_full == 0 & hls_in_stream_empty == 0 & hls_in_stream_mOutPtr == 0) | \
    (hls_in_stream_full == 1 & hls_in_stream_empty == 0 & hls_in_stream_mOutPtr == 1) \
)} -type {temporary} -update_db;
#
assume -name {inv - valid slice stream pointer} -env \
{ ( \
    (hls_slice_stream_full == 0 & hls_slice_stream_empty == 1 & hls_slice_stream_mOutPtr == 3) | \
    (hls_slice_stream_full == 0 & hls_slice_stream_empty == 0 & hls_slice_stream_mOutPtr == 0) | \
    (hls_slice_stream_full == 1 & hls_slice_stream_empty == 0 & hls_slice_stream_mOutPtr == 1) \
)} -type {temporary} -update_db;
#
assume -name {inv - valid stencil stream pointer} -env \
{ ( \
    (hls_stencil_stream_full == 0 & hls_stencil_stream_empty == 1 & hls_stencil_stream_mOutPtr == 3) | \
    (hls_stencil_stream_full == 0 & hls_stencil_stream_empty == 0 & hls_stencil_stream_mOutPtr == 0) | \
    (hls_stencil_stream_full == 1 & hls_stencil_stream_empty == 0 & hls_stencil_stream_mOutPtr == 1) \
)} -type {temporary} -update_db;

# assume time out will not happen
#assume -name {inv - no timeout} -env \
{ ( \
    hls_timeout_LB1D == 0 \
)} -type {temporary} -update_db;

# axi config
assume -name {inv - axi config} -env \
{ ( \
    hls_U.ap_start == 1 & \
    hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_Loop_1_proc_U0.in_axi_stream_V_value_V_0_areset_d == 0 \
)} -type {temporary} -update_db;

#assume -env \
{ counter == 0 & hls_arg_0_TVALID == 1 |=> ( \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it1 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it2 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it3 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it4 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it5 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it6 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it7 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it8 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it9 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_CS_fsm == 2) \
)} -type {temporary} -update_db;

assume -env \
{ counter == 0 |=> ( \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it1 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it2 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it3 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it4 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it5 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it6 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it7 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it8 == 1) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_reg_ppiten_pp0_it9 == 0) & \
    (hls_U.hls_target_Loop_1_proc_U0.ap_CS_fsm == 2) \
)} -type {temporary} -update_db;

assume -name {tmp - block corner case} -env \
{ counter == 0 |=> ( \
    ila_U.LB2D_proc_x < 100 & \
    ila_U.LB2D_proc_x > 8 & \
    ila_U.LB2D_shift_x < 100 & \
    ila_U.LB2D_shift_x > 8 & \
    hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.exitcond3_reg_702 == 0 & \
    hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.row_reg_327 >= 8 & \
    hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.row_reg_327 < 100 & \
    hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.row_1_reg_693 >= 8 & \
    hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.row_1_reg_693 < 100 & \
    (hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.icmp_reg_698 == \
     hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_buf_proc_U0.icmp_fu_382_p2) & \
    (hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.icmp_reg_1260 == \
     hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.icmp_fu_197_p2) & \
    hls_U.hls_target_Loop_1_proc_U0.p_p2_in_bounded_stencil_stream_V_value_V_read == 0 \
)} -type {temporary} -update_db;

assume -name {tmp - inv for corner case} -env \
{ counter == 0 |=> ( \
    hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.i_0_i_i_reg_152 != 488 |-> \
    hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_U0.hls_target_call_U0.hls_target_call_Loop_LB2D_shift_proc_U0.ap_CS_fsm == 4 \
)} -type {temporary} -update_db;




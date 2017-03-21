# Assumptions for proving write instruction 
# Compositional verification: decode, invariant, ...
#
# decode 
assume -name {comp - decode} -env \
{ counter == 0 |=> ( \
    arg_1_TVALID == 1 & \
    arg_0_TREADY == 0 & \
    ila_arg_1_TREADY == 1 & \
    hls_arg_1_TREADY == 1 \
)} -type {temporary} -update_db;

assume -name {comp - protocol} -env \
{ counter == 0 |-> ( \
    ila_arg_1_TREADY == 1 & \
    hls_arg_1_TREADY == 1 \
)} -type {temporary} -update_db;

# turn off all other instruction (read and write) throughout verification
assume -name {comp - per instr} -env \
{ counter != 1 |-> ( \
    arg_1_TVALID == 0 & \
    arg_0_TREADY == 0 \
)} -type {temporary} -update_db;

# two models should have same architectural states
assume -name {arch equal - control} -env \
{ counter == 0 |=> ( \
    ila_LB1D_it_1 == hls_LB1D_it_1 & \
    ila_LB1D_p_cnt == hls_LB1D_p_cnt & \
    ila_arg_0_TDATA  == hls_arg_0_TDATA & \
    ila_arg_0_TVALID == hls_arg_0_TVALID & \
    ila_arg_1_TREADY == hls_arg_1_TREADY & \
    ila_in_stream_full == hls_in_stream_full & \
    ila_in_stream_empty == hls_in_stream_empty & \
    ila_slice_stream_full == hls_slice_stream_full & \
    ila_slice_stream_empty == hls_slice_stream_empty & \
    ila_stencil_stream_full == hls_stencil_stream_full & \
    ila_stencil_stream_empty == hls_stencil_stream_empty & \
    ila_LB2D_proc_x == hls_LB2D_proc_x & \
    ila_LB2D_proc_y == hls_LB2D_proc_y & \
    ila_LB2D_proc_w == hls_LB2D_proc_w & \
    ila_LB2D_shift_x == hls_LB2D_shift_x & \
    ila_LB2D_shift_y == hls_LB2D_shift_y & \
    ila_gb_p_cnt == hls_gb_p_cnt & \
    ila_gb_pp_it_1 == hls_gb_pp_it_1 & \
    ila_gb_pp_it_2 == hls_gb_pp_it_2 & \
    ila_gb_pp_it_3 == hls_gb_pp_it_3 & \
    ila_gb_pp_it_4 == hls_gb_pp_it_4 & \
    ila_gb_pp_it_5 == hls_gb_pp_it_5 & \
    ila_gb_pp_it_6 == hls_gb_pp_it_6 & \
    ila_gb_pp_it_7 == hls_gb_pp_it_7 & \
    ila_gb_pp_it_8 == hls_gb_pp_it_8 & \
    ila_gb_pp_it_9 == hls_gb_pp_it_9 & \
    ila_gb_exit_it_1 == hls_gb_exit_it_1 & \
    ila_gb_exit_it_2 == hls_gb_exit_it_2 & \
    ila_gb_exit_it_3 == hls_gb_exit_it_3 & \
    ila_gb_exit_it_4 == hls_gb_exit_it_4 & \
    ila_gb_exit_it_5 == hls_gb_exit_it_5 & \
    ila_gb_exit_it_6 == hls_gb_exit_it_6 & \
    ila_gb_exit_it_7 == hls_gb_exit_it_7 & \
    ila_gb_exit_it_8 == hls_gb_exit_it_8 \
)} -type {temporary} -update_db;
#
assume -name {arch equal - data} -env \
{ counter == 0 |=> ( \
    ila_in_stream_buff_0 == hls_in_stream_buff_0 & \
    ila_in_stream_buff_1 == hls_in_stream_buff_1 & \
    ila_slice_stream_buff_0 == hls_slice_stream_buff_0 & \
    ila_slice_stream_buff_1 == hls_slice_stream_buff_1 & \
    ila_stencil_stream_buff_0 == hls_stencil_stream_buff_0 & \
    ila_stencil_stream_buff_1 == hls_stencil_stream_buff_1 & \
    ila_LB1D_buff == hls_LB1D_buff & \
    ila_LB2D_shift_0 == hls_LB2D_shift_0 & \
    ila_LB2D_shift_1 == hls_LB2D_shift_1 & \
    ila_LB2D_shift_2 == hls_LB2D_shift_2 & \
    ila_LB2D_shift_3 == hls_LB2D_shift_3 & \
    ila_LB2D_shift_4 == hls_LB2D_shift_4 & \
    ila_LB2D_shift_5 == hls_LB2D_shift_5 & \
    ila_LB2D_shift_6 == hls_LB2D_shift_6 & \
    ila_LB2D_shift_7 == hls_LB2D_shift_7 \
)} -type {temporary} -update_db;

#assume -name {arch equal - start refine} -env \
{ counter == 0 |=> ( \
    (hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_Loop_1_proc_U0.in_stream_V_value_V_write == 0 & \
     ila_in_stream_full == hls_in_stream_full & \
     ila_in_stream_empty == hls_in_stream_empty) | \
    (hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_Loop_1_proc_U0.in_stream_V_value_V_write == 1 & \
     ila_in_stream_full == 1 & ila_in_stream_empty == 0 & \
     hls_in_stream_full == 0 & hls_in_stream_empty == 0) | \
    (hls_U.hls_target_linebuffer_1_U0.hls_target_linebuffer_Loop_1_proc_U0.in_stream_V_value_V_write == 1 & \
     ila_in_stream_full == 0 & ila_in_stream_empty == 0 & \
     hls_in_stream_full == 0 & hls_in_stream_empty == 1) \
)} -type {temporary} -update_db;




# Assumptions for proving read instruction
# Execute: start and stop models
#
# start
assume -name {exec - start model} -env \
{ counter <= 2 |-> ( \
    hls_complete == 0 & \
    ila_complete == 0 \
)} -type {temporary} -update_db;

# stop
assume -name {exec - ila continue} -env \
{ (counter > 1) |-> ( \
    ( \
    ((ila_U.arg_1_TREADY == 0) & (ila_U.in_stream_full == 0)) | \
    ((ila_U.in_stream_empty == 0) & \
        ((ila_U.slice_stream_full == 0) | (ila_U.LB2D_proc_y < 8))) | \
    ((ila_U.slice_stream_empty == 0) & \
        ((ila_U.stencil_stream_full == 0) | (ila_U.LB2D_shift_x < 8))) | \
    ((ila_U.arg_0_TVALID == 0) & \
        (((ila_U.gb_exit_it_1 == 0) & (ila_U.stencil_stream_empty == 0)) | \
        ((ila_U.gb_exit_it_1 == 1) & (ila_U.gb_exit_it_8 == 0)))) \
    ) \
    |=> \
    (ila_complete == 0) \
)} -type {temporary} -update_db;
# 
assume -name {exec - ila break} -env \
{ (counter > 1) |-> ( \
   ~( \
    ((ila_U.arg_1_TREADY == 0) & (ila_U.in_stream_full == 0)) | \
    ((ila_U.in_stream_empty == 0) & \
        ((ila_U.slice_stream_full == 0) | (ila_U.LB2D_proc_y < 8))) | \
    ((ila_U.slice_stream_empty == 0) & \
        ((ila_U.stencil_stream_full == 0) | (ila_U.LB2D_shift_x < 8))) | \
    ((ila_U.arg_0_TVALID == 0) & \
        (((ila_U.gb_exit_it_1 == 0) & (ila_U.stencil_stream_empty == 0)) | \
         ((ila_U.gb_exit_it_1 == 1) & (ila_U.gb_exit_it_8 == 0)))) \
    ) \
    |=> \
    (ila_complete == 1) \
)} -type {temporary} -update_db;
#
assume -name {exec - hls continue} -env \
{ (counter > 1) |-> ( \
    ( \
    ((hls_arg_1_TREADY == 0) & (hls_in_stream_full == 0)) | \
    ((hls_in_stream_empty == 0) & \
        ((hls_slice_stream_full == 0) | (hls_LB2D_proc_y < 8))) | \
    ((hls_slice_stream_empty == 0) & \
        ((hls_stencil_stream_full == 0) | (hls_LB2D_shift_x < 8))) | \
    ((hls_arg_0_TVALID == 0) & \
        (((hls_gb_exit_it_1 == 0) & (hls_stencil_stream_empty == 0)) | \
         ((hls_gb_exit_it_1 == 1) & (hls_gb_exit_it_8 == 0)))) \
    ) \
    | hls_arg_1_TREADY != 1 \
    |=> \
    (hls_complete == 0) \
)} -type {temporary} -update_db;
#
assume -name {exec - hls break} -env \
{ (counter > 1) |-> ( \
   ~( \
    ((hls_arg_1_TREADY == 0) & (hls_in_stream_full == 0)) | \
    ((hls_in_stream_empty == 0) & \
        ((hls_slice_stream_full == 0) | (hls_LB2D_proc_y < 8))) | \
    ((hls_slice_stream_empty == 0) & \
        ((hls_stencil_stream_full == 0) | (hls_LB2D_shift_x < 8))) | \
    ((hls_arg_0_TVALID == 0) & \
        (((hls_gb_exit_it_1 == 0) & (hls_stencil_stream_empty == 0)) | \
         ((hls_gb_exit_it_1 == 1) & (hls_gb_exit_it_8 == 0)))) \
    ) \
    & hls_arg_1_TREADY == 1 \
    |=> \
    (hls_complete == 1) \
)} -type {temporary} -update_db;

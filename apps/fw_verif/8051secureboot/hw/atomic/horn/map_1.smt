; mapping from ILA to HW_REG_WRITE_char for (sha_reg + 1)
(rule (let ((a!1 (and (HW_REG_WRITE_chr@_1
                  @sha_regs_0
                  HW_REG_WRITE_chr@%_call_0
                  HW_REG_WRITE_chr@%_3_0
                  HW_REG_WRITE_chr@%_4_0
                  HW_REG_WRITE_chr@%_2_0
                  HW_REG_WRITE_chr@%data_0
                  HW_REG_WRITE_chr@%addr_0)
                true
    ;; 
    (= addr_state 65025)
    (= addr_rdaddr 65028)
    (= addr_wraddr 65032)
    (= addr_len 65036)
    ;
    (= HW_REG_WRITE_chr@%addr_0 (+ @sha_regs_0 1))
    (= cmd 2)
    (= cmdaddr 65025)
    (= cmddata HW_REG_WRITE_chr@%data_0)
    (= sha_state (select HW_REG_WRITE_chr@%_call_0 addr_state))
    (= sha_rdaddr (select HW_REG_WRITE_chr@%_call_0 addr_rdaddr))
    (= sha_wraddr (select HW_REG_WRITE_chr@%_call_0 addr_wraddr))
    (= sha_len (select HW_REG_WRITE_chr@%_call_0 addr_len))
    ;
    (rel.decode_fe00 cmd cmdaddr b59)
    ;b59
    (rel.instr_fe01_sha_state_nxt cmd cmdaddr sha_state sha_state_nxt)
    (rel.instr_fe01_sha_len_nxt sha_len sha_len_nxt)
    (rel.instr_fe01_sha_rdaddr_nxt sha_rdaddr sha_rdaddr_nxt)
    (rel.instr_fe01_sha_wraddr_nxt sha_wraddr sha_wraddr_nxt)
    (rel.instr_fe01_XRAM_nxt XRAM cmd cmdaddr cmddata sha_state XRAM_nxt)
    ;
    (= XRAM_state (store XRAM_nxt addr_state sha_state_nxt))
    (= XRAM_len (store XRAM_state addr_len sha_len_nxt))
    (= XRAM_rdaddr (store XRAM_len addr_rdaddr sha_rdaddr_nxt))
    (= XRAM_wraddr (store XRAM_rdaddr addr_wraddr sha_wraddr_nxt))
    (= HW_REG_WRITE_chr@%_store_0 XRAM_wraddr))))
    ;; store
  (=> a!1
      (HW_REG_WRITE_chr@.split
        @sha_regs_0
        HW_REG_WRITE_chr@%_call_0
        HW_REG_WRITE_chr@%_store_0
        HW_REG_WRITE_chr@%_3_0
        HW_REG_WRITE_chr@%_4_0
        HW_REG_WRITE_chr@%_2_0
        HW_REG_WRITE_chr@%data_0
        HW_REG_WRITE_chr@%addr_0))))


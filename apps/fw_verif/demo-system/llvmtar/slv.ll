; ModuleID = '/home/byhuang/workspace/ILA/apps/fw_verif/demo-system/fwsrc/slv.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.MB_ITF_t = type { %struct.STS, %struct.R_CMD, %struct.R_DAT0, %struct.R_DAT1, %struct.R_SIZE, %struct.S_CMD, %struct.S_DAT0, %struct.S_DAT1, %struct.S_SIZE, %struct.ACK }
%struct.STS = type { i32, %struct.field }
%struct.field = type { i32, i32 }
%struct.R_CMD = type { i32 }
%struct.R_DAT0 = type { i32 }
%struct.R_DAT1 = type { i32 }
%struct.R_SIZE = type { i32 }
%struct.S_CMD = type { i32 }
%struct.S_DAT0 = type { i32 }
%struct.S_DAT1 = type { i32 }
%struct.S_SIZE = type { i32 }
%struct.ACK = type { i32 }

@gSlvFlag = global [2 x i32] zeroinitializer, align 4
@gMbCtx = global [8 x i32] zeroinitializer, align 16
@mst_sram = external global [0 x i8], align 1
@reg_MB = external global %struct.MB_ITF_t*, align 8
@reg_slv_int = external global i32, align 4

; Function Attrs: nounwind uwtable
define void @mainSlv() #0 {
entry:
  %slvBuff = alloca i8*, align 8
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i32, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @gSlvFlag, i64 0, i64 0), align 4
  %tobool = icmp ne i32 %0, 0
  %lnot = xor i1 %tobool, true
  br i1 %lnot, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %1 = load i32, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @gSlvFlag, i64 0, i64 1), align 4
  %conv = zext i32 %1 to i64
  %call = call noalias i8* @malloc(i64 %conv) #4
  store i8* %call, i8** %slvBuff, align 8
  %2 = load i8*, i8** %slvBuff, align 8
  %3 = load i32, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @gSlvFlag, i64 0, i64 1), align 4
  %conv1 = zext i32 %3 to i64
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* getelementptr inbounds ([0 x i8], [0 x i8]* @mst_sram, i32 0, i32 0), i64 %conv1, i32 1, i1 false)
  %4 = load i32, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @gSlvFlag, i64 0, i64 1), align 4
  %cmp = icmp ule i32 %4, 32
  %conv2 = zext i1 %cmp to i32
  call void @__VERIFIER_assert(i32 %conv2)
  store i32 4, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 0), align 16
  store i32 0, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 3), align 4
  call void @sendMsgSlv2Mst()
  %5 = load i8*, i8** %slvBuff, align 8
  call void @free(i8* %5) #4
  ret void
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #2

declare void @__VERIFIER_assert(i32) #3

; Function Attrs: nounwind
declare void @free(i8*) #1

; Function Attrs: nounwind uwtable
define void @sendMsgSlv2Mst() #0 {
entry:
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = load %struct.MB_ITF_t*, %struct.MB_ITF_t** @reg_MB, align 8
  %1 = call i32 bitcast (i32 (...)* @read_reg_MB_STS_busy to i32 ()*)()
  %tobool = icmp ne i32 %1, 0
  br i1 %tobool, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %2 = load i32, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 1), align 4
  %3 = load %struct.MB_ITF_t*, %struct.MB_ITF_t** @reg_MB, align 8
  call void @write_reg_MB_S_DAT0(i32 %2)
  %4 = load i32, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 2), align 8
  %5 = load %struct.MB_ITF_t*, %struct.MB_ITF_t** @reg_MB, align 8
  call void @write_reg_MB_S_DAT1(i32 %4)
  %6 = load i32, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 3), align 4
  %7 = load %struct.MB_ITF_t*, %struct.MB_ITF_t** @reg_MB, align 8
  call void @write_reg_MB_S_SIZE(i32 %6)
  %8 = load i32, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 0), align 16
  %9 = load %struct.MB_ITF_t*, %struct.MB_ITF_t** @reg_MB, align 8
  call void @write_reg_MB_S_CMD(i32 %8)
  ret void
}

; Function Attrs: nounwind uwtable
define void @intHdl() #0 {
entry:
  %0 = load i32, i32* @reg_slv_int, align 4
  %cmp = icmp eq i32 %0, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  br label %return

if.end:                                           ; preds = %entry
  call void @getMbCtx()
  call void @handleCmd()
  store i32 0, i32* @reg_slv_int, align 4
  br label %return

return:                                           ; preds = %if.end, %if.then
  ret void
}

; Function Attrs: nounwind uwtable
define void @getMbCtx() #0 {
entry:
  %0 = load %struct.MB_ITF_t*, %struct.MB_ITF_t** @reg_MB, align 8
  call void @write_reg_MB_ACK(i32 1)
  %1 = load %struct.MB_ITF_t*, %struct.MB_ITF_t** @reg_MB, align 8
  %2 = call i32 bitcast (i32 (...)* @read_reg_MB_R_CMD to i32 ()*)()
  store i32 %2, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 4), align 16
  %3 = load %struct.MB_ITF_t*, %struct.MB_ITF_t** @reg_MB, align 8
  %4 = call i32 bitcast (i32 (...)* @read_reg_MB_R_DAT0 to i32 ()*)()
  store i32 %4, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 5), align 4
  %5 = load %struct.MB_ITF_t*, %struct.MB_ITF_t** @reg_MB, align 8
  %6 = call i32 bitcast (i32 (...)* @read_reg_MB_R_DAT1 to i32 ()*)()
  store i32 %6, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 6), align 8
  %7 = load %struct.MB_ITF_t*, %struct.MB_ITF_t** @reg_MB, align 8
  %8 = call i32 bitcast (i32 (...)* @read_reg_MB_R_SIZE to i32 ()*)()
  store i32 %8, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 7), align 4
  ret void
}

; Function Attrs: nounwind uwtable
define void @handleCmd() #0 {
entry:
  %0 = load i32, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 4), align 16
  switch i32 %0, label %sw.default [
    i32 1, label %sw.bb
    i32 3, label %sw.bb1
  ]

sw.bb:                                            ; preds = %entry
  store i32 2, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 0), align 16
  store i32 4, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 1), align 4
  store i32 1, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 3), align 4
  call void @sendMsgSlv2Mst()
  br label %sw.epilog

sw.bb1:                                           ; preds = %entry
  %1 = load i32, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 7), align 4
  %cmp = icmp eq i32 %1, 1
  %conv = zext i1 %cmp to i32
  call void @__VERIFIER_assert(i32 %conv)
  %2 = load i32, i32* getelementptr inbounds ([8 x i32], [8 x i32]* @gMbCtx, i64 0, i64 5), align 4
  store i32 %2, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @gSlvFlag, i64 0, i64 1), align 4
  store i32 1, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @gSlvFlag, i64 0, i64 0), align 4
  br label %sw.epilog

sw.default:                                       ; preds = %entry
  br label %sw.epilog

sw.epilog:                                        ; preds = %sw.default, %sw.bb1, %sw.bb
  ret void
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }
attributes #3 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (tags/RELEASE_381/final)"}
declare i32 @read_reg_MB_R_CMD(...)
declare void @write_reg_MB_S_DAT0(i32)
declare i32 @read_reg_MB_R_DAT1(...)
declare void @write_reg_MB_S_SIZE(i32)
declare i32 @read_reg_MB_R_DAT0(...)
declare void @write_reg_MB_ACK(i32)
declare i32 @read_reg_MB_R_SIZE(...)
declare i32 @read_reg_MB_STS_busy(...)
declare void @write_reg_MB_S_CMD(i32)
declare void @write_reg_MB_S_DAT1(i32)

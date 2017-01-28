dir=.
llvmBc=${dir}/fw.bc
tarChc=${dir}/fw.smt2
tmpChc=${dir}/tmp.smt2

/Users/BoYaunHuang/workSpace/myBuildDir/clang/clang+llvm-3.6.0-x86_64-apple-darwin/bin/clang -c -O0 -emit-llvm -D__SEAHORN__ -S -m32 -I/Users/BoYaunHuang/workspace/seaHornILA/seahorn/seahorn_debug/build/run/include -o secureboot.bc secureboot.c
/Users/BoYaunHuang/workSpace/myBuildDir/clang/clang+llvm-3.6.0-x86_64-apple-darwin/bin/clang -c -O0 -emit-llvm -D__SEAHORN__ -S -m32 -I/Users/BoYaunHuang/workspace/seaHornILA/seahorn/seahorn_debug/build/run/include -o secureboot.bc -o rsa.bc rsa.c
/Users/BoYaunHuang/workspace/seaHornILA/seahorn/seahorn_debug/build/run/bin/llvm-link -o ${llvmBc} secureboot.bc rsa.bc
rm rsa.bc
rm secureboot.bc

sea fe secureboot.c rsa.c -o ${llvmBc}

seahorn -keep-shadows=true -horn-inter-proc -horn-step=small -horn-sem-lvl=mem ${llvmBc} -o ${tmpChc}
rm ${llvmBc}

#python blockMMIO.py ${tmpChc} ${tarChc} HW_REG_WRITE_chr HW_REG_WRITE_int HW_REG_WRITE_ptr
python blockMMIO.py ${tmpChc} ${tarChc} HW_REG_WRITE_chr 

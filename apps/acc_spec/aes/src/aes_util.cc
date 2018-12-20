/// \file Utility function for constructing the AES model
///  Hongce Zhang (hongcez@princeton.edu)

#include "aes_ila.h"

ExprRef AES::unknown_range(unsigned low, unsigned high) {
  unsigned width = (unsigned) std::ceil(std::log2( high + 1) );
  auto val = unknown(width)();
  model.add_init( (val >= low) & (val <=high) );
}

FuncRef AES::unknown_choice(const ExprRef & a, const ExprRef & b) {
  return Ite(unknown(1) == 1, a, b);
}

// input_slice.width == slice_width
// no_slice * slice_width == reg.width
ExprRef AES::slice_update(
    const ExprRef & reg, 
    const ExprRef & idx, 
    const ExprRef & input_slice,  
    unsigned long base_addr, 
    unsigned no_slice, 
    unsigned slice_width) 
{
  auto new_value = Ite( idx - base_addr == 0, input_slice, reg(slice_width - 1,0) );
  for( unsigned i = 1; i < no_slice; ++ i ) 
    new_value = Concat(  
      Ite( idx - base_addr == i, 
        input_slice, 
        reg(slice_width*i + slice_width - 1, slice_width*i)), 
      new_value );
  return 

}

ExprRef AES::slice_read(
    const ExprRef & reg, 
    const ExprRef & idx, 
    unsigned long base_addr, 
    unsigned no_slice, 
    unsigned slice_width)
{
  auto slice = reg(slice_width - 1, 0);
  for( unsigned i = 1; i < no_slice; ++ i ) 
    slice = 
      Ite ( 
        idx - base_addr == i,
        reg(slice_width*i + slice_width - 1, slice_width*i),
        slice);
  return slice;
}

//---------------------------------------------------------------------

#include <string>
unsigned nondet_counter = 0;

FuncRef AES::unknown(unsigned width) {
  return FuncRef("unknown" + std::to_string(nondet_counter++) , width);
}
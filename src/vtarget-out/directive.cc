/// \file Source for Interace Directive in Refinement relations in Verilog Verification Target Generation
// --- Hongce Zhang

#include <ilang/util/log.h>
#include <ilang/util/str_util.h>
#include <ilang/util/container_shortcut.h>
#include <ilang/vtarget-out/directive.h>

namespace ilang {

  // static function
  bool IntefaceDirectiveRecorder::beginsWith(const std::string & c, const std::string & s) { 
    return c.find(s) == 0;
  }

  // static function
  bool IntefaceDirectiveRecorder::isSpecialInputDir(const std::string & c)  {
   return beginsWith(c,"**"); 
  }

  // static function
  bool IntefaceDirectiveRecorder::isSpecialInputDirCompatibleWith(const std::string & c, const SignalInfoBase & vlg_sig ) {
    ILA_ASSERT(isSpecialInputDir(c));
    if(c == "**KEEP**") return true;
    if(c == "**NC**")   return true;
    if(c == "**SO**")   return vlg_sig.is_output();
    if(c == "**RESET**" || c == "**CLOCK**") return vlg_sig.is_output();
    if(beginsWith(c, "**MEM**")) {
      auto first_dot_loc = c.find(".");
      auto port_name = c.substr(first_dot_loc + 1);
      if( beginsWith(port_name,"waddr") || beginsWith(port_name,"wdata") || beginsWith(port_name,"wen") || 
          beginsWith(port_name,"raddr") || beginsWith(port_name,"ren") )
        return vlg_sig.is_output();
      if( beginsWith(port_name,"rdata") ) return vlg_sig.is_input();
      ILA_ERROR<<"Unknown IO directive in refinement relations:"<<c;
      return false;
    }
    ILA_ERROR<<"Unknown IO directive in refinement relations:"<<c;
    return false;
  }
  // static function
  bool IntefaceDirectiveRecorder::interfaceDeclareTop(const std::string & c) {
    ILA_ASSERT(isSpecialInputDir(c));
    if(c == "**KEEP**") return true;
    if(c == "**NC**")   return false;
    if(c == "**SO**")   return true;
    if(c == "**RESET**" || c == "**CLOCK**") return false;
    if(beginsWith(c, "**MEM**")) {
      auto first_dot_loc = c.find(".");
      auto port_name = c.substr(first_dot_loc + 1);
      if( beginsWith(port_name,"waddr") || beginsWith(port_name,"wdata") || beginsWith(port_name,"wen") || 
          beginsWith(port_name,"raddr") || beginsWith(port_name,"ren") )
        return false;
      if( beginsWith(port_name,"rdata") ) 
        return false; // 
      ILA_ERROR<<"Unknown IO directive in refinement relations:"<<c;
      return true;
    }
    ILA_ERROR<<"Unknown IO directive in refinement relations:"<<c;
    return true;   
  }

  void IntefaceDirectiveRecorder::ConnectModuleInputAddWire(const std::string & short_name, unsigned width)  {
    if( IN(short_name , mod_inst_rec ) ) {
      ILA_ERROR << short_name << "is already connected. Ignored.";
      return;
    }
    input_wires.push_back( {short_name, width} );
    mod_inst_rec.insert( {short_name, std::make_pair({ inf_dir_t::INPUT ,  short_name}) } );
  }

  void IntefaceDirectiveRecorder::ConnectModuleOutputAddWire(const std::string & short_name, unsigned width)  {
    if( IN(short_name , mod_inst_rec ) ) {
      ILA_ERROR << short_name << "is already connected. Ignored.";
      return;
    }
    output_wires.push_back( {short_name, width} );
    mod_inst_rec.insert( {short_name, std::make_pair({ inf_dir_t::SO ,  short_name}) } );
  }


  /// Return a string used for instantiating
  std::string IntefaceDirectiveRecorder::GetVlgModInstString(void) const {
    ModuleInstSanityCheck(); // make sure we didn't miss any wire

  }
  /// Add signals to the wrapper_generator
  void IntefaceDirectiveRecorder::VlgAddTopInteface(VerilogGeneratorBase & gen) const {
    for()
  }
  
  void IntefaceDirectiveRecorder::ModuleInstSanityCheck() const {
    for (auto && signal_conn_pair : mod_inst_rec) {
      const auto & conn_tp = signal_conn_pair.second.first;
      const auto & the_wire_connected_to_the_port = signal_conn_pair.second.second;
      if (conn_tp == inf_dir_t::NC || conn_tp == inf_dir_t::CLOCK || conn_tp == inf_dir_t::RESET )
        continue ; // no need to check them, will be declared 
      bool found = false;
      if(!found)
        for( auto && w : output_wires ) 
          if( w.first == the_wire_connected_to_the_port ) 
            { found = true; break; }
      if(!found)
        for( auto && w : input_wires ) 
          if( w.first == the_wire_connected_to_the_port ) 
            { found = true; break; }
      if(!found)
        for( auto && w : internal_wires ) 
          if( w.first == the_wire_connected_to_the_port ) 
            { found = true; break; }
      ILA_ASSERT(found) << "Connecting signal: "<< the_wire_connected_to_the_port 
        << " tp: " << conn_tp << " is not declared. Implementation bug!";
    }
  }

  /// Used to tell this module about the refinement relations , ... , ila interface checker?
  void IntefaceDirectiveRecorder::RegisterInterface(
    const SignalInfoBase & vlg_sig, 
    const std::string & refstr,
    ila_input_checker_t chk,
    ila_mem_checker_t   mget
    ) {

    auto & short_name = vlg_sig.get_signal_name();
    bool is_input     = vlg_sig.is_input();
    bool is_output    = vlg_sig.is_output();
    unsigned width    = vlg_sig.get_width();

    // temporary fix if cannot get its width
    if (width == 0 ) { 
      ILA_WARN << "assuming " << short_name << " has width 1"; 
      width = 1; 
    }
    if (not is_input and not is_output)
      ILA_ERROR << "Implementation bug: I/O direction is unknown for " << short_name ;

    if (is_input and is_output) { 
      ILA_WARN << "not handling inout wire: " << short_name << ", will not be connected"; 
      // nc and skip
      mod_inst_rec.push_back( {short_name, std::make_pair({ inf_dir_t::NC ,  ""}) } );
      return;
    }

    if( refstr == "" ) {
      ILA_WARN << "Verilog I/O:"<<short_name << " is not mentioned in the input mapping";
      // add an KEEP directive
      mod_inst_rec.insert( {short_name, std::make_pair({ inf_dir_t::KEEP ,  short_name}) } );
      if( is_input ) 
        input_wires.push_back( {short_name, width} );
      else if(is_output)
        output_wires.push_back( {short_name, width} );

      return;
    }

    if ( isSpecialInputDir(refstr) ) {
      ILA_ERROR_IF( ! isSpecialInputDirCompatibleWith(refstr, vlg_sig )) 
        << short_name 
        << " in refinement (special directive) does not match the verilog interface.";

      if (refstr == "**KEEP**") {
        if(is_input)  ConnectModuleInputAddWire ("__VLG_I_" + short_name, width);
        if(is_output) ConnectModuleOutputAddWire("__VLG_O_" + short_name, width);
      } else if (refstr == "**NC**") {
        mod_inst_rec.push_back( {short_name, std::make_pair({ inf_dir_t::NC ,  ""}) } );
      } else if ( refstr == "**SO**" ) {
        ILA_ERROR_IF ( ! is_output ) << "Forcing a non-output signal to be connected as output:" << short_name;
        ConnectModuleOutputAddWire(short_name, width);
      } else if ( refstr ==  "**RESET**" ) {
        mod_inst_rec.insert( {short_name, std::make_pair({ inf_dir_t::RESET ,  "rst"}) } );
      } else if ( refstr == "**CLOCK**" ) {
        mod_inst_rec.insert( {short_name, std::make_pair({ inf_dir_t::CLOCK ,  "clk"}) } );
      } else if ( beginsWith(refstr, "**MEM**") ) {
        // now we need to keep recording it , how many mems mentiond, how many connected
        //1. get the remaining part
        if( refstr.length() <= 7 ) { ILA_ERROR << "Bad mem directive:" << refstr; return; }
        auto dirm = refstr.substr(7); // [7:]
        auto dirv = Split(dirm,".");
        if( dirv.size() < 2 or dirv.size() > 3 )  { ILA_ERROR << "Bad mem directive:" << refstr; return; }
        if( dirv.size() == 2 ) dirv.push_back("0"); // refer to 0 by default
        const auto & ila_mem_name = dirv[0];
        const auto & port_name = dirv[1];
        unsigned port_no = StrToInt(dirv[2]);
        // add internal wire __MEM_ilaname_no_..._
        //
        
        auto ila_minfo = mget(ila_mem_name);
        if(ila_minfo.first == 0 or ila_minfo.second == 0) {
          ILA_ERROR << "No such memory in ila:"<< ila_mem_name << " refered by refinement relation.";
          return;
        }
        auto addr_w = ila_minfo.first; auto data_w = ila_minfo.second;

        auto new_wire_name = 
          "__MEM_"+  VerilogGeneratorBase::sanitizeName(ila_mem_name) 
          + "_" + dirv[2] + "_" + port_name;

#define ADD_PORT_WIRE( name, w, INFO1, INFO2, io , assign, inf_d  )         \
        do {                                                                \
          ILA_ERROR_IF ( width != (w) )                                     \
            << INFO1 " width does not match: vlg:"                          \
            << short_name << "(" << width << ") "                           \
            << " ila: "                                                     \
            << ila_mem_name << "." INFO2 " (" << (w) << ")";                \
          ILA_ERROR_IF ( ! io ) << name " should be an input to the verilog module."; \
          (assign) = new_wire_name;                                         \
          mod_inst_rec.insert( {short_name, { (inf_d), new_wire_name } } ); \
          internal_wires.push_back( {new_wire_name, (w)} );                 \
        } while(0);

        if(port_name == "rdata") {
          ADD_PORT_WIRE( "rdata" , data_w, "Data" , "data", is_input, abs_mems[ila_mem_name].rports[port_no].rdata,  inf_dir_t::MEM_R_D );
        } else if (port_name == "raddr") {
          ADD_PORT_WIRE( "raddr" , addr_w, "Addr" , "addr", is_output, abs_mems[ila_mem_name].rports[port_no].raddr, inf_dir_t::MEM_R_A );
        } else if (port_name == "ren") {
          ADD_PORT_WIRE( "ren" ,1, "Enable signal" , "en",  is_output, abs_mems[ila_mem_name].rports[port_no].ren,   inf_dir_t::MEM_R_EN );
        } else if (port_name == "wdata") {
          ADD_PORT_WIRE( "wdata" , data_w, "Data" , "data", is_output, abs_mems[ila_mem_name].wports[port_no].wdata, inf_dir_t::MEM_W_D);
        } else if (port_name == "waddr") {
          ADD_PORT_WIRE( "waddr" , addr_w, "Addr" , "addr", is_output, abs_mems[ila_mem_name].wports[port_no].waddr, inf_dir_t::MEM_W_A );
        } else if (port_name == "wen") {
          ADD_PORT_WIRE( "wen" ,1, "Enable signal" , "en",  is_output, abs_mems[ila_mem_name].wports[port_no].wen,   inf_dir_t::MEM_W_EN );
        } else{
          ILA_ERROR << "no such port:" << port_name 
            << " on built-in mem abstraction for " 
            << ila_mem_name;
        }
        return;
      } else
        ILA_ERROR << "Ignoring unknown verilog interface directive:" << refstr;

      // decide how to connect and signals to create
      return;
    } // end of isSpecialInputDir

    // exists and not special input directive, connected the name they are given.
    // this name should be an ila-input, maybe we can check 

    if(is_input) {
      // TODO: FIXME: NEED TO CHECK w. ILA
      // ILA_ERROR_IF( refstr is not an ila input )
      ILA_ERROR_IF( not chk(refstr, vlg_sig) ) 
        << "Uncompatible input refinement:" 
        << refstr << "for " << short_name
        << ". Connect anyway. Please check."
      ConnectModuleInputAddWire( "__ILA_I_" + refstr, width);
    }
    if(is_output) {
      ILA_ERROR << "Cannot map output signals to ILA input for :" << refstr << ", left unconnected.";
      mod_inst_rec.push_back( {short_name, std::make_pair({ inf_dir_t::NC ,  ""}) } ); 
    } // ignoring it
    // okay we are done now
  } // IntefaceDirectiveRecorder::RegisterInterface


  void IntefaceDirectiveRecorder::Clear() {
    mod_inst_rec.clear();
    input_wires.clear();
    internal_wires.clear();
    output_wires.clear();
    abs_mems.clear();
  }

  // ------------------------------------------------------------------------

  // static function
  bool StateMappingDirectiveRecorder::isSpecialStateDir(const std::string & c)
  {
    return IntefaceDirectiveRecorder::beginsWith(c,"**"); 
  }

} // namespace ilang

/*        ILA_ERROR_IF ( width != data_w ) 
            << "Data width does not match: vlg:" 
            << short_name << "(" << width << ") "
            << " ila: " 
            << ila_mem_name << ".data (" << data_w << ")";
          ILA_ERROR_IF ( ! is_input ) << "rdata should be an input to the verilog module.";

          abs_mems[ila_mem_name].rports[port_no].rdata = new_wire_name;
          mod_inst_rec.insert( {short_name, {inf_dir_t::MEM_R_D, new_wire_name } } );
          internal_wires.push_back( {new_wire_name, data_w} ); */
/*

          ILA_ERROR_IF ( width != addr_w ) 
            << "Addr width does not match: vlg:" 
            << short_name << "(" << width << ") "
            << " ila: " 
            << ila_mem_name << ".addr (" << addr_w << ")";
          ILA_ERROR_IF ( ! is_output ) << "raddr should be an output of the verilog module.";

          abs_mems[ila_mem_name].rports[port_no].raddr = new_wire_name;
          mod_inst_rec.insert( {short_name, {inf_dir_t::MEM_R_A, new_wire_name } } );
          internal_wires.push_back( {new_wire_name, addr_w} );


          ILA_ERROR_IF ( width != 1 ) 
            << "Enable signal width does not match: vlg:" 
            << short_name << "(" << width << ") "
            << " ila: " 
            << ila_mem_name << ".ren (" << 1 << ")";
          ILA_ERROR_IF ( ! is_output ) << "ren should be an output of the verilog module.";

          abs_mems[ila_mem_name].rports[port_no].ren = new_wire_name;
          mod_inst_rec.insert( {short_name, {inf_dir_t::MEM_R_EN, new_wire_name } } );
          internal_wires.push_back( {new_wire_name, 1} );

*/
/// \file Source for Interace Directive in Refinement relations in Verilog
/// Verification Target Generation
// --- Hongce Zhang

#include <ilang/util/container_shortcut.h>
#include <ilang/util/log.h>
#include <ilang/util/str_util.h>
#include <ilang/vtarget-out/directive.h>

namespace ilang {

#define VLG_TRUE "`true"


// ---------------------- class IntefaceDirectiveRecorder ------------------------------- //
// static function
bool IntefaceDirectiveRecorder::beginsWith(const std::string& c,
                                           const std::string& s) {
  return c.find(s) == 0;
}

// static function
bool IntefaceDirectiveRecorder::isSpecialInputDir(const std::string& c) {
  return beginsWith(c, "**");
}

// static function
bool IntefaceDirectiveRecorder::isSpecialInputDirCompatibleWith(
    const std::string& c, const SignalInfoBase& vlg_sig) {
  ILA_ASSERT(isSpecialInputDir(c));
  if (c == "**KEEP**")
    return true;
  if (c == "**NC**")
    return true;
  if (c == "**SO**")
    return vlg_sig.is_output();
  if (c == "**RESET**" || c == "**CLOCK**")
    return (vlg_sig.is_input() and vlg_sig.get_width() == 1);
  if (beginsWith(c, "**MEM**")) {
    auto first_dot_loc = c.find(".");
    auto port_name = c.substr(first_dot_loc + 1);
    if (beginsWith(port_name, "waddr") || beginsWith(port_name, "wdata") ||
        beginsWith(port_name, "wen") || beginsWith(port_name, "raddr") ||
        beginsWith(port_name, "ren"))
      return vlg_sig.is_output();
    if (beginsWith(port_name, "rdata"))
      return vlg_sig.is_input();
    ILA_ERROR << "Unknown IO directive in refinement relations:" << c;
    return false;
  }
  ILA_ERROR << "Unknown IO directive in refinement relations:" << c;
  return false;
}

// static function
/*
bool IntefaceDirectiveRecorder::interfaceDeclareTop(const std::string & c) {
  ILA_ASSERT(isSpecialInputDir(c));
  if(c == "**KEEP**") return true;
  if(c == "**NC**")   return false;
  if(c == "**SO**")   return true;
  if(c == "**RESET**" || c == "**CLOCK**") return false;
  if(beginsWith(c, "**MEM**")) {
    auto first_dot_loc = c.find(".");
    auto port_name = c.substr(first_dot_loc + 1);
    if( beginsWith(port_name,"waddr") || beginsWith(port_name,"wdata") ||
beginsWith(port_name,"wen") || beginsWith(port_name,"raddr") ||
beginsWith(port_name,"ren") ) return false; if( beginsWith(port_name,"rdata") )
      return false; //
    ILA_ERROR<<"Unknown IO directive in refinement relations:"<<c;
    return true;
  }
  ILA_ERROR<<"Unknown IO directive in refinement relations:"<<c;
  return true;
}*/

void IntefaceDirectiveRecorder::ConnectModuleInputAddWire(
    const std::string& short_name, unsigned width) {
  if (IN(short_name, mod_inst_rec)) {
    ILA_ERROR << short_name << "is already connected. Ignored.";
    return;
  }
  input_wires.push_back({short_name, width});
  mod_inst_rec.insert(
      {short_name, inf_connector_t({inf_dir_t::INPUT, short_name})});
}

void IntefaceDirectiveRecorder::ConnectModuleOutputAddWire(
    const std::string& short_name, unsigned width) {
  if (IN(short_name, mod_inst_rec)) {
    ILA_ERROR << short_name << "is already connected. Ignored.";
    return;
  }
  output_wires.push_back({short_name, width});
  mod_inst_rec.insert(
      {short_name, inf_connector_t({inf_dir_t::SO, short_name})});
}

/// Return a string used for instantiating
std::string IntefaceDirectiveRecorder::GetVlgModInstString(
    VerilogGeneratorBase& gen) const {
  ModuleInstSanityCheck(gen); // make sure we didn't miss any wire
  std::string retStr;
  std::string sep;
  for (auto&& signal_conn_pair : mod_inst_rec) {
    const auto& signal_name = signal_conn_pair.first;
    const auto& conn_tp = signal_conn_pair.second.first;
    const auto& the_wire_connected_to_the_port = signal_conn_pair.second.second;

    retStr += sep + "    ." + signal_name + "(" +
              the_wire_connected_to_the_port + ")";
    sep = ",\n";
  }
  retStr += "\n";
  return retStr;
} // you can append after it

/// Add signals to the wrapper_generator
void IntefaceDirectiveRecorder::VlgAddTopInteface(
    VerilogGeneratorBase& gen) const {
  for (auto&& w : output_wires) {
    gen.add_output(w.first, w.second);
    gen.add_wire(w.first, w.second, true);
  }
  for (auto&& w : input_wires) {
    gen.add_input(w.first, w.second);
    gen.add_wire(w.first, w.second, true);
  }
  for (auto&& w : internal_wires) {
    gen.add_wire(w.first, w.second, true);
  }
}

void IntefaceDirectiveRecorder::ModuleInstSanityCheck(
    VerilogGeneratorBase& gen) const {
  for (auto&& signal_conn_pair : mod_inst_rec) {
    const auto& conn_tp = signal_conn_pair.second.first;
    const auto& the_wire_connected_to_the_port = signal_conn_pair.second.second;
    if (conn_tp == inf_dir_t::NC)
      continue; // no need to check them, will be declared

    if (IN(the_wire_connected_to_the_port, gen.wires))
      continue; // if found okay

    ILA_ASSERT(false) << "Connecting signal: " << the_wire_connected_to_the_port
                      << " tp: " << conn_tp
                      << " is not declared. Implementation bug!";
  }
}
// register extra state out name
void IntefaceDirectiveRecorder::RegisterExtraWire(const std::string & io_name, const std::string & outside_name) {
  if( not IN(io_name, mod_inst_rec ))
    mod_inst_rec.insert( { { io_name , inf_connector_t({ inf_dir_t::SO, outside_name})} } );
  else {
    ILA_ERROR<<io_name << " has been connected already.";
  }
}

/// Used to tell this module about the refinement relations ,  , ila interface
/// checker?
void IntefaceDirectiveRecorder::RegisterInterface(const SignalInfoBase& vlg_sig,
                                                  const std::string& refstr,
                                                  ila_input_checker_t chk,
                                                  ila_mem_checker_t mget) {

  auto short_name = vlg_sig.get_signal_name();
  bool is_input = vlg_sig.is_input();
  bool is_output = vlg_sig.is_output();
  unsigned width = vlg_sig.get_width();

  // temporary fix if cannot get its width
  if (width == 0) {
    ILA_WARN << "assuming " << short_name << " has width 1";
    width = 1;
  }
  if (not is_input and not is_output)
    ILA_ERROR << "Implementation bug: I/O direction is unknown for "
              << short_name;

  if (is_input and is_output) {
    ILA_WARN << "not handling inout wire: " << short_name
             << ", will not be connected";
    // nc and skip
    mod_inst_rec.insert({short_name, inf_connector_t({inf_dir_t::NC, ""})});
    return;
  }

  if (refstr == "") {
    ILA_WARN << "Verilog I/O:" << short_name
             << " is not mentioned in the input mapping";
    // add an KEEP directive
    mod_inst_rec.insert(
        {short_name, inf_connector_t({inf_dir_t::KEEP, short_name})});
    if (is_input)
      input_wires.push_back({short_name, width});
    else if (is_output)
      output_wires.push_back({short_name, width});

    return;
  }

  if (isSpecialInputDir(refstr)) {
    ILA_ERROR_IF(!isSpecialInputDirCompatibleWith(refstr, vlg_sig))
        << short_name
        << " in refinement (special directive) does not match the verilog "
           "interface.";

    if (refstr == "**KEEP**") {
      if (is_input) {
        if (IN(short_name, mod_inst_rec)) {
          ILA_ERROR << short_name << " has already been connected";
        } else {
          input_wires.push_back({"__VLG_I_" + short_name, width});
          mod_inst_rec.insert(
              {short_name,
               inf_connector_t({inf_dir_t::INPUT, "__VLG_I_" + short_name})});
        }
      }
      if (is_output) {
        if (IN(short_name, mod_inst_rec))
          ILA_ERROR << short_name << "is already connected. Ignored.";
        else {
          output_wires.push_back({"__VLG_O_" + short_name, width});
          mod_inst_rec.insert(
              {short_name,
               inf_connector_t({inf_dir_t::SO, "__VLG_O_" + short_name})});
        }
      }
    } else if (refstr == "**NC**") {
      mod_inst_rec.insert({short_name, inf_connector_t({inf_dir_t::NC, ""})});
    } else if (refstr == "**SO**") {
      ILA_ERROR_IF(!is_output)
          << "Forcing a non-output signal to be connected as output:"
          << short_name;
      ConnectModuleOutputAddWire(short_name, width);
    } else if (refstr == "**RESET**") {
      if (_reset_vlg)
        mod_inst_rec.insert(
            {short_name, inf_connector_t({inf_dir_t::RESET, "rst"})});
      else
        mod_inst_rec.insert(
            {short_name, inf_connector_t({inf_dir_t::RESET, "dummy_reset"})});
    } else if (refstr == "**CLOCK**") {
      mod_inst_rec.insert(
          {short_name, inf_connector_t({inf_dir_t::CLOCK, "clk"})});
    } else if (beginsWith(refstr, "**MEM**")) {
      // now we need to keep recording it , how many mems mentiond, how many
      // connected
      // 1. get the remaining part
      if (refstr.length() <= 7) {
        ILA_ERROR << "Bad mem directive:" << refstr;
        return;
      }
      auto dirm = refstr.substr(7); // [7:]
      auto dirv = Split(dirm, ".");
      if (dirv.size() < 2 or dirv.size() > 3) {
        ILA_ERROR << "Bad mem directive:" << refstr;
        return;
      }
      if (dirv.size() == 2)
        dirv.push_back("0"); // refer to 0 by default
      const auto& ila_mem_name = dirv[0];
      const auto& port_name = dirv[1];
      unsigned port_no = StrToInt(dirv[2]);
      // add internal wire __MEM_ilaname_no_..._
      //

      auto ila_minfo = mget(ila_mem_name);
      if (ila_minfo.first == 0 or ila_minfo.second == 0) {
        ILA_ERROR << "No such memory in ila:" << ila_mem_name
                  << " refered by refinement relation.";
        return;
      }
      auto addr_w = ila_minfo.first;
      auto data_w = ila_minfo.second;

      auto new_wire_name = "__MEM_" +
                           VerilogGeneratorBase::sanitizeName(ila_mem_name) +
                           "_" + dirv[2] + "_" + port_name;

#define ADD_PORT_WIRE(name, w, INFO1, INFO2, io, assign, inf_d)                \
  do {                                                                         \
    ILA_ERROR_IF(width != (w))                                                 \
        << INFO1 " width does not match: vlg:" << short_name << "(" << width   \
        << ") "                                                                \
        << " ila: " << ila_mem_name << "." INFO2 " (" << (w) << ")";           \
    ILA_ERROR_IF(!io) << name " should be an input to the verilog module.";    \
    (assign) = new_wire_name;                                                  \
    mod_inst_rec.insert({short_name, {(inf_d), new_wire_name}});               \
    internal_wires.push_back({new_wire_name, (w)});                            \
  } while (0);

      if (port_name == "rdata") {
        ADD_PORT_WIRE("rdata", data_w, "Data", "data", is_input,
                      abs_mems[ila_mem_name].vlg_rports[port_no].rdata,
                      inf_dir_t::MEM_R_D);
        abs_mems[ila_mem_name].SetDataWidth(data_w);
      } else if (port_name == "raddr") {
        ADD_PORT_WIRE("raddr", addr_w, "Addr", "addr", is_output,
                      abs_mems[ila_mem_name].vlg_rports[port_no].raddr,
                      inf_dir_t::MEM_R_A);
        abs_mems[ila_mem_name].SetAddrWidth(addr_w);
      } else if (port_name == "ren") {
        ADD_PORT_WIRE("ren", 1, "Enable signal", "en", is_output,
                      abs_mems[ila_mem_name].vlg_rports[port_no].ren,
                      inf_dir_t::MEM_R_EN);
      } else if (port_name == "wdata") {
        ADD_PORT_WIRE("wdata", data_w, "Data", "data", is_output,
                      abs_mems[ila_mem_name].vlg_wports[port_no].wdata,
                      inf_dir_t::MEM_W_D);
        abs_mems[ila_mem_name].SetDataWidth(data_w);
      } else if (port_name == "waddr") {
        ADD_PORT_WIRE("waddr", addr_w, "Addr", "addr", is_output,
                      abs_mems[ila_mem_name].vlg_wports[port_no].waddr,
                      inf_dir_t::MEM_W_A);
        abs_mems[ila_mem_name].SetAddrWidth(addr_w);
      } else if (port_name == "wen") {
        ADD_PORT_WIRE("wen", 1, "Enable signal", "en", is_output,
                      abs_mems[ila_mem_name].vlg_wports[port_no].wen,
                      inf_dir_t::MEM_W_EN);
      } else {
        ILA_ERROR << "no such port:" << port_name
                  << " on built-in mem abstraction for " << ila_mem_name;
      }
      return;
    } else
      ILA_ERROR << "Ignoring unknown verilog interface directive:" << refstr;

    // decide how to connect and signals to create
    return;
  } // end of isSpecialInputDir

  // exists and not special input directive, connected the name they are given.
  // this name should be an ila-input, maybe we can check

  if (is_input) {
    // TODO: FIXME: NEED TO CHECK w. ILA
    // ILA_ERROR_IF( refstr is not an ila input )
    ILA_ERROR_IF(not chk(refstr, vlg_sig))
        << "Uncompatible input refinement:" << refstr << " for " << short_name
        << ". Connect anyway. Please check.";

    mod_inst_rec.insert({short_name, {inf_dir_t::INPUT, "__ILA_I_" + refstr}});
  }
  if (is_output) {
    ILA_ERROR << "Cannot map output signals to ILA input for :" << refstr
              << ", left unconnected.";
    mod_inst_rec.insert({short_name, inf_connector_t({inf_dir_t::NC, ""})});
  } // ignoring it
  // okay we are done now
} // IntefaceDirectiveRecorder::RegisterInterface

void IntefaceDirectiveRecorder::Clear(bool reset_vlg) {
  mod_inst_rec.clear();
  input_wires.clear();
  internal_wires.clear();
  output_wires.clear();
  abs_mems.clear();
  _reset_vlg = reset_vlg;
}

std::string IntefaceDirectiveRecorder::ConnectMemory(
  const std::string & directive, 
  const std::string & ila_state_name,
  const std::map<unsigned, rport_t> & rports, 
  const std::map<unsigned, wport_t> & wports  )
{
  ILA_ASSERT(beginsWith(directive, "**"));
  if(not beginsWith(directive,"**MEM**")) {
    ILA_ERROR << directive << " is not a recognized directive!";
    return VLG_TRUE;
  }

  auto mem_name = directive.substr(7);
  auto pos = abs_mems.find(mem_name);
  if( pos == abs_mems.end() ) {
    ILA_ERROR << directive << " refers to a nonexisting memory!";
    return VLG_TRUE;    
  }

  pos->second.mem_name = mem_name;
  pos->second.ila_map_name = ila_state_name;
  // copy the ports
  ILA_ERROR_IF(pos->second.ila_rports.size() != 0 || pos->second.ila_wports.size() != 0 ) << mem_name << " seems to have been connected.";
  //pos->second.ila_rports = rports;
  pos->second.ila_rports.insert(rports.begin(), rports.end());
  //pos->second.ila_wports = wports;
  pos->second.ila_wports.insert(wports.begin(), wports.end());

  return pos->second.MemEQSignalName();
} // ConnectMemory


// ------------------------------------------------------------------------

// static function
bool StateMappingDirectiveRecorder::isSpecialStateDir(const std::string& c) {
  return IntefaceDirectiveRecorder::beginsWith(c, "**");
}

} // namespace ilang

/*        ILA_ERROR_IF ( width != data_w )
            << "Data width does not match: vlg:"
            << short_name << "(" << width << ") "
            << " ila: "
            << ila_mem_name << ".data (" << data_w << ")";
          ILA_ERROR_IF ( ! is_input ) << "rdata should be an input to the
   verilog module.";

          abs_mems[ila_mem_name].vlg_rports[port_no].rdata = new_wire_name;
          mod_inst_rec.insert( {short_name, {inf_dir_t::MEM_R_D, new_wire_name }
   } ); internal_wires.push_back( {new_wire_name, data_w} ); */
/*

          ILA_ERROR_IF ( width != addr_w )
            << "Addr width does not match: vlg:"
            << short_name << "(" << width << ") "
            << " ila: "
            << ila_mem_name << ".addr (" << addr_w << ")";
          ILA_ERROR_IF ( ! is_output ) << "raddr should be an output of the
   verilog module.";

          abs_mems[ila_mem_name].vlg_rports[port_no].raddr = new_wire_name;
          mod_inst_rec.insert( {short_name, {inf_dir_t::MEM_R_A, new_wire_name }
   } ); internal_wires.push_back( {new_wire_name, addr_w} );


          ILA_ERROR_IF ( width != 1 )
            << "Enable signal width does not match: vlg:"
            << short_name << "(" << width << ") "
            << " ila: "
            << ila_mem_name << ".ren (" << 1 << ")";
          ILA_ERROR_IF ( ! is_output ) << "ren should be an output of the
   verilog module.";

          abs_mems[ila_mem_name].vlg_rports[port_no].ren = new_wire_name;
          mod_inst_rec.insert( {short_name, {inf_dir_t::MEM_R_EN, new_wire_name
   } } ); internal_wires.push_back( {new_wire_name, 1} );

*/
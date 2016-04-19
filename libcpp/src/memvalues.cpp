#include <memvalues.hpp>

#include <ast.hpp>
#include <util.hpp>
#include <exception.hpp>
#include <boost/python.hpp>
#include <smt.hpp>

namespace ila
{
    // ---------------------------------------------------------------------- //
    z3::context MemValues::c_eq;
    z3::solver MemValues::S_eq(c_eq);

    // ---------------------------------------------------------------------- //
    MemValues::MemValues()
      : MAX_ADDR(0)
      , def_value(0)
    {
    }

    MemValues::MemValues(int aw, int dw, const py::object& dv)
      : type(NodeType::getMem(aw, dw))
      , MAX_ADDR(mp_int_t(1) << aw)
      , def_value(to_cpp_int(dv))
    {
    }

    MemValues::MemValues(int aw, int dw, 
                         const map_t values, const mp_int_t def_value)
      : type(NodeType::getMem(aw, dw))
      , MAX_ADDR(mp_int_t(1) << aw)
      , def_value(def_value)
      , values(values)
    {
    }

    MemValues::MemValues(
        Z3ExprAdapter& c, const z3::model& m, const MemVar* mem)
      : type(mem->type)
      , MAX_ADDR(mp_int_t(1) << type.addrWidth)
    {
        using namespace z3;

        // extract model.
        expr mexp = c.getExpr(mem);
        expr mval = m.eval(mexp);
        func_decl mfd = mval.decl();

        // a bunch of assertions copied from stackoverflow.
        ILA_ASSERT(Z3_get_decl_kind(c.ctx(), mfd) == Z3_OP_AS_ARRAY, "Expected array op.");
        ILA_ASSERT(Z3_is_app(c.ctx(), mval), "Expected application.");
        ILA_ASSERT(Z3_get_decl_num_parameters(c.ctx(), mfd) == 1, "Expected one parameter.");
        ILA_ASSERT( Z3_get_decl_parameter_kind(c.ctx(), mfd, 0) == Z3_PARAMETER_FUNC_DECL, "Expected parameter to a function declaration.");

        // get the function interpretation (also SO).
        func_decl ffd = func_decl(c.ctx(), Z3_get_decl_func_decl_parameter(c.ctx(), mfd, 0));
        func_interp fip = m.get_func_interp(ffd);

        // now walk through the values.
        for (unsigned i=0, n=fip.num_entries(); i < n; i++) {
            func_entry ei = fip.entry(i);
            expr eki = ei.arg(0);
            expr evi = ei.value();
            std::string ski(Z3_get_numeral_string(c.ctx(), eki));
            std::string svi(Z3_get_numeral_string(c.ctx(), evi));

            auto ki = boost::lexical_cast<mp_int_t>(ski);
            auto vi = boost::lexical_cast<mp_int_t>(svi);
            values[ki] = vi;
        }
        std::string sdef(Z3_get_numeral_string(c.ctx(), fip.else_value()));
        def_value = boost::lexical_cast<mp_int_t>(sdef);
    }

    MemValues::MemValues(const MemValues& that)
      : type(that.type)
      , MAX_ADDR(that.MAX_ADDR)
      , def_value(that.def_value)
      , values(that.values)
    {
    }

    MemValues::~MemValues()
    {
    }

    // ---------------------------------------------------------------------- //
    MemValues& MemValues::operator=(const MemValues& that)
    {
        type = that.type;
        MAX_ADDR = that.MAX_ADDR;
        def_value = that.def_value;
        values = that.values;
        return *this;
    }

    // ---------------------------------------------------------------------- //
    py::object MemValues::getDefault() const
    {
        return to_pyint(def_value);
    }

    void MemValues::setDefault(const py::object& o)
    {
        try {
            mp_int_t dv = to_cpp_int(o);
            def_value = dv;
        } catch(const boost::bad_lexical_cast&) {
            throw PyILAException(PyExc_ValueError, "Invalid default value.");
        }
    }

    py::object MemValues::getValues() const
    {
        py::list l;
        for (auto p : values) {
            auto ai = to_pyint(p.first);
            auto di = to_pyint(p.second);
            auto ti = make_tuple(ai, di);
            l.append(ti);
        }
        return l;
    }

    mp_int_t MemValues::getItemInt(const mp_int_t& index) const
    {
        auto pos = values.find(index);
        if (pos == values.end()) return def_value;
        else return pos->second;
    }

    py::object MemValues::getItem(const py::object& index_) const
    {
        try {
            mp_int_t index = to_cpp_int(index_);
            if (index < 0 || index >= MAX_ADDR) {
                throw PyILAException(PyExc_IndexError, "Index out of range.");
                return py::object();
            }
            return to_pyint(getItemInt(index));
        } catch(const boost::bad_lexical_cast&) {
            throw PyILAException(PyExc_ValueError, "Invalid index value.");
        }
    }

    void MemValues::setItem(const py::object& index_, const py::object& value_)
    {
        mp_int_t index;
        mp_int_t value;

        try {
            index = to_cpp_int(index_);
        } catch (const boost::bad_lexical_cast&) {
            throw PyILAException(PyExc_ValueError, "Invalid index value.");
        }

        try {
            value = to_cpp_int(value_);
        } catch (const boost::bad_lexical_cast&) {
            throw PyILAException(PyExc_ValueError, "Invalid value.");
        }

        if (value == def_value) {
            auto pos = values.find(index);
            if (pos != values.end()) {
                values.erase(pos);
            }
        } else {
            values[index] = value;
        }
    }
    
    bool MemValues::operator==(const MemValues& that) const
    {
        // different types then definitely not equal.
        if (type != that.type) return false;
        // these are syntactic checks.
        if (def_value != that.def_value) return false;
        for ( auto it : values ) {
            if (getItemInt(it.first) != that.getItemInt(it.first)) {
                return false;
            }
        }
        for ( auto it : that.values ) {
            if (getItemInt(it.first) != getItemInt(it.first)) {
                return false;
            }
        }
        return true;
    }

    bool MemValues::semanticEqual(const MemValues& that) const
    {
        using namespace z3;
        using namespace boost;

        if (type != that.type) return false;
        if (*this == that) return true;
        
        std::string name("addr_" + lexical_cast<std::string>(type.addrWidth));
        expr addr = c_eq.bv_const(name.c_str(), type.addrWidth);
        expr mem1 = toZ3(c_eq);
        expr mem2 = that.toZ3(c_eq);
        expr rd1 = select(mem1, addr);
        expr rd2 = select(mem2, addr);
        S_eq.push();
        S_eq.add(rd1 != rd2);
        auto res = S_eq.check();
        S_eq.pop();

        return (res == unsat);
    }

    z3::expr MemValues::toZ3(z3::context& c) const
    {
        auto addrsort = c.bv_sort(type.addrWidth);
        auto datasort = c.bv_sort(type.dataWidth);

        auto defstr = boost::lexical_cast<std::string>(def_value);
        auto defexpr = c.bv_val(defstr.c_str(), type.dataWidth);

        auto e1 = const_array(addrsort, defexpr);
        for (auto p : values) {
            auto addrstr = boost::lexical_cast<std::string>(p.first);
            auto addrexpr = c.bv_val(addrstr.c_str(), type.addrWidth);
            auto datastr = boost::lexical_cast<std::string>(p.second);
            auto dataexpr = c.bv_val(datastr.c_str(), type.dataWidth);
            auto e2 = store(e1, addrexpr, dataexpr);
            e1 = e2;
        }
        return e1;
    }

    std::ostream& operator<<(std::ostream& out, const MemValues& mv)
    {
        bool first = true;
        out << "[";

        for (auto p : mv.values) {
            if (!first) { out << " "; } else { first = false; }
            out << std::hex << "0x" << p.first << ": " << "0x" << p.second;
        }

        if (!first) { out << " "; } 
        out << "default: 0x" << std::hex << mv.def_value << std::dec << "]";

        return out;
    }

}

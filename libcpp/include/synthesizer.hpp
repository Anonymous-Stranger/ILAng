#ifndef __SYNTHESIZER_HPP_DEFINED__
#define __SYNTHESIZER_HPP_DEFINED__

#include <map>
#include <set>
#include <memory>
#include <string>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <z3++.h>

#include <common.hpp>
#include <ast.hpp>
#include <smt.hpp>
#include <synrewriter.hpp>
#include <abstraction.hpp>

namespace ila
{

    // ---------------------------------------------------------------------- //
    struct SupportVars
    {
        bool canFixUp;

        // set of booleans.
        std::set<const BoolVar*> bools;
        // set of bitvectors.
        std::set<const BitvectorVar*> bitvecs;

        // set of memories.
        struct mem_info_t {
            const MemVar* mem;
            const BitvectorExpr* addr;
            const BitvectorExpr* rddata;

            bool operator==(const mem_info_t& that) const {
                return mem->equal(that.mem) &&
                       addr->equal(that.addr) &&
                       rddata->equal(that.rddata);
            }
        };
        std::vector<mem_info_t> rdexprs;

        // visitor function.
        void operator() (const Node* n);
        // reset.
        void clear();
        // make the rdexprs unique.
        void uniquifyRdExprs();

        // check if this expression depends on the support.
        bool depCheck(z3::context& c, z3::solver& S, const nptr_t& ex);

        SupportVars() : canFixUp(true) {}
    };

    // ---------------------------------------------------------------------- //
    struct DistInput
    {
        std::map<std::string, MemValues>    mems;
        std::map<std::string, std::string>  bitvecs;
        std::map<std::string, bool>         bools;
        std::vector<std::string>            rdaddrs;

        DistInput(
            Abstraction& a, Z3ExprAdapter& c, z3::model& m, SupportVars& sv);
        void fixUp(SupportVars& s, Z3ExprAdapter& c, z3::model& m);
        void toPython(py::dict& d);

        bool getBoolValue(const std::string& n) const;
        std::string getBitvecStr(const std::string& n) const;
        const MemValues& getMemValues(const std::string& n) const;
    };
    std::ostream& operator<<(std::ostream& out, const DistInput& di);

    // ---------------------------------------------------------------------- //
    struct SimOutput
    {
        boost::variant<MemValues, std::string, bool> out;
        // default constructor.
        SimOutput();
        // constructor with a particular output.
        SimOutput(const NodeType& nt, const py::object& r);
        // copy constructor.
        SimOutput(const SimOutput& that);
        // assignment operator.
        SimOutput& operator=(const SimOutput& that);
        // equality check.
        bool operator==(const SimOutput& that) const;
        // initialize with a particular output.
        void initOutput(const NodeType& nt, const py::object& r);
    };
    std::ostream& operator<<(std::ostream& out, const SimOutput& simout);

    typedef boost::shared_ptr<SimOutput> simout_ptr_t;

    // ---------------------------------------------------------------------- //
    struct DITreeNode;
    typedef boost::shared_ptr<DITreeNode> dtree_ptr_t;

    struct DITreeNode
    {
        typedef std::pair<simout_ptr_t, dtree_ptr_t> outpair_t;
        typedef std::vector<outpair_t> outpair_vec_t;

        std::auto_ptr<DistInput> inputs;
        outpair_vec_t outputs;
        nptr_t result;

        DITreeNode(
            Abstraction& a, Z3ExprAdapter& c, z3::model& m, SupportVars& sv);
        DITreeNode();
        ~DITreeNode();
    };

    class Synthesizer;
    struct DITree
    {
        enum mode_t { REPLAY, INSERT } mode;
        bool reuseModels;

        // pointer to synthesizer.
        Synthesizer& syn;
        // head of the tree.
        dtree_ptr_t  head;
        // in replay mode - this is the pointer to the next dist input node.
        dtree_ptr_t  replay_ptr;
        // in insert mode, this is the location where the next node will be
        // inserted.
        dtree_ptr_t* insert_ptr;

        DistInput* getDistInput(const z3::expr& y);
        void setOutput(const simout_ptr_t& out);
        nptr_t getExpr(const z3::expr& y, const nptr_t& ex, int i);

        DITree(Synthesizer& s);
        void reset(bool reuseModels);
        void rewind();
    };

    // ---------------------------------------------------------------------- //
    struct SimoutAdapter : public boost::static_visitor<z3::expr>
    {
        Z3ExprRewritingAdapter& adapter;
        const Node* node;
        z3::expr r_expr;
        z3::expr r_cnst;

        // constructor.
        SimoutAdapter(Z3ExprRewritingAdapter& a, const Node* n);

        // visitors.
        z3::expr operator() (bool res) const;
        z3::expr operator() (const std::string& res) const;
        z3::expr operator() (const MemValues& mv) const;
    };

    // ---------------------------------------------------------------------- //
    class Synthesizer
    {
    protected:
        static const char* suffix1;
        static const char* suffix2;

        Abstraction& abs;
        SupportVars decodeSupport;

        int MAX_SYN_ITER;
        z3::context c;
        z3::solver S, Sp;
        Z3ExprAdapter c1, c2;

        DITree ditree;

        void _initSolverAssumptions(
            const nptr_vec_t& assumps,
            Z3ExprAdapter& c1,
            Z3ExprAdapter& c2);

        void _addExpr(
            const nptr_t& expr,
            Z3ExprAdapter& c1,
            Z3ExprAdapter& c2);

        std::unique_ptr<DistInput> _getDistInput(z3::expr y);

        nptr_t _synthesize(
            const std::string& name, 
            const nptr_t& de_expr, const nptr_t& expr,
            PyObject* pyfun);

        nptr_t _synthesizeEx(
            const std::string& name, 
            const nptr_t& de_expr, const nptr_t& expr,
            PyObject* pyfun);

        z3::expr _createSynMiter(const nptr_t& ex);

    public:
        // constructor.
        Synthesizer(Abstraction& abs);
        // destructor.
        ~Synthesizer();

        // synthesize all.
        void synthesizeAll(PyObject* pyfun);


        friend class DITree;
    };
    // ---------------------------------------------------------------------- //
}

#endif

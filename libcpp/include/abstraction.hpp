#ifndef __ABSTRACTION_HPP_DEFINED__
#define __ABSTRACTION_HPP_DEFINED__

#include <set>
#include <map>
#include <vector>
#include <string>
#include <boost/python.hpp>

#include <common.hpp>
#include <ast.hpp>
#include <smt.hpp>
#include <imexport.hpp>
#include <cppsimgen.hpp>

namespace ila
{
    class Abstraction;
    class AbstractionWrapper;
    typedef boost::shared_ptr<Abstraction> abstraction_ptr_t;

    struct assump_visitor_i {
        virtual void useAssump(const nptr_t& a) = 0;
    };

    class Abstraction
    {
    public:
        // type of the state element/input.
        enum state_t { INP, REG, BIT, MEM, FUN };

        struct uabstraction_t {
            // when is the abstraction valid.
            nptr_t valid;
            // the abstraction.
            abstraction_ptr_t abs; 
        };

        // list of microabstractions.
        typedef std::map<std::string, uabstraction_t> uabs_map_t;

    private:
        static int objCnt;
        int MAX_SYN_ITER;
    protected:

        // parent abstraction.
        Abstraction* parent;

        // name of this abstraction.
        const std::string name;

        // list of known names.
        std::map<std::string, state_t> names;

        // list of inputs
        nmap_t inps;
        // list of registers.
        nmap_t regs;
        // list of bits.
        nmap_t bits;
        // list of memories.
        nmap_t mems;
        // list of functions.
        nmap_t funs;

        // fetch
        nptr_t fetchExpr;
        nptr_t fetchValid;

        // decode
        nptr_vec_t decodeExprs;

        // assumptions.
        nptr_vec_t assumps;

        // list of sub-abstractions.
        uabs_map_t uabs;


        void initMap(nmap_t& from_m, nmap_t& to_m);

        void extractModelValues(
            Z3ExprAdapter& c,
            z3::model& m, 
            py::dict& result
        );

        NodeRef* getVar(const nmap_t& m, const std::string& name);
        void addVar(state_t st, nmap_t& m, nptr_t& n);

    public:
        int paramSyn;

        // Constructor.
        Abstraction(const std::string& name);

        // Constructor for a micro-abstraction
        Abstraction(Abstraction* parent, const std::string& name);

        // Destructor.
        ~Abstraction();

        // Get a new ID.
        static int getObjId();

        // Create a bitvector input.
        NodeRef* addInp(const std::string& name, int width);

        // Create a boolean variable.
        NodeRef* addBit(const std::string& name);

        // Create a bitvector variable.
        NodeRef* addReg(const std::string& name, int width);

        // Create a memory.
        NodeRef* addMem(const std::string& name, int addrW, int dataW);

        // Create a function.
        NodeRef* addFun(const std::string& name, int retW, const py::list& l);

        // Get an existing boolean.
        NodeRef* getBit(const std::string& name);
        // Get an existing bitvector.
        NodeRef* getReg(const std::string& name);
        // Get an existing memory.
        NodeRef* getMem(const std::string& name);
        // Get an existing function.
        NodeRef* getFun(const std::string& name);

        // add a var if it does not exist.
        void addVar(nptr_t& nref);

        // Set the init value for this var.
        void setInit(const std::string& name, NodeRef* n);
        // Get the initial value for this var.
        NodeRef* getInit(const std::string& name) const;

        // Set the next template for this var.
        void setNext(const std::string& name, NodeRef* n);
        // Get the next template.
        NodeRef* getNext(const std::string& name) const;

        // Create a uabstraction.
        AbstractionWrapper* addUAbs(
            const std::string& name,
            NodeRef* valid);
        // Get an existing uabstraction.
        AbstractionWrapper* getUAbs(const std::string& name);
        // Connect a microinstruction.
        void connectUInst(
            const std::string& name, 
            const abstraction_ptr_t& uabs);

        // Create a bitvector constant with a long integer.
        NodeRef* bvConstLong(py::long_ l, int width);
        // Create a bitvector constant with an integer.
        NodeRef* bvConstInt(unsigned int l, int width);

        // Create a boolean constant (from a bool).
        NodeRef* boolConstB(bool b);
        // Create a boolean constant (from an integer: nonzero = true).
        NodeRef* boolConstI(int b);
        // Create a boolean constant (from an python long).
        NodeRef* boolConstL(py::long_ b);

        // Create a memory constant (from a memvalues object).
        NodeRef* memConst(const MemValues& mv);

        // return the current fetch expression.
        NodeRef* getFetchExpr() const;
        // set the fetch expression.
        void setFetchExpr(NodeRef* expr);
        // return the fetch valid expression.
        NodeRef* getFetchValid() const;
        // set the fetch valid expresssion.
        void setFetchValid(NodeRef* expr);
        // set decode.
        void setDecodeExpressions(const py::list& l);
        // get decode expressions.
        py::list getDecodeExpressions() const;

        // add an assumption.
        void addAssumption(NodeRef* expr);
        // get all assumptions.
        py::list getAllAssumptions() const;

        // the real synthesize function.
        void synthesizeAll(PyObject* fun);

        // synthesize only one register.
        void synthesizeReg(const std::string& name, PyObject* fun);

        // the synthesis function.
        NodeRef* synthesizeElement(
            const std::string& name, 
            NodeRef* expr, PyObject* fun);

        // the export function that export only one expression.
        void exportOneToFile(NodeRef* node,
                             const std::string& fileName) const;
        // the export function that export the overall model.
        void exportAllToFile(const std::string& fileName) const;

        // the import function that import only one expression.
        NodeRef* importOneFromFile(const std::string& fileName);
        // the import function that import the overall model.
        void importAllFromFile(const std::string& fileName);

        // the simulator generating function.
        void generateSim(const std::string& fileName) const;

        // check equality function.
        bool areEqual(NodeRef* left, NodeRef* right) const;
        // check quality under assumption
        bool areEqualAssump(NodeRef* assump, NodeRef* left, NodeRef* right);
        // check after unrolling.
        bool areEqualUnrolled(unsigned n, NodeRef* reg, NodeRef* exp);

        // get memories.
        const nmap_t& getMems() const { return mems; }
        // get (bitvector) inputs.
        const nmap_t& getInps() const { return inps; }
        // get bitvectors.
        const nmap_t& getRegs() const { return regs; }
        // get booleans.
        const nmap_t& getBits() const { return bits; }
        // get functions.
        const nmap_t& getFuns() const { return funs; }

        // visit each assumption.
        void forEachAssump(assump_visitor_i& i) const;
        // get all assumptions in a vector.
        void getAllAssumptions(nptr_vec_t& assump_vec) const;

        // collect all assumptions in a vector.
        struct assump_collector_t : public assump_visitor_i {
            nptr_vec_t& vec;
            assump_collector_t(nptr_vec_t& v) : vec(v) {}
            void useAssump(const nptr_t& a);
        };
            

        friend class Synthesizer;

    protected:
        nptr_t _synthesize(
            const std::string& name, 
            const nptr_vec_t& assumps, const nptr_t& expr,
            PyObject* pyfun);

        bool checkAndInsertName(state_t st, const std::string& name);

        // does the next expr exist?
        bool doesNextExist(const nmap_t& m) const;
        // which is the map containing this node?
        nmap_t* getMap(const std::string& name, NodeRef* n);
        // what is the map containing this name?
        nmap_t::const_iterator findInMap(const std::string& name) const;
        nmap_t::iterator findInMap(const std::string& name);

        friend class AbstractionWrapper;

    private:
        // Set inputs, states, and functions to the simulator generator.
        void addVarToSimulator(CppSimGen* gen, bool force) const;
        // Set next value to the function.
        void setUpdateToFunction(CppSimGen* gen, CppFun* fun, 
                                 nptr_t valid) const;
    };

    // This class contains a shared pointer to an underlying
    // abstraction.  We use this to wrap up objects and pass them
    // around in python and the shared_ptr does the refcounting for us.
    struct AbstractionWrapper
    {
        abstraction_ptr_t abs;

        // constructor.
        AbstractionWrapper(const std::string& name) 
          : abs(new Abstraction(name)) 
        {
        }

        // constructor for microabstraction.
        AbstractionWrapper(Abstraction* parent, 
                           const std::string& name)
          : abs(new Abstraction(parent, name))
        {
        }

        // constructor with existing pointer.
        AbstractionWrapper(const abstraction_ptr_t& a)
          : abs(a)
        {
        }

        // destructor.
        ~AbstractionWrapper() {}; 

        // Create a bitvector input.
        NodeRef* addInp(const std::string& name, int width) {
            return abs->addInp(name, width);
        }

        // Create a boolean variable.
        NodeRef* addBit(const std::string& name) {
            return abs->addBit(name);
        }

        // Create a bitvector variable.
        NodeRef* addReg(const std::string& name, int width) {
            return abs->addReg(name, width);
        }

        // Create a memory.
        NodeRef* addMem(const std::string& name, int addrW, int dataW) {
            return abs->addMem(name, addrW, dataW);
        }

        // Create a function.
        NodeRef* addFun(const std::string& name, int retW, const py::list& l) {
            return abs->addFun(name, retW, l);
        }

        // Get an existing boolean.
        NodeRef* getBit(const std::string& name) {
            return abs->getBit(name);
        }

        // Get an existing bitvector.
        NodeRef* getReg(const std::string& name) {
            return abs->getReg(name);
        }

        // Get an existing memory.
        NodeRef* getMem(const std::string& name) {
            return abs->getMem(name);
        }

        // Get an existing function.
        NodeRef* getFun(const std::string& name) {
            return abs->getFun(name);
        }

        // Set the init value for this var.
        void setInit(const std::string& name, NodeRef* n) {
            abs->setInit(name, n);
        }
        // Get the initial value for this var.
        NodeRef* getInit(const std::string& name) const {
            return abs->getInit(name);
        }

        // Set the next template for this var.
        void setNext(const std::string& name, NodeRef* n) {
            abs->setNext(name, n);
        }
        // Get the next template.
        NodeRef* getNext(const std::string& name) const {
            return abs->getNext(name);
        }

        // Create a microabstraction.
        AbstractionWrapper* addUAbs(const std::string& name, NodeRef* valid)
        {
            return abs->addUAbs(name, valid);
        }
        // Get an existing microabstraction
        AbstractionWrapper* getUAbs(const std::string& name)
        {
            return abs->getUAbs(name);
        }
        // Connect a variable to its corresponding value in the ubs
        void connectUInst(const std::string& name, const AbstractionWrapper* uabs)
        {
            abs->connectUInst(name, uabs->abs);
        }


        // Create a bitvector constant with a long integer.
        NodeRef* bvConstLong(py::long_ l, int width) {
            return abs->bvConstLong(l, width);
        }

        // Create a bitvector constant with an integer.
        NodeRef* bvConstInt(unsigned int l, int width) {
            return abs->bvConstInt(l, width);
        }

        // Create a boolean constant (from a bool).
        NodeRef* boolConstB(bool b) {
            return abs->boolConstB(b);
        }

        // Create a boolean constant (from an integer: nonzero = true).
        NodeRef* boolConstI(int b) {
            return abs->boolConstI(b);
        }

        // Create a boolean constant (from an python long).
        NodeRef* boolConstL(py::long_ b) {
            return abs->boolConstL(b);
        }

        // Create a memory constant (from a memvalues object).
        NodeRef* memConst(const MemValues& mv) {
            return abs->memConst(mv);
        }

        // return the current fetch expression.
        NodeRef* getFetchExpr() const {
            return abs->getFetchExpr();
        }

        // set the fetch expression.
        void setFetchExpr(NodeRef* expr) {
            abs->setFetchExpr(expr);
        }

        // return the fetch valid expression.
        NodeRef* getFetchValid() const {
            return abs->getFetchValid();
        }

        // set the fetch valid expresssion.
        void setFetchValid(NodeRef* expr) {
            abs->setFetchValid(expr);
        }

        // set decode.
        void setDecodeExpressions(const py::list& l) {
            abs->setDecodeExpressions(l);
        }

        // get decode expressions.
        py::list getDecodeExpressions() const {
            return abs->getDecodeExpressions();
        }

        // add an assumption.
        void addAssumption(NodeRef* expr) {
            abs->addAssumption(expr);
        }

        // get all assumptions.
        py::list getAllAssumptions() const {
            return abs->getAllAssumptions();
        }

        // the real synthesize function.
        void synthesizeAll(PyObject* fun) {
            return abs->synthesizeAll(fun);
        }

        // synthesize only one register.
        void synthesizeReg(const std::string& name, PyObject* fun) {
            return abs->synthesizeReg(name, fun);
        }

        // the synthesis function.
        NodeRef* synthesizeElement(
            const std::string& name, 
            NodeRef* expr, PyObject* fun) {
            return abs->synthesizeElement(name, expr, fun);
        }

        // the export function that export only one expression.
        void exportOneToFile(NodeRef* node,
                             const std::string& fileName) const 
        {
             abs->exportOneToFile(node, fileName);
        }

        // the export function that export the overall model.
        void exportAllToFile(const std::string& fileName) const
        {
            abs->exportAllToFile(fileName);
        }

        // the import function that import only one expression.
        NodeRef* importOneFromFile(const std::string& fileName)
        {
            return abs->importOneFromFile(fileName);
        }

        // the import function that import the overall model.
        void importAllFromFile(const std::string& fileName)
        {
            abs->importAllFromFile(fileName);
        }

        // the simulator generating function.
        void generateSim(const std::string& fileName) const
        {
            abs->generateSim(fileName);
        }

        // check equality function.
        bool areEqual(NodeRef* left, NodeRef* right) const
        {
            return abs->areEqual(left, right);
        }

        // check quality under assumption
        bool areEqualAssump(NodeRef* assump, NodeRef* left, NodeRef* right)
        {
            return abs->areEqualAssump(assump, left, right);
        }

        // check equality after unrolling.
        bool areEqualUnrolled(unsigned n, NodeRef* reg, NodeRef* exp)
        {
            return abs->areEqualUnrolled(n, reg, exp);
        }

        int getEnParamSyn() const {
            return abs->paramSyn;
        }

        void setEnParamSyn(int en) {
            abs->paramSyn = en;
        }

        std::string getName() const {
            return abs->name;
        }

        // get memories.
        const nmap_t& getMems() const { return abs->getMems(); }
        // get (bitvector) inputs.
        const nmap_t& getInps() const { return abs->getInps(); }
        // get bitvectors.
        const nmap_t& getRegs() const { return abs->getRegs(); }
        // get booleans.
        const nmap_t& getBits() const { return abs->getBits(); }
        // get functions.
        const nmap_t& getFuns() const { return abs->getFuns(); }
    };

}
#endif

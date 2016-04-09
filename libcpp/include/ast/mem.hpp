#ifndef __AST_MEM_HPP_DEFINED__
#define __AST_MEM_HPP_DEFINED__

#include <vector>
#include <utility>

#include <ast.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <memvalues.hpp>

namespace ila 
{
    class Abstraction;

    // ---------------------------------------------------------------------- //
    // base class for all memory expressions.
    class MemExpr : public Node {
    public:

        // constructor.
        MemExpr(Abstraction* c, int addrWidth, int dataWidth);
        // constructor for ChoiceExpr.
        MemExpr(Abstraction* c, NodeType t);
        // destructor.
        virtual ~MemExpr();

    };

    // ---------------------------------------------------------------------- //
    // memory variables.
    class MemVar : public MemExpr {
    public:
        // constructor.
        MemVar(Abstraction* c, const std::string& n, int addrWidth, int dataWidth);
        // destructor.
        virtual ~MemVar();
        // clone.
        virtual Node* clone() const;
        // equality method.
        virtual bool equal(const Node* that) const;
        // stream output.
        virtual std::ostream& write(std::ostream& out) const;
    };

    // ---------------------------------------------------------------------- //
    // bitvector constants.
    struct MemConst : public MemExpr {
        // fields.
        MemValues memvalues;

        // constructor with longs.
        MemConst(Abstraction* c, const MemValues& mv);
        // copy constructor.
        MemConst(const MemConst& that);
        // destructor.
        virtual ~MemConst();

        // clone
        virtual Node* clone() const;
        // equality method.
        virtual bool equal(const Node* that) const;
        // stream output.
        virtual std::ostream& write(std::ostream& out) const;
    };

    // ---------------------------------------------------------------------- //
    // write to memory operator.
    class MemWr : public MemExpr {
    protected:
        // data members //
        boost::shared_ptr<Node> mem;
        boost::shared_ptr<Node> addr;
        boost::shared_ptr<Node> data;
    public:
        // constructor
        MemWr(
            boost::shared_ptr<Node> m,
            boost::shared_ptr<Node> a,
            boost::shared_ptr<Node> d);
        // copy constructor.
        MemWr(const MemWr& that);
        // destructor.
        virtual ~MemWr();

        // clone.
        virtual Node* clone() const;
        // equality.
        virtual bool equal(const Node* that) const;
        // stream output.
        virtual std::ostream& write(std::ostream& out) const;

        // number of operands.
        virtual unsigned nArgs() const;
        // operand i.
        virtual boost::shared_ptr<Node> arg(unsigned i) const;
    };

}
#endif


#ifndef __AST_BITVEC_HPP_DEFINED__
#define __AST_BITVEC_HPP_DEFINED__

#include <iostream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <z3++.h>
#include <assert.h>
#include <type.hpp>

#include <ast.hpp>

namespace ila 
{
    class Context;

    // ---------------------------------------------------------------------- //
    // Bitvector expressions are derived from this class.
    class BitvectorExpr : public Node {
    public:
        // constructor.
        BitvectorExpr(Context* c, int width);
        // destructor.
        virtual ~BitvectorExpr();
        // operators.
        virtual Node* complement() const;
    };

    // ---------------------------------------------------------------------- //
    // Bitvector variables.
    class BitvectorVar : public BitvectorExpr {
    public:
        // constructor.
        BitvectorVar(Context* c, std::string n, int width) ;

        // destructor.
        virtual ~BitvectorVar();

        // clone.
        virtual Node* clone() const;

        // stream output.
        virtual std::ostream& write(std::ostream& out) const;

    };

    // ---------------------------------------------------------------------- //
    // Bitvector operators.
    class BitvectorOp : public BitvectorExpr {
    public:
        // Number of operands.
        enum Arity { UNARY, BINARY, TERNARY } arity; 

        // What is the operation?
        enum Op { 
            // unary
            NEGATE, COMPLEMENT, LNOT, NONZERO,  
            // binary.
            ADD, SUB, AND, OR, XOR, XNOR, NAND, NOR,
            // ternary
            IF
        } op;

        static const char* operatorNames[];

        // the operands themselves.
        std::vector< boost::shared_ptr<Node> > args;

        // Don't forget to update these helper functions below.
        static bool isUnary(Op op) { return op >= NEGATE && op <= NONZERO; }
        static bool isBinary(Op op) { return op >= ADD && op <= NOR; }
        static bool isTernary(Op op) { return op >= IF && op <= IF; }
        static int getUnaryResultWidth(Op op, boost::shared_ptr<Node> n);
        static bool checkUnaryOpWidth(Op op, boost::shared_ptr<Node> n, int width);

        // constructors.
        BitvectorOp(Context* c, Op op, boost::shared_ptr<Node> n1);
        BitvectorOp(Context* c, Op op, boost::shared_ptr<Node> n1, boost::shared_ptr<Node> n2);

        // destructors.
        virtual ~BitvectorOp();

        // clone.
        virtual Node* clone() const;

        // stream output.
        virtual std::ostream& write(std::ostream& out) const;
    };
}
#endif

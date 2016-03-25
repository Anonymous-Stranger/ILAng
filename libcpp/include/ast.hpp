#ifndef __AST_HPP_DEFINED__
#define __AST_HPP_DEFINED__

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <z3++.h>
#include <assert.h>
#include <type.hpp>

#include <ast/node.hpp>
#include <ast/bitvec.hpp>
#include <ast/bool.hpp>
#include <boost/shared_ptr.hpp>

namespace ila
{
    struct NodeRef 
    {
        // ------------------------ MEMBERS ----------------------------- //
        boost::shared_ptr<Node> node;

        // --------------- CONSTRUCTOR/DESTRUCTOR ----------------------- //
        // Default constructor. DO NOT USE. Will throw an exception.
        NodeRef(); 
        // Constructor. DO NOT USE. Call factory methods in Context instead.
        NodeRef(Node* node);
        // Copy constructor.
        NodeRef(const NodeRef& nr);
        // Destructor.
        ~NodeRef();

        // assignment operator.
        NodeRef& operator=(const NodeRef& other);

        // ---------------------- METHODS ------------------------------- //
        std::string getName() const;
        NodeType getType() const;
        void doSomething();

        // ---------------------- OPERATORS ----------------------------- //
        NodeRef* complement() const; 
        NodeRef* negate() const;

        NodeRef* logicalNot() const;
        NodeRef* logicalAnd(NodeRef* other) const;
        NodeRef* logicalOr(NodeRef* other) const;
        NodeRef* logicalXor(NodeRef* other) const;
        NodeRef* logicalXnor(NodeRef* other) const;

        NodeRef* add(NodeRef* other) const;
        NodeRef* addInt(int r) const;
        NodeRef* raddInt(int r) const;
        NodeRef* sub(NodeRef* other) const;
        NodeRef* subInt(int r) const;
        NodeRef* rsubInt(int r) const;

    private:
        // ---------------------- HELPERS ----------------------------- //
        NodeRef* _unOp(BoolOp::Op boolOp, BitvectorOp::Op bvOp, const char* opName) const;
        NodeRef* _binOp(BoolOp::Op boolOp, BitvectorOp::Op bvOp, 
                        const char* opName, NodeRef* other) const;
        NodeRef* _binOp(BitvectorOp::Op op, NodeRef* other) const;
        NodeRef* _binOp(BitvectorOp::Op op, int r) const;
        NodeRef* _binOpR(BitvectorOp::Op op, int r) const;
    };

    // stream output.
    std::ostream& operator<<(std::ostream& out, const NodeRef& node);
}

#endif 

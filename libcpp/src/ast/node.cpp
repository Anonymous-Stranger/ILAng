#include <ast.hpp>
#include <util.hpp>
#include <exception.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>


namespace ila
{
    int Node::totalObjCnt = 0;

    Node::~Node()
    {
        std::cout << "node destroyed; name:" << name << std::endl;
    }

    void Node::_initName()
    {
        name = "n" + boost::lexical_cast<std::string>(id);
        std::cout << "node created." << std::endl;
    }

    Node* Node::clone() const
    {
        return new Node();
    }

    Node* Node::complement() const
    {
        throw PyILAException(PyExc_NotImplementedError, "Complement not implemented.");
        return NULL;
    }

    Node* Node::negate() const
    {
        throw PyILAException(PyExc_NotImplementedError, "Negate not implemented.");
    }

    void Node::doSomething()
    {
        using namespace z3;
        context c;

        expr x = c.bool_const("x");
        expr y = c.bool_const("y");
        expr conjecture = !(x && y) == (!x || !y);
        
        solver s(c);
        // adding the negation of the conjecture as a constraint.
        s.add(!conjecture);
        std::cout << s << "\n";
        switch (s.check()) {
        case unsat:   std::cout << "de-Morgan is valid\n"; break;
        case sat:     std::cout << "de-Morgan is not valid\n"; break;
        case unknown: std::cout << "unknown\n"; break;
        }
    }
}

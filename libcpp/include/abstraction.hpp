#ifndef __ABSTRACTION_HPP_DEFINED__
#define __ABSTRACTION_HPP_DEFINED__

#include <string>
#include <ast.hpp>
#include <boost/python.hpp>

namespace ila
{
    class Abstraction
    {
    private:
        int objCnt;
    protected:
        // Get a new ID.
        int getObjId();
    public:
        // Constructor.
        Abstraction();
        // Destructor.
        ~Abstraction();
        // Create a bitvector variable.
        NodeRef* addRegister(const std::string& name, int width);
        // Create a bitvector constant with a long integer.
        NodeRef* bvConstLong(boost::python::long_ l, int width);
        // Create a bitvector constant with an integer.
        NodeRef* bvConstInt(int l, int width);

        friend class Node;
    };
}
#endif

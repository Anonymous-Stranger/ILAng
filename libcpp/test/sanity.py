import ila

def main():
    c = ila.Context()
    n = c.BitvectorVar("test", 16)
    print n.name
    print n.type
    print n.type.bitwidth
    n.doSomething()
    del n

    print 'creating new bitvecvar'
    m = c.BitvectorVar('mambo', 24)
    n = m
    del m
    print n.name
    p = ~n
    print p.name
    print p.type
    print p.type.bitwidth



if __name__ == '__main__':
    main()

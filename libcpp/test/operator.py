import ila

def main():
    c = ila.Abstraction()
#    n = ila.Node()

    # test on boolOp
    top = c.bool(True)
    bot = c.bool(False)
    Band = (top & bot)      # 0
    Bor = (top | bot)       # 1
    Bxor = (Band ^ Bor)     # 1
    Bnor = ila.nor(Band, Bor)       # 0
    Bxnor = ila.xnor(Bxor, Bnor)    # 0
    Bnand = ila.nand(Bnor, Bxnor)   # 1
    nBnor = ~Bnor           # 1
    assert c.areEqual(nBnor, top)

    b1 = c.bit('b1')
    b2 = c.bit('b2')
    b3 = c.bit('b3')
    b4 = (b1 & b2) ^ b3
    b5 = ila.xnor(ila.nand(b1, b2), b3)
    assert c.areEqual(b4, b5)

    b6 = ila.ite(b1, b2, b3)
    b7 = (b1 & b2) | (~b1 & b3)
    assert c.areEqual(b6, b7)

    # test on bitvectorOp
    x = c.reg('x', 8)
    y = c.reg('y', 8)
    c0 = c.const(0, 8)
    c1 = c.const(1, 8)
    c2 = c.const(2, 8)
    c4 = c.const(4, 8)
    c6 = c.const(6, 8)
    c8 = c.const(8, 8)
    v1 = (x == c4)
    v2 = x << 1
    v3 = c4 << 1

    assert c.areEqual(c8, v3)
    assert c.areEqual(c8, (c4 + c4))
    assert c.areEqual(c4, (c8 - c4))
    assert c.areEqual(c8, (c4 * c2))
    assert c.areEqual(c4, (c8 / c2))
    v4 = ila.ite(v1, v2, v3)    # 8
    assert c.areEqual(v4, c8)
    assert c.areEqual(v4, v3)
    assert c.areEqual(v4, (4 + c4))
    assert c.areEqual(-c4, (c4 - 8))
    assert c.areEqual(v4, (2 * c4))
    assert c.areEqual(v4 >> 2, (v3 / c2) - 2)
    assert c.areEqual(c8 % 5, 7 % (1 << c2))
    assert c.areEqual( (x < y) ^ (y <= x), (x == y) | (x != y) )
    assert c.areEqual( (x > y) | (x == y) | ~(x >= y), top )
    assert c.areEqual( ~x ^ x, y ^ ~y)
    assert c.areEqual( ~x, ila.nand(x, x) )
    v5 = ~ila.nor(c2, c4)   # 00000110
    assert c.areEqual( ~v5, ila.xnor(c4, c2))
    v6 = c2 - c4    # 11111110
    v7 = 3 - c8     # 11111011
    v8 = ~(c2 - 2)  # 11111111
    assert c.areEqual( v8, ~c0)
    assert c.areEqual( v8 - 1, v6)
    assert c.areEqual( c4 + c1, -v7) # 00000101
    assert c.areEqual( ila.sdiv(c4, c2), c2)
    assert c.areEqual( ila.sdiv(-c4, c2), -c2)
    assert c.areEqual( ila.sdiv(v5, -4), -c1)
    assert c.areEqual( ila.srem(v5, -4), c2)
    # -6  = -4 * 1 + -2  ??
    assert c.areEqual( ila.sdiv(-6, -c4), c1)
    assert c.areEqual( ila.srem(-v5, -c4), -c2)
    assert c.areEqual( x - ila.srem(x, y), ila.sdiv(x, y) * y )
    assert c.areEqual( x - ila.urem(x, y), (x / y) * y )
    assert c.areEqual( ila.ashr(v6, 1), v8)
    assert c.areEqual( ila.slt(v7, v6), top)

    s1 = c.const(1, 4)
    s2 = c.const(2, 4)
    v9 = ila.concat(s1, s2) # 00010010
    v10 = (c1 << 4) + c2
    assert c.areEqual(v9, v10)
    v11 = ila.rrotate(v9, 2) # 10000100
    v12 = ila.lrotate(v9, 6)
    assert c.areEqual(v11, v12)
    s3 = c.const(9, 4)
    v13 = ila.extract(v9, 1, 4)
    assert c.areEqual(s3, v13)

if __name__ == '__main__':
    main()

import ila

"""
W x = 1 |  R y = 1
W y = 1 |  R x = 0
	
"""
#--------- CG ------------------
# Dummy global state for init
dummy = ila.Abstraction("dummy")
dummy.reg('x', 8)
dummy.reg('y', 8)
dummy.set_init('x', ila.const(0,8))
dummy.set_init('y', ila.const(0,8))


#--------- C1 ------------------

c1 = ila.Abstraction("c1")

x  = c1.reg('x', 8)
y  = c1.reg('y', 8)
pc1 = c1.reg('pc1',8)

c1.set_init('pc1', ila.const(0, 8 ) )

instc11 = c1.inst("W_x_1", pc1 == 0)
instc11.set_next('x',  ila.const(1, 8) )
instc11.set_next('pc1', ila.const(1, 8) )

instc12 = c1.inst("W_y_1", pc1 == 1)
instc12.set_next('y',  ila.const(1, 8) )
instc12.set_next('pc1', ila.const(2, 8) )


#--------- C2 ------------------

c2 = ila.Abstraction("c2")

x  = c2.reg('x', 8)
y  = c2.reg('y', 8)
r1 = c2.reg('r1', 8)
r2 = c2.reg('r2', 8)
pc2 = c2.reg('pc2',8)

c2.set_init('pc2', ila.const(0, 8 ) )

instc21 = c2.inst("R_x_to_r1", pc2 == 0)
instc21.set_next('r1',  x)
instc21.set_next('pc2', ila.const(1, 8) )

instc22 = c2.inst("R_y_to_r2", pc2 == 1)
instc22.set_next('r2',  y)
instc22.set_next('pc2', ila.const(2, 8) )


#--------- Check ------------------
tso = ila.MCM_TSO()

unroller = ila.newMcmUnroller(
	nTh     = [1,1] ,
	entity  = [c1, c2] , 
	program = [ ["W_x_1","W_y_1"], ["R_x_to_r1", "R_y_to_r2"] ] , 
	dummyInit = dummy,
	mm      = tso )

ila.enablelog("mcmUnroller.print")
ila.enablelog("mcmUnroller.icheck")
ila.enablelog("mcmUnroller.check")
ila.setloglevel(3,"")

unroller.unroll()
unroller.addPropertyOnEnding( (r1 == 1) & (r2 == 2) )
unroller.finalizePiFun()
print unroller.pushInstConstraintAndCheck()
unroller.connectByAnd()
print unroller.check()
unroller.printModel()





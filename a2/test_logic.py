# test_logic.py

import logic as logic

"""
@purpose - Test the logic expression evaluator. 
   Failed tests will print to the screen with the test number and
      the failed expression
   Passed tests will NOT print anything.
"""

d = {
   "a" : True,
   "b" : False,
   "c" : True,
   "d" : True,
   "e" : True,
   "f" : True,
   "doodoo" : True,
   "booboo" : False
}

global_fail_flag = False
fail_count = [0]
num_tests = [0]

def attempt(num,expected,e):
   rs = False
   try:
      num_tests[0]+=1
      rs = (expected == logic.eval_expr(e,d))
   finally:
      if( rs != True):
         global_fail_flag = True
         fail_count[0]+= 1
         print("{:<10d} : {}".format(num,e) )

attempt(1,False,"a * b");
attempt(2,True ,"a + b");
attempt(3,False,"a -> b");
attempt(4,False,"a / b");   
attempt(5,False,"~a");
attempt(6,True,"(a * b) + doodoo + booboo");
attempt(7,True,"((a * b) + doodoo + booboo)");
attempt(8,True,"((a * b) + (c * d)) -> f");

print("{}/{} Tests Passed".format(num_tests[0] - fail_count[0],num_tests[0]))
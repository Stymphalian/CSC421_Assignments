import sys
import match

global_fail_flag = False
fail_count = [0]
num_tests = [0]

def attempt(num,expected,e1,e2):
   rs = False
   try:
      num_tests[0]+=1
      rs = (expected == match.match(e1,e2))
   finally:
      if( rs != True):
         global_fail_flag = True
         fail_count[0]+= 1
         print("{:<10d} : {},{}".format(num,e1,e2) )

attempt(1,
   (True,{"x":"Fred","y":"George"}),
   "Brother(Fred,George)",
   "Brother(x,y)"
)

attempt(2,
   (True,{"x":"Dog(Fred)","y":"Dog(George)"}),
   "Brother(Dog(Fred),Dog(George))",
   "Brother(x,y)"
)

attempt(3,
   (True,{"x":"Fred","y":"Dog(George)"}),
   "Brother(Dog(Fred),Dog(George))",
   "Brother(Dog(x),y)"
)

attempt(4,
   (False,{}),
   "Brother(Dog(Fred),Dog(George))",
   "Brother(Dog(x),Dog(x))"
)

attempt(5,
   (True,{"x":"A"}),
   "Family(Mother(A),Father(A),A)",
   "Family(Mother(x),Father(x),x)"
)

attempt(6,
   (True,{"x":"A"}),
   "Family(Mother(A),Father(A),A)",
   "Family(Mother(x),Father(A),x)"
)

attempt(7,
   (True,{"x":"A","y":"A"}),
   "Family(Mother(A),Father(A),A)",
   "Family(Mother(x),Father(y),x)"
)

attempt(8,
   (False,{}),
   "Family(Mother(A),Father(A),A)",
   "Family(Mother(x),y,y)"
)

attempt(9,
   (True,{"x":"A","y":"Father(A)"}),
   "Family(Mother(A),Brother(Father(A)),Father(A))",
   "Family(Mother(x),Brother(y),y)"
)
attempt(10,
   (True,{"x":"A","y":"Father(A)"}),
   "Family(Mother(A),Father(A),Brother(Father(A)))",
   "Family(Mother(x),y,Brother(y))"
)

attempt(11,
   (False,{}),
   "Family(Mother(A))",
   "Family()"
)

print("{}/{} Tests Passed".format(num_tests[0] - fail_count[0],num_tests[0]))

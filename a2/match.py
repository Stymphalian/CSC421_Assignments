
class Node:   
   def __init__(self):
      self.isVariable = False
      
      # for constants - just the string (i.e George)
      # for variables - the identifier (i.e x)
      # for formulas - The relation name ( i.e BrotherOf )
      self.value = None
      
      # if the exp is a formula then this will contain a list
      # of all the children nodes      
      # i.e 
      # (Brother(George,Dog(Fred)))
      # [George,Dog(Fred)]
      self.children = []

   """
   @purpose - convert the node into its string representation
      This is important to keep the given format as it is necesary
      for the test cases
   """
   def __str__(self):
      if( self.isVariable):
         return str(self.value)
      elif(len(self.children) == 0):
         return str(self.value)
      else:
         size = len(self.children)
         s = self.value + "("         
         for i in range(0,size):
            s += str(self.children[i])
            if( i != size-1):
               s += ","
         s += ")"
         return s

   def __repr__(self):
      return self.__str__() 

"""
@purpose - helper function used to determine if the given expression
   is a variable or if it is a constant/formula
@parameter e - (string) the expression to evaluate
"""
def isVariable(e):
   if len(e.split("(")) != 1:
      return False
   if( e[0].isupper()):
      return False

   return True

"""
@purpose - parse the given string expression and form an expression tree.
@parameter e - (string) the expression in which to parse.
@return - a Node which represents the root node of the expression tree
"""
def parse(e):
   # remove all the whitespace from the string   
   e = "".join(e.split())

   # Create a new node object
   exp = Node()
   exp.isVariable = isVariable(e)

   if( exp.isVariable == True):
      # the expression is just a constant or an expression
      exp.value = e
      exp.children = []
      return exp
   
   # This is a formula or constant.
   # retrieve the name of the formula or constant
   exp.value = ""
   left_count = 0
   i = 0
   w = ""
   while( i < len(e)):
      c = e[i]      
      i += 1
      # if the expression is a formula, the name ends
      # when we read the first '('
      if( c == "("):
         left_count += 1
         break
      w += c
   exp.value = w

   # parse the rest of the formula for the arguments
   w = ""
   while( i < len(e)):
      c = e[i]
      i += 1      

      if c == "(":
         left_count += 1
      elif  c == ")":
         left_count -= 1
         if( left_count == 0):
            # we have reached the end of the formula
            break
      elif c == ",":
         if(left_count == 1):
            # dump the recorded word
            # recusively call parse on the word
            exp.children.append(parse(w))
            w = ""
            continue      
      w += c

   # make sure that the brackets are balanced
   if( left_count != 0):
      raise Exception("Invalid Expression. Unmatched brackets.")

   # add the last word into the list         
   if len(w) != 0:
      exp.children.append(parse(w))

   return exp

"""
@purpose - determine if exp1 matches exp2
@parameter exp1 - (string) expression in which to match. 
   Cannot contain any vairables
@parameter exp2 - (string) expression which contains
   formulas, constants or variables
@return (bool,dict) - returns a pair which represents the outcome of the match
   if true, then dict contains the dictionary of assignments
   if false, then dict is {}
"""
def match(exp1,exp2):      

   # e1 - expression tree which only contains formulas or constants
   # e2 - expression tree which contains formulas, constants or variables
   # d  - a dictionary holding the currently recorded assignments
   #  Note, that d is modified mutated through recursive calls to the _match
   # return - return true if the expressions match, false otherwise
   def _match(e1,e2,d):
      if( e2.isVariable):
         # e2 is already assigned in the dict d
         if(e2.value in d):
            #variable is already assigned in the dictionary
            if _match(e1,d[e2.value],d):
               # if e1 matches with the dictioanry then we are okay.
               return True
            else:
               # e1 does not match with the dictionary so false
               return False
         else:
            # this is a new variable, assign it and keep going
            d[e2.value] = e1
            return True      
      else:
         # e1 cannot be a variables
         if e1.isVariable:
            raise Exception("Invalid Expression")
         
         # e1 and e2 must have the same value
         if(e1.value != e2.value):
            return False

         # children length must match
         if(len(e1.children) != len(e2.children)):
            return False

         # all the children must match
         for i in range(0,len(e1.children)):
            rs = _match(e1.children[i],e2.children[i],d)
            if( rs == False):
               return False
         
         return True

   d = {}
   rs = _match(parse(exp1),parse(exp2),d)
   if( rs == True):
      d = { k : str(v) for k,v in d.items()}
   else:
      d = {}
   return (rs,d)

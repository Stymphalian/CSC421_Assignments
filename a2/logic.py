# logic.py

operators = {
   "*" : 0, # and
   "+" : 0, # or
   "->" : 1, # implies
   "/" : 1, # iff
   "~" : 2, # negation
}
operator_args = {
   "*" : 2,
   "+" : 2,
   "->" : 2,
   "/" : 2,
   "~" : 1,
}
operator_fns = {
   "*" : lambda p : p[0] and p[1],
   "+" : lambda p : p[0] or p[1],
   "->" : lambda p : (not p[0]) or p[1],   
   "/" : lambda p : (not p[0] and p[1]) and (not p[1] and p[0]),
   "~" : lambda p : not p[0],
}

class node:   
   def __init__(self):
      # 0 means operator
      # 1 means value
      self.is_operator = False
      self.value = None
      self.children = []

   def set(self,elem):
      self.is_operator = elem in operators.keys()
      self.value = elem
      return self

   def __str__(self):
      if( self.is_operator):
         return "{{{} {}}}".format(self.value,self.children)
      else:
         return "{}".format(self.value)

   def __repr__(self):
      return self.__str__() 

   def toLowestForm(self):
      if( self.is_operator):         
         if(self.value == "->"):
            # convert to the form (~a + b)
            a = self.children[0]
            b = self.children[1]
            self.children = []

            # change to an "or" operator
            self.value = "+"

            # ~a
            left = node().set("~")
            left.children.append(a)
            self.children.append(left) 

            # b
            self.children.append(b)

         elif(self.value == "/"):
            # convert into the form (a->b) * (b->a)
            a = self.children[0]
            b = self.children[1]

            # change to the "and" operator
            self.value = "*"

            # (a->b)
            p1 = node().set("->")
            p1.children = [a,b]
            p1.toLowestForm()

            # ( b->a)
            p2 = node().set("->")
            p2.children = [b,a]
            p2.toLowestForm()

            self.children = []
            self.children.append(p1)
            self.children.append(p2)

      return self

"""
@purpose - process the string expression into a list of tokens
@parameter e - the string representing the expressoin 
   (i.e  a * b + (c -> d) )
@return - return the list tokens
   (i.e [ "a","*","b","+","(","c","->","d",")" ])
"""
def processIntoTokens(e):
   e = "".join(e.split())   
   tokens = []      
   w = ""
   skip_flag = False
   left_count = 0
   right_count = 0

   for i in range(0,len(e)):
      # see if we should skip the the next token
      if(skip_flag):         
         skip_flag = False
         continue

      c = e[i]            

      # if the token is an operator then just append it
      if( c in ["(",")","*","+","~","/"]):      

         # keep counts on the number of brackets
         if( c == "("):
            left_count += 1
         if( c == ")"):
            right_count += 1

         # if we were recording a identifier, dump it
         # into the tokens array
         if(len(w)!= 0):
            tokens.append(w)
            w = ""

         # add the operator to the token list
         tokens.append(c)

      elif(c=="-"):
         # special case for the '->' operator
         if( i+1 < len(e) and e[i+1]=='>'):

            #record the identifier
            if(len(w)!= 0):
               tokens.append(w)
               w = ""

            # add the operator
            tokens.append("->")
            # we want to skip the token
            skip_flag = True
         else:
            raise Exception("Invalid Expression")
      else: 
         # record the next character in the identifier
         w += c

   # dump the last identifier
   if(len(w) != 0):
      tokens.append(w)

   if(left_count != right_count):
      # ensures matching brackets
      raise Exception("Invalid Expression")
   else:
      return tokens


"""
@purpose: determine the index of the matching brackets
@parameter e: the token list in which to search for the matching brackets
@parameter i: the current position index
@return : the index of the matching bracket
"""
def posMatchingBracket(e,i):
   left_count = 0
   for j in range(i,len(e)):
      c = e[j]      
      if( c == "("):
         left_count += 1
      elif( c == ")"):
         left_count -=1
         if(left_count == 0):
            return j   

"""
@purpose - recursively parse the tokens list an creates an expression tree.
@param e - a list of tokens. Run the expression throughh processIntoTokens
         in order to create the list of tokens
@return - return a node object. 
@references - http://www.engr.mun.ca/~theo/Misc/exp_parsing.htm
"""
def parseExpressionTokens(e):   
   i = 0
   size = len(e)

   # helper funtion which creates an operator node.
   # it takes the required number of arguments from the elem list
   def apply(op,elems):
      num_args = operator_args[op.value]

      # check to see that we have enough arguments
      if(len(elems) < num_args):       
         raise Exception("Invalid Expression")
      
      # create the node with the operator and the required arguments                  
      op.children = []
      for j in range(0,num_args):
         op.children = [elems.pop()] + op.children

      op = op.toLowestForm()         
      return op
   
   ops = []
   elems = []
   while( i < size):
      c = e[i]
      if c == "(" :         
         # we have a sub-expression. evaulate this recursively
         # and place it onto the element stack
         end = posMatchingBracket(e,i)         
         rs = parseExpressionTokens(e[i+1:end])
         elems.append(rs)
         i = end
      
      elif c in operators.keys():
         # we are processing an operator

         if(len(ops) > 0):
            # there are some operators to compare against
            cand = operators[c]            
            champ = operators[ops[-1].value]

            # if we are trying to push an operator with lower precedence
            # on top of an operator with greater precendence
            # we pop off the operator and create a node for that operator
            # (popping off the required number of elements 
            #     from the element stack)
            if( cand <= champ):
               op = ops.pop()
               op = apply(op,elems)

               # push the result back onto the element stack
               elems.append(op)
         
         # add the operator
         n = node().set(c)
         ops.append(n)
      else:
         n = node().set(c)
         elems.append(n)
      i += 1 

   # process the rest of the operators with the remaining arguemnts
   while(len(ops) > 0 ):      
      op = apply(ops.pop(),elems)
      elems.append(op)
   
   return elems[0]


"""
@purpose - evaluate the expression tree given by e, using the predicate
   specifed in the dictionary 
@parameter e - is a node that represents the root of the expression tree in 
   which we want to evaluate
@parameter d - a dictionary of assignments to the variables
"""
def evalExpressionTree(e,d):      
   def _eval(e):
      if e.is_operator:      
         children_rs = tuple(map(_eval,e.children))
         return operator_fns[e.value](children_rs)
      else:
         if e.value.lower() in "true":
            return True
         elif e.value.lower() == "false":
            return False            
         elif e.value in d:
            rs = d[e.value]            
            if( rs == True or rs == False):
               return rs
            else:
               return eval_expr(rs,d)
         else:
            raise Exception("\'{}\' identifier not specified".format(e.value))
   return _eval(e)


"""
@purpose - convenience function which
      processes the expression into a token list
      parses the token list into an expression tree      
"""
def parse_expr(e):
   toks = processIntoTokens(e)
   return parseExpressionTokens(toks)

"""
@purpose - convenience function which
      processes the expression into a token list
      parses the token list into an expression tree
      evaluates the expression tree into the given value
"""
def eval_expr(e,d):
   toks = processIntoTokens(e)
   return evalExpressionTree(parseExpressionTokens(toks),d)
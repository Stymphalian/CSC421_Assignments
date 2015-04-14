# main_logic.py

import sys
import logic as logic


# dictionary holding the assignment of identifiers to expressions
d = {}

while(True):   
   line = sys.stdin.readline()

   # empty line
   if(len(line) == 0):
      continue

   # remove the end-line character
   if(line[-1] == "\n"):
      line = line[:-1]

   # echo the command back to the user
   print(line)

   # split on the tokens to decide if it is a command
   # or an assignment
   toks = line.split("=")
   if( len(toks) == 1):            
      # command
      if(toks[0] == "eval"):
         for e in d:
            if( d[e] == True or d[e] == False):
               continue
            print("formula {} : {} => {}".format(e,d[e],logic.eval_expr(d[e],d)) )

      elif(toks[0] == "ls"):         
         for e in d:
            print("{} : {}".format(e,d[e]))

      elif(toks[0] == "clear"):
         d = {}         
      elif(toks[0] == "exit"):
         exit()

   elif( len(toks) == 2):      
      # an assignment
      iden = toks[0].strip()
      exp = toks[1].strip()

      if exp.lower() == "true":
         d[iden] = True
      elif exp.lower() == "false":
         d[iden] = False
      else:
         d[iden] = exp   
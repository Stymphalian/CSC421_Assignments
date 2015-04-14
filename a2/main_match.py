import sys
import match

state = 2
exps = ["",""]

while(True):   
   line = sys.stdin.readline()

   # empty line
   if(len(line) == 0):
      continue

   # remove the end-line character
   if(line[-1] == "\n"):
      line = line[:-1]

   # if the state is 2 then we are looking for input from the user
   if( state == 2):
      if( line == "match"):
         state = 0
         exps = []
      elif( line == "exit"):
         exit()
   else:
      # look for two lines of expression      
      exps.append(line)
      state = (state + 1)%3

      if( state == 2):         
         rs,d = match.match(exps[0],exps[1])
         if( rs == True):
            print("is true with " + str(d))
         else:
            print("fails")
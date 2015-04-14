get_segment([],A,[],[]).
get_segment([X|Rest],A,[A|Seg],Result):-
   X = A,
   get_segment(Rest,A,Seg,Result).
get_segment([X|Rest],A,[],[X|Rest]):- X \= A.

make_sub([],[]).
make_sub([X|Rest],[Block|Result]):-
   get_segment([X|Rest],X,Block,Remain),
   make_sub(Remain,Result).
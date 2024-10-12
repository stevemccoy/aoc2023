%
% Advent of Code 2023 - Day 17, Clumsy Crucible.
%
% Cribbed from AOC 2023 Day 19.
% 

:- use_module(library(clpfd)).
:- use_module(library(pairs)).
:- working_directory(_, 'c:/users/stephen.mccoy/github/aoc2023/pl/day17/').

:- dynamic row_digits/2.

% List utils.

% List concatenation.
conc([], L, L).
conc([H | Tail], L1, [H | L2]) :-
	conc(Tail, L1, L2).

% S is a sub-sequence of L.	
sublist(S, L) :-
	conc(_, L2, L),
	conc(S, _, L2).	



% DCG for input file data.

digit(D) --> [D],
	{	code_type(D, digit)
	}.

digits([D | Tail]) --> digit(D), !, digits(Tail).
digits([]) --> [].

code_to_digit(C, D) :-
	D is C - 48.

integer(F) --> digits(Digits),
	{	number_codes(F, Digits)
	}.

int_list([Head | Tail]) --> integer(Head), ",", int_list(Tail).
int_list([Head]) --> integer(Head).
int_list([]) --> [].

identifier_code(C) --> [C],
	{	code_type(C, alpha)
	}.

identifier_codes([HC | TC]) --> identifier_code(HC), identifier_codes(TC).
identifier_codes([C]) --> identifier_code(C).

vname(Name) --> identifier_codes(Codes),
	{	string_codes(Name, Codes)
	}.

ruleop(lt) --> "<".
ruleop(gt) --> ">".
ruleop(eq) --> "=".

clause([Var, Op, Value, Result]) --> vname(Var), ruleop(Op), integer(Value), ":", vname(Result).

clause_list([Head | Tail]) --> clause(Head), ",", !, clause_list(Tail).
clause_list([]) --> [].

input_line(N) --> digits(CL),
	{ CL \== [], maplist(code_to_digit, CL, DL), assertz(row_digits(N, DL)) }.

% Utility.

file_line(File, Line) :-
    setup_call_cleanup(open(File, read, In),
        stream_line(In, Line),
        close(In)).

stream_line(In, Line) :-
    repeat,
    (   read_line_to_string(In, Line0),
        Line0 \== end_of_file
    ->  Line0 = Line
    ;   !,
        fail
    ).

% Read the input file.

read_input_line(N, Stream) :- 
	read_line_to_string(Stream, String),
	( String = ""
	  ;
      ( string_codes(String, Chars),
        phrase(input_line(N), Chars, [])
      )
    ).

read_input_lines(N, Stream) :-
	read_input_line(N, Stream),
	!,
	N1 is N + 1,
	read_input_lines(N1, Stream).
read_input_lines(_,_).

read_input_data(FileName) :-
	retractall(row_digits(_,_)),
	open(FileName, read, Stream),
	read_input_lines(1, Stream),
	close(Stream).


% Lookup the square cost on the grid.
grid(C, R, Cost) :-
	row_digits(R, DL),
	nth1(C, DL, Cost).

% Lookup for legal changes of direction.
move_changes([n,e,s,w,n,e]).

% Effect of each move.
move_delta(n, 0, -1).
move_delta(e, 1, 0).
move_delta(s, 0, 1).
move_delta(w, -1, 0).

% Move options.
move_options(1, 1, _, [e, s]) :- !.
move_options(_, _, OldMove, [NM1,NM2]) :- 
	move_changes(MCL),
	sublist([NM1,OldMove,NM2], MCL).

% Cost of making a move.
subpath_cost([C,R],[_,0],[C,R],0).
subpath_cost([C1,R1],[Dir,Dist1],[C2,R2],Cost2) :-
	Dist1 #> 0,
	move_delta(Dir, DC, DR),
	C3 #= C1 + DC,
	R3 #= R1 + DR,
	grid(C3, R3, StepCost),
	Dist2 #= Dist1 - 1,
	subpath_cost([C3,R3],[Dir,Dist2],[C2,R2],Cost1),
	Cost2 #= StepCost + Cost1.

% Search specification.
% goal([13,13,_,_]).
goal(_/_/_/13/13).

s(_/G1/D1/C1/R1, F2/G2/D2/C2/R2, Cost) :-
	% Select an appropriate next move (direction and distance).
	move_options(C1, R1, D1, MoveOptions),
	member(D2, MoveOptions),
	member(Dist2, [1,2,3]),
	% Make sure we're still on the grid and find cost of the move.
	subpath_cost([C1,R1],[D2,Dist2],[C2,R2],Cost),
	G2 #= G1 + Cost,
	h(C2/R2, HCost),
	F2 #= G2 + HCost.

% s(Node1, Node2, Cost) = Cost of moving from Node1 to successor Node2 in state space,
% taking Move2 for distance Dist2.
%% s([C1,R1,Move1,_],[C2,R2,Move2,Dist2],Cost) :-
%% 	% Select an appropriate next move (direction and distance).
%% 	move_options(C1, R1, Move1, MoveOptions),
%% 	member(Move2, MoveOptions),
%% 	member(Dist2, [1,2,3]),
%% 	% Make sure we're still on the grid and find cost of the move.
%% 	subpath_cost([C1,R1],[Move2,Dist2],[C2,R2],Cost).

%% h([C,R,_,_],HCost) :-
h(C/R, HCost) :-
	goal(_/_/_/GC/GR),
	DC #= GC - C,
	DR #= GR - R,
	abs(DC, ADC),
	abs(DR, ADR),
	HCost #= ADR + ADC.


% merge_nodes() --- Merge 2 sorted lists together - needs testing!

merge_nodes([], L, L).
merge_nodes(L, [], L).
merge_nodes([F1-Node1 | Tail1], [F2-Node2 | Tail2], [F1-Node1 | Tail3] ) :-
	F1 < F2, !,
	merge_nodes(Tail1, [F2-Node2 | Tail2], Tail3).
merge_nodes(List1, [F2-Node2 | Tail2], [F2-Node2 | Tail3]) :-
	merge_nodes(List1, Tail2, Tail3).


% Accessor for the f cost.
get_f_score(F/_/_/_/_, F).


% Best First Search (naive version).

bfs([], _) :-
	!,
	format('Search exhausted with no goal found!\n').

bfs([F-Node1 | Tail], _) :-
	goal(Node1),
	!,
	format('Goal found with cost: ~w\n~w\n', [F, Node1]),
	format('Tail is:\n~w\n', [Tail]).

bfs([_-_/_/_/C/R | Tail], Closed) :-
	member(C/R, Closed),
	!,
	%% format('~w seen already - ignored.\n', [C/R]),
	bfs(Tail, Closed).

bfs([_-Node1 | TailIn], Closed1) :-
	Node1 = _/_/_/C/R,
	findall(Node2, s(Node1, Node2, _), Successors),
	map_list_to_pairs(get_f_score, Successors, Pairs),
	keysort(Pairs, SortedSuccessors), 
	format('~w -> ~w\n', [Node1, SortedSuccessors]),
	merge_nodes(SortedSuccessors, TailIn, TailOut),
	!,
	bfs(TailOut, [C/R | Closed1]).

% Depth first limited.

depthfirst2(Node, Solution, MaxDepth) :-
	depthfirst2([], Node, Solution, MaxDepth).

depthfirst2(Path, Node, [Node | Path], _) :-
	goal(Node).

depthfirst2(Path, Node, Solution, MaxDepth) :-
	MaxDepth > 0,
	s(Node, Node1, _),
	Node1 = _/_/_/C/R,
	not(member(_/_/_/C/R, Path)),
	Max1 is MaxDepth - 1,
	depthfirst2([Node | Path], Node1, Solution, Max1).



/*
	State: F/G/L/C/R
	F score
	G cost
	L last move (direction)
	C column
	R row

	OPEN = list of state, sorted by F

	Search to start at 1,1 and go to 13,13. Minimum total (excepting the start position).

	Rules about number of steps allowed before turn, and directions to move.
	Start 1,1
	Moves from (n, e, s, w)
	Net offset (+12,+12).
	Legal changes of direction: n <-> e <-> s <-> w <-> n
	No streaks of more than 3 in same direction.
	Never move off grid (1,1)-(13,13).

	For minimum total cost, no cycles!

	State:

	(Col,Row), LastMove, [Moves,...], GCost, HCost, FCost
*/


part1(FileName) :-
	writeln("Advent of Code 2023, Day 17, Part 1:"),
	read_input_data(FileName),
	h(1/1, H),
	Start = H/0/x/1/1,
	bfs([H-Start], []).

part2(FileName) :-
	writeln("Advent of Code 2024, Day NN, Part 2:"),
	read_input_data(FileName),
/*	[X,M,A,S] ins 1..4000,
	build_tree("in", [X,M,A,S], Tree),
	tree_combinations(Tree, "A", TotalCount),
*/	format('Total number of passing combinations = ~w\n', [0]).

%
% Advent of Code 2023 - Day 19, Aplenty
% 

:- use_module(library(clpfd)).
:- working_directory(_, 'c:/users/stephen.mccoy/github/aoc2023/pl/day19/').

:- dynamic partdef/4, ruledef/3.

% DCG for input file.

digit(D) --> [D],
	{	code_type(D, digit)
	}.

digits([D | Tail]) --> digit(D), !, digits(Tail).
digits([]) --> [].

integer(F) --> digits(Digits),
	{	number_codes(F, Digits)
	}.

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

rule_spec([Name, ClauseList, Default]) --> 
	vname(Name), "{", clause_list(ClauseList), vname(Default), "}".

parts_rating([X,M,A,S]) -->
	"{x=", integer(X), ",m=", integer(M), ",a=", integer(A), ",s=", integer(S), "}".

input_line --> rule_spec([Name, ClauseList, Default]),
	{ assertz(ruledef(Name, ClauseList, Default)) }.

input_line --> parts_rating([X, M, A, S]),
	{ assertz(partdef(X, M, A, S)) }.

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

read_input_line(Stream) :- 
	read_line_to_string(Stream, String),
	( String = ""
	  ;
      ( string_codes(String, Chars),
        phrase(input_line, Chars, [])
      )
    ).

read_input_lines(Stream) :-
	read_input_line(Stream),
	!,
	read_input_lines(Stream).
read_input_lines(_).

read_input_data(FileName) :-
	retractall(ruledef(_,_,_)),
	retractall(partdef(_,_,_,_)),
	open(FileName, read, Stream),
	read_input_lines(Stream),
	close(Stream).

% Solve the challenge.

clause_accepted(["x", Op, Value, Result], [X, _, _, _], Result) :-
	( Op = lt, X #< Value ; Op = gt, X #> Value ).
clause_accepted(["m", Op, Value, Result], [_, M, _, _], Result) :-
	( Op = lt, M #< Value ; Op = gt, M #> Value ).
clause_accepted(["a", Op, Value, Result], [_, _, A, _], Result) :-
	( Op = lt, A #< Value ; Op = gt, A #> Value ).
clause_accepted(["s", Op, Value, Result], [_, _, _, S], Result) :-
	( Op = lt, S #< Value ; Op = gt, S #> Value ).

project_from("A", _, "A").
project_from("R", _, "R").
project_from(Source, Scores, Dest) :-
	ruledef(Source, ClauseList, _),
	member(Clause, ClauseList),
	clause_accepted(Clause, Scores, D1),
	format('Accepted ~w -> ~w\n', [Clause, D1]),
	!,
	project_from(D1, Scores, Dest).
project_from(Source, Scores, Dest) :-
	ruledef(Source, _, Default),
	format('Default ~w -> ~w\n', [Source, Default]),
	project_from(Default, Scores, Dest).

part_accepted([X,M,A,S]) :-
	project_from("in", [X,M,A,S], "A").

% Part 2. Build tree from rule definitions.

less_than_split(X, V, Then, Else) :-
	fd_set(X, S1),
	Then in_set S1,
	Then #< V,
	fd_set(Then, S2),
	fdset_subtract(S1, S2, S3),
	Else in_set S3.

greater_than_split(X, V, Then, Else) :-
	fd_set(X, S1),
	Then in_set S1,
	Then #> V,
	fd_set(Then, S2),
	fdset_subtract(S1, S2, S3),
	Else in_set S3.
	
branch(["x", lt, Value, Result], [X,M,A,S], [XT,M,A,S], [XF,M,A,S], Result) :-
	less_than_split(X, Value, XT, XF).
branch(["m", lt, Value, Result], [X,M,A,S], [X,MT,A,S], [X,MF,A,S], Result) :-
	less_than_split(M, Value, MT, MF).
branch(["a", lt, Value, Result], [X,M,A,S], [X,M,AT,S], [X,M,AF,S], Result) :-
	less_than_split(A, Value, AT, AF).
branch(["s", lt, Value, Result], [X,M,A,S], [X,M,A,ST], [X,M,A,SF], Result) :-
	less_than_split(S, Value, ST, SF).

branch(["x", gt, Value, Result], [X,M,A,S], [XT,M,A,S], [XF,M,A,S], Result) :-
	greater_than_split(X, Value, XT, XF).
branch(["m", gt, Value, Result], [X,M,A,S], [X,MT,A,S], [X,MF,A,S], Result) :-
	greater_than_split(M, Value, MT, MF).
branch(["a", gt, Value, Result], [X,M,A,S], [X,M,AT,S], [X,M,AF,S], Result) :-
	greater_than_split(A, Value, AT, AF).
branch(["s", gt, Value, Result], [X,M,A,S], [X,M,A,ST], [X,M,A,SF], Result) :-
	greater_than_split(S, Value, ST, SF).

% reduce_domain(InScores, ClauseList, Default, ResultList)
reduce_domain(Scores, [], Default, [Default/Scores]).
reduce_domain(Score1, [Clause1 | Tail], Default, [Dest1/ScoreTrue | TailResult]) :-
	branch(Clause1, Score1, ScoreTrue, ScoreFalse, Dest1),
	reduce_domain(ScoreFalse, Tail, Default, TailResult).

% Build a rule tree containing CLP bound variables for scores.
build_tree(Source, Scores, Source/Results) :-
	ruledef(Source, ClauseList, Default),
	!,
	reduce_domain(Scores, ClauseList, Default, DomainResults),
	findall(SubTree1, (
		member(Loc1/Score1, DomainResults),
		build_tree(Loc1, Score1, SubTree1)
	), Results).
build_tree("A", Scores, "A"/Scores).
build_tree("R", Scores, "R"/Scores).

% Count the number of combinations represented by a single leaf node.
count_combinations([X,M,A,S], Count) :-
	fd_size(X, XS),
	fd_size(M, MS),
	fd_size(A, AS),
	fd_size(S, SS),
	Count is XS * MS * AS * SS.

% Find total number of combinations having the specified Value.
tree_combinations(Value/Scores, Value, Count) :-
	format("Counted Leaf ~w\n", [Value/Scores]),
	count_combinations(Scores, Count), 
	!.
tree_combinations(OtherValue/Scores, Value, 0) :-
	member(OtherValue, ["A","R"]),
	Value \== OtherValue,
	format("Not Counted Leaf ~w\n", [OtherValue/Scores]),
	!.
tree_combinations(_/Results, Value, Count) :-
	findall(SubTreeCount, (
		format("Subtree ~w\n", [Results]),
		member(Result1, Results),
		tree_combinations(Result1, Value, SubTreeCount)
	), CountList),
	sum_list(CountList, Count).

day19_part1(FileName) :-
	writeln("Advent of Code 2023, Day 19, Part 1:"),
	read_input_data(FileName),
	findall(PS, (partdef(X,M,A,S), part_accepted([X,M,A,S]), sum_list([X,M,A,S], PS)), PL),
	sum_list(PL, Total),
	format('Total part rating for accepted parts = ~w:\n', [Total]).

day19_part2(FileName) :-
	writeln("Advent of Code 2023, Day 19, Part 2:"),
	read_input_data(FileName),
	[X,M,A,S] ins 1..4000,
	build_tree("in", [X,M,A,S], Tree),
	tree_combinations(Tree, "A", TotalCount),
	format('Total number of passing combinations = ~w\n', [TotalCount]).

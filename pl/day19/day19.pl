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

project_from("A", _, "A") :- !.
project_from("R", _, "R") :- !.
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

day19_part1(FileName) :-
	read_input_data(FileName),
	findall(PS, (partdef(X,M,A,S), part_accepted([X,M,A,S]), sum_list([X,M,A,S], PS)), PL),
	sum_list(PL, Total),
	format('Total part rating for accepted parts = ~w:\n', [Total]).

day19_part2(FileName, [X,M,A,S]) :-
	read_input_data(FileName),
	[X,M,A,S] ins 1..4000,
	part_accepted([X,M,A,S]).



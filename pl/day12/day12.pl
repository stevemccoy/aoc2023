%
% Advent of Code 2023 - Day 12, Hot Springs
% 

:- use_module(library(clpfd)).
:- use_module(library(thread)).

:- working_directory(_, 'c:/users/stephen.mccoy/github/aoc2023/pl/day12/').

:- dynamic spring_line/3.
:- dynamic solution_count/2.

% List utilities.

% List concatenation.
conc([], L, L).
conc([H | Tail], L1, [H | L2]) :-
	conc(Tail, L1, L2).

% S is a sub-sequence of L, from index From.
sublist(S, L, From) :-
	S = [_|_],					% S not empty.
	conc(Before, L2, L),
	conc(S, _, L2),
	length(Before, From).

% repeat_value(V, N, List) 
repeat_value(_, 0, []).
repeat_value(V, N, [V | Tail]) :-
	N #> 0,
	M #= N - 1,
	repeat_value(V, M, Tail).

% DCG for input file.

input_line(N, Condition, Groups) --> condition(Condition), " ", groups(Groups),
	{ assertz(spring_line(N, Condition, Groups)) }.

condition(Condition) --> status_list(Condition).

groups(Groups) --> int_list(Groups).

status_list([S1 | Tail]) --> status(S1), !, status_list(Tail).
status_list([]) --> [].

status(0) --> ".".
status(1) --> "#".
status(X) --> "?", { X in 0..1 }.

int_list([Head | Tail]) --> integer(Head), ",", int_list(Tail).
int_list([Head]) --> integer(Head).
int_list([]) --> [].

digit(D) --> [D],
	{	code_type(D, digit)
	}.

digits([D | Tail]) --> digit(D), !, digits(Tail).
digits([]) --> [].

integer(F) --> digits(Digits),
	{	number_codes(F, Digits)
	}.


% DCG for condition sequence.

cond_sequence([BC | GroupTail], [ZC1 | SolutionTail]) -->
	opt_zeros(ZC1), block(BC), continuation(GroupTail, SolutionTail), opt_zeros(_).

continuation([BC | GroupTail], [ZC2 | SolutionTail]) -->
	separator, opt_zeros(ZC1), block(BC), continuation(GroupTail, SolutionTail),
	{	ZC2 #= ZC1 + 1
	}.
continuation([], []) --> [].

separator --> [0].

opt_zeros(N) --> [0], opt_zeros(M), { N #= M + 1 }.
opt_zeros(0) --> [].

block(N) --> [1], block(M), { N #= M + 1 }.
block(1) --> [1].


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

read_input_line(Stream, M) :- 
	read_line_to_string(Stream, String),
	( String = ""
	  ;
      ( string_codes(String, Chars),
        phrase(input_line(M, _, _), Chars, [])
      )
    ).

read_input_lines(Stream, M) :-
	read_input_line(Stream, M),
	!,
	N #= M + 1,
	read_input_lines(Stream, N).
read_input_lines(_, _).

read_input_data(FileName) :-
	retractall(spring_line(_,_,_)),
	open(FileName, read, Stream),
	read_input_lines(Stream, 1),
	close(Stream).

% Parse single line String using DCG.
process_string(String, C1, G1) :-
	string_codes(String, Chars1),
	phrase(condition(C1), Chars1, [_ | Chars2]),
	phrase(groups(G1), Chars2, []).

% Find matching solution given the conditions in Template and Groups information.
solution(Template, Groups, Solution, Results) :-
	% Reduce a list to match with Template.
	length(Template, N),
	length(Solution, N),
	Solution ins 0..1,
	Solution = Template,
	!,
	phrase(cond_sequence(Groups, Results), Solution, []).


% Replace unbound entries in Template with x.
translate_template([], []).
translate_template([H | Tail], [H | TTail]) :-
	ground(H), 
	!,
	translate_template(Tail, TTail).
translate_template([_ | Tail], [x | TTail]) :-
	translate_template(Tail, TTail).


% Make a copy of a given condition list, creating new variables for any
% ungrounded entries.
copy_list([], []).
copy_list([H | Tail1], [H | Tail2]) :-
	ground(H),
	!,
	copy_list(Tail1, Tail2).
copy_list([_ | Tail1], [H2 | Tail2]) :-
	H2 in 0..1,
	copy_list(Tail1, Tail2).

% Expand either a conditions list or a groups list a given number of times.
expand_list(L1, 1, _, L2) :-
	copy_list(L1, L2).
expand_list(L1, N, "?", Multiple) :-
	N #> 1,
	M #= N - 1,
	expand_list(L1, M, "?", Tail),
	copy_list(L1, L2),
	X in 0..1,
	conc(L2, [X | Tail], Multiple).
expand_list(L1, N, ",", Multiple) :-
	N #> 1,
	M #= N - 1,
	expand_list(L1, M, ",", Tail),
	copy_list(L1, L2),
	conc(L2, Tail, Multiple).

% For Part 2, expand the conditions and groups lists by 5 multiples.
expand_lists(CL1, GL1, CL2, GL2) :-
	expand_list(CL1, 5, "?", CL2),
	expand_list(GL1, 5, ",", GL2).


% find_solution(Condition, Groups, Result).

ordered([GC1], NB, [GS1]) :-
	NB #>= GS1 + GC1.
ordered([GC1, GC2 | GTail], NB, [GS1, GS2 | GSTail]) :-
	GS2 #> GS1 + GC1,
	ordered([GC2 | GTail], NB, [GS2 | GSTail]).

block_template(N, T) :-
	repeat_value(1, N, Ones),
	conc([0 | Ones], [0], T).

% Condition must be padded front and back with 0.
match_condition(Condition, [GC1], [GS1]) :-
	block_template(GC1, Block),
	!,
	sublist(Block, Condition, From),
	GS1 #= From + 1.
match_condition(Condition, [GC1, GC2 | GCTail], [GS1, GS2 | GSTail]) :-
	block_template(GC1, Block),
	!,
	sublist(Block, Condition, From),
	GS1 #= From + 1,
	GS2 #> GS1 + GC1,
	match_condition(Condition, [GC2 | GCTail], [GS2 | GSTail]).


% Difference list append.
dapp(A-B,B-C,A-C).

% Want to rewrite the matching and counting function in terms of difference list operations on
% the conditions list, reducing the scope as groups are matched or consumed
% (e.g. in ignoring 0 values).

dblock(N, Block) :-
	repeat_value(1, N, BL1),
	dapp(BL1-[], [0]-[], Block).

matchable_sequence([], 0, []).
matchable_sequence([H | Rest], 0, Rest) :-
	!,
	H \== 1.
matchable_sequence([H | Tail], N, Rest) :-
	H \== 0,
	M #= N - 1,
	matchable_sequence(Tail, M, Rest).

% match_group(Condition-Tail, GC, PathCount)
match_group([], [N | GTail], 0).
match_group([0 | Tail], [N | GTail], PC) :-
	!,
	match_group(Tail, N, PC).
match_group([1 | Tail], N, PC) :-
	matchable_sequence([1 | Tail], N, Rest),
	match_group(Rest, )



count_faults([], 0).
count_faults([H | Tail], N) :-
	ground(H),
	H = 1,
	!,
	count_faults(Tail, M),
	N #= M + 1.
count_faults([_ | Tail], N) :-
	count_faults(Tail, N).

find_solution(Condition, Groups, Results) :-
	length(Condition, NB),
	length(Groups, NG),
	sum_list(Groups, FaultCount),
	length(Results, NG),
	Results ins 1..NB,
	conc([0 | Condition], [0], PaddedCondition),
	!,
	match_condition(PaddedCondition, Groups, Results),
	count_faults(PaddedCondition, FaultCount).


% Solve the challenge.

day12_part1(FileName) :-
	writeln("Advent of Code 2023, Day 12, Part 1:"),
	read_input_data(FileName),
	findall(NS1, (
		spring_line(_, Condition, Groups),
		findall(Result, (find_solution(Condition, Groups, Result)), SL1),
%		findall(Solution, (solution(Condition, Groups, Solution, _)), SL2),
		length(SL1, NS1)
	), CountList),
	writeln(CountList),
	sum_list(CountList, Total),
	format('Total count of combinations = ~w\n', [Total]).

record_solutions :-
	concurrent_forall(
		(	spring_line(LN, Condition1, Groups1),
			expand_lists(Condition1, Groups1, Condition, Groups),
			findall(1, (find_solution(Condition, Groups, _)), SL1),
			length(SL1, NS)
		),
		(	assertz(solution_count(LN, NS)),
			writeln(LN)
		),
		[threads(8)]
	).

day12_part2(FileName) :-
	writeln('Advent of Code 2023, Day 12, Part 2:'),
	read_input_data(FileName),
	findall(NS1, (
		spring_line(_, C1, G1),
		expand_lists(C1, G1, C2, G2),
		findall(1, (find_solution(C2, G2, _)), SL1),
		length(SL1, NS1)
	), CountList),
	writeln(CountList),
	sum_list(CountList, Total),
	format('Total count of combinations = ~w\n', [Total]).

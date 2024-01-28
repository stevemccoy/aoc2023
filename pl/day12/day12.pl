%
% Advent of Code 2023 - Day 12, Hot Springs
% 

:- use_module(library(clpfd)).
:- working_directory(_, 'c:/users/stephen.mccoy/github/aoc2023/pl/day12/').

:- dynamic spring_line/2.

% DCG for input file.

input_line(Condition, Groups) --> condition(Condition), " ", groups(Groups),
	{ assertz(spring_line(Condition, Groups)) }.

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

cond_sequence(List) --> opt_zeros(ZC1), block(BC), continuation(CL), opt_zeros(ZC2),
	{	conc([ZC1, BC | CL], [ZC2], List)
	}.

continuation([ZC2, BC | CL]) --> separator, opt_zeros(ZC1), block(BC), continuation(CL),
	{	ZC2 #= ZC1 + 1
	}.
continuation([]) --> [].

separator --> [0].

opt_zeros(N) --> [0], opt_zeros(M), { N #= M + 1 }.
opt_zeros(0) --> [].

block(N) --> [1], block(M), { N #= M + 1 }.
block(1) --> [1].

% DCG for sequence. ??

%% matching_cond_sequence(Groups) --> opt_zeros(_), block(BC), separator.


% List utilities.

% List concatenation.
conc([], L, L).
conc([H | Tail], L1, [H | L2]) :-
	conc(Tail, L1, L2).

% S is a sub-sequence of L, from index From to index To inclusive.
sublist(S, L, From, To) :-
	S = [_|_],					% S not empty.
	conc(Before, L2, L),
	conc(S, _, L2),
	length(Before, From),
	length(S, SL),
	To #= From + SL - 1.

% repeat_value(V, N, List) 
repeat_value(_, 0, []).
repeat_value(V, N, [V | Tail]) :-
	N #> 0,
	M #= N - 1,
	repeat_value(V, M, Tail).

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
        phrase(input_line(_, _), Chars, [])
      )
    ).

read_input_lines(Stream) :-
	read_input_line(Stream),
	!,
	read_input_lines(Stream).
read_input_lines(_).

read_input_data(FileName) :-
	retractall(spring_line(_,_)),
	open(FileName, read, Stream),
	read_input_lines(Stream),
	close(Stream).


% Find matching solution given the conditions in Template and Groups information.
solution(Template, Groups, Solution) :-
	% Reduce a list to match with Template.
	length(Template, N),
	length(Solution, N),
	Solution ins 0..1,
	Solution = Template,
	% Now, make a matching template for the group counts (including spaces for the zeros).
	findall([C, _], (member(C, Groups)), CL1),
	flatten(CL1, CL2),
	GroupCounts = [_ | CL2],
	!,
	phrase(cond_sequence(GroupCounts), Solution, []).


%% reduce_solution([0 | Tail], Groups, Count) :-
%% 	reduce_solution(Tail, Count)

% Replace unbound entries in Template with x.
translate_template([], []).
translate_template([H | Tail], [H | TTail]) :-
	ground(H), 
	!,
	translate_template(Tail, TTail).
translate_template([_ | Tail], [x | TTail]) :-
	translate_template(Tail, TTail).


%% count_solutions(Template, Groups, Count) :-
%% 	translate_template(Template, Conditions),


munch_zeros([], []).
munch_zeros([0 | Tail], MTail) :-
	!,
	munch_zeros(Tail, MTail).
munch_zeros(L, L).

%% match_group([], Group) :-
%% 	repeat_value(1, Group, L1),





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

% Solve the challenge.

day12_part1(FileName) :-
	writeln("Advent of Code 2023, Day 12, Part 1:"),
	read_input_data(FileName),
	findall(NS, (
		spring_line(Condition, Groups),
		findall(Solution, (solution(Condition, Groups, Solution)), SL1),
		length(SL1, NS)
	), CountList),
	writeln(CountList),
	sum_list(CountList, Total),
	format('Total count of combinations = ~w\n', [Total]).

day12_part2(FileName) :-
	writeln("Advent of Code 2023, Day 12, Part 2:"),
	read_input_data(FileName),
	findall(NS, (
		spring_line(Condition1, Groups1),
		expand_lists(Condition1, Groups1, Condition, Groups),
		findall(Solution, (solution(Condition, Groups, Solution)), SL1),
		length(SL1, NS)
	), CountList),
	writeln(CountList),
	sum_list(CountList, Total),
	format('Total count of combinations = ~w\n', [Total]).

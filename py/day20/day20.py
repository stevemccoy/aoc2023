import fileinput
import re

# Global variables.
button_count = 0
pulse_counts = dict(zero=0, one=0)
quitting_time = False

def read_input_lines(filename):
	lines = []
	pattern = r'^(?P<ty>[&%])?(?P<lhs>.+) -> (?P<rhs>.+)$'
	with fileinput.FileInput(files = (filename), mode = 'r') as input:
		for raw_line in input:
			line = raw_line.strip(' \n')
			m = re.match(pattern, line)
			if m:
				d = m.groupdict()
				d['rhs'] = re.split(r'\W+', d['rhs'])
				lines.append(d)
	return lines

def initialise_machine(instructions):
	machine = {}
	# Decode the instructions first.
	for d in instructions:
		mname = d['lhs']
		mtype = d['ty']
		if not mtype:
			mtype = 'b'
		moutputs = d['rhs']
		machine[mname] = dict(name=mname, type=mtype, state=0, outputs=moutputs, inputs={})
	# Add any modules mentioned in outputs but not declared.
	unknowns = []
	for name in machine.keys():
		for out in machine[name]['outputs']:
			if out not in machine:
				unknowns.append(out)
	for out in unknowns:
		machine[out] = dict(name=out, type='-', state=0, outputs=[], inputs={})
	# Compile input lists for each module.
	for name in machine.keys():
		for out in machine[name]['outputs']:
			machine[out]['inputs'][name] = 0
	return machine

def machine_state(machine):
	return [m['state'] for m in machine.values()]

def push_button(pulse_train):
	global button_count
	button_count += 1
	pulse_train.append(('button', 'broadcaster', 0))

def reset_counts():
	global button_count
	global pulse_counts
	global quitting_time
	button_count = 0
	pulse_counts = dict(zero=0, one=0)
	quitting_time = False

def grab_pulse(pulse_train):
	global pulse_counts
	(src, dest, pulse) = pulse_train.pop()
	if pulse:
		pulse_counts['one'] += 1
	else:
		pulse_counts['zero'] += 1
	return (src, dest, pulse)

def process_pulse(machine, pulse_train):
	global quitting_time
	if len(pulse_train) == 0:
		return False
	(src, dest, pulse) = grab_pulse(pulse_train)
	if dest not in machine:
		print("ERROR: Unknown module", dest, "quitting...")
		exit(1)
	if dest == 'rx' and pulse == 0:
		quitting_time = True
		return True
	m = machine[dest]
	t = m['type']
	if t == 'b':
		# Broadcast.
		for out in m['outputs']:
			pulse_train.append((dest, out, pulse))
		pass
	elif t == '%':
		# Flip-flop.
		if pulse == 0:
			if m['state'] == 1:
				m['state'] = 0
				for out in m['outputs']:
					pulse_train.append((dest, out, 0))
			else:
				m['state'] = 1
				for out in m['outputs']:
					pulse_train.append((dest, out, 1))
		else:
			# Ignores high pulse.
			pass
	elif t == '&':
		# Conjunction.
		m['inputs'][src] = pulse
		outpulse = 0
		for v in m['inputs'].values():
			if v == 0:
				outpulse = 1
		for out in m['outputs']:
			pulse_train.append((dest, out, outpulse))
	elif t == '-':
		# Other (output?) unit.
		pass
	else:
		print("ERROR - Unrecognised module type", t, "quitting...")
		exit(1)
	machine[dest] = m
	return True

def process_button_push(machine):
	pulse_train = []
	push_button(pulse_train)
	while process_pulse(machine, pulse_train):
		if quitting_time:
			return True
	return False

def part1(filename):
	instructions = read_input_lines(filename)
	machine = initialise_machine(instructions)
	reset_counts();
	for i in range(1000):
		process_button_push(machine)
	# Return value.
	return pulse_counts['one'] * pulse_counts['zero']

def part2(filename):
	instructions = read_input_lines(filename)
	machine = initialise_machine(instructions)
	reset_counts()
	while not process_button_push(machine):
		pass

		# if button_count % 1000 == 0:
		# 	print(machine_state(machine))
	return button_count

print('Advent of Code 2023\nDay 20: Pulse Propagation')
print('Part 1.')
count = part1('input20.txt')
print(f"Result = {count}\n")

print('Part 2.')
count = part2('input20.txt')
print(f"Result = {count}\n")

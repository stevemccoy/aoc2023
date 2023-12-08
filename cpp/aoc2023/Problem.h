#pragma once

using namespace std;

#include <string>

class Problem
{
public:
	Problem() {}
	virtual ~Problem() {}

	string title() const { return m_title; }
	string description() const { return m_description; }

	// Run the problem.
	virtual void execute() = 0;
	virtual void part1(const char* fileName) = 0;
	virtual void part2(const char* fileName) = 0;

protected:
	void init(string t, string d) { m_title = t; m_description = d; }

private:
	string m_title;
	string m_description;
};

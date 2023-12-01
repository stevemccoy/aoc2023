#pragma once

using namespace std;

#include <string>

class Problem
{
public:
	Problem(string t, string d) : m_title(t), m_description(d) {}
	virtual ~Problem() {}

	string title() const { return m_title; }
	string description() const { return m_description; }

	// Run the problem.
	virtual void execute() = 0;

private:
	string m_title;
	string m_description;
};


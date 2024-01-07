#pragma once
#include "File.cpp"
class Compare {
public:
	bool operator()(File* a, File* b);
};


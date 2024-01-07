#include "Compare.h"
#include "File.cpp"

bool Compare::operator()(File* a, File* b) {
	return a->getPriority() > b->getPriority();
}
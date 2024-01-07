#pragma once
#include "CommonElement.cpp"
#include <iostream>

class File : public CommonElement{
public:
	File(string name, int characters, int lines, string owner, int priority, time_t time, string exe, string type, string val) :
		CommonElement(name, characters, lines, owner, priority, time, exe, type, val) { }

	File() : CommonElement(name, "file") {}

	File(string name) : CommonElement(name, "file" ,".txt") {}

	File(File& obj) {
		setName(obj.getName());
		setCharacters(obj.getCharacters());
		setLines(obj.getLines());
		setOwner(obj.getOwner());
		setReadOnly();
		setPriority(obj.getPriority());
		setPrintingTime(obj.getPrintingTime());
		setExtension(obj.getExtension());
		setType(obj.getType());
		setData(obj.getData());
	}
};
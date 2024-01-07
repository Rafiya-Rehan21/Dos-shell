#pragma once
#include<iostream>
#include<vector>
#include<ctime>
#include <iomanip>
using namespace std;

class CommonElement{
protected:
	string name;
	string path;
	int characters;
	int lines;
	char dateTime[0x30];
	string owner;
	bool readOnly;
	int priority;
	time_t printingTime;
	string extension;
	double size;
	string type;
	string data;

	CommonElement* parentDir;;

public:
	CommonElement(string name,int characters,int lines,string owner,int priority, time_t time,string exe,string type, string val) {
		setName(name);
		setCharacters(characters);
		setLines(lines);
		setOwner(owner);
		setReadOnly();             
		setPriority(priority);
		setPrintingTime(time);
		setExtension(exe);
		setType(type);
		setData(val);
	}

	CommonElement(string name, string type) {
		setName(name);
		setType(type);
	}

	CommonElement(string type) {
		setType(type);
	}

	CommonElement(string name, string type, string exe) {
		setName(name);
		setType(type);
		setExtension(exe);
	}

	CommonElement(){}

	void setName(string name) {
		this->name = name;
	}

	string getName() {
		return name;
	}

	string getPath() {
		if (parentDir == nullptr) {
			return "";
		}
		return parentDir->getPath() + "\\" + this->name;
	}

	void setCharacters(int characters) {
		this->characters = characters;
	}

	int getCharacters() {
		return characters;
	}

	void setLines(int lines) {
		this->lines = lines;
	}

	int getLines() {
		return lines;
	}

	int getAvgCharacters() {
		return getCharacters() / getLines();
	}
	
	char* getDateTime() {
		time_t now = time(0);
		ctime_s(dateTime, sizeof(dateTime), &now);

		return dateTime;
	}

	void setOwner(string owner) {
		this->owner = owner;
	}

	string getOwner() {
		return owner;
	}

	void setReadOnly() {
		this->readOnly = true;
	}

	bool getReadOnly() {
		return readOnly;
	}

	void setPriority(int n) {
		this->priority = n;
	}

	int getPriority() {
		return priority;
	}

	void setPrintingTime(time_t time) {
		this->printingTime = time;
	}

	time_t getPrintingTime() {
		return printingTime;
	}

	double getSize() {
		const int bytesPerCharacter = 1;

		double fileSizeInBytes = getCharacters() * bytesPerCharacter;

		size = (fileSizeInBytes) / (1024.0 * 1024.0);

		std::cout << std::fixed << std::setprecision(2);

		if (size < 1.0) {
			double sizeInKB = fileSizeInBytes / 1024.0;
			return size = sizeInKB;
		}

		return size;
	}

	void setType(string type) {
		this->type = type;
	}

	string getType() {
		return type;
	}


	void setExtension(string exe) {
		if (exe == ".text" || exe == ".txt") {
			this->extension = exe;
		}
		else {
			return;
		}
	}

	string getExtension() {
		return extension;
	}

	void setData(string value) {
		this->data = value;
		setCharacters(value.size());
		setLines(count(value.begin(), value.end(), '\n') + 1);
	}

	string getData() {
		return data;
	}

	void setParentDir(CommonElement* parent) {
		parentDir = parent;
	}

	CommonElement* getParentDir() {
		return parentDir;
	}

	bool isDirectory() {
		return type == "dir";
	}
};
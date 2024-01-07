#include "Keyboard/Keyboard.h"
#include"Directory.cpp"
#include"CommonElement.cpp"
#include "Printing.cpp"
#include"Editor.h"
#include "File.cpp"
#include<iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <fstream>
#pragma comment(lib, "user32.lib")
using namespace std;

#define SECOND_ARG(cmd, line) line.substr(cmd.length() + 1)

void dir(Directory*);
Directory* cd(Directory*, string);
Directory* parentDir(Directory* current);
Directory* currentDir(Directory* current);
Directory* rootDirectory(Directory* current);
void attrib(Directory* cwd, string name);
void deleteFile(Directory*, string);
void mkdir(Directory* cwd, string name);
void ver();
void rmdir(Directory* cwd, string name);
void pwd(Directory*);
void help();
void rename(Directory*, string, string);
void tree(Directory* cwd, int level = 0);
void move(Directory*, string, string);
void copy(Directory* cwd, string filename, string newLocation);
void find(Directory* cwd, string fileName);
void create(Directory* cwd, string name);
string changePrompt(string pmt);
void edit(Directory* cwd, File*);
void convert(Directory* cwd, string exe);
void fileAttributesInput(File* current);
bool findF(Directory* current, string name, string str);
void findStr(Directory* current, string str);
void print(Directory* cwd, string name);
void pPrint(Directory* cwd, string name);
void save(Directory*, File*);
void savetree(Directory*);
void loadtree(Directory*, string, ifstream* inp = nullptr);
void displaySize(double size);
void start();

Printing printQueues;

vector<string> split(const string& str, const string& delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos) pos = str.length();
		string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

int main() {

	start();

	Directory rootDir;
	File* currentFile = nullptr;

	Directory* cwd = &rootDir;
	string cmd = "", line = "", rootPath = "V:\\", prompt = "> ";

	while (cmd != "exit") {
		cout << rootPath << cwd->getPath() << prompt << ' ';
		getline(cin >> ws, line);

		vector<string> args = split(line, " ");
		cmd = args[0];

		printQueues.updateQueues();

		if (cmd == "dir") {
			dir(cwd);
		}
		else if (cmd == "cd") {
			cwd = cd(cwd, args[1]);
		}
		else if (cmd == "exit") {
			cout << "Exiting" << endl;
		}
		else if (cmd == "cd..") {
			cwd = parentDir(cwd);
		}
		else if (cmd == "cd.") {
			cwd = currentDir(cwd);
		}
		else if (cmd == "cd\\") {
			cwd = rootDirectory(cwd);
		}
		else if (cmd == "attrib") {
			attrib(cwd, args[1]);
		}
		else if (cmd == "del") {
			deleteFile(cwd, args[1]);
		}
		else if (cmd == "mkdir") {
			mkdir(cwd, args[1]);
		}
		else if (cmd == "ver") {
			ver();
		}
		else if (cmd == "rmdir") {
			rmdir(cwd, args[1]);
		}
		else if (cmd == "pwd") {
			pwd(cwd);
		}
		else if (cmd == "rename") {
			rename(cwd, args[1], args[2]);
		}
		else if (cmd == "help") {
			help();
		}
		else if (cmd == "tree") {
			tree(cwd);
		}
		else if (cmd == "move") {
			move(cwd, args[1], args[2]);
		}
		else if (cmd == "copy") {
			copy(cwd, args[1], args[2]);
		}
		else if (cmd == "format") {
			cwd->format();
		}
		else if (cmd == "find") {
			find(cwd, args[1]);
		}
		else if (cmd == "create") {
			create(cwd, args[1]);
		}
		else if (cmd == "edit") {
			currentFile = cwd->getFile(args[1]);
			edit(cwd, currentFile);
		}
		else if (cmd == "prompt") {
			prompt=changePrompt(args[1]);
		}
		else if (cmd == "convert") {
			convert(cwd, args[1]);
		}
		else if (cmd == "findf") {
			bool find=findF(cwd, args[1], args[2]);

			if (find) {
				cout << "String Found" << endl;
			}
		}
		else if (cmd == "findstr") {
			findStr(cwd, args[1]);
		}
		else if (cmd == "print") {
			print(cwd, args[1]);
		}
		else if (cmd == "pprint") {
			pPrint(cwd, args[1]);
		}
		else if (cmd == "queue") {
			printQueues.printQueue();
		}
		else if (cmd == "pqueue") {
			printQueues.printPriorityQueue();
		}
		else if (cmd == "save") {
			save(cwd, currentFile);
		}
		else if (cmd == "savetree") {
			savetree(cwd);
		}
		else if (cmd == "loadtree") {
			loadtree(cwd, args[1]);
		}
		else {
			cout << "Invalid command: " << line << endl;
		}
	}
}

void dir(Directory* cwd) {
	for (CommonElement* elem : cwd->getElements()) {
		if (elem->isDirectory()) {
			cout << "DIR" << "\t\t" << elem->getName() << "\t\t\t" << elem->getDateTime();
		}
		else {
			cout << "FILE" << "\t\t" << elem->getName() + elem->getExtension() << "\t\t" << elem->getDateTime();
		}

		cout << endl;
	}
}

Directory* cd(Directory* cwd, string newDir) {
	if (newDir == "..") {
		CommonElement* elem = cwd->getParentDir();

		if (elem != nullptr) {
			return (Directory*) elem;
		}
		else {
			return cwd;
		}
	}

	for (CommonElement* elem : cwd->getElements()) {
		if (elem->isDirectory() && elem->getName() == newDir)
			return (Directory *)elem;
	}

	return cwd;
}

Directory* parentDir(Directory* current) {
	if (current->getParentDir() != nullptr) {
		return (Directory*)current->getParentDir();
	}
	return (Directory*)current;
}

Directory* currentDir(Directory* current) {
	return (Directory*)current;
}

Directory* rootDirectory(Directory* current) {
	while (current->getParentDir() != nullptr) {
		current = (Directory *) current->getParentDir();
	}

	return current;
}

void attrib(Directory* cwd, string name) {
	File* exists = cwd->getFile(name);
	if (exists == nullptr) {
		cout << "No such file exists" << endl;
		return;
	}

	cout << "Name:" << exists->getName() << endl;
	cout << "Characters: " << exists->getCharacters() << endl;
	cout << "Lines: " << exists->getLines() << endl;
	cout << "Owner: " << exists->getOwner() << endl;
	cout << "Read Only [0->No / 1->Yes]: " << exists->getReadOnly() << endl;
	cout << "Priority: " << exists->getPriority() << endl;
	cout << "Printing Time: " << exists->getPrintingTime() << endl;
	cout << "Extension: " << exists->getExtension() << endl;
	cout << "Size: " << exists->getSize(); displaySize(exists->getSize());cout << endl;
	cout << "Type: " << exists->getType() << endl;
	cout << "Contents: " << exists->getData() << endl;
}

void deleteFile(Directory* cwd, string path) {
	if (cwd->deleteFile(path)) {
		cout << path << " deleted successfully" << endl;
	}
	else {
		cout << "No file named " << path << endl;
	}
}

void mkdir(Directory* cwd, string name) {
	Directory* newDir = new Directory(name);
	cwd->addChildElement(newDir);
	cwd->getDateTime();

	cout << "Directory created successfully" << endl;
}

void ver() {
	cout << "Microsoft Windows [Version 10.0.22621.2715]" << endl;
}

void rmdir(Directory* cwd, string path) {
	if (cwd->deleteDir(path)) {
		cout << path << " deleted successfully" << endl;
	}
	else {
		cout << "No directory named " << path << endl;
	}
}

void pwd(Directory* cwd) {
	cout << cwd->getPath() << endl;
}

void help() {
	cout << R"V0G0N(ATTRIB		Displays file (name provided as input) attributes.
CD		Displays the name of or changes the current directory.
CD.		Prints working directory (i.e. your current directory in your tree structure)
CD..		Change directory to previous directory
CD\		Changes directory to root directory (i.e. V:\>;)
CONVERT		Asks two types and converts extension of all files of one type to another type.
COPY		Copies one file in the current directory to another location (directory).
CREATE		Creates a file with the name provided and allows the user to enter contents of the file.
DEL		Delete a file whose name is provided in input.
DIR		Displays a list of files and subdirectories in a directory.
EDIT		Opens a file (loads into a linked list) and allows the user to edit and save the contents of the file.
EXIT		Quits the program
FIND		Searches for a file in your current virtual directory whose name is provided as input.
FINDF		Searches for a text string in the currently open file or the file whose name is provided as input.
FINDSTR		Searches for strings in all files of your current virtual directory, prints names of files with the string
FORMAT		Formats the current virtual directory i.e. empties the current directory and all subdirectories.
HELP		Provides Help information for all these commands.
LOADTREE	Load a given tree in your tree structure. Given tree is in a file named tree.txt
MKDIR		Creates a virtual directory.
MOVE		Moves one file (whose name is provided as input) from one directory to another directory.
PPRINT		Adds a text file to the priority based print queue, and displays the current priority queue.
PROMPT		Changes the Windows command prompt (for example from V:\>; to V$).
PRINT		Adds a text file to the print queue, and displays the current queue.
PQUEUE		Shows current state of the priority based print queue, with time left for each element
PWD		Prints working directory
QUEUE		Shows current state of the print queue, with time left for each element
RENAME		Renames a file whose current and new name is provided as input.
RMDIR		Removes a directory along with its contents.
SAVE		Saves the currently open file to disk.
TREE		Displays the complete directory structure.
VER		Displays the version of your program.)V0G0N" << endl;
}

void rename(Directory* cwd, string currentName, string newName) {
	File* fl = cwd->getFile(currentName);

	if (fl != nullptr) {
		fl->setName(newName);
		cout << "File renamed successfully" << endl;
	}
	else {
		cout << "No file named " << currentName << endl;
	}
}

void tree(Directory* cwd, int level) {
	vector<CommonElement*> elements = cwd->getElements();
	for (auto it = elements.begin(); it != elements.end(); it++) {
		if ((*it)->isDirectory()) {
			for (int i = 0; i < level; i++) cout << '\t';

			cout << (*it)->getName() << endl;
			tree((Directory*)*it, level + 1);
		}
	}
}

void move(Directory* cwd, string filename, string newLocation) {
	File* fl = cwd->getFile(filename);
	Directory* newDir = cwd->getDirectory(newLocation);

	if (fl != nullptr && newDir != nullptr) {
		cwd->deleteFile(filename);
		newDir->addChildElement(fl);
	}
	else {
		if (fl == nullptr)
			cout << "No file named " << filename << endl;
		if (newDir == nullptr)
			cout << "Invalid directory name" << endl;
	}
}

void copy(Directory* cwd, string filename, string newLocation) {
	File* fl = cwd->getFile(filename);
	Directory* newDir = cwd->getDirectory(newLocation);

	if (fl != nullptr && newDir != nullptr) {
		File* newFile = new File(*fl);
		newDir->addChildElement(newFile);
	}
	else {
		if (fl == nullptr)
			cout << "No file named " << filename << endl;
		if (newDir == nullptr)
			cout << "Invalid directory name" << endl;
	}
}

void find(Directory*  cwd, string fileName) {
	if (cwd->getFile(fileName) != nullptr) {
		cout << "file found" << endl;
	}
	else {
		cout << "no file found" << endl;
	}
}

void create(Directory* cwd, string name) {

	File* exists = cwd->getFile(name);
	if (exists != nullptr) {
		cout << "file already exists" << endl;
		return;
	}

	File* newFile = new File(name);
	cwd->addChildElement(newFile);
	cwd->getDateTime();

	fileAttributesInput(newFile);

	cout << "File created successfully" << endl;
}

string changePrompt(string prompt) {
	return prompt;
}

void edit(Directory* cwd, File * exists) {
	string confirm;

	if (exists == nullptr) {
		cout << "No such file exists" << endl;
		return;
	}

	cout << "Do you want to add/edit text [y/Y]" << endl;
	cin >> confirm;

	bool running = confirm == "y" || confirm == "Y";
	Key key(-1, 0);
	Keyboard keyboard;
	TextBuffer textEditor;

	textEditor.load(exists->getData());

	system("cls");
	textEditor.display();

	while (running) {
		key = keyboard.getKey();

		if (key.isPrintable) {
			textEditor.add(key.character);
			textEditor.display();
		}
		else {
			if (key.keyCode == 'Z') {
				textEditor.undo();
				system("cls");
				textEditor.display();
			}
			if (key.keyCode == 'Y') {
				textEditor.redo();
				system("cls");
				textEditor.display();
			}

			if (key.keyCode == VK_RETURN) {
				textEditor.newLine();
				system("cls");
				textEditor.display();
			}

			if (key.keyCode == VK_BACK) {
				textEditor.backspace();
				system("cls");
				textEditor.display();
			}

			if (key.keyCode == VK_RIGHT) {
				textEditor.moveCursorRight();
				textEditor.display();
			}

			if (key.keyCode == VK_LEFT) {
				textEditor.moveCursorLeft();
				textEditor.display();
			}

			if (key.keyCode == VK_UP) {
				textEditor.moveCursorUp();
				textEditor.display();
			}

			if (key.keyCode == VK_DOWN) {
				textEditor.moveCursorDown();
				textEditor.display();
			}

			if (key.keyCode == VK_ESCAPE) {
				running = false;
			}
		}
	}

	exists->setData(textEditor.save());
}

void convert(Directory* cwd, string exe) {
	vector<CommonElement*> elements = cwd->getElements();

	for (auto it = elements.begin(); it != elements.end(); it++) {
		if (!(*it)->isDirectory()) {
			(*it)->setExtension(exe);
		}
	}
}

void fileAttributesInput(File* current) {
	int priority; float time; string confirm; string owner;

	cout << "Owner: ";
	cin >> owner;
	current->setOwner(owner);

	cout << "Priority Number for File: ";
	cin >> priority;
	current->setPriority(priority);

	cout << "Enter time to print: ";
	cin >> time;
	current->setPrintingTime(time);

	cout << "Do you want to set Read Only Attribute [Y/y]? ";
	cin >> confirm;
	if (confirm == "y" || confirm == "Y") {
		current->setReadOnly();
	}
}

bool findF(Directory* current,string name, string str) {
	File* exists = current->getFile(name);

	if (exists == nullptr) {
		cout << "file do not exists" << endl;
		return false;
	}

	if (exists->getData().find(str)!=string::npos) {
		return true;
	}

	return false;
}

void findStr(Directory* cwd, string str) {
	vector<CommonElement*> elements = cwd->getElements();

	for (auto it = elements.begin(); it != elements.end(); it++) {
		if (!(*it)->isDirectory()) {
			if ((*it)->getData().find(str) != string::npos) {
				cout << (*it)->getName() << endl;
			}
			
		}
	}
}

void print(Directory* cwd, string name) {
	File* file = cwd->getFile(name);

	if (file != nullptr) {
		printQueues.addToPrintQueue(file);
		printQueues.printQueue();
	}
	else {
		cout << "No such file exists" << endl;
		return;
	}
}

void pPrint(Directory* cwd, string name) {
	File* file = cwd->getFile(name);

	if (file != nullptr) {
		printQueues.addToPriorityQueue(file);
		printQueues.printPriorityQueue();
	}
	else {
		cout << "No such file exists" << endl;
		return;
	}
}

void save(Directory* cwd, File* file) {
	if (file == nullptr) {
		cout << "No such file exists" << endl;
	}

	ofstream out(file->getName() + file->getExtension());

	out << file->getData();
	out.close();
}

void savetree(Directory* cwd) {
	if (cwd == nullptr) return;

	ofstream out("tree.txt", ios_base::app);

	vector<CommonElement*> elements = cwd->getElements();

	out << cwd->getName() << ',' << elements.size() << endl;

	for (auto it = elements.begin(); it != elements.end(); it++) {
		if ((*it)->isDirectory()) {
			savetree((Directory*) *it);
		}
		else {
			out << (*it)->getName() << (*it)->getExtension() << endl;
		}
	}
}

void loadtree(Directory* cwd, string line, ifstream* input) {
	ifstream inp;

	if (input == nullptr) {
		inp.open(line);
		input = &inp;

		getline(*input, line);
	}

	string name = line.substr(0, line.find(','));
	int elems = stoi(line.substr(name.length() + 1));

	cwd->setName(name);

	for (int i = 0; i < elems; i++) {
		CommonElement* elem = nullptr;

		getline(*input, line);

		if (line.find(',') == string::npos) {
			elem = new File();

			elem->setName(line.substr(0, line.find('.')));
			elem->setExtension(line.substr(elem->getName().length()));
		}
		else {
			elem = new Directory();
			loadtree((Directory *)elem, line, input);
		}

		cwd->addChildElement(elem);
	}
}

void displaySize(double size) {
	if (size < 1.0) {
		cout << " KB";
	}
	else {
		cout << " MB";
	}
}

void start() {
	string registrationNumber, course;

	registrationNumber = "2022-CS-182";
	course = "Data Structures and Algorithms CS-261L";

	cout << registrationNumber << "\t\t" << course << endl;
}
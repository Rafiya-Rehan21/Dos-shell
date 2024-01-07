#include "CommonElement.cpp"
#include "File.cpp"
#include <iostream>

class Directory : public CommonElement {
private:
	vector<CommonElement*>CD;
public:

	Directory(string name, int characters, int lines, string owner, int priority, time_t time, string exe, string type, string val) :
		CommonElement(name, characters, lines, owner, priority, time, exe, type, val) { }

	Directory(string name) : CommonElement(name, "dir") {}

	Directory() :CommonElement("dir") {}

	void addChildElement(CommonElement* dir) {
		CD.push_back(dir);
		dir->setParentDir(this);
	}

	vector<CommonElement*> getElements() {
		return CD;
	}

	File* getFile(string name) {
		for (auto it = CD.begin(); it != CD.end(); it++) {
			if ((*it)->getName() == name && !(*it)->isDirectory()) {
				return (File *)*it;
			}
		}

		return nullptr;
	}

	Directory* getDirectory(string name) {
		for (auto it = CD.begin(); it != CD.end(); it++) {
			if ((*it)->getName() == name && (*it)->isDirectory()) {
				return (Directory*)*it;
			}
		}

		return nullptr;
	}

	bool deleteFile(string& name) {
		for (auto it = CD.begin(); it != CD.end(); it++) {
			if ((*it)->getName() == name && !(*it)->isDirectory()) {
				CD.erase(it);
				return true;
			}
		}

		return false;
	}

	bool deleteDir(string& name) {
		for (auto it = CD.begin(); it != CD.end(); it++) {
			if ((*it)->getName() == name && (*it)->isDirectory()) {
				CD.erase(it);
				return true;
			}
		}

		return false;
	}

	void format() {
		CD.clear();
	}
};
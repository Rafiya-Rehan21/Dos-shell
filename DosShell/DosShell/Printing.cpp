#pragma once
#include "Compare.h"
#include "File.cpp"
#include <iostream>
#include <queue>
#include <vector>

class Printing {
public:
	time_t pTime, pPriorityTime;
	queue<File*> queue;
	priority_queue<File*, vector<File*>,Compare> priorityPrint;

	void addToPrintQueue(File* file) {
		if (queue.empty()) {
			pTime = time(0);
		}

		queue.push(file);
	}

	void addToPriorityQueue(File* file) {
		if (priorityPrint.empty())
			pPriorityTime = time(0);
		else if (file->getPriority() < priorityPrint.top()->getPriority()) {
			priorityPrint.top()->setPrintingTime(time(0) - pPriorityTime);
			pPriorityTime = time(0);
		}

		priorityPrint.push(file);
	}

	void printPriorityQueue() {
		int size = priorityPrint.size();
		vector<File*> top;
		time_t diff = pPriorityTime - time(0), curTime = 0;

		for (int i = 0; i < size; i++) {
			File* pt = priorityPrint.top();
			top.push_back(pt);

			curTime = pt->getPrintingTime() + diff;
			cout << pt->getName() << ' ' << curTime << endl;
			diff = max((int)curTime, 0);

			priorityPrint.pop();
		}

		for (int j = 0; j < size; j++) {
			priorityPrint.push(top[j]);
		}
	}

	void printQueue() {
		time_t diff = pTime - time(0), curTime = 0;

		for (int i = 0; i < queue.size(); i++) {
			File* top = queue.front();

			curTime = top->getPrintingTime() + diff;
			cout << top->getName() << ' ' << curTime << endl;
			diff = max((int)curTime, 0);

			queue.pop();
			queue.push(top);
		}
	}

	void updateQueues() {
		time_t diff = pTime - time(0), curTime = 0;

		for (int i = 0; i < queue.size(); i++) {
			File* top = queue.front();

			curTime = top->getPrintingTime() + diff;
			diff = max((int)curTime, 0);

			queue.pop();

			if (curTime > 0)
			{
				queue.push(top);
			}
			else {
				pTime = time(0);
			}
		}

		diff = pPriorityTime - time(0), curTime = 0;

		for (int i = 0; i < priorityPrint.size(); i++) {
			File* top = priorityPrint.top();

			curTime = top->getPrintingTime() + diff;
			diff = max((int)curTime, 0);

			priorityPrint.pop();

			if (curTime > 0)
			{
				priorityPrint.push(top);
			}
			else {
				pPriorityTime = time(0);
			}
		}
	}
};
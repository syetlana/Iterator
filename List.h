#include <exception>

#pragma once

enum class Iterators {
	STEP,
	VALUE,
	PREDICATE
};

struct Node {
	Node* next;
	int value;
	Node(int aValue) : value(aValue), next(nullptr) {}
};

class List {
	friend class ListIterator;
private:
	size_t size = 0, lsize;
	Node* front;
	void Push(int aValue) {
		Node* tmp = new Node(aValue);
		if (size > 0)
			tmp->next = front;
		front = tmp;
		++size;
	};
	void Pop() {
		Node* tmp = front;
		front = front->next;
		delete tmp;
		--size;
	};
public:
	List(size_t aSize) : front(nullptr) {
		if (aSize < 0)
			throw std::exception("Impossible to create a list: invalid list capacity");
		lsize = aSize;
	}
	virtual ~List() {}
	int getTop() const {
		if (size == 0)
			throw std::exception("Impossible to execute GetFirst: list is empty");
		return front->value;
	}

	bool isEmpty() const {
		return size == 0;
	}

	size_t getListCapacity() const {
		return lsize;
	};

	size_t getStackSize() const {
		return size;
	};

	bool append(int aValue) {
		if (size == lsize)
			return false;
		Push(aValue);
		return true;
	}

	bool remove() {
		if (size == 0)
			return false;
		Pop();
		return true;
	}

	ListIterator* createIterator(Iterators its, const int aStep = 0, bool(*aFunction)(int) = nullptr) const;
};


class ListIterator {
	friend class List;
protected:
	const List* pList;
	Node* iterator;
public:
	ListIterator(const List* aPList) : pList(aPList) {}
	int operator*() {
		if (iterator == nullptr)
			throw std::exception("Impossible to get value");
		return iterator->value;
	}
	ListIterator& operator++() {
		if (!next())
			throw std::exception("Impossible to get to the next step");
		return *this;
	}
	bool first() {
		iterator = pList->front;
		return nextItem();
	};
	bool next() {
		iterator = iterator->next;
		return nextItem();
	};
	virtual bool nextItem() = 0;
};

class ListIteratorStep : public ListIterator
{
private:
	int step;
public:
	friend class List;
	ListIteratorStep(const List* aPList, int aStep = 1) : ListIterator(aPList) {
		if (aStep < 0)
			throw std::exception("Impossible to create a ListIteratorStep: step can't be <0");
		step = aStep - 1;
	}

	bool nextItem() override
	{
		for (auto i = 0; iterator != nullptr && i < step; i++)
			iterator = iterator->next;
		return iterator != nullptr;
	}

};

class ListIteratorPredicate : public ListIterator
{
private:
	bool(*function)(int);
	bool nextItem() override {
		while (iterator != nullptr && !(function(iterator->value)))
			iterator = iterator->next;
		return iterator != nullptr;
	}
public:
	ListIteratorPredicate(const List* aPList, bool(*aFunction)(int)) : ListIterator(aPList), function(aFunction) {}
};


class ListIteratorValue : public ListIterator
{
private:
	int value;
	bool nextItem() override {
		while (iterator != nullptr && iterator->value != this->value)
			iterator = iterator->next;
		return iterator != nullptr;
	}
public:
	ListIteratorValue(const List* aPList, int aValue) : ListIterator(aPList), value(aValue) {}

};

ListIterator* List::createIterator(Iterators its, const int aStep, bool(*aFunc)(int)) const {
	if (its == Iterators::STEP)
		return new ListIteratorStep(this, aStep);
	if (its == Iterators::VALUE)
		return new ListIteratorValue(this, aStep);
	if (its == Iterators::PREDICATE)
		return new ListIteratorPredicate(this, *aFunc);
	return nullptr;
}
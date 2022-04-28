#include "List.h"
#include <iostream>
#include <time.h>
#include <windows.h>

using namespace std;

bool function(int num) {
    return num >= 3;
}

unsigned counting(ListIterator* li) {
	unsigned cnt = 0;
	if (li->first())
		do {
			cnt++;
			cerr << **li << " ";
		} while (li->next());
	cerr << endl;
	return cnt;
}

int main()
{
    try {
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        srand(time(NULL));
        unsigned capacity, count;
        cout << "Введите объём стека: ";
        cin >> capacity;
        List list(capacity);
        cout << "Введите количество элементов для заполнения стека: ";
        cin >> count;
        for (auto i = 0; i < count; i++) {
            int x = rand() % 10;
            if (list.append(x))
                cout << x << " ";
        }
        
        cout << endl;
        int pS, pV;
        cout << "Введите шаг: ";
        cin >> pS;
        ListIterator* litS = list.createIterator(Iterators::STEP, pS);
        ListIterator* litS_1 = list.createIterator(Iterators::STEP, pS);
        cout << "Подсчет: количество шагов длины "<< pS << " : " << counting((ListIteratorStep*)litS) << endl << endl;

        cout << "Введите значение: ";
        cin >> pV;
        ListIterator* litV = list.createIterator(Iterators::VALUE, pV);
        cout << "Подсчет: количество элементов со значением " << pV << " : " << counting((ListIteratorValue*)litV) << endl << endl;
        
        ListIterator* litP = list.createIterator(Iterators::PREDICATE, 0, function);
        cout << "Подсчет: количество предикатных элементов " << " : " << counting((ListIteratorPredicate*)litP) << endl << endl;

        litS_1->first();
        cout << "\tЗначение вершины стека : " << list.getTop() << endl;

        cout << "\t\tВместительность листа: " << list.getListCapacity() <<
            "\tРазмер стека: " << list.getStackSize() << endl;

        for (int i = 0; i < count / 4; i++) {
            list.remove();
        }

        cout << "\t\tВместительность листа после удаления: " << list.getListCapacity() <<
            "\tРазмер стека после удаления: " << list.getStackSize() << endl;

    }
    catch (exception ex) {
        cout << ex.what();
    }
}


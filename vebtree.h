#pragma once
#pragma once

#include <memory>

#define UInt64 unsigned long long

using namespace std;
    /*Класс Veb реализует структуру данных, известную как дерево Ван Эмде Боаса.
        Структура данных хранит числа от 0 до одного числа(обозначим его за U), являющегося степенью двойки.
        Поддерживает добавление и удаление элементов, нахождение минимума и максимума, нахождение конкретного числа, а также элементов перед ним и после него.
        Реализована и функция Mark, использующаяся главным образом для наглядного тестирования. Древо включает в себя два поддерева, а также одно вспомогательное древо.*/
class Veb
{
public:
    int K;
    int N;
    UInt64 lmsk;
    unique_ptr<unique_ptr<Veb>[]> Subtrees; //Поддеревья
    unique_ptr<Veb> Aux; //Вспомогательное древо
    UInt64 TreeMin; //Минимум древа
    UInt64 TreeMax; //Максимум древа
    UInt64 NoneK;
    UInt64 NoneN;
    int SpecFlag = 0;

    Veb(int k); //Конструктор класса
    //Подробное описание следующих методов имеется в vebtree.cpp
    bool Empty();
    UInt64 GetMax();
    UInt64 GetMin();
    UInt64 Merge(UInt64 h, UInt64 l);
    UInt64 High(UInt64 v);
    UInt64 Low(UInt64 v);
    bool Add(UInt64 v);
    void Mark(UInt64 v);
    bool Lookup(UInt64 v);
    UInt64 FindNext(UInt64 v);
    UInt64 FindPrev(UInt64 v);
    bool Remove(UInt64 v);
};

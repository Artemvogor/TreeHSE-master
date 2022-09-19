#pragma once
#include "vebtree.h"

Veb::Veb(int k) // Конструктор
{
    K = k;// Размерность дерева в битах
    N = k >> 1;// 2^N = 2^(K - 1) - количество поддеревьев
    NoneK = (UInt64)1 << K; // "Служебное" число, соответствующее значению "отсутствует"
    NoneN = (UInt64)1 << N;// Количество поддеревьев
    lmsk = NoneN - 1;// Маска для выделения младшей части числа (т.е. младшей половины бит)
    Subtrees = make_unique<unique_ptr<Veb>[]>(NoneN);// Создаём массив поддеревьев
    if (K >= 2) Aux = make_unique<Veb>(N);// Вспомогательное дерево для поиска непустых поддеревьев
    TreeMin = NoneK;// Дерево создано, в нём пока ничего нет
    TreeMax = NoneK;
}

bool Veb::Empty()
{
    return ((TreeMin == NoneK) && (TreeMax == NoneK));// Дерево пустое, если и мин, и макс = "отсутствует"
}

UInt64 Veb::GetMax()
{
    return TreeMax; //Getter функция для получения максимума древа
}

UInt64 Veb::GetMin()
{
    return TreeMin; //Getter функция для получения минимума древа.
}

UInt64 Veb::Merge(UInt64 h, UInt64 l)
{
    return (h << N) | l;// Соединяем значение l со значением h, сдвинутым в старшую половину (т.е. на половину разрядности текущего дерева)
}

UInt64 Veb::High(UInt64 v)
{
    return (UInt64)v >> N;
}

UInt64 Veb::Low(UInt64 v)
{
    return (UInt64)(v & lmsk);
}

bool Veb::Add(UInt64 v)
{
    if ((v == TreeMin) || (v == TreeMax)) // Если число на одном из краёв текущего дерева, то оно там уже есть, добавлять не нужно
    {
        SpecFlag = 2;
        return false;
    }
    bool retv = false;
    if (Empty())// Если дерево пустое, то тогда его мин и макс будут содержать одно и то же число - v
    {
        TreeMin = TreeMax = v;
        SpecFlag = 1;
        retv = true;
    }
    if (v < TreeMin) // Если v меньше минимума, то это новый минимум
    {
        SpecFlag = 3;
        TreeMin = v;
        retv = true;
    }
    if ((v > TreeMax) || (TreeMax == NoneK)) // Если v больше максимума (но этот максимум не NoneK, которое больше чего угодно), то v - новый максимум
    {
        SpecFlag = 3;
        TreeMax = v;
        retv = true;
    }
    if (K == 1) // Если дерево однобитное (этажи ниже создавать уже нельзя), то выходим
    {
        SpecFlag = 1;
        return retv;
    }
    UInt64 vh = High(v);// Иначе расщепляем v на две части и создаём этаж ниже
    UInt64 vl = Low(v);

    if (Subtrees[vh].get() == nullptr) Subtrees[vh] = make_unique<Veb>(N);// Если поддерево для старшей половины v ещё не создано, то создаём его
    if (Subtrees[vh]->Empty()) Aux->Add(vh);// Если это дерево пустое (новое - или не новое, но пустое), то добавляем его в Aux (это "каталог поиска")
    return Subtrees[vh]->Add(vl);// Добавляем теперь vl в поддерево номер vh, если всё нормально, то оно вернёт true (а если на нижних этажах окажется, что такое число всё-таки уже есть, то false)
}

void Veb::Mark(UInt64 v) // Для визуализации (и отладки). Прокрасить путь, по которому мы проходим в поисках числа (даже если поиски неуспешны - докуда дойдём)
{
    if ((v == TreeMin) || (v == TreeMax))
    {
        SpecFlag = 1;
        if (K > 1)
        {
            UInt64 vh = High(v);
            UInt64 vl = Low(v);
            if (Subtrees[vh].get() != nullptr) Subtrees[vh]->Mark(vl);
        }
    }
}

bool Veb::Lookup(UInt64 v)
{
    if ((v == TreeMin) || (v == TreeMax)) // Если число на одном из краёв текущего поддерева, то оно нашлось. Прокрашиваем (Mark) ветви на всех этажах вниз, где находятся части этого числа.
    {
        Mark(v);
        return true;
    }
    if (K == 1) return false;// Если мы на самом нижнем этаже, а числа нет ни в TreeMin, ни в TreeMax, значит, его нет вовсе.
    UInt64 vh = High(v);// Делим число на две части
    UInt64 vl = Low(v);
    if (Subtrees[vh].get() == nullptr) return false;// Если старшей половины нет в поддеревьях, число точно не найдётся
    // ****
    if (Subtrees[vh]->Lookup(vl))// Иначе пытаемся найти младшую половину в поддереве старшей. (Кстати, а почему бы, вообще-то, не поискать vh в Aux?)
    {
        SpecFlag = 1;
        return true;// Ура, мы его нашли, выходим с хорошей новостью!
    }
    return false;// А если дошли сюда, то не нашли.
}

UInt64 Veb::FindNext(UInt64 v)// Поиск соседа справа. Важное примечание: ищем САМО ТАКОЕ ЧИСЛО v ИЛИ его правого соседа. Примерно так: "Позовите Сашу. А если его нет дома, то пусть придёт его сосед справа."
{ // Возможно, было бы правильнее назвать FindEqualOrNext
    if (Empty()) return NoneK; // Если наше дерево пустое, то никаких соседей для v в нём точно нет
    if ((v <= TreeMin) && (TreeMin != NoneK)) return TreeMin; // Если TreeMin есть, но v его меньше, - то тогда TreeMin и есть его ближайший сосед справа
    if (v > TreeMax) return NoneK;// Если v больше, чем TreeMax, то соседей справа у него точно нет (проверять TreeMax на NoneK нет смысла, потому что если он равен NoneK, а v ещё и больше, то тоже никаких соседей быть не может, разве что можно было бы выкинуть исключение)
//    if (K == 1) return (v == TreeMax ? v : NoneK); 
    if (K == 1) return (v <= TreeMax ? TreeMax : NoneK); // На нижнем этаже у числа, большего TreeMin (уже проверили) и меньшего либо равного TreeMax, правым соседом может быть только TreeMax
    UInt64 vh = High(v);// А если мы не на нижнем этаже, то ищем соседа справа для младшей половины числа в поддереве старшей
    UInt64 vl = Low(v);
    if (Subtrees[vh].get() != nullptr)// Если есть поддерево для vh
    {
        if (vl <= Subtrees[vh]->GetMax())// если vl где-то внутри поддерева (или, возможно, меньше его минимума), то ищем там
        {
            UInt64 q = Subtrees[vh]->FindNext(vl);
            if (q == NoneN) q = Subtrees[vh]->GetMax();// Надо бы проверить, точно ли эта строчка правильная. Вроде бы да, потому что если vl > Subtrees[vh]->GetMax(), мы сюда не должны дойти.
            return Merge(vh, q); // Возвращаем значение, сращенное из vh и того, что нашлось в поддереве
        }
    }
    if (Aux.get() != nullptr)// Если же поддерева для vh не оказалось, берём ближайшее поддерево справа и ищем соседа там
    {
        vh = Aux->FindNext(vh + 1);
        if (vh != NoneN)// Если такое поддерево есть (и непустое, поэтому есть в Aux), то
        {
            Subtrees[vh]->SpecFlag = 1;
            return Merge(vh, Subtrees[vh]->GetMin());// соседом справа для v окажется его МИНИМУМ (т.к. там все числа по определению больше v)
        }
    } // ****
    return Merge(vh, GetMax());// Если дошли сюда, то считаем соседом справа максимальное значение в текущем поддереве
}

UInt64 Veb::FindPrev(UInt64 v) //Поиск соседа слева(аналогия с FindNext)
{
    if (Empty()) return NoneK; 
    if ((v >= TreeMax) && (TreeMax != NoneK)) return TreeMax;
    if (v < TreeMin) return NoneK;
    if (K == 1) return (v == TreeMin ? v : NoneK);
    UInt64 vh = High(v);
    UInt64 vl = Low(v);
    if (Subtrees[vh].get() != nullptr)
    {
        if (vl >= Subtrees[vh]->GetMin())
        {
            UInt64 q = Subtrees[vh]->FindPrev(vl);
            if (q == NoneN) q = Subtrees[vh]->GetMin();
            return Merge(vh, q);
        }
    }
    if (Aux.get() != nullptr)
    {
        if (vh == 0) return NoneK;
        vh = Aux->FindPrev(vh - 1);
        if (vh != NoneN)
        {
            Subtrees[vh]->SpecFlag = 1;
            return Merge(vh, Subtrees[vh]->GetMax());
        }
        else return NoneK;
    }
    return Merge(vh, GetMin());
    //return NoneK;
}
bool Veb::Remove(UInt64 v)
{
    UInt64 t;
    bool retv = false;
    if (v == TreeMin) // Если v - минимальное значение в дереве, то
    {
        t = TreeMin + 1;// ищем новый минимум, который должен быть правым соседом (и не меньше прежнего минимума + 1)
        TreeMin = NoneK;
        TreeMin = FindNext(t);
        SpecFlag = 4;
        retv = true;// рапортуем, что число найдено и удалено
    } 
    if (v == TreeMax) // Аналогично, если v - максимальное значение
    {
        t = TreeMax - 1;// ищем новый максимум, который должен быть левым соседом (меньше либо равен прежнему максимуму - 1)
        TreeMax = NoneK;
        TreeMax = FindPrev(t);
        SpecFlag = 4;
        retv = true;// рапортуем, что число найдено и удалено
    }


    if (K == 1) return retv;// Если мы на самом нижнем этаже, то выходим

    UInt64 vh = High(v);// Иначе разделяем число на старшую и младшую половины и идём на этаж вниз
    UInt64 vl = Low(v);
    if (Subtrees[vh].get() == nullptr) return retv;// Если vh в поддеревьях не находится, выходим  // ****
    // Вот здесь бы и посмотреть в Aux? // ****
    retv |= Subtrees[vh]->Remove(vl);// Пробуем найти и удалить следы числа v (его младшую половину) из нижних этажей. Если его не было на краях текущего дерев, но нашлось где-то там, то Remove вернёт true, и наш результат здесь должен быть true, поэтому |=
    if (retv) // Если число успешно удалено, то:
    {
        SpecFlag = 5;// Прокрашиваем соответственно
        if (Subtrees[vh]->Empty())// и удаляем ключ vh из Aux
        {
            Aux->Remove(vh);
        }
    }
    return retv;
}
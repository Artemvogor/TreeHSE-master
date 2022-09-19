#pragma once
#include "vebtree.h"

Veb::Veb(int k) // �����������
{
    K = k;// ����������� ������ � �����
    N = k >> 1;// 2^N = 2^(K - 1) - ���������� �����������
    NoneK = (UInt64)1 << K; // "���������" �����, ��������������� �������� "�����������"
    NoneN = (UInt64)1 << N;// ���������� �����������
    lmsk = NoneN - 1;// ����� ��� ��������� ������� ����� ����� (�.�. ������� �������� ���)
    Subtrees = make_unique<unique_ptr<Veb>[]>(NoneN);// ������ ������ �����������
    if (K >= 2) Aux = make_unique<Veb>(N);// ��������������� ������ ��� ������ �������� �����������
    TreeMin = NoneK;// ������ �������, � �� ���� ������ ���
    TreeMax = NoneK;
}

bool Veb::Empty()
{
    return ((TreeMin == NoneK) && (TreeMax == NoneK));// ������ ������, ���� � ���, � ���� = "�����������"
}

UInt64 Veb::GetMax()
{
    return TreeMax; //Getter ������� ��� ��������� ��������� �����
}

UInt64 Veb::GetMin()
{
    return TreeMin; //Getter ������� ��� ��������� �������� �����.
}

UInt64 Veb::Merge(UInt64 h, UInt64 l)
{
    return (h << N) | l;// ��������� �������� l �� ��������� h, ��������� � ������� �������� (�.�. �� �������� ����������� �������� ������)
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
    if ((v == TreeMin) || (v == TreeMax)) // ���� ����� �� ����� �� ���� �������� ������, �� ��� ��� ��� ����, ��������� �� �����
    {
        SpecFlag = 2;
        return false;
    }
    bool retv = false;
    if (Empty())// ���� ������ ������, �� ����� ��� ��� � ���� ����� ��������� ���� � �� �� ����� - v
    {
        TreeMin = TreeMax = v;
        SpecFlag = 1;
        retv = true;
    }
    if (v < TreeMin) // ���� v ������ ��������, �� ��� ����� �������
    {
        SpecFlag = 3;
        TreeMin = v;
        retv = true;
    }
    if ((v > TreeMax) || (TreeMax == NoneK)) // ���� v ������ ��������� (�� ���� �������� �� NoneK, ������� ������ ���� ������), �� v - ����� ��������
    {
        SpecFlag = 3;
        TreeMax = v;
        retv = true;
    }
    if (K == 1) // ���� ������ ���������� (����� ���� ��������� ��� ������), �� �������
    {
        SpecFlag = 1;
        return retv;
    }
    UInt64 vh = High(v);// ����� ���������� v �� ��� ����� � ������ ���� ����
    UInt64 vl = Low(v);

    if (Subtrees[vh].get() == nullptr) Subtrees[vh] = make_unique<Veb>(N);// ���� ��������� ��� ������� �������� v ��� �� �������, �� ������ ���
    if (Subtrees[vh]->Empty()) Aux->Add(vh);// ���� ��� ������ ������ (����� - ��� �� �����, �� ������), �� ��������� ��� � Aux (��� "������� ������")
    return Subtrees[vh]->Add(vl);// ��������� ������ vl � ��������� ����� vh, ���� �� ���������, �� ��� ����� true (� ���� �� ������ ������ ��������, ��� ����� ����� ��-���� ��� ����, �� false)
}

void Veb::Mark(UInt64 v) // ��� ������������ (� �������). ���������� ����, �� �������� �� �������� � ������� ����� (���� ���� ������ ��������� - ������ �����)
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
    if ((v == TreeMin) || (v == TreeMax)) // ���� ����� �� ����� �� ���� �������� ���������, �� ��� �������. ������������ (Mark) ����� �� ���� ������ ����, ��� ��������� ����� ����� �����.
    {
        Mark(v);
        return true;
    }
    if (K == 1) return false;// ���� �� �� ����� ������ �����, � ����� ��� �� � TreeMin, �� � TreeMax, ������, ��� ��� �����.
    UInt64 vh = High(v);// ����� ����� �� ��� �����
    UInt64 vl = Low(v);
    if (Subtrees[vh].get() == nullptr) return false;// ���� ������� �������� ��� � �����������, ����� ����� �� �������
    // ****
    if (Subtrees[vh]->Lookup(vl))// ����� �������� ����� ������� �������� � ��������� �������. (������, � ������ ��, ������-��, �� �������� vh � Aux?)
    {
        SpecFlag = 1;
        return true;// ���, �� ��� �����, ������� � ������� ��������!
    }
    return false;// � ���� ����� ����, �� �� �����.
}

UInt64 Veb::FindNext(UInt64 v)// ����� ������ ������. ������ ����������: ���� ���� ����� ����� v ��� ��� ������� ������. �������� ���: "�������� ����. � ���� ��� ��� ����, �� ����� ����� ��� ����� ������."
{ // ��������, ���� �� ���������� ������� FindEqualOrNext
    if (Empty()) return NoneK; // ���� ���� ������ ������, �� ������� ������� ��� v � �� ����� ���
    if ((v <= TreeMin) && (TreeMin != NoneK)) return TreeMin; // ���� TreeMin ����, �� v ��� ������, - �� ����� TreeMin � ���� ��� ��������� ����� ������
    if (v > TreeMax) return NoneK;// ���� v ������, ��� TreeMax, �� ������� ������ � ���� ����� ��� (��������� TreeMax �� NoneK ��� ������, ������ ��� ���� �� ����� NoneK, � v ��� � ������, �� ���� ������� ������� ���� �� �����, ����� ��� ����� ���� �� �������� ����������)
//    if (K == 1) return (v == TreeMax ? v : NoneK); 
    if (K == 1) return (v <= TreeMax ? TreeMax : NoneK); // �� ������ ����� � �����, �������� TreeMin (��� ���������) � �������� ���� ������� TreeMax, ������ ������� ����� ���� ������ TreeMax
    UInt64 vh = High(v);// � ���� �� �� �� ������ �����, �� ���� ������ ������ ��� ������� �������� ����� � ��������� �������
    UInt64 vl = Low(v);
    if (Subtrees[vh].get() != nullptr)// ���� ���� ��������� ��� vh
    {
        if (vl <= Subtrees[vh]->GetMax())// ���� vl ���-�� ������ ��������� (���, ��������, ������ ��� ��������), �� ���� ���
        {
            UInt64 q = Subtrees[vh]->FindNext(vl);
            if (q == NoneN) q = Subtrees[vh]->GetMax();// ���� �� ���������, ����� �� ��� ������� ����������. ����� �� ��, ������ ��� ���� vl > Subtrees[vh]->GetMax(), �� ���� �� ������ �����.
            return Merge(vh, q); // ���������� ��������, ��������� �� vh � ����, ��� ������� � ���������
        }
    }
    if (Aux.get() != nullptr)// ���� �� ��������� ��� vh �� ���������, ���� ��������� ��������� ������ � ���� ������ ���
    {
        vh = Aux->FindNext(vh + 1);
        if (vh != NoneN)// ���� ����� ��������� ���� (� ��������, ������� ���� � Aux), ��
        {
            Subtrees[vh]->SpecFlag = 1;
            return Merge(vh, Subtrees[vh]->GetMin());// ������� ������ ��� v �������� ��� ������� (�.�. ��� ��� ����� �� ����������� ������ v)
        }
    } // ****
    return Merge(vh, GetMax());// ���� ����� ����, �� ������� ������� ������ ������������ �������� � ������� ���������
}

UInt64 Veb::FindPrev(UInt64 v) //����� ������ �����(�������� � FindNext)
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
    if (v == TreeMin) // ���� v - ����������� �������� � ������, ��
    {
        t = TreeMin + 1;// ���� ����� �������, ������� ������ ���� ������ ������� (� �� ������ �������� �������� + 1)
        TreeMin = NoneK;
        TreeMin = FindNext(t);
        SpecFlag = 4;
        retv = true;// ���������, ��� ����� ������� � �������
    } 
    if (v == TreeMax) // ����������, ���� v - ������������ ��������
    {
        t = TreeMax - 1;// ���� ����� ��������, ������� ������ ���� ����� ������� (������ ���� ����� �������� ��������� - 1)
        TreeMax = NoneK;
        TreeMax = FindPrev(t);
        SpecFlag = 4;
        retv = true;// ���������, ��� ����� ������� � �������
    }


    if (K == 1) return retv;// ���� �� �� ����� ������ �����, �� �������

    UInt64 vh = High(v);// ����� ��������� ����� �� ������� � ������� �������� � ��� �� ���� ����
    UInt64 vl = Low(v);
    if (Subtrees[vh].get() == nullptr) return retv;// ���� vh � ����������� �� ���������, �������  // ****
    // ��� ����� �� � ���������� � Aux? // ****
    retv |= Subtrees[vh]->Remove(vl);// ������� ����� � ������� ����� ����� v (��� ������� ��������) �� ������ ������. ���� ��� �� ���� �� ����� �������� �����, �� ������� ���-�� ���, �� Remove ����� true, � ��� ��������� ����� ������ ���� true, ������� |=
    if (retv) // ���� ����� ������� �������, ��:
    {
        SpecFlag = 5;// ������������ ��������������
        if (Subtrees[vh]->Empty())// � ������� ���� vh �� Aux
        {
            Aux->Remove(vh);
        }
    }
    return retv;
}
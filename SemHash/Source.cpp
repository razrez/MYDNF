#include "windows.h"
#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>  
#include <chrono>
#include <tchar.h>
using namespace std;

typedef int T;  // ��� ���������
typedef int hashTableIndex;// ������ � ���-�������
int hashTableSize;
T* hashTable;
bool* used;

hashTableIndex myhash(T data);
void insertData(T data);
void deleteData(T data);
bool findData(T data);
int dist(hashTableIndex a, hashTableIndex b);

int DataGenerator(int& val)
{
    ofstream write;
    write.open("inPut.txt", ofstream::app);
    srand(time(NULL));
    int a = val;
    write << a << " ";
    for (int i = 0; i < a; i++)
        write << rand() % 100 << " ";
    Sleep(2000);
    write << endl;
    return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {

    int vals = 100;// �������� ��������� �� 100 �� 10�
    for (int i = 0; i < 100; i++) {
        DataGenerator(vals);
        vals += 100;
    }
    ifstream reader("inPut.txt");
    //ofstream timer("time.txt");
    //ofstream size("size.txt");
    //ofstream search("search.txt");
    //ofstream insert("insert.txt");
    ofstream del("del.txt");
    string str;

    //������
    while (getline(reader, str)) {
        int arsize;
        reader >> arsize; //��� ������� ������ �������
        int* arr = new int[arsize]();
        istringstream ss(str);
        for (int i = 0; i < arsize; i++)
            ss >> arr[i];


        //
        //
        int i;
        int maxnum = arsize;
        hashTableSize = arsize;
        hashTable = new T[hashTableSize];

        used = new bool[hashTableSize];
        for (i = 0; i < hashTableSize; i++) {
            hashTable[i] = 0;
            used[i] = false;
        }


        // ��������� �������
        //for (i = 0; i < maxnum; i++)
        //    a[i] = rand();
        // 

        auto begin = std::chrono::steady_clock::now();
        // ���������� ���-������� ���������� �������
        //for (i = 0; i < maxnum; i++) insertData(arr[i]);

        // ����� ��������� ������� �� ���-�������
        //for (i = maxnum - 1; i >= 0; i--) findData(arr[i]);

        // ����� ��������� ������� � ���� List.txt
        /*ofstream out("List.txt");
        for (i = 0; i < maxnum; i++) {
            out << arr[i];
            if (i < maxnum - 1) out << "\t";
        }
        out.close();*/

        // ���������� ���-������� � ���� HashTable.txt
        //out.open("HashTable.txt");
        /*for (i = 0; i < hashTableSize; i++) {
            out << i << "  :  " << used[i] << " : " << hashTable[i] << endl;
        }*/
        //out.close();

        // ������� ���-�������
        for (i = maxnum - 1; i >= 0; i--) {
            deleteData(arr[i]);
        }
        //
        //

        auto end = std::chrono::steady_clock::now();
        auto elapsed_mics = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

        del << elapsed_mics.count() << endl;
        //size << arsize << endl;
    }
    cout << "end" << endl;
    return 0;
}

// ���-������� ���������� ��������
hashTableIndex myhash(T data) {
    return (data % hashTableSize);
}

// ������� ������ �������������� � ������� �������� � �������
void insertData(T data) {
    hashTableIndex bucket;
    bucket = myhash(data);
    while (used[bucket] && hashTable[bucket] != data)
        bucket = (bucket + 1) % hashTableSize;
    if (!used[bucket]) {
        used[bucket] = true;
        hashTable[bucket] = data;
    }
}

// ������� ������ ��������, ������ data
bool findData(T data) {
    hashTableIndex bucket;
    bucket = myhash(data);
    while (used[bucket] && hashTable[bucket] != data)
        bucket = (bucket + 1) % hashTableSize;
    return used[bucket] && hashTable[bucket] == data;
}

//������� �������� �������� �� �������
void deleteData(T data) {
    int bucket, gap;
    bucket = myhash(data);
    while (used[bucket] && hashTable[bucket] != data)
        bucket = (bucket + 1) % hashTableSize;
    if (used[bucket] && hashTable[bucket] == data) {
        used[bucket] = false;
        gap = bucket;
        bucket = (bucket + 1) % hashTableSize;
        while (used[bucket]) {
            if (bucket == myhash(hashTable[bucket]))
                bucket = (bucket + 1) % hashTableSize;
            else if (dist(myhash(hashTable[bucket]), bucket) < dist(gap, bucket))
                bucket = (bucket + 1) % hashTableSize;
            else {
                used[gap] = true;
                hashTable[gap] = hashTable[bucket];
                used[bucket] = false;
                gap = bucket;
                bucket++;
            }
        }
    }
}

// ������� ���������� ���������� �� a �� b (�� ������� �������, ����� �������) 
int dist(hashTableIndex a, hashTableIndex b) {
    return (b - a + hashTableSize) % hashTableSize;
}
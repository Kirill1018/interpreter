#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <locale>
#include <sstream>
using namespace std;
enum ConsoleColor {
    Black, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray,
    LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
};
void SetColor(int text, int background)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
void GotoXY(int X, int Y)
{
    HANDLE hConsole;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { X, Y };
    SetConsoleCursorPosition(hStdOut, coord);
}
#include <stdio.h>
/*0 Ч черный
1 Ч синий
2 Ч зеленый
3 Ч голубой
4 Ч красный
5 Ч лиловый
6 Ч желтый
7 Ч белый
8 Ч серый
9 Ч свело - синий
A Ч светло - зеленый
B Ч светло - голубой
— Ч светло - красный
E Ч светло - желтый
F Ч €рко - белый
system("color F0");*/  // ”становка белого фона и черного текста
const int maxline = 100;
class tree
{
public:
	char* data;
	int g = 1;
	tree* parent, * left, * right;
	int tree_amount(tree* _tree)
	{
		if (_tree == 0) return 0;
		return tree_amount(_tree->right) + tree_amount(_tree->left) + 1;
	}
    tree* tree_insert(tree* _tree, char* item)
    {
        tree* z = new tree;
        z->data = new char[maxline + 1];
        strcpy(z->data, item);
        z->left = z->right = 0;//присвоение значений объектов области пам€ти
        tree* y = 0, * x = _tree;//указатели объектов класса
        while (x != 0)
        {
            y = x;
            if (strcmp(z->data, x->data) < 0) x = x->left;
            else if (strcmp(z->data, x->data) > 0) x = x->right;
            else
            {
                ++(x->g);
                return 0;
            }
        }
        z->parent = y;
        if (y == 0) return z;
        else if (strcmp(z->data, y->data) < 0) y->left = z;
        else if (strcmp(z->data, y->data) > 0) y->right = z;
    }
    int tree_point(tree* _tree, tree** arr, int i = 0)
    {
        if (!_tree) return i;
        i = tree_point(_tree->left, arr, i);
        arr[i++] = _tree;
        i = tree_point(_tree->right, arr, i);
        return i;
    }
    tree** point_sort(tree** arr, int t)
    {
        int d = t;
        tree* count;
        d = d / 2;
        while (d > 0)
        {
            for (int i = 0; i < t - d; i++)
            {
                int j = i;
                while (j >= 0 && arr[j]->g < arr[j + d]->g)
                {
                    count = arr[j];
                    arr[j] = arr[j + d];
                    arr[j + d] = count;
                    j--;
                }
            }
            d = d / 2;
        }
        return arr;
    }
    void print_freq(tree** arr, int t)
    {
        for (int i = 0; i < t; i++)
        {
            ofstream out("res.txt", ios::app);
            if (out.is_open()) out << arr[i]->data << arr[i]->g << " ";
        }
        SetColor(2, 0);
        cout << "частотный словарь записан в файл" << endl;
    }
    void upp_lett(char *word, int i)
    {
        word[i] = (char)tolower((unsigned char)word[i]);
    }
    void freq_dict()
    {
        tree* _tree = 0;//указатель на объект класса
        FILE* in = fopen("data.txt", "r");
        int t = 0, i = 0;//итератор и параметр цикла
        char* word = new char[maxline + 1];
        while (!feof(in))
        {
            fscanf(in, "%s", word);
            int tal = strlen(word);
            for (int i = 0; i < tal; i++)
            {
                upp_lett(word, i);
                if (ispunct((unsigned char)word[i]) || word[i] == ' ' || word[i] == (char)(133) || word[i] == (char)(187) || word[i] == (char)(150) || word[i] == (char)(151) || word[i] == (char)(171)) for (int j = i; j < tal; j++) word[i] = word[j + 1];
            }
            if (strcmp(word, ""))
            {
                if (_tree == 0) _tree = tree_insert(_tree, word);
                else tree_insert(_tree, word);
            }
            strcpy(word, "");
        }
        fclose(in);
        t = tree_amount(_tree);
        tree** arr = new tree * [t];
        tree_point(_tree, arr, i);
        point_sort(arr, t);
        print_freq(arr, t);
    }
    void interpreter()
    {
        typedef multimap<string, string> strMap;
        strMap enru;
        strMap::iterator it;
        stringstream ss;
        string en, ru;
        ifstream fin("words.txt", ios::in | ios::binary);
        string buff;
        while (!fin.eof())
        {
            getline(fin, buff);
            ss << buff;
            ss >> en >> ru;
            enru.insert(make_pair(en, ru));
        }
        ofstream fout("engl-russ transl.txt", ios::app), output("russ-engl transl.txt", ios::app);
        int x = 0;//первый элемент цикла
        while (x < 3)
        {
            SetColor(3, 0);
            cout << "выбор перевода - 0=англо-русский перевод, 1=русско-английский перевод" << endl;
            bool translation;
            cin >> translation;
            switch (translation)
            {
            case 0:
                SetColor(4, 0);
                cout << "ввод слова - ";
                cin >> buff;
                ru.clear();
                ss.clear();
                ss << buff;
                for (ss >> en; !en.empty(); ss >> en)
                {
                    it = enru.find(en);
                    if (it != enru.end()) ru += it->second;
                    else ru += "ошибка";
                    ru += " ";
                    en.clear();
                }
                SetColor(5, 0);
                cout << "перевод - " << ru << endl;
                if (fout.is_open()) fout << buff << " " << ru << endl;
                break;
            case 1:
                SetColor(6, 0);
                cout << "ввод слова - ";
                cin >> buff;
                en.clear();
                ss.clear();
                ss << buff;
                for (ss >> ru; !ru.empty(); ss >> ru)
                {
                    it = enru.find(ru);
                    if (it != enru.end()) en += it->second;
                    else en += "ошибка";
                    en += " ";
                    ru.clear();
                }
                SetColor(7, 0);
                cout << "перевод - " << en << endl;
                if (output.is_open()) output << buff << " " << en << endl;
                break;
            }
            x++;
        }
        SetColor(8, 0);
        cout << "добавление слова... (английского €зыка=0), (русского €зыка=1)" << endl;
        ofstream out("words.txt", ios::app);
        bool language;
        cin >> language;
        switch (language)
        {
        case 0:
            if (out.is_open())
            {
                SetColor(9, 0);
                cout << "английское слово - ";
                cin >> en;
                SetColor(10, 0);
                cout << "русское слово - ";
                cin >> ru;
                out << en << " " << ru << endl;
                out << ru << " " << en << endl;
            }
            break;
        case 1:
            if (out.is_open())
            {
                SetColor(11, 0);
                cout << "русское слово - ";
                cin >> ru;
                SetColor(12, 0);
                cout << "английское слово - ";
                cin >> en;
                out << ru << " " << en << endl;
                out << en << " " << ru << endl;
            }
            break;
        }
    }
};
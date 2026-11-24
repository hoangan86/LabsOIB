#include <iostream>
#include <string>
#include <windows.h>
#include <unordered_set>
#include <clocale>
#include <cstdio>
#include <fcntl.h>
#include <io.h>

using namespace std;

const wstring alphabet =
L"АаБбВвГгДдЕеЁёЖжЗзИиЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщЪъЫыЬьЭэЮюЯя"
L"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
L"0123456789-+=()*№#!?., \"':;";

const wstring key =
L"?ИhшLX=О*2cW'ТIПMэФj78Щаk-юr6№9т3ЭPKЗbmЯaуD)С!ВзлЫБFбyчщAvЖS0Уяq(G\"EойдР.TцzZQxotiONнJR#ЛdЦgHс:ЕUжBЬnвпfХхЁЧеёY1ми5фЪlЙ4pАws, euШ;+ьъкгДVКМрыЮCГН";

bool validateKey(const wstring& alphabet, const wstring& key)
{
    if (alphabet.size() != key.size())
        return false;

    unordered_set<wchar_t> alphabetChars;
    unordered_set<wchar_t> keyChars;

    for (size_t i = 0; i < alphabet.size(); i++)
    {
        wchar_t ch = alphabet[i];

        if (alphabetChars.find(ch) != alphabetChars.end()) 
            return false;

        alphabetChars.insert(ch);
    }

    for (size_t i = 0; i < key.size(); i++)
    {
        wchar_t ch = key[i];

        if (alphabetChars.find(ch) == alphabetChars.end())
            return false;

        if (keyChars.find(ch) != keyChars.end())
            return false;
    }

    return true;
}

wstring encrypt(const wstring& text, const wstring& alphabet, const wstring& key)
{
    wstring result;

    for (size_t i = 0; i < text.size(); i++)
    {
        wchar_t ch = text[i];

        size_t pos = alphabet.find(ch);

        if (pos == wstring::npos)
            result += ch;
        else
            result += key[pos];
    }

    return result;
}

wstring decrypt(const wstring& text, const wstring& alphabet, const wstring& key)
{
    wstring result;

    for (size_t i = 0; i < text.size(); i++)
    {
        wchar_t ch = text[i];

        size_t pos = key.find(ch);

        if (pos == wstring::npos)
            result += ch;
        else
            result += alphabet[pos];
    }

    return result;
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    wcout << L"   Лабораторная работа №1\n";
    wcout << L"   Одноалфавитная подстановка\n\n";

    // Проверка ключа
    if (!validateKey(alphabet, key))
    {
        wcout << L"Ошибка: некорректный ключ!\n";
        wcout << L"Проверьте длину ключа и повторяющиеся символы.\n";
        return 1;
    }

    int choice;
    wstring text;
    wstring result;

    // Выбор операции
    wcout << L"Выберите операцию:\n";
    wcout << L"1. Шифрование\n";
    wcout << L"2. Расшифрование\n";
    wcout << L"Ваш выбор: ";

    wcin >> choice;
    wcin.ignore(10000, L'\n');

    // Ввод исходного текста
    if (choice == 1)
    {
        wcout << L"\nВведите текст для шифрования: ";
        getline(wcin, text);

        result = encrypt(text, alphabet, key);

        wcout << L"\nЗашифрованный текст: " << result << L'\n';
    }
    else if (choice == 2)
    {
        wcout << L"\nВведите текст для расшифрования: ";
        getline(wcin, text);

        result = decrypt(text, alphabet, key);

        wcout << L"\nРасшифрованный текст: " << result << L'\n';
    }
    else
    {
        wcout << L"\nОшибка: выберите 1 или 2!\n";
        return 1;
    }

    wcout << L"\nПрограмма завершена.\n";

    return 0;
}
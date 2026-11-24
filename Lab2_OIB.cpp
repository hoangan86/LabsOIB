// Лабораторная работа №2. Многоалфавитная одноконтурная подстановка
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cstdio>
#include <fcntl.h>
#include <io.h>

using namespace std;

// Исходный алфавит: 145 символов, индексы 0..144
const wstring alphabet =
L"АаБбВвГгДдЕеЁёЖжЗзИиЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщЪъЫыЬьЭэЮюЯя"
L"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
L"0123456789-+=()*№#!?., \"':;";

// Ключ: d = 3 алфавита замены B0, B1, B2 (перестановки алфавита)
const vector<wstring> keys =
{
    // B0
    L"JГЫ,6Ln9DЭШиуэ)ь№дtjIб7dКAyваWТw!х(u4"
    L"'ЗBЕюme:ЮД\"тgсЬqNРшЛH#гкЁ8щMPS+ыеИki3"
    L"CYъ;azsRёfзжБxоМАcQhЧ1црЙ=мнС2FE0pXяЦ"
    L"УНbGKйOХ?-oВ ПUrлЩФфпОчVЪZTЯlv5Ж*.",
    // B1
    L"hc8IEТOЯЖQGdX*Uэ5zq,W7я(2а.R№гшж)ФИы\""
    L"йVл!зЭщpоРО0-МPГфZs3 xLFвDЬиЁmбВмеM6C"
    L"ЫN9#rwюgfШЧНКЛBSСёi:nДЩЕuJ4З=1eнyрYkv"
    L"'KХ+БсbАПцH;tьЪУoпухЙъЦчAдTj?тlЮaк",
    // B2
    L"ЧЭ#яюkДDъlY?ж+ХУ=oА(*iИ-yдф9JF5ФЩ2\"z№"
    L"XuнцщЛQЫsЬn) о60ыЗйтqчTmиВшёЙ'еc18ЮRС"
    L"ОлWNэ;gМUБxТP:хбfhAР7MOKVсЁeКHЪjwGь4a"
    L"ЖрумгЦНZка,LвзbItB!ПSrpЯdЕпE3.ГvШC"
};

// Проверка: алфавит замены - перестановка исходного алфавита (та же длина, символы только из алфавита, без повторов)
bool validateKey(const wstring& alphabet, const wstring& key)
{
    if (alphabet.size() != key.size())
        return false;

    unordered_set<wchar_t> alphabetChars;
    unordered_set<wchar_t> keyChars;

    for (size_t i = 0; i < alphabet.size(); i++)
    {
        if (!alphabetChars.insert(alphabet[i]).second)
            return false;                  // повтор в алфавите
    }

    for (size_t i = 0; i < key.size(); i++)
    {
        wchar_t ch = key[i];

        if (alphabetChars.find(ch) == alphabetChars.end())
            return false;                  // символа нет в алфавите

        if (!keyChars.insert(ch).second)
            return false;                  // повтор в ключе
    }

    return true;
}

// true, если все символы текста есть в алфавите (bad - первый лишний)
bool checkText(const wstring& text, const wstring& alphabet,
    wchar_t& bad)
{
    for (size_t i = 0; i < text.size(); i++)
    {
        if (alphabet.find(text[i]) == wstring::npos)
        {
            bad = text[i];
            return false;
        }
    }

    return true;
}

// c_i = B_j[p]: j = (i - 1) mod d (i с 1), p - индекс символа в A
wstring encrypt(const wstring& text, const wstring& alphabet,
    const vector<wstring>& keys)
{
    wstring result;

    for (size_t i = 0; i < text.size(); i++)
    {
        size_t j = i % keys.size();     // i с 0, поэтому j = i mod d
        size_t pos = alphabet.find(text[i]);

        result += keys[j][pos];
    }

    return result;
}

// m_i = A[q], где q - индекс символа в строке B_j
wstring decrypt(const wstring& text, const wstring& alphabet,
    const vector<wstring>& keys)
{
    wstring result;

    for (size_t i = 0; i < text.size(); i++)
    {
        size_t j = i % keys.size();
        size_t pos = keys[j].find(text[i]);

        result += alphabet[pos];
    }

    return result;
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    wcout << L"   Лабораторная работа №2\n";
    wcout << L"   Многоалфавитная одноконтурная подстановка\n\n";

    // Проверка ключей
    for (size_t j = 0; j < keys.size(); j++)
    {
        if (!validateKey(alphabet, keys[j]))
        {
            wcout << L"Ошибка: некорректный алфавит замены B" << j << L"!\n";
            wcout << L"Проверьте длину и повторяющиеся символы.\n";
            return 1;
        }
    }

    while (true)
    {
        wcout << L"Выберите операцию:\n";
        wcout << L"1. Шифрование\n";
        wcout << L"2. Расшифрование\n";
        wcout << L"3. Выход\n";
        wcout << L"Ваш выбор: ";

        wstring choice;
        if (!getline(wcin, choice))
            break;

        if (choice == L"3")
            break;

        if (choice != L"1" && choice != L"2")
        {
            wcout << L"\nОшибка: выберите 1, 2 или 3!\n\n";
            continue;
        }

        wstring text;
        wchar_t bad = L' ';

        if (choice == L"1")
            wcout << L"\nВведите текст для шифрования: ";
        else
            wcout << L"\nВведите текст для расшифрования: ";

        if (!getline(wcin, text))
            break;

        if (text.empty())
        {
            wcout << L"\nОшибка: введена пустая строка!\n\n";
            continue;
        }

        if (!checkText(text, alphabet, bad))
        {
            wcout << L"\nОшибка: символ «" << bad << L"» отсутствует в алфавите!\n\n";
            continue;
        }

        if (choice == L"1")
            wcout << L"\nЗашифрованный текст: " << encrypt(text, alphabet, keys) << L"\n\n";
        else
            wcout << L"\nРасшифрованный текст: " << decrypt(text, alphabet, keys) << L"\n\n";
    }

    wcout << L"\nПрограмма завершена.\n";

    return 0;
}
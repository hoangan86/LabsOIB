#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cstdio>
#include <fcntl.h>
#include <io.h>

using namespace std;

// Исходный алфавит: 145 символов
const wstring alphabet =
L"АаБбВвГгДдЕеЁёЖжЗзИиЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщЪъЫыЬьЭэЮюЯя"
L"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
L"0123456789-+=()*№#!?., \"':;";

const vector<vector<wstring>> contours =
{
    // Контур 1 
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
        L"ЖрумгЦНZка,LвзbItB!ПSrpЯdЕпE3.ГvШC",
    },

    // Контур 2 
    {
        // C0
        L"дБrюлfИУщOёBq=жцs2bЁяЯш6IхPZ+0?Ц9iзПM"
        L"mDФ5пyнЗ8ЕEЖГку4оХ#йЛЩ)7*GК3ОeQz;№gcЭ"
        L"ыKВuМ,: adеLxгpр.tчoЙэRбvXиЮ-wСJНмCТS"
        L"\"FnЪЫWЧVс'YАДlъAвР(ШhтфUьаЬjNk1T!H",
        // C1
        L"ЖWиmwЙЁJъrMpDэj86ПHtSызg=(ёVв)СЛ'RAЧд"
        L"Юкf2.\"zгK#!ЪnyжуячdOFЦ0aпe,kИ4EВ-sГ 1"
        L"фмЫюvhlI*P;7ОЩЬНцcЯьасZтNУш№5РеGx+qbД"
        L"ЕА:охXХToYC?нБUuФй9ЭQЗТiBб3ШКрLлщМ",
        // C2
        L"cЛг№лзСoXWТБшмZhх OjйxЪ;'ё4рdЁэQrЧфВы"
        L"Pжк#ГCЭugS6юzЩАaiEVbBсtп*53ИЮ9дmk+Rsъ"
        L"вР2(:О.ФTJцоч1n0UКfЦНKХYqПFelЯЕМGДwЫУ"
        L"\"=Dтьyещ?LpMи,ЖяvN!7)IH8ЗбуЬнШаAЙ-",
        // C3
        L"i7Ъ'?ДкПGcjDьБ4УФфЖЛ0sб CРъd(ЮоwЕHдЯY"
        L"ItиЧe\"W2KpLqгFзМMZXюжSэхн)ЗgnvСЭйЁ,51"
        L"све+шцЩbпOКTХЙаыBhJ-м!ёЬНxЦШaтuoВ8k9A"
        L":яUчRE;mИ=щуО3l#*QГ6Тf.рЫyNzPл№rАV",
    }
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
            return false; // повтор в алфавите
    }

    for (size_t i = 0; i < key.size(); i++)
    {
        wchar_t ch = key[i];
        if (alphabetChars.find(ch) == alphabetChars.end())
            return false; // символа нет в алфавите
        if (!keyChars.insert(ch).second)
            return false; // повтор в ключе
    }
    return true;
}

// true, если все символы текста есть в алфавите (bad - первый лишний)
bool checkText(const wstring& text, const wstring& alphabet, wchar_t& bad)
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

// Один контур, шифрование: c_i = B_j[p], j = i mod d (i с 0), p - индекс символа в A
wstring encryptContour(const wstring& text, const wstring& alphabet,
    const vector<wstring>& keys)
{
    wstring result;
    for (size_t i = 0; i < text.size(); i++)
    {
        size_t j = i % keys.size();
        size_t pos = alphabet.find(text[i]);
        result += keys[j][pos];
    }
    return result;
}

// Один контур, расшифрование: m_i = A[q], где q - индекс символа в строке B_j
wstring decryptContour(const wstring& text, const wstring& alphabet,
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

// Шифрование: текст по очереди проходит через контуры 1, 2, ...
wstring encrypt(const wstring& text, const wstring& alphabet,
    const vector<vector<wstring>>& contours)
{
    wstring result = text;
    for (size_t k = 0; k < contours.size(); k++)
    {
        result = encryptContour(result, alphabet, contours[k]);
        wcout << L"После контура " << k + 1 << L": " << result << L"\n";
    }
    return result;
}

// Расшифрование: контуры применяются в обратном порядке (последний - первым)
wstring decrypt(const wstring& text, const wstring& alphabet,
    const vector<vector<wstring>>& contours)
{
    wstring result = text;
    for (size_t k = contours.size(); k > 0; k--)
    {
        result = decryptContour(result, alphabet, contours[k - 1]);
        wcout << L"После снятия контура " << k << L": " << result << L"\n";
    }
    return result;
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    wcout << L" Лабораторная работа №3\n";
    wcout << L" Многоалфавитная многоконтурная подстановка\n\n";

    // Проверка ключей
    for (size_t k = 0; k < contours.size(); k++)
    {
        for (size_t j = 0; j < contours[k].size(); j++)
        {
            if (!validateKey(alphabet, contours[k][j]))
            {
                wcout << L"Ошибка: некорректный алфавит замены в контуре " << k + 1 << L", номер " << j << L"!\n";
                wcout << L"Проверьте длину и повторяющиеся символы.\n";
                return 1;
            }
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

        wcout << L"\n";
        if (choice == L"1")
        {
            wstring result = encrypt(text, alphabet, contours);
            wcout << L"\nЗашифрованный текст: " << result << L"\n\n";
        }
        else
        {
            wstring result = decrypt(text, alphabet, contours);
            wcout << L"\nРасшифрованный текст: " << result << L"\n\n";
        }
    }

    wcout << L"\nПрограмма завершена.\n";
    return 0;
}
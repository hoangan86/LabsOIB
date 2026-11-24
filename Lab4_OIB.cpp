#include <iostream>
#include <string>
#include <io.h>
#include <fcntl.h>

using namespace std;

// Алфавит из 145 символов
const wstring ALPHABET =
L"АаБбВвГгДдЕеЁёЖжЗзИиЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщЪъЫыЬьЭэЮюЯя"
L"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
L"0123456789-+=()*№#!?., \"':;";

const int N = (int)ALPHABET.size(); // длина алфавита

// Ищет символ в алфавите, возвращает индекс (0..144) или -1, если символа нет
int indexOf(wchar_t c) {
    for (int i = 0; i < N; i++) {
        if (ALPHABET[i] == c) {
            return i;
        }
    }
    return -1;
}

// Проверка ключа: не пустой, все символы есть в алфавите, символы не повторяются
bool keyIsCorrect(const wstring& key) {
    if (key.empty()) {
        return false;
    }
    for (size_t i = 0; i < key.size(); i++) {
        if (indexOf(key[i]) == -1) {
            return false;
        }
        for (size_t j = i + 1; j < key.size(); j++) {
            if (key[i] == key[j]) {
                return false;
            }
        }
    }
    return true;
}

// Шифрование (decrypt = false) или расшифровка (decrypt = true)
wstring vigenere(const wstring& text, const wstring& key, bool decrypt) {
    wstring result = L"";

    for (size_t i = 0; i < text.size(); i++) {
        int p = indexOf(text[i]);

        if (p == -1) {
            // символ не из алфавита оставляем как есть
            result += text[i];
            continue;
        }

        // буква ключа выбирается по номеру символа в сообщении, ключ повторяется по кругу
        int k = indexOf(key[i % key.size()]);
        int r;
        if (!decrypt) {
            r = (p + k) % N;
        }
        else {
            r = (p - k + N) % N;
        }
        result += ALPHABET[r];
    }
    return result;
}

// Ввод ключа с клавиатуры, пока не будет введён правильный
wstring inputKey() {
    wstring key;
    while (true) {
        wcout << L"Введите ключевое слово (символы не должны повторяться): ";
        getline(wcin, key);
        if (keyIsCorrect(key)) {
            return key;
        }
        wcout << L"Ключ некорректный! Он не может быть пустым, содержать символы вне алфавита или повторяющиеся символы." << endl;
    }
}

int main() {
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);

    wcout << L" Лабораторная работа №4\n";
    wcout << L" Многоалфавитная подстановка по таблице Вижинера\n";

    while (true) {
        wcout << endl << L"Что желаете сделать?" << endl;
        wcout << L"1. Зашифровать" << endl;
        wcout << L"2. Расшифровать" << endl;
        wcout << L"0. Выход" << endl;

        wstring choice;
		wcout << L"Ваш выбор: ";
        getline(wcin, choice);
        if (!wcin) {
            break;
        }

        if (choice == L"0") {
            break;
        }
        else if (choice == L"1" || choice == L"2") {
            wstring text;
            if (choice == L"1") {
                wcout << L"\nВведите текст для шифрования: ";
            }
            else {
                wcout << L"\nВведите текст для расшифровки: ";
            }
            getline(wcin, text);
            wstring key = inputKey();

            if (choice == L"1") {
                wcout << L"\nЗашифрованный текст: " << vigenere(text, key, false) << endl;
            }
            else {
                wcout << L"\nРасшифрованный текст: " << vigenere(text, key, true) << endl;
            }
        }
        else {
            wcout << L"Некорректный ввод! Введите 1, 2 или 0." << endl;
        }
    }

    wcout << L"\nПрограмма завершена.\n";
    return 0;
}
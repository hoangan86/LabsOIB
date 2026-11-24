#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <io.h>
#include <fcntl.h>

using namespace std;

// Символ, которым дополняется последний неполный блок
const wchar_t PAD = L'_';

// Ввод целого числа больше 0, повторяется, пока не будет введено правильное значение
int inputPositive(const wstring& prompt) {
    while (true) {
        wcout << prompt;
        wstring line;
        getline(wcin, line);
        wstringstream ss(line);
        int value;
        wchar_t extra;
        // число прочитано, после него ничего нет, и оно больше 0
        if (ss >> value && !(ss >> extra) && value > 0) {
            return value;
        }
        wcout << L"Некорректный ввод! Введите целое число больше 0." << endl;
    }
}

// Проверка ключа: ровно n чисел, каждое от 1 до n, числа не повторяются
bool keyIsCorrect(const vector<int>& key, int n) {
    if ((int)key.size() != n) {
        return false;
    }
    vector<bool> used(n + 1, false); // used[k] = true, если число k уже встречалось
    for (int k : key) {
        if (k < 1 || k > n || used[k]) {
            return false;
        }
        used[k] = true;
    }
    return true;
}

// Ввод ключа с клавиатуры, пока не будет введён правильный
vector<int> inputKey(int n) {
    while (true) {
        wcout << L"Введите ключ из " << n << L" чисел через пробел (например: 3 1 4 2): ";
        wstring line;
        getline(wcin, line);
        wstringstream ss(line);
        vector<int> key;
        int k;
        while (ss >> k) {
            key.push_back(k);
        }
        // если чтение остановилось не в конце строки, значит встретилось не число
        if (ss.eof() && keyIsCorrect(key, n)) {
            return key;
        }
        wcout << L"Ключ некорректный! Нужно " << n << L" разных чисел от 1 до " << n << L"." << endl;
    }
}

// Номер столбца (с 0), над которым в ключе стоит число num
int findColumn(const vector<int>& key, int num) {
    for (int j = 0; j < (int)key.size(); j++) {
        if (key[j] == num) {
            return j;
        }
    }
    return -1; // не встретится, так как ключ уже проверен
}

// Дополнение текста символами PAD до длины, кратной размеру блока
wstring addPadding(wstring text, int blockSize) {
    while (text.size() % blockSize != 0) {
        text += PAD;
    }
    return text;
}

// Удаление символов дополнения в конце текста
wstring removePadding(wstring text) {
    while (!text.empty() && text.back() == PAD) {
        text.pop_back();
    }
    return text;
}

// Пробел в таблице показывается точкой, чтобы его было видно
wchar_t showChar(wchar_t c) {
    if (c == L' ') {
        return L'·';
    }
    return c;
}

// Вывод таблиц всех блоков на экран
void printBlocks(const wstring& text, int rows, int cols, const vector<int>& key) {
    int blockSize = rows * cols;
    int blockCount = (int)text.size() / blockSize;

    wcout << L"\nПорядок чтения столбцов:";
    for (int num = 1; num <= cols; num++) {
        wcout << L" " << findColumn(key, num) + 1;
    }
    wcout << L"\n(пробел показан символом «·»)" << endl;

    for (int b = 0; b < blockCount; b++) {
        // строка с числами ключа над столбцами
        wcout << L"Ключ   |";
        for (int j = 0; j < cols; j++) {
            wcout << setw(3) << key[j] << L" |";
        }
        wcout << endl;

        // разделительная линия
        wcout << L"-------+";
        for (int j = 0; j < cols; j++) {
            wcout << L"----+";
        }
        wcout << endl;

        // строки таблицы
        for (int r = 0; r < rows; r++) {
            wcout << L"Стр." << setw(2) << r + 1 << L" |";
            for (int j = 0; j < cols; j++) {
                wcout << L"  " << showChar(text[b * blockSize + r * cols + j]) << L" |";
            }
            wcout << endl;
        }
    }
}

// Шифрование простой перестановкой (текст уже дополнен до полного блока)
wstring encrypt(const wstring& text, int rows, int cols, const vector<int>& key) {
    int blockSize = rows * cols;
    wstring result = L"";
    for (size_t start = 0; start < text.size(); start += blockSize) {
        // столбцы читаются в порядке чисел ключа: 1, 2, ..., cols
        for (int num = 1; num <= cols; num++) {
            int col = findColumn(key, num);
            // столбец читается сверху вниз
            for (int r = 0; r < rows; r++) {
                result += text[start + r * cols + col];
            }
        }
    }
    return result;
}

// Расшифровка: части криптограммы записываются обратно в столбцы
// (результат возвращается вместе с символами дополнения)
wstring decrypt(const wstring& cipher, int rows, int cols, const vector<int>& key) {
    int blockSize = rows * cols;
    wstring result(cipher.size(), L' ');
    for (size_t start = 0; start < cipher.size(); start += blockSize) {
        size_t pos = start; // текущий символ криптограммы
        for (int num = 1; num <= cols; num++) {
            int col = findColumn(key, num);
            for (int r = 0; r < rows; r++) {
                result[start + r * cols + col] = cipher[pos];
                pos++;
            }
        }
    }
    return result;
}

int main() {
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);

    wcout << L" Лабораторная работа №5\n";
    wcout << L" Простая перестановка\n";

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
            if (text.empty()) {
                wcout << L"Текст не может быть пустым!" << endl;
                continue;
            }

            int rows = inputPositive(L"Введите количество строк: ");
            int cols = inputPositive(L"Введите количество столбцов: ");
            vector<int> key = inputKey(cols);

            if (choice == L"1") {
                wstring full = addPadding(text, rows * cols);
                printBlocks(full, rows, cols, key);
                wcout << L"\nЗашифрованный текст: [" << encrypt(full, rows, cols, key) << L"]" << endl;
            }
            else {
                // длина криптограммы всегда кратна размеру блока
                if (text.size() % (rows * cols) != 0) {
                    wcout << L"Ошибка: длина криптограммы (" << text.size() << L") не кратна размеру блока (" << rows * cols << L")." << endl;
                    continue;
                }
                wstring plain = decrypt(text, rows, cols, key);
                printBlocks(plain, rows, cols, key);
                wcout << L"\nРасшифрованный текст: [" << removePadding(plain) << L"]" << endl;
            }
        }
        else {
            wcout << L"Некорректный ввод! Введите 1, 2 или 0." << endl;
        }
    }

    wcout << L"\nПрограмма завершена.\n";
    return 0;
}
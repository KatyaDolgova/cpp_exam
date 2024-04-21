#include <iostream>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <algorithm>
using namespace std;

const int boardSize = 4; // размер игровой доски
constexpr int totalPairs = (boardSize * boardSize) / 2; //общее количество пар
const int maxGuesses = 10; //максимальное количество попыток
int sum = 0;
constexpr int sizeArr = totalPairs * 2;

int Random(int min, int max) {
    return min + rand() % (max - min + 1);
}

void randBoard(int board[][boardSize]) {

    int nums[sizeArr];
    int generatedPairs = 0;
    int availablePairs = totalPairs; // переменная для отслеживания доступных пар
    while (generatedPairs < totalPairs) {
        int num = Random(1, totalPairs);
        //Проверяем, что число еще не добавлено в массив
        bool found = false;
        for (int i = 0; i < generatedPairs; i++) {
            if (nums[i] == num) {
                found = true;
                break;
            }
        }
        // Если число не найдено, добавляем его в массив дважды (пару)
        if (!found) {
            nums[generatedPairs] = num;
            nums[generatedPairs + 1] = num;
            generatedPairs += 2;
        }
    }

    // Заполняем игровую доску случайными числами из массива nums
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            int randomIndex = Random(0, totalPairs - 1);
            board[i][j] = nums[randomIndex];
            // Обмениваем выбранное число с последним элементом массива
            swap(nums[randomIndex], nums[availablePairs - 1]);
            availablePairs -= 2;
        }
    }
}
void printBoard(int board[][boardSize], bool revealed[][boardSize]) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (revealed[i][j]) {
                cout << board[i][j] << " ";
            }
            else {
                cout << "* ";
            }
        }
        cout << endl;
    }
}
bool isGameOver(bool revealed[][boardSize]) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (!revealed[i][j]) {
                return false;
            }
        }
    }
    return true;
}
void selectCell(int& row, int& col) {
    cout << "Введите координаты ячейки (строка столбец): ";
    cin >> row >> col;
    row--;
    col--;
}
//музыка
void beep(int frequency, int duration)
{
    Beep(frequency, duration);
}

//выполнение одного хода игры
void playTurn(int board[][boardSize], bool revealed[][boardSize], int& remainingPairs, int& remainingGuesses) {
    int row1, col1, row2, col2;
    selectCell(row1, col1);
    // Проверка, что координаты находятся в допустимых пределах
    if (row1 < 0 || row1 >= boardSize || col1 < 0 || col1 >= boardSize || revealed[row1][col1]) {
        cout << "Некорректные координаты" << endl;
        return;
    }
    // Помечаем первую ячейку как открытую
    revealed[row1][col1] = true;
    printBoard(board, revealed);
    selectCell(row2, col2);
    if (row2 < 0 || row2 >= boardSize || col2 < 0 || col2 >= boardSize || revealed[row2][col2]) {
        cout << "Некорректные координаты" << endl;
        return;
    }
    revealed[row2][col2] = true;
    printBoard(board, revealed);
    if (board[row1][col1] == board[row2][col2]) {
        
        cout << "Пара найдена!" << endl;
        remainingPairs--;
        sum++;
        int melody[] = { 2000, 2500, 3000, 3500 };
        int durations[] = { 200, 300, 400, 500 };
        for (int i = 0; i < sizeof(melody) / sizeof(int); i++)
        {
            beep(melody[i], durations[i]);
        }
        cout << "\nНажмите на клавишу для продолжения";
        _getch();
        system("cls");

    }
    else {
        cout << "Неправильная пара" << endl;
        remainingGuesses--;
        sum++;
        int melody[] = { 500, 1000, 1500, 2000 };
        int durations[] = { 100, 200, 300, 400 };
        for (int i = 0; i < sizeof(melody) / sizeof(int); i++)
        {
            beep(melody[i], durations[i]);
        }
        revealed[row1][col1] = false;
        revealed[row2][col2] = false;
        cout << "\nНажмите на клавишу для продолжения";
        _getch();
        system("cls");
    }
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    cout << "===============================\n";
    cout << "Добро пожаловать в игру Память!\n";
    cout << "===============================\n";
    cout << "Смысл игры заключается в том, чтобы найти одинаковые символы\n";

    int time = 0;
    int board[boardSize][boardSize];
    bool revealed[boardSize][boardSize] = { false };

    randBoard(board);
    int remainingPairs = totalPairs;
    int remainingGuesses = maxGuesses;
    time += clock();
    while (true) {
        
        printBoard(board, revealed);
        if (isGameOver(revealed)) {
            cout << "Вы выиграли!" << endl;
            break;
        }
        if (remainingGuesses == 0) {
            cout << "Вы проиграли!" << endl;
            break;
        }
        cout << "Осталось попыток: " << remainingGuesses << endl;
        playTurn(board, revealed, remainingPairs, remainingGuesses);
    }
    int end = clock();
    int t = (end - time) / CLOCKS_PER_SEC;
    cout << "Количество попыток: " << sum << endl;
    cout << "Затраченное время: " << t << " секунд";
    return 0;
}
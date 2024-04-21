#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <string>
#include <map>
#include <iomanip>

using namespace std;
// Класс для хранения информации о затратах
class Expense
{
public:
    string category; // категория
    double amount;   // сумма
    time_t date;     // дата

    bool operator<(const Expense& exp) const // Сравнение значений для сортировки
    {
        return (date < exp.date); //переопределение оператора
    }
};
// Класс для хранения данных о кошельке
class Wallet
{
public:
    string name;
    double balance;
};

class Report
{
private:
    map<string, double> amountByCategory; //объект-отчет, в нем хранится словарь [категория]: сумма
    map<string, double>::iterator _i; //Этот член класса хранит итератор для обхода (поиска и модификации) элементов в словаре.


public:

    void clear() //Метод  отчищает отчет, так как создать отчет можно несколько раз подряд, предыдущие значения лучше отчищать.
    {
        amountByCategory.clear();
    }
    void add(string& categoryName, double& amount) //Метод  ищет категорию, если ее нет, создает новую и добавляет значение, если есть, то прибавляет
    {
        _i = amountByCategory.find(categoryName);
        if (_i == amountByCategory.end())
            amountByCategory[categoryName] = amount;
        else
            amountByCategory[categoryName] += amount;
    }

    string ToString() //Метод выводит строку
    {

        string result = "";
        for (const auto& [category, amount] : amountByCategory)
            result += category + ": " + to_string(amount) + '\n';
        return result;
    }
};

// функция для ковертирования строки в timestamp
time_t convertStringToDate(string& raw) //принимает строку raw в качестве входного аргумента и возвращает временную метку типа time_t
{
    tm timeInfo = {}; //Эта строка создает объект структуры `tm`, который представляет структурированное время, и инициализирует его 
    //значениями по умолчанию. Эта структура будет использоваться для хранения даты и времени после преобразования.
    get_time(&timeInfo, "%d.%m.%Y"); // вызов функции для форматирования даты и времени.
    return mktime(&timeInfo); // вызов функции для форматирования даты и времени.
}


// класс для управления кошельками и картами
class WalletsManager
{
private:
    vector<Wallet> wallets;   // кошельки и карты
    vector<Expense> expenses; // затраты
    Report report; //экземпляр класса для отчета

public:
    // функция добавления карты
    void addNewWallet(string name)
    {
        wallets.push_back({ name, 0.0 });
    }
    // метод для проверки корректности пользовательского ввода
    bool checkWalletNum(const int& num)
    {
        if (num > 0 && num <= wallets.size())
            return true;
        return false;
    }
    // метод для получения списка кошельков  нужно для того, чтоб пользователь мог выбирать куда ему вносить изменения  
    string getWalletsListAsString()
    {
        string result;
        if (wallets.size() == 0)
            result = "";
        else
        {
            result = "Выберите карту из списка:\n";
            for (int i = 0; i < wallets.size(); i++)
                result += (to_string(i + 1)) + ". " + wallets[i].name + "\n";
        }
        return result;
    }
    // Вывод баланса
    double getWalletBalance(const int& num)
    {
        return wallets[num - 1].balance;
    }

    // метод для пополнения кошельков и карт
    void topUpWallet(const int& num, const double& amount)
    {
        wallets[num - 1].balance += amount;
    }

    // метод для внесения затрат  обработка обращается по индексу к кошельку
    void addExpense(const int& num, const string& category, double amount, time_t date)
    {
        wallets[num - 1].balance -= amount;
        expenses.push_back({ category, amount, date });
        sort(expenses.begin(), expenses.end()); //при добавлении новый траты каждый раз сортируем массив по дате
    }

    string generateReport(const time_t& startDate, const time_t& stopDate) // метод получает даты и проходится по тратам
    {
        if (expenses.empty()) {
            return "Трат за указанный период нет";
        }

        int i = 0;
        for (; i < expenses.size(); i++) {
            if (startDate <= expenses[i].date) {
                break;
            }
        }

        if (i == expenses.size()) {
            return "Трат за указанный период нет";
        }

        report.clear();
        while (expenses[i].date <= stopDate && i < expenses.size()) {
            report.add(expenses[i].category, expenses[i].amount);
            i++;
        }

        return report.ToString();
    }
};
void generateReportMenu(WalletsManager& walletsManager, string& message)
{
    int period;
    string strDate;
    time_t startDate, stopDate;

    cout << "Введите дату начала периода в формате (ДД.ММ.ГГГГ):" << endl;
    cin >> strDate;
    startDate = convertStringToDate(strDate);

    while (true) {
        cout << "Выберите период:" << endl
            << "1. За день" << endl
            << "2. За неделю" << endl
            << "3. За месяц" << endl
            << "0. Закрыть" << endl;
        cin >> period;

        if (period == 1 || period == 2 || period == 3 || period == 0) {
            switch (period) {
            case 0:
                message = "";
                return;
            case 1:
                stopDate = startDate + 86400;
                break;
            case 2:
                stopDate = startDate + 604800;
                break;
            case 3:
                stopDate = startDate + 2629743;
                break;
            default:
                break;
            }

            message = walletsManager.generateReport(startDate, stopDate);
            return;
        }
        else {
            cout << "Некорректный ввод, попробуйте еще раз" << endl;
        }
    }
}

void topUpWalletMenu(WalletsManager& walletsManager, string& message)
{
    string list = walletsManager.getWalletsListAsString();
    if (list == "")
        message = "Кошельков не найдено. Создайте новый и попробуйте снова.";
    else
    {
        int walletNum;
        int amount;
        cout << list;
        while (true)
        {
            cin >> walletNum;
            if (walletsManager.checkWalletNum(walletNum))
                break;
            else
                cout << "Некорректный ввод, попробуйте еще раз" << endl;
        }
        cout << "Введите сумму, на которую хотите пополнить:" << endl;
        cin >> amount;
        walletsManager.topUpWallet(walletNum, amount);
        message = "Карта пополнена";
    }
}
// функция для вывода баланса кошелька
void displayWalletBalance(WalletsManager& walletsManager, string& message)
{
    string list = walletsManager.getWalletsListAsString();
    if (list == "")
        message = "Кошельков не найдено. Создайте новый и попробуйте снова.";
    else
    {
        int walletNum;
        int amount;
        cout << list;
        while (true)
        {
            cin >> walletNum;
            if (walletsManager.checkWalletNum(walletNum))
                break;
            else
                cout << "Некорректный ввод, попробуйте еще раз" << endl;
        }
        message = "Баланс карты = " + to_string(walletsManager.getWalletBalance(walletNum)) + '\n';
        return;

    }
}


void addNewWalletMenu(WalletsManager& walletsManager, string& message)
{
    string walletName;
    cout << "Введите название карты:" << endl;
    cin >> walletName;
    walletsManager.addNewWallet(walletName);
    message = "Карта с именем " + walletName + " добавлена";
}

void addExpenseMenu(WalletsManager& walletsManager, string& message)
{
    string list = walletsManager.getWalletsListAsString();
    if (list == "")
        message = "Кошельков не найдено. Создайте новый и попробуйте снова.";
    else
    {
        int walletNum, amount;
        string category, strDate;
        time_t date;
        cout << list;
        while (true)
        {
            cin >> walletNum;
            if (walletsManager.checkWalletNum(walletNum))
                break;
            else
                cout << "Некорректный ввод, попробуйте еще раз" << endl;
        }
        cout << "Введите сумму расходов:" << endl;
        cin >> amount;
        cout << "Введите категорию (еда, транспорт и тд):" << endl;
        cin >> category;
        cout << "Введите дату расходов в формате (ДД.ММ.ГГГГ):" << endl;
        cin >> strDate;
        date = convertStringToDate(strDate);
        walletsManager.addExpense(walletNum, category, amount, date);
        message = "Трата добавлена";
    }
}

void printMenu(string& message)
{
    system("cls");
    cout << message << endl;
    cout << "-----------------------------" << endl;
    cout << "           М Е Н Ю           " << endl;
    cout << "-----------------------------" << endl;
    cout << "1. Добавить карту" << endl;
    cout << "2. Пополнить баланс" << endl;
    cout << "3. Внесение затрат" << endl;
    cout << "4. Посмотреть баланс" << endl;
    cout << "5. Отчеты по затратам и категориям" << endl;
    cout << "0 - Выход" << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    WalletsManager manager;
    int menu;
    string message = "";
    while (true)
    {
        printMenu(message);
        cin >> menu;
        switch (menu)
        {
        case 0:
            return 0;
        case 1:
            addNewWalletMenu(manager, message);
            break;
        case 2:
            topUpWalletMenu(manager, message);
            break;
        case 3:
            addExpenseMenu(manager, message);
            break;
        case 4: {
            displayWalletBalance(manager, message);
        }
        case 5: {
            generateReportMenu(manager, message);
            break;
        }
        default:
            message = "Некорректный ввод";
            break;
        }

    }
}
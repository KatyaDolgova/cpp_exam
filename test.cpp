#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;


// Класс для представления вопросов и ответов
class Question {
public:
    string questionText; //вопрос
    vector<string> options; //варианты ответов
    int correctAnswerIndex; //индекс правильного ответа

    Question(const string& text, const vector<string>& opts, int correct) : questionText(text), options(opts), correctAnswerIndex(correct) {}//конструктор
};
// Класс для представления тестов
class Test {
public:
    string testName; //имя теста
    vector<Question> questions; //вектор вопросов
    Test(const string& name) : testName(name) {} //конструктор
    Test() :Test("") {} //констркетор по умолчанию
    void addQuestion(const string& text, const vector<string>& opts, int correct) {
        questions.push_back(Question(text, opts, correct));
    }
    string getTestName() const {
        return testName;
    }
};
//класс результатов
class Result {
public:
    string username; //имя пользователя
    string testName; //имя теста
    int correctAnswers; //правильные ответы
    int totalQuestions; //количество вопросов 
    double score; //процент правильных ответов
    int grade; //оценка

    //конструктор
    Result(string uname, string test, int correct, int total, double sc, int grade) :
        username(uname), testName(test), correctAnswers(correct), totalQuestions(total), score(sc), grade(grade) {}
};
//класс пользователя
class User {
public:
    string username; //имя пользователя
    string password; //пароль
    string fullName;//логин
    string address;//адрес
    string phone;//телефон
    int role;  // 0 тестируемый, 1 админ
    //конструктор
    User(string uname, string pwd, string name, string addr, string ph, int r) :
        username(uname), password(pwd), fullName(name), address(addr), phone(ph), role(r) {}
    //конструктор по умолчанию
    User() :User("", "", "", "", "", 0) {}
    //функция возвращающая имя пользователя (геттер)
    string getUsername() { return username; }
};
//класс реализации
class TestingSystem {
private:
    map<string, vector<Test>> categories;
    vector<Result> results; //результаты
    User admin; //пользователь админ
public:
    vector<User> users; //пользователи
    // Конструктор класса TestingSystem
    TestingSystem() {
        // инициализация данных админа
        admin = User("admin", "password", "", "", "", 1);
        users.push_back(admin);
    }
    void addCategory(const string& categoryName) { //функция добавления категори 
        //принимает строку categoryName, которая представляет название категории, и использует ее в качестве ключа для добавления новой категории в словарь 
        //categories.Каждая категория связывается с вектором тестов.
        categories[categoryName] = vector<Test>(); //categories[categoryName] создает новую запись в словаре с ключом 
        //categoryName и связывает его с пустым вектором тестов с помощью оператора присваивания = vector<Test>().
    }
    void addTestToCategory(const string& categoryName, const string& testName) {
        categories[categoryName].push_back(Test(testName)); //Test(testName) создает объект Test с именем testName push_back добавляет этот созданный объект 
        //Test в конец вектора тестов, хранящегося в категории с именем categoryName
    }
    void addQuestionToTest(const string& categoryName, const string& testName, const string& question, const vector<string>& options, int correctIndex) {
        if (categories.find(categoryName) != categories.end()) {//проверка существования категории с указанным именем в карте `categories`
            vector<Test>& tests = categories[categoryName]; //создает ссылку `tests` на вектор тестов в указанной категории.
            for (Test& test : tests) { // цикл проходит через каждый тест в векторе тестов данной категории
                if (test.testName == testName) { //проверяет, соответствует ли название текущего теста имени теста, в который нужно добавить вопрос.
                    test.addQuestion(question, options, correctIndex); //создает новый объект `Question` и добавляет его к текущему тесту
                    break;
                }

            }
        }
    }
    void printAllCategories() {
        cout << "Доступные категории:" << endl;
        for (auto const& category : categories) { //цикл проходит через все элементы карты categories, где category представляет пару ключ-значение в этой карте
            cout << category.first << endl; // Метод `first` используется для доступа к ключу в каждой паре в карте
        }
    }
    void printAllTestsInCategory(const string& categoryName) {
        if (categories.find(categoryName) != categories.end()) {
            cout << "Тесты в категории '" << categoryName << "':" << endl;
            for (const Test& test : categories[categoryName]) {
                cout << test.getTestName() << endl;
            }
        }
    }
    void displayQuestion(const Question& question) {
        cout << "Вопрос: " << question.questionText << endl;
        cout << "Варианты ответов:" << endl;
        for (size_t i = 0; i < question.options.size(); i++) {
            cout << i + 1 << ". " << question.options[i] << endl;
        }
    }
    void takeTest(const string& categoryName, const string& testName, const string& username) {
        if (categories.find(categoryName) != categories.end()) {
            for (Test& test : categories[categoryName]) {
                if (test.testName == testName) {
                    for (User& user : users) {
                        if (user.username == username) {
                            int correctAnswers = 0;
                            for (const Question& question : test.questions) {
                                displayQuestion(question);
                                int userAnswer;
                                cout << "Введите номер правильного ответа: ";
                                cin >> userAnswer;
                                if (userAnswer == question.correctAnswerIndex + 1) {
                                    correctAnswers++;
                                }
                            }
                            double score = (static_cast<double>(correctAnswers) / test.questions.size()) * 100;
                            int grade = calculateGrade(score); // Определение оценки на основе количества правильных ответов
                            Result userResult(username, test.testName, correctAnswers, test.questions.size(), score, grade);
                            results.push_back(userResult);
                            cout << "Результаты теста '" << test.testName << "':" << endl;
                            cout << "Правильные ответы: " << correctAnswers << " из " << test.questions.size() << endl;
                            cout << "Правильных ответов: " << score << " % " << endl;
                            cout << "Оценка: " << grade << endl;
                            return;
                        }
                    }
                    cout << "Пользователь не найден." << endl;
                    return;
                }
            }
            cout << "Тест не найден." << endl;
            return;
        }
        cout << "Категория не найдена." << endl;
    }

    int calculateGrade(double score) {
        if (score >= 95.0) {
            return 12;
        }
        else if (score >= 90.0) {
            return 11;
        }
        else if (score >= 85.0) {
            return 10;
        }
        else if (score >= 80.0) {
            return 9;
        }
        else if (score >= 75.0) {
            return 8;
        }
        else if (score >= 70.0) {
            return 7;
        }
        else if (score >= 65.0) {
            return 6;
        }
        else if (score >= 60.0) {
            return 5;
        }
        else if (score >= 55.0) {
            return 4;
        }
        else if (score >= 50.0) {
            return 3;
        }
        else if (score >= 30.0) {
            return 2;
        }
        else {
            return 1;
        }
    }
    // Функция для удаления пользователя

    void deleteUser() {
        printUsers();
        string username;
        cout << "Введите имя: ";
        cin >> username;
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (it->username == username) {
                users.erase(it);
                break;
            }
        }
        system("cls");
        cout << endl << "Пользователь удален." << endl;
    }

    // Функция для добавления пользователя
    void addUser() {
        string username, password, fullName, address, phone;
        int role = 0;
        cout << "Введите имя: "; //заполнение экземпляра
        cin >> username;
        cout << "Введите пароль: ";
        cin >> password;
        cout << "Введите логин: ";
        cin >> fullName;
        cout << "Введите адрес: ";
        cin >> address;
        cout << "Введите телефон: ";
        cin >> phone;

        users.push_back(User(username, password, fullName, address, phone, role));
        system("cls");
        cout << "Пользователь добавлен." << endl;
    }

    // Функция для изменения информации о пользователе
    void editUser() {
        printUsers();
        string chekusername, newPassword, newFullName, newAddress, newPhone;
        int role = 0;
        cout << "Введите имя: "; //заполнение экземпляра
        cin >> chekusername;
        cout << "Введите новый пароль: ";
        cin >> newPassword;
        cout << "Введите новый логин: ";
        cin >> newFullName;
        cout << "Введите новый адрес: ";
        cin >> newAddress;
        cout << "Введите новый телефон: ";
        cin >> newPhone;
        for (User& user : users) {
            if (user.username == chekusername) {
                user.password = newPassword;
                user.fullName = newFullName;
                user.address = newAddress;
                user.phone = newPhone;
                break;
            }
        }
        system("cls");
        cout << endl << "Данные изменены." << endl;
    }
    void viewStatistics() {
        int totalUsers = users.size();
        int totalTests = 0;
        for (const auto& category : categories) {
            totalTests += category.second.size();
        }
        double totalScore = 0.0;
        int totalResults = results.size();
        for (const auto& result : results) {
            totalScore += result.score;
        }
        double averageScore = (totalResults > 0) ? (totalScore / totalResults) : 0.0;

        cout << "Статистика системы тестирования:" << endl;
        cout << "Общее количество пользователей: " << totalUsers << endl;
        cout << "Общее количество тестов: " << totalTests << endl;
        cout << "Общее количество результатов: " << totalResults << endl;
        cout << "Средний балл по результатам: " << averageScore << endl;
    }

    // Функции для пользователя
    void registerUser(const string& uname, const string& pwd, const string& name, const string& addr, const string& ph);//функция регистрации пользователя
    bool login(const string& uname, const string& pwd); //функция для входа пользователя
    void viewPreviousResults(const string& uname); //функция результатов пользователя
    // Функции для администратора
    void setAdminCredentials(); //функция входа админа
    void changeAdminCredentials(const string& newUname, const string& newPwd); //функция для изменения данных админа 
    void manageTestingSystem(const int& action, const string& categoryName = "", const string& testName = "", const string& question = "", const vector<string>& options = {}, int correctIndex = 0); //функция для редактирования тестов
    void printUsers(); //вывод пользователей

};
void TestingSystem::printUsers() {
    for (const auto& user : users) {
        cout << user.username << ", " << user.password << ", " << user.fullName << ", " << user.address << ", " << user.phone << endl;
    }
}


void TestingSystem::registerUser(const string& uname, const string& pwd, const string& name, const string& addr, const string& ph) {
    for (const auto& user : users) {
        if (user.username == uname) {
            system("cls");
            cout << "Пользователь с таким логином уже существует." << endl;
            return;
        }
    }
    User newUser(uname, pwd, name, addr, ph, 0);  // Создание нового пользователя с ролью "гость"
    users.push_back(newUser);
    system("cls");
    cout << "Регистрация завершена." << endl;
}

bool TestingSystem::login(const string& uname, const string& pwd) {
    while (true) {
        for (auto& user : users) {
            if (user.username == uname && user.password == pwd) {
                cout << "Добро пожаловать, " << user.fullName << " !" << endl;
                return false;
            }
        }
        cout << "Неправильный логин или пароль. Попробуйте еще раз." << endl;
        return true;
    }
}

void TestingSystem::viewPreviousResults(const string& uname) {
    for (const auto& result : results) {
        if (result.username == uname) {
            system("cls");
            cout << "Тест: " << result.testName << ", правильные ответы: " << result.correctAnswers
                << ", общее количество вопросов: " << result.totalQuestions << ", оценка: " << result.score << endl;
        }
        else cout << "Предыдущие результаты тестирования отсутствуют." << endl;
    }

}

void TestingSystem::setAdminCredentials() {
    string uname;
    string pwd;
    while (true) {

        cout << "Введите имя: ";
        cin >> uname;
        cout << "Введите пароль: ";
        cin >> pwd;
        if (admin.username.empty() && admin.password.empty()) {
            User newAdmin(uname, pwd, "", "", "", 1);  // Создание нового администратора
            admin = newAdmin;
            system("cls");
            cout << "Учетные данные администратора установлены." << endl;
            return;
        }
        else {
            if (admin.username == uname && admin.password == pwd) {
                system("cls");
                cout << "Вход выполнен. " << endl;
                return;
            }
            else {
                system("cls");
                cout << "Неверные данные, попробуйте ещё раз." << endl;
            }
        }
    }
}
void TestingSystem::manageTestingSystem(const int& action, const string& categoryName, const string& testName, const string& question, const vector<string>& options, int correctIndex) {

    if (action == 1) {
        addCategory(categoryName);
        system("cls");
        cout << "Категория '" << categoryName << "' добавлена" << endl;
    }
    else if (action == 2) {
        addTestToCategory(categoryName, testName);
        system("cls");
        cout << "Тест '" << testName << "' добавлен в категорию '" << categoryName << "'" << endl;
    }
    else if (action == 3) {
        addQuestionToTest(categoryName, testName, question, options, correctIndex);
        system("cls");
        cout << "Вопрос добавлен в тест '" << testName << "' из категории '" << categoryName << "'" << endl;
    }
    else {
        cout << "Недопустимое действие для администратора" << endl;
    }
}

void TestingSystem::changeAdminCredentials(const string& newUname, const string& newPwd) {
    if (!users.empty() && admin.username != "" && admin.password != "") {
        admin.username = newUname;
        admin.password = newPwd;
        system("cls");
        cout << "Учетные данные администратора изменены." << endl;

    }
    else {
        cout << "Невозможно изменить учетные данные администратора." << endl;
    }
}

void main() {
    setlocale(LC_ALL, "rus");

    TestingSystem systemTest; //экземпляр класса
    systemTest.addCategory("Math"); //добавление категории
    systemTest.addTestToCategory("Math", "Algebra"); //добавление теста в категорию
    systemTest.addQuestionToTest("Math", "Algebra", "Сколько будет 2 плюс 2?", { "3", "4", "5" }, 1); //добавление вопросов в тест
    systemTest.addQuestionToTest("Math", "Algebra", "Чему равен корень из 16?", { "4", "8", "2" }, 0);//добавление вопросов в тест

    systemTest.addCategory("Physics");
    systemTest.addTestToCategory("Physics", "Mechanics");
    systemTest.addQuestionToTest("Physics", "Mechanics", "Формула выражения механической работы", { " A=F x V", "A=F x S", "A=F x S" }, 1);
    systemTest.addQuestionToTest("Physics", "Mechanics", "Неравномерное движение бывает:?", { " равноускоренное", "равнозамедленное", "равноускоренное и равнозамедленное" }, 2);
    while (true) {
        system("cls");
        int person;
        cout << "Вход как: \n1. Администратор \n2. Пользователь\n";
        cin >> person;


        if (person == 1) {

            systemTest.setAdminCredentials(); //регистрация или вход
            bool EXIT = true;
            int menu;
            while (EXIT) {
                string show = "----------------------------- \n           М Е Н Ю           \n-----------------------------\n1. Изменить данные админа\n2. Управление пользователями\n3. Просмотр статистики\n4. Управление тестированием\n5 - Выход\n";
                cout << show;
                cin >> menu; //выбор действия
                switch (menu) {
                case 0: break;
                case 1: {
                    system("cls");
                    string nName;
                    string nPas;
                    cout << "Введите новое имя: ";
                    cin >> nName;
                    cout << "Введите новый пароль: ";
                    cin >> nPas;
                    systemTest.changeAdminCredentials(nName, nPas); //изменение данных админа
                    break;
                }
                case 2: {
                    system("cls");
                    int action;
                    bool exit = true;
                    while (exit) {
                        string text = "1. Создать пользователя\n2. Удалить пользователя\n3. Изменить информацию о пользователе\n4 - Выход\n";
                        //ShowMenu(text);
                        cout << text;
                        cin >> action;
                        switch (action) {

                        case 1: { systemTest.addUser(); //создаётся пользователь user
                            break; }
                        case 2: { systemTest.deleteUser();//удаляется пользователь user
                            break; }
                        case 3: {
                            systemTest.editUser();
                            break; }
                        case 4: {exit = false; break; }

                        default: cout << "Некорректный ввод, попробуйте еще раз" << endl;
                        }
                    }

                    break;
                }
                case 3: {
                    system("cls");
                    systemTest.viewStatistics();//функция просмотра статистики
                    break;
                }
                case 4: {
                    system("cls");
                    int action;
                    bool exit = true;
                    while (exit) {
                        string menu = "1. Добавить категорию\n2. Добавить тест в категорию\n3. Добавить вопросы к тесту\n4 - Выход\n";
                        cout << menu;
                        //ShowMenu(menu);
                        cin >> action;
                        string categoryName, testName, question;
                        vector<string> options; //вектор правильных ответов
                        int correctIndex; //индекс правильного ответа




                        switch (action) {
                        case 1: {
                            systemTest.printAllCategories();
                            cout << "Введите название категории: ";
                            cin >> categoryName;

                            systemTest.manageTestingSystem(1, categoryName, "", "", {}, 0);
                            break;
                        }
                        case 2: {
                            systemTest.printAllCategories();
                            cout << "Введите название категории: ";
                            cin >> categoryName;
                            systemTest.printAllTestsInCategory(categoryName);
                            cout << "Введите название теста: ";
                            cin >> testName;
                            systemTest.manageTestingSystem(2, categoryName, testName, "", {}, 0);
                            break;
                        }
                        case 3: {
                            systemTest.printAllCategories();
                            cout << "Введите название категории: ";
                            cin >> categoryName;
                            systemTest.printAllTestsInCategory(categoryName);
                            cout << "Введите название теста: ";
                            cin >> testName;
                            cin.ignore();
                            cout << "Введите вопрос: ";
                            getline(cin, question);
                            string option = "";
                            cout << "Введите вариант ответа (напишите 'exit' для прекращения ввода)" << endl;
                            while (option != "exit") {
                                cin >> option;
                                options.push_back(option);
                            }
                            cout << "Введите номер правильного ответа: " << endl;
                            cin >> correctIndex;
                            systemTest.manageTestingSystem(3, categoryName, testName, question, options, correctIndex - 1);
                            break;

                        }
                        case 4: {exit = false; break; }

                        default: cout << "Некорректный ввод, повторите ещё раз." << endl;
                        }
                    }

                    break;
                }
                case 5: { EXIT = false; break; }
                default:
                    cout << "Некорректный ввод, повторите ещё раз." << endl;


                }
            }
        }
        if (person == 2) {
            system("cls");
            int regis;
            cout << "Хотите зарегистрироваться?\n1-да\n2-нет\n";
            cin >> regis;
            if (regis == 1) {
                string uname, pwd, name, addr, ph;

                cout << "Введите имя: ";
                cin >> uname;
                cout << "Введите пароль: ";
                cin >> pwd;
                cout << "Введите логин: ";
                cin >> name;
                cout << "Введите адрес: ";
                cin >> addr;
                cout << "Введите телефон: ";
                cin >> ph;

                systemTest.registerUser(uname, pwd, name, addr, ph); //регистрация при необходимости

            }

            string username, password;
            bool Extrance = true;
            while (Extrance) {
                system("cls");
                cout << "ВХОД" << endl;

                cout << "Введите имя: ";
                cin >> username;
                cout << "Введите пароль: ";
                cin >> password;
                system("cls");
                Extrance = systemTest.login(username, password); //функция входа

            }

            int testir;
            bool exit = true;
            while (exit) {
                string menu = "1. Пройти тест\n2. Мои результаты\n3 - Выход\n";
                cout << menu;
                cin >> testir;
                switch (testir) {

                case 1: {
                    system("cls");
                    string testName, categName;
                    systemTest.printAllCategories();
                    cout << "Введите название категории: ";
                    cin >> categName;
                    systemTest.printAllTestsInCategory(categName);
                    cout << "Введите название теста: ";
                    cin >> testName;
                    systemTest.takeTest(categName, testName, username);

                    break;

                }
                case 2: {
                    system("cls");
                    systemTest.viewPreviousResults(username);

                    break;
                }
                case 3: {exit = false; break; }
                default: cout << "Некорректный ввод, попробуйте еще раз." << endl;
                }

            }

        }
    }

}
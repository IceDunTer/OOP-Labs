#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <ctime>

using namespace std;

// Класс для роверов
class Rover {
private:
    string id;
    double maxLoad;
    int speed;
    int workingMinutes;
    int remainingMinutes;

public:
    Rover(string id, double maxLoad, int speed) :
        id(id), maxLoad(maxLoad), speed(speed), workingMinutes(660), remainingMinutes(660) {
        maxLoad = static_cast<double>(floor(maxLoad));
    }

    string getId() const { return id; }
    double getMaxLoad() const { return maxLoad; }
    int getSpeed() const { return speed; }
    int getRemainingMinutes() const { return remainingMinutes; }

    bool canDeliver(double weight, int distance) {
        if (weight > maxLoad || distance > 2000) {
            return false;
        }
        int deliveryTime = (distance / speed) * 2;
        return (remainingMinutes >= deliveryTime);
    }

    void useTime(int distance) {
        int timeUsed = (distance / speed) * 2;
        remainingMinutes -= timeUsed;
    }
};

// Родительский класс для всей почты
class Mail {
protected:
    int id;
    string type;
    double weight;
    string tariff;
    int distance;
    double deliveryCost;
    string roverId;

public:
    static int nextId;
    Mail(double weight, string tariff, int distance) :
        id(nextId++), weight(weight), tariff(tariff), distance(distance), roverId("Ручная доставка") {}

    virtual void calculateCost() {
        double baseCost = distance / 10.0;
        if (tariff == "экспресс") {
            baseCost *= 1.5;
        }
        deliveryCost = baseCost + 15;
    }

    virtual void assignRover(vector<Rover>& rovers) {
        roverId = "Ручная доставка";
    }

    int getId() const { return id; }
    string getType() const { return type; }
    double getWeight() const { return weight; }
    int getDistance() const { return distance; }
    string getTariff() const { return tariff; }
    virtual string getFragility() const { return "Нет"; }
    double getDeliveryCost() const { return deliveryCost; }
    string getRoverId() const { return roverId; }

    virtual ~Mail() {}
};

int Mail::nextId = 1;

// Письма от Mail
class Letter : public Mail {
public:
    Letter(double weight, string tariff, int distance) : Mail(weight, tariff, distance) {
        type = "письмо";
    }

    void assignRover(vector<Rover>& rovers) override {
        for (auto& rover : rovers) {
            if (rover.canDeliver(weight, distance)) {
                rover.useTime(distance);
                roverId = rover.getId();
                return;
            }
        }
        roverId = "Ручная доставка";
    }
};

// Бандероли от Mail
class ParcelPost : public Mail {
public:
    ParcelPost(double weight, string tariff, int distance) : Mail(weight, tariff, distance) {
        type = "бандероль";
    }

    virtual void calculateCost() {
        double baseCost = distance / 10.0;
        if (tariff == "экспресс") {
            baseCost *= 1.5;
        }
        deliveryCost = baseCost + 25;
    }

    void assignRover(vector<Rover>& rovers) override {
        for (auto& rover : rovers) {
            if (rover.canDeliver(weight, distance)) {
                rover.useTime(distance);
                roverId = rover.getId();
                return;
            }
        }
        roverId = "Ручная доставка";
    }
};

// Посылки от Mail
class Package : public Mail {
private:
    bool isFragile;

public:
    Package(double weight, string tariff, int distance, bool isFragile) :
        Mail(weight, tariff, distance), isFragile(isFragile) {
        type = "посылка";
    }

    void calculateCost() override {
        double baseCost = distance / 10.0;
        if (tariff == "экспресс") {
            baseCost *= 1.5;
        }
        deliveryCost = baseCost + 35;
        if (isFragile) {
            deliveryCost += 25;
        }
    }

    string getFragility() const override {
        return isFragile ? "Да" : "Нет";
    }

    void assignRover(vector<Rover>& rovers) override {
        for (auto& rover : rovers) {
            if (rover.canDeliver(weight, distance)) {
                rover.useTime(distance);
                roverId = rover.getId();
                return;
            }
        }   
        if (distance > 2000 || weight > 20) {
            roverId = "Ручная доставка";
            return;
        }
    }
};

// Функции для ввода с проверкой
int inputNumber(const string& prompt, int minValue, int maxValue) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue && value <= maxValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ошибка ввода. Пожалуйста, введите значение от " << minValue << " до " << maxValue << endl;
    }
}

double inputDouble(const string& prompt, double minValue, double maxValue) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue && value <= maxValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ошибка ввода. Пожалуйста, введите значение от " << minValue << " до " << maxValue << endl;
    }
}

// Таблица заказов
void printOrdersTable(const vector<Mail*>& orders) {
    if (orders.empty()) {
        cout << "Нет заказов для отображения.\n";
        return;
    }

    cout << "\n=============================================================================================================\n";
    cout << "| ID |     Тип     | Вес (кг) | Расст. (м) |   Тариф   | Хрупкость | Стоимость |      ID ровера      |\n";
    cout << "=============================================================================================================\n";

    for (const auto& order : orders) {
        cout << "| " << setw(2) << order->getId() << " | "
            << setw(10) << order->getType() << " | "
            << setw(8) << fixed << setprecision(2) << order->getWeight() << " | "
            << setw(10) << order->getDistance() << " | "
            << setw(9) << order->getTariff() << " | "
            << setw(9) << order->getFragility() << " | "
            << setw(9) << fixed << setprecision(2) << order->getDeliveryCost() << " | "
            << setw(18) << order->getRoverId() << " |\n";
    }

    cout << "=============================================================================================================\n";
}

// Таблица роверов
void printRoversTable(const vector<Rover>& rovers) {
    if (rovers.empty()) {
        cout << "Нет роверов для отображения.\n";
        return;
    }

    cout << "\n===========================================================\n";
    cout << "| ID  | Макс. нагрузка | Скорость | Осталось мин |\n";
    cout << "===========================================================\n";

    for (const auto& rover : rovers) {
        cout << "| " << rover.getId() << " | "
            << setw(14) << rover.getMaxLoad() << " | "
            << setw(8) << rover.getSpeed() << " | "
            << setw(12) << rover.getRemainingMinutes() << " |\n";
    }

    cout << "===========================================================\n";
}

// Функции для работы с роверами
void addRover(vector<Rover>& rovers) {
    cout << "\n=== Добавление нового ровера ===\n";

    string id;
    if (rovers.size() < 9) {
        id = "R0" + to_string(rovers.size() + 1);
    }
    else {
        id = "R" + to_string(rovers.size() + 1);
    }

    double maxLoad = inputDouble("Введите максимальную нагрузку (10.0-20.0 кг): ", 10.0, 20.0);
    int speed = inputNumber("Введите скорость (30-50 м/мин): ", 30, 50);

    rovers.emplace_back(id, maxLoad, speed);
    cout << "Ровер " << id << " успешно добавлен!\n";
}

void removeRover(vector<Rover>& rovers) {
    if (rovers.empty()) {
        cout << "Нет роверов для удаления.\n";
        return;
    }

    printRoversTable(rovers);
    int index = inputNumber("Введите номер ровера для удаления (1-" + to_string(rovers.size()) + "): ", 1, rovers.size());

    cout << "Ровер " << rovers[index - 1].getId() << " удален.\n";
    rovers.erase(rovers.begin() + index - 1);
}

// Функции для работы с заказами
void addOrder(vector<Mail*>& orders, vector<Rover>& rovers) {
    cout << "\n=== Добавление нового заказа ===\n";

    cout << "Выберите тип заказа:\n";
    cout << "1 - Письмо\n";
    cout << "2 - Бандероль\n";
    cout << "3 - Посылка\n";
    int typeChoice = inputNumber("Ваш выбор (1-3): ", 1, 3);

    double weight = inputDouble("Введите вес (кг): ", 0.01, 100.0);

    cout << "Выберите тариф:\n";
    cout << "1 - Стандарт\n";
    cout << "2 - Экспресс\n";
    int tariffChoice = inputNumber("Ваш выбор (1-2): ", 1, 2);

    string tariff = (tariffChoice == 1) ? "стандарт" : "экспресс";

    int distance = inputNumber("Введите расстояние (м): ", 10, 10000);

    Mail* newOrder = nullptr;

    if (typeChoice == 1) {
        newOrder = new Letter(weight, tariff, distance);
    }
    else if (typeChoice == 2) {
        newOrder = new ParcelPost(weight, tariff, distance);
    }
    else if (typeChoice == 3) {
        cout << "Хрупкий товар? (1 - да, 0 - нет): ";
        int fragileChoice = inputNumber("", 0, 1);
        newOrder = new Package(weight, tariff, distance, fragileChoice == 1);
    }

    newOrder->calculateCost();
    newOrder->assignRover(rovers);
    orders.push_back(newOrder);

    cout << "Заказ #" << newOrder->getId() << " успешно добавлен!\n";
}

void removeOrder(vector<Mail*>& orders) {
    if (orders.empty()) {
        cout << "Нет заказов для удаления.\n";
        return;
    }

    printOrdersTable(orders);
    int id = inputNumber("Введите ID заказа для удаления: ", 1, Mail::nextId - 1);

    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if ((*it)->getId() == id) {
            delete* it;
            orders.erase(it);
            cout << "Заказ #" << id << " удален.\n";
            return;
        }
    }
    cout << "Заказ с ID " << id << " не найден.\n";
}

// Функция для расчета доходов
void calculateIncome(const vector<Mail*>& orders, const vector<Rover>& rovers) {
    if (orders.empty() || rovers.empty()) {
        cout << "Недостаточно данных для расчета доходов.\n";
        return;
    }

    double totalIncome = 0;
    vector<double> roverIncomes(rovers.size(), 0);

    for (const auto& order : orders) {
        totalIncome += order->getDeliveryCost();

        for (size_t i = 0; i < rovers.size(); ++i) {
            if (order->getRoverId() == rovers[i].getId()) {
                roverIncomes[i] += order->getDeliveryCost();
                break;
            }
        }
    }

    cout << "\nДоходы:\n";
    for (size_t i = 0; i < rovers.size(); ++i) {
        double monthlyIncome = roverIncomes[i] * 30 - 56000;
        cout << "Ровер " << rovers[i].getId() << ":\n";
        cout << "  Доход за месяц: " << fixed << setprecision(2) << monthlyIncome << " руб.\n";
        cout << "  Доход за год: " << monthlyIncome * 12 << " руб.\n";
    }

    double totalMonthlyIncome = 0;
    for (double income : roverIncomes) {
        totalMonthlyIncome += income * 30 - 56000;
    }
    cout << "\nОбщий доход за год: " << totalMonthlyIncome * 12 << " руб.\n";
}

//Случайные данные
void randomiseData(vector<Rover>& rovers, vector<Mail*>& orders) {
    rovers.emplace_back("R01", 10.0, 30);

    orders.push_back(new Letter(0.1, "стандарт", 250));
    orders.push_back(new Letter(0.1, "стандарт", 1480));
    orders.push_back(new Letter(0.1, "экспресс", 520));
    orders.push_back(new Letter(0.1, "экспресс", 790));

    srand(time(0));
    orders.push_back(new Package(10.0, "экспресс", 350 + rand() % 1351, rand() % 2));
    orders.push_back(new Package(15.0, "стандарт", 350 + rand() % 1351, rand() % 2));
    orders.push_back(new Package(4.0, "экспресс", 350 + rand() % 1351, rand() % 2));
    orders.push_back(new Package(6.0, "экспресс", 350 + rand() % 1351, rand() % 2));

    for (int i = 0; i < 6; ++i) {
        double weight = 0.3 + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (1.2 - 0.3)));
        string tariff = (i < 5) ? "стандарт" : "экспресс";
        int distance = 560 + rand() % 1841;
        orders.push_back(new ParcelPost(weight, tariff, distance));
    }

    for (auto& order : orders) {
        order->calculateCost();
        order->assignRover(rovers);
    }
}

// Главное меню
void mainMenu(vector<Rover>& rovers, vector<Mail*>& orders) {
    while (true) {
        cout << "\n=== Главное меню ===\n";
        cout << "1. Показать таблицу заказов\n";
        cout << "2. Показать таблицу роверов\n";
        cout << "3. Добавить новый заказ\n";
        cout << "4. Добавить новый ровер\n";
        cout << "5. Удалить заказ\n";
        cout << "6. Удалить ровер\n";
        cout << "7. Рассчитать доходы\n";
        cout << "0. Выход\n";

        int choice = inputNumber("Ваш выбор (0-7): ", 0, 7);

        switch (choice) {
        case 1:
            printOrdersTable(orders);
            break;
        case 2:
            printRoversTable(rovers);
            break;
        case 3:
            addOrder(orders, rovers);
            break;
        case 4:
            addRover(rovers);
            break;
        case 5:
            removeOrder(orders);
            break;
        case 6:
            removeRover(rovers);
            break;
        case 7:
            calculateIncome(orders, rovers);
            break;
        case 0:
            return;
        default:
            cout << "Некорректный выбор. Пожалуйста, попробуйте снова.\n";
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    vector<Rover> rovers;
    vector<Mail*> orders;

    randomiseData(rovers, orders);

    mainMenu(rovers, orders);

    // Очистка памяти
    for (auto& order : orders) {
        delete order;
    }

    return 0;
}

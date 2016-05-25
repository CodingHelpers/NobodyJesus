#include <iostream>
#include <fstream>
#include <stddef.h>
#include <stdio.h>
#include <vector>
#include <string.h>

// Имя файла для чтения
#define FILENAME "prices.txt"

// Структура PRICE
struct PRICE {
    char name[32];
    char shop[32];
    int  price;
};

// Открыть и прочитать файл в буффер, си
int ReadFile(const char* filename, PRICE* buffer, int buflen) {
    // Открываем файл
    FILE* f = fopen(filename, "r");

    // Првоеряем открыт ли файл
    if(f == NULL) {
        printf("Error loading file.\n");
        exit(1);
    }

    // Буффер для чтения строки из файла
    char line[512];

    // Количество прочитанных структур
    int cnt = 0;

    // Пока не достингут конец файла
    while(!feof(f)) {
        // Читаем из файла строку в буффер
        fgets(line, 512, f);

        // Если строка пустая, читаем следующую
        if(*line == '\0' || *line == '\n') {
            continue;
        }

        // Указатель на наименование товара
        char* name = line;

        // Указатель на имя магазина
        char* shop = strchr(name, ' ');
        if(!shop) {
            return -1;
        }

        // После первого пробела после имени товара двигаемся до первой буквы, в случае если там много проблеов
        while(*shop == ' ') ++shop;

        // Указатель на стоимость
        char* price = strchr(shop, ' ');
        if(!price) {
            return -1;
        }

        // После первого пробела после имени магазина двигаемся до первой буквы, в случае если там много проблеов
        while(*price == ' ') ++price;

        if(cnt < buflen) {
            // Копируем данные в структуру
            strncpy(buffer[cnt].name, name, (size_t) (strchr(name, ' ') - name));
            strncpy(buffer[cnt].shop, shop, (size_t) (strchr(shop, ' ') - shop));
            buffer[cnt].price = atoi(price);
            cnt++;
        } else {
            // Буффер слишком маленький
            return -2;
        }
    }

    // Возвращаем количество структур
    return cnt;
}

// Открыть и прочитать файл в буффер, c++
std::vector<PRICE> ReadFile(const std::string& filename) {
    // Массив структур
    std::vector<PRICE> buffer;

    // Поток файлового ввода
    std::ifstream f(filename.c_str());

    // Проверяем открыт ли файл
    if(!f.is_open()) {
        std::cout << "Error loading file.\n";
        exit(1);
    }

    // Читаем из файла во временную структуру
    PRICE temp;
    while(f >> temp.name >> temp.shop >> temp.price) {
        // Заносим прочитанную структуру в массив
        buffer.push_back(temp);
    }

    // Возвращаем массив втруктур
    return buffer;
}

// Поменять местами данные в двух структурах
void swap(PRICE* t1, PRICE* t2) {
    PRICE temp;
    memcpy(&temp, t1, sizeof(PRICE));
    memcpy(t1, t2, sizeof(PRICE));
    memcpy(t2, &temp, sizeof(PRICE));
}

// Вывод одной структуры на экран
void print(PRICE* price) {
    std::cout << "[Item = "  << price->name  << "], "
              << "[Shop = "  << price->shop  << "], "
              << "[Price = " << price->price << "]."
              << std::endl;
}

// Вывод всех переданных структур на экран
void printAll(PRICE* prices, int cnt) {
    for(int i = 0; i < cnt; i++) {
        print(prices+i);
    }
}

// Загрузка и сортировка
void LoadAndSort(const char* filename) {
    // Массив структур, куда они будут прочитаны из файла
    PRICE prices[512];
    int cnt = ReadFile(filename, prices, 512);

    // Сортировка пузырьком
    for(int i = 0; i < cnt; i++) {
        for(int j = 1; j < cnt; j++) {
            // Если структуры не по порядку
            if(strcmp(prices[j].shop, prices[j-1].shop) < 0) {
                // Меняем их местами
                swap(prices+j, prices+j-1);
            }
        }
    }

    // Выводим на экран все структуры
    printAll(prices, cnt);
}

// закрузка и поиск
void LoadAndFind(const std::string& filename, const std::string shop) {
    // Вектор структур
    std::vector<PRICE> prices = ReadFile(filename);

    // Проход по всем структурам в поиске нужной
    for(PRICE& p : prices) {
        // Если название магазина в структуре и переданное совпадают
        if(strcmp(p.shop, shop.c_str()) == 0) {
            // Выводим структуру на экран
            print(&p);
        }
    }
}

int main() {
    // Загрузка и сортировка
    LoadAndSort(FILENAME);

    // Ввод магазина для поиска
    std::string shop;
    std::cout << "Enter shop name: ";
    std::cin  >> shop;

    // Загрузка и поиск
    LoadAndFind(FILENAME, shop);
    return 0;
}

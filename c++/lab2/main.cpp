#include <iostream>
#include <fstream>
#include <stddef.h>
#include <stdio.h>
#include <vector>
#include <string.h>

#define FILENAME "prices.txt"

struct PRICE {
    char name[32];
    char shop[32];
    int  price;
};

// Открыть и прочитать файл в буффер, си
int ReadFile(const char* filename, PRICE* buffer, int buflen) {
    FILE* f = fopen(filename, "r");
    if(f == NULL) {
        printf("Error loading file.\n");
        exit(1);
    }

    char line[512];
    int cnt = 0;
    while(!feof(f)) {
        fgets(line, 512, f);

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

    return cnt;
}

// Открыть и прочитать файл в буффер, c++
std::vector<PRICE> ReadFile(const std::string& filename) {
    std::vector<PRICE> buffer;
    std::ifstream f(filename.c_str());
    if(!f.is_open()) {
        std::cout << "Error loading file.\n";
        exit(1);
    }

    PRICE temp;
    while(f >> temp.name >> temp.shop >> temp.price) {
        buffer.push_back(temp);
    }

    return buffer;
}

void swap(PRICE* t1, PRICE* t2) {
    PRICE temp;
    memcpy(&temp, t1, sizeof(PRICE));
    memcpy(t1, t2, sizeof(PRICE));
    memcpy(t2, &temp, sizeof(PRICE));
}

void print(PRICE* price) {
    std::cout << "[Item = "  << price->name  << "], "
              << "[Shop = "  << price->shop  << "], "
              << "[Price = " << price->price << "]."
              << std::endl;
}

void printAll(PRICE* prices, int cnt) {
    for(int i = 0; i < cnt; i++) {
        print(prices+i);
    }
}

void LoadAndSort(const char* filename) {
    PRICE prices[512];
    int cnt = ReadFile(filename, prices, 512);

    for(int i = 0; i < cnt; i++) {
        for(int j = 1; j < cnt; j++) {
            if(strcmp(prices[j].shop, prices[j-1].shop) < 0) {
                swap(prices+j, prices+j-1);
            }
        }
    }

    printAll(prices, cnt);
}

void LoadAndFind(const std::string& filename, const std::string shop) {
    std::vector<PRICE> prices = ReadFile(filename);

    for(PRICE& p : prices) {
        if(strcmp(p.shop, shop.c_str()) == 0) {
            print(&p);
        }
    }
}

int main() {
    LoadAndSort(FILENAME);

    std::string shop;
    std::cout << "Enter shop name: ";
    std::cin  >> shop;

    LoadAndFind(FILENAME, shop);
    return 0;
}

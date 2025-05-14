#include <clocale>
#include <iostream>

#include "menu.hpp"
#include "menu_functions.hpp"
#include "menu_items.hpp"

int main() {
    std::setlocale(LC_ALL, "");

    std::cout << "К сожалению вы не прошли в СПбГУ, но Навигатор Поступления поможет вам выбрать учебное заведение " << std::endl;

    const BM::MenuItem* current = &BM::MAIN;
    do {
        current = current->func(current);
    } while (true);

    return 0;
}
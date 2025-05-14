#include "menu_functions.hpp"

#include <cstdlib>
#include <iostream>

const BM::MenuItem* BM::show_menu(const MenuItem* current) {
    for (int i = 1; i < current->children_count; i++) {
        std::cout << current->children[i]->title << std::endl;
    }
    std::cout << current->children[0]->title << std::endl;
    std::cout << "Навигатор Поступления > ";

    int user_input;
    std::cin >> user_input;
    std::cout << std::endl;

    return current->children[user_input];
}

const BM::MenuItem* BM::exit(const MenuItem* current) {
    std::exit(0);
}

const BM::MenuItem* BM::universities(const MenuItem* current) {
    std::cout << current->title << std::endl << std::endl;
    return current->parent;
}

const BM::MenuItem* BM::universities_church(const MenuItem* current) {
    // TODO
    std::cout << current->title << std::endl << std::endl;
    return current->parent;
}

const BM::MenuItem* BM::universities_dgtu(const MenuItem* current) {
    // TODO
    std::cout << current->title << std::endl << std::endl;
    return current->parent;
}

const BM::MenuItem* BM::universities_alabuga(const MenuItem* current) {
    // TODO
    std::cout << current->title << std::endl << std::endl;
    return current->parent;
}

const BM::MenuItem* BM::universities_synergy(const MenuItem* current) {
    // TODO
    std::cout << current->title << std::endl << std::endl;
    return current->parent;
}

const BM::MenuItem* BM::universities_ryazan(const MenuItem* current) {
    // TODO
    std::cout << current->title << std::endl << std::endl;
    return current->parent;
}

const BM::MenuItem* BM::universities_go_back(const MenuItem* current) {
    // TODO
    std::cout << current->title << std::endl << std::endl;
    return current->parent->parent;
}




const BM::MenuItem* BM::zaborostroyeniye(const MenuItem* current) {
    // TODO
    std::cout << current->title << std::endl << std::endl;
    return current->parent;
}

const BM::MenuItem* BM::fishing(const MenuItem* current) {
    // TODO
    std::cout << current->title << std::endl << std::endl;
    return current->parent;
}

const BM::MenuItem* BM::tarology(const MenuItem* current) {
    // TODO
    std::cout << current->title << std::endl << std::endl;
    return current->parent;
}

const BM::MenuItem* BM::tgsha_go_back(const MenuItem* current) {
    // TODO
    std::cout << current->title << std::endl << std::endl;
    return current->parent->parent;
}
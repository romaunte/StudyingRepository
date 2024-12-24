#include "menu_items.hpp"
#include <cstddef>
#include "menu_functions.hpp"

const BM::MenuItem BM::ZABOROSTROYENIYE = {
    "1 - Факультет Заборостроения", BM::zaborostroyeniye, &BM::UNIVERSITIES_TGSHA
};
const BM::MenuItem BM::FISHING = {
    "2 - Факультет Рыболовства", BM::fishing, &BM::UNIVERSITIES_TGSHA
};
const BM::MenuItem BM::TAROLOGY = {
    "3 - Факультет Тарологии", BM::tarology, &BM::UNIVERSITIES_TGSHA
};
const BM::MenuItem BM::TGSHA_GO_BACK = {
    "0 - Выйти в предыдущее меню", BM::tgsha_go_back, &BM::UNIVERSITIES_TGSHA
};

namespace {
    const BM::MenuItem* const tgsha_children[] = {
        &BM::TGSHA_GO_BACK,
        &BM::ZABOROSTROYENIYE,
        &BM::FISHING,
        &BM::TAROLOGY
    };
    const int tgsha_size = sizeof(tgsha_children) / sizeof(tgsha_children[0]);
}


const BM::MenuItem BM::UNIVERSITIES_TGSHA = {
    "1 - Тверская Сельскохозяйственная Академия", BM::show_menu, &BM::UNIVERSITIES, tgsha_children, tgsha_size
};
const BM::MenuItem BM::UNIVERSITIES_CHURCH = {
    "2 - Церковно-Приходская Школа", BM::universities_church, &BM::UNIVERSITIES
};
const BM::MenuItem BM::UNIVERSITIES_DGTU = {
    "3 - ДГТУ (Виноделие)", BM::universities_dgtu, &BM::UNIVERSITIES
};
const BM::MenuItem BM::UNIVERSITIES_ALABUGA = {
    "4 - Алабуга Политех", BM::universities_alabuga, &BM::UNIVERSITIES
};
const BM::MenuItem BM::UNIVERSITIES_SYNERGY = {
    "5 - Синергия", BM::universities_synergy, &BM::UNIVERSITIES
};
const BM::MenuItem BM::UNIVERSITIES_RYAZAN = {
    "6 - Рязанское Кадетское Училище", BM::universities_ryazan, &BM::UNIVERSITIES
};
const BM::MenuItem BM::UNIVERSITIES_GO_BACK = {
    "0 - Выйти в главное меню", BM::universities_go_back, &BM::UNIVERSITIES
};

namespace {
    const BM::MenuItem* const study_children[] = {
        &BM::UNIVERSITIES_GO_BACK,
        &BM::UNIVERSITIES_TGSHA,
        &BM::UNIVERSITIES_CHURCH,
        &BM::UNIVERSITIES_DGTU,
        &BM::UNIVERSITIES_ALABUGA,
        &BM::UNIVERSITIES_SYNERGY,
        &BM::UNIVERSITIES_RYAZAN
    };
    const int study_size = sizeof(study_children) / sizeof(study_children[0]);
}

const BM::MenuItem BM::UNIVERSITIES = {
    "1 - Посмотреть подходящие учебные заведения", BM::show_menu, &BM::MAIN, study_children, study_size
};
const BM::MenuItem BM::EXIT = {
    "0 - Я уже студент", BM::exit, &BM::MAIN
};

namespace {
    const BM::MenuItem* const main_children[] = {
        &BM::EXIT,
        &BM::UNIVERSITIES
    };
    const int main_size = sizeof(main_children) / sizeof(main_children[0]);
}

const BM::MenuItem BM::MAIN = {
    nullptr, BM::show_menu, nullptr, main_children, main_size
};
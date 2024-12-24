#pragma once

#include "menu.hpp"

namespace BM {
    const MenuItem* show_menu(const MenuItem* current);

    const MenuItem* exit(const MenuItem* current);
    const MenuItem* universities(const MenuItem* current);

    const MenuItem* universities_church(const MenuItem* current);
    const MenuItem* universities_dgtu(const MenuItem* current);
    const MenuItem* universities_alabuga(const MenuItem* current);
    const MenuItem* universities_synergy(const MenuItem* current);
    const MenuItem* universities_ryazan(const MenuItem* current);
    const MenuItem* universities_go_back(const MenuItem* current);

    const MenuItem* zaborostroyeniye(const MenuItem* current);
    const MenuItem* fishing(const MenuItem* current);
    const MenuItem* tarology(const MenuItem* current);
    const MenuItem* tgsha_go_back(const MenuItem* current);
}
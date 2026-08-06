#include <array>
#include <cstdio>
#include <cstdint>

#include "caravan.hpp"

struct player_t {
    std::array<caravan_t, 3> caravans{};
    //std::array<card_t, 8> current_cards{};
    card_t* deck = nullptr;
};

struct table_t {
    player_t player_a;
    player_t player_b;
};

constexpr card_t queen(card_t::type_t::queen, card_t::suit_t::club);
constexpr card_t king(card_t::type_t::king, card_t::suit_t::club);

constexpr card_t one(card_t::type_t::one, card_t::suit_t::club);
constexpr card_t two(card_t::type_t::two, card_t::suit_t::club);
constexpr card_t three(card_t::type_t::three, card_t::suit_t::club);

int main() {
    caravan_t c;
    
    if (c.try_place_card(one)) {
        printf("Placed!\n");
    }

    if (c.try_place_card(two)) {
        printf("Placed!\n");
    }

    if (c.try_place_card(three)) {
        printf("Placed!\n");
    }

    if (c.try_place_card(king, 0)) {
        printf("Placed!\n");
    }

    printf("Current caravan value: %d\n", (int)c.value());

    //if (c.try_place_card(queen, 0)) {
    //  printf("Placed!\n");
    //}
}
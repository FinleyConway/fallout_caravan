#include <array>
#include <vector>
#include <cstdint>

enum class card_suit_t {
    none,
    club,
    spade,
    diamond,
    heart
};

enum class card_type_t : uint8_t {
    none,
    one = 1,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    joker
};

struct card_t {
    uint8_t value() const {
        return static_cast<uint8_t>(type);
    }

    bool is_special() const {
        return 
            type == card_type_t::jack   ||
            type == card_type_t::queen  ||
            type == card_type_t::king   ||
            type == card_type_t::joker;
    }

    card_type_t type = card_type_t::none;
    card_suit_t suit = card_suit_t::none;
};

struct card_row_t {
    card_t card;
    uint8_t king_modifiers = 0; 
    uint8_t queen_modifiers = 0; 

    explicit card_row_t(card_t card) : card(card) {
    }

    uint8_t value() const {
        return card.value() * king_modifiers;
    }
};

struct caravan_t {
private:
    enum class direction_t {
        none,
        ascending,
        descending
    };

public:
    bool try_place_card(card_t card, uint8_t row = 0) {
        if (card.is_special()) {

        }

        // has a card not been placed yet?
        if (m_cards.empty()) {
            m_current_suit = card.suit;
            m_cards.emplace_back(card);

            return true;
        }

        return try_place_number_card(card);
    } 

    uint8_t value() const {
        uint8_t value = 0;

        for (const auto& row : m_cards) {
            value += row.value();
        }

        return value;
    }

private:
    bool try_place_number_card(card_t card) {
        // has a card not been placed yet?
        if (m_cards.empty()) {
            m_current_suit = card.suit;
            m_cards.emplace_back(card);

            return true;
        }

        // get the previously placed card
        const card_t& previous = m_cards.back().card;
        
        // cant play the same numerical value in sequence
        if (card.value() == previous.value()) return false;

        // has the direction not been decided yet?
        if (m_direction == direction_t::none) {
            m_direction = card.value() > previous.value()
                ? direction_t::ascending
                : direction_t::descending;
        }

        bool follows_direction = is_follows_direction(card.value(), previous.value());
        bool follows_suit = card.suit == m_current_suit;

        if (!follows_direction && !follows_suit) return false;

        m_cards.emplace_back(card);
        m_current_suit = card.suit;

        return true;
    }

    bool is_follows_direction(uint8_t card_value, uint8_t prev_card_value) const {
        return m_direction == direction_t::ascending
            ? card_value > prev_card_value
            : card_value < prev_card_value;
    }

    void on_queen_add(card_t queen) {
        if (m_direction == direction_t::none) {
            return;
        }

        if (m_direction == direction_t::ascending) {
            m_direction = direction_t::descending;
        }
        else {
            m_direction = direction_t::ascending;
        }

        m_current_suit = queen.suit;
    }

private:
    std::vector<card_row_t> m_cards;

    direction_t m_direction = direction_t::none; 
    card_suit_t m_current_suit = card_suit_t::none;
};

struct player_t {
    std::array<caravan_t, 3> caravans{};
    std::array<card_t, 8> current_cards{};
    card_t* deck = nullptr;
};

struct table_t {
    player_t player_a;
    player_t player_b;
};

int main() {

}
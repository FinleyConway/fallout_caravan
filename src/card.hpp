#pragma once

#include <cstdint>

class card_t {
public:
    enum class suit_t {
        none,
        club,
        spade,
        diamond,
        heart
    };

    enum class type_t : uint8_t {
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

public:
    constexpr card_t(type_t type, suit_t suit) : m_type(type), m_suit(suit) {
    }

public:
    constexpr type_t type() const {
        return m_type;
    }

    constexpr suit_t suit() const {
        return m_suit;
    }

    constexpr uint8_t value() const {
        constexpr uint8_t jack_value = static_cast<uint8_t>(type_t::jack);
        uint8_t value = static_cast<uint8_t>(m_type);

        // face cards have no value
        if (value >= jack_value) {
            return 0;
        }

        return value;
    }

    constexpr bool is_special() const {
        return 
            m_type == type_t::jack   ||
            m_type == type_t::queen  ||
            m_type == type_t::king   ||
            m_type == type_t::joker;
    }

private:
    type_t m_type;
    suit_t m_suit;
};
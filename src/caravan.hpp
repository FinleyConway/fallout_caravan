#pragma once

#include <vector>
#include <cstdint>
#include <stdio.h>

#include "card.hpp"

class caravan_t {
private:
    struct card_row_t {
        card_t card;
        uint8_t king_modifiers = 0; 
        uint8_t queen_modifiers = 0; 

        explicit card_row_t(card_t card) : card(card) {
        }

        uint8_t value() const {
            if (king_modifiers > 0) {
                const uint8_t offset = 1;

                return card.value() * (king_modifiers + offset);    
            }

            return card.value();
        }
    };

public:
    enum class direction_t {
        none,
        ascending,
        descending
    };


public:
    bool try_place_card(card_t card, uint8_t row = 0) {
        if (card.is_special()) {
            // has the caravan got a card  yet or out of bounds
            if (m_cards.empty() || row >= m_cards.size()) {
                return false;
            }

            switch (card.type()) {
                case card_t::type_t::jack:  return on_jack_add(card, row);
                case card_t::type_t::queen: return on_queen_add(card, row);
                case card_t::type_t::king:  return on_king_add(card, row);
                case card_t::type_t::joker: return false;
                default: return false;
            };
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

    direction_t direction() const {
        return m_direction;
    }

    card_t::suit_t suit() const {
        return m_current_suit;
    }

private:
    bool try_place_number_card(card_t card) {
        // has a card not been placed yet?
        if (m_cards.empty()) {
            m_current_suit = card.suit();
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

        if (!is_follows_direction(card.value(), previous.value())) return false;

        m_cards.emplace_back(card);
        m_current_suit = card.suit();

        return true;
    }

    bool is_follows_direction(uint8_t card_value, uint8_t prev_card_value) const {
        return m_direction == direction_t::ascending
            ? card_value > prev_card_value
            : card_value < prev_card_value;
    }

    bool on_queen_add(card_t queen, uint8_t row) {
        card_row_t& card_row = m_cards[row];

        // selected the previously placed card?
        if (row == m_cards.size() - 1) {
            if (m_direction == direction_t::ascending) {
                m_direction = direction_t::descending;
            }
            else {
                m_direction = direction_t::ascending;
            }

            m_current_suit = queen.suit();
        }

        card_row.queen_modifiers++;

        return true;
    }

    bool on_king_add(card_t king, uint8_t row) {
        card_row_t& card_row = m_cards[row];

        card_row.king_modifiers++;

        return true;
    }

    bool on_jack_add(card_t jack, uint8_t row) {
        // removes card row for caravan
        m_cards.erase(m_cards.begin() + row);

        // update the deck to reflect on changes
        // have we removed all cards, set back to default?
        if (m_cards.empty()) {
            m_direction = direction_t::none;
            m_current_suit = card_t::suit_t::none;

            return true;
        }

        // TODO: reflect on queen modifier too
        const card_t& top = m_cards.back().card;

        // update suit
        m_current_suit = top.suit();

        // set direction to none if only one card
        if (m_cards.size() == 1) {
            m_direction = direction_t::none;

            return true;
        }

        // check if asc or des based on the top 2 previous cards
        const card_t& below_top = (m_cards.end() - 2)->card;

        if (top.value() >= below_top.value()) {
            m_direction = direction_t::ascending;
        }
        else {
            m_direction = direction_t::descending;
        }

        return true;
    }

private:
    std::vector<card_row_t> m_cards;
    direction_t m_direction = direction_t::none; 
    card_t::suit_t m_current_suit = card_t::suit_t::none;
};
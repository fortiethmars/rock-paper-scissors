#pragma once

#include <cstdint>
#include <optional>
#include <random>
#include <string>
#include <string_view>

namespace rps {

enum class Choice { rock, paper, scissors };
enum class Outcome { win, loss, draw };

std::optional<Choice> parseChoice(std::string_view input);
std::string toString(Choice choice);
Outcome determineOutcome(Choice player, Choice computer);
std::string roundMessage(Choice player, Choice computer, Outcome outcome);

struct Score {
    unsigned playerWins{};
    unsigned computerWins{};
    unsigned draws{};
    unsigned rounds{};
};

class GameSession {
public:
    explicit GameSession(unsigned targetWins);

    Outcome playRound(Choice player, Choice computer);
    bool finished() const noexcept;
    std::optional<Outcome> matchResult() const noexcept;
    unsigned targetWins() const noexcept;
    const Score& score() const noexcept;

private:
    unsigned targetWins_;
    Score score_;
};

class ComputerPlayer {
public:
    ComputerPlayer();
    explicit ComputerPlayer(std::uint32_t seed);
    Choice choose();

private:
    std::mt19937 engine_;
};

}  // namespace rps

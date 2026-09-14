#include "rps/game.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace rps {
namespace {

std::string normalize(std::string_view input) {
    const auto first = std::find_if_not(input.begin(), input.end(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    });
    const auto last = std::find_if_not(input.rbegin(), input.rend(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    }).base();

    if (first >= last) {
        return {};
    }

    std::string result(first, last);
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return result;
}

}  // namespace

std::optional<Choice> parseChoice(std::string_view input) {
    const std::string value = normalize(input);
    if (value == "rock" || value == "r") {
        return Choice::rock;
    }
    if (value == "paper" || value == "p") {
        return Choice::paper;
    }
    if (value == "scissors" || value == "s") {
        return Choice::scissors;
    }
    return std::nullopt;
}

std::string toString(Choice choice) {
    switch (choice) {
        case Choice::rock: return "rock";
        case Choice::paper: return "paper";
        case Choice::scissors: return "scissors";
    }
    throw std::logic_error("unknown choice");
}

Outcome determineOutcome(Choice player, Choice computer) {
    if (player == computer) {
        return Outcome::draw;
    }
    const bool wins =
        (player == Choice::rock && computer == Choice::scissors) ||
        (player == Choice::paper && computer == Choice::rock) ||
        (player == Choice::scissors && computer == Choice::paper);
    return wins ? Outcome::win : Outcome::loss;
}

std::string roundMessage(Choice player, Choice computer, Outcome outcome) {
    if (outcome == Outcome::draw) {
        return "It's a draw.";
    }

    std::string action;
    if ((player == Choice::rock && computer == Choice::scissors) ||
        (computer == Choice::rock && player == Choice::scissors)) {
        action = "Rock crushes scissors.";
    } else if ((player == Choice::paper && computer == Choice::rock) ||
               (computer == Choice::paper && player == Choice::rock)) {
        action = "Paper covers rock.";
    } else {
        action = "Scissors cut paper.";
    }
    return action + (outcome == Outcome::win ? " You win!" : " Computer wins!");
}

GameSession::GameSession(unsigned targetWins) : targetWins_(targetWins) {
    if (targetWins == 0) {
        throw std::invalid_argument("target wins must be positive");
    }
}

Outcome GameSession::playRound(Choice player, Choice computer) {
    if (finished()) {
        throw std::logic_error("the match is already finished");
    }
    const Outcome outcome = determineOutcome(player, computer);
    ++score_.rounds;
    if (outcome == Outcome::win) {
        ++score_.playerWins;
    } else if (outcome == Outcome::loss) {
        ++score_.computerWins;
    } else {
        ++score_.draws;
    }
    return outcome;
}

bool GameSession::finished() const noexcept {
    return score_.playerWins >= targetWins_ || score_.computerWins >= targetWins_;
}

std::optional<Outcome> GameSession::matchResult() const noexcept {
    if (!finished()) {
        return std::nullopt;
    }
    return score_.playerWins >= targetWins_ ? Outcome::win : Outcome::loss;
}

unsigned GameSession::targetWins() const noexcept { return targetWins_; }
const Score& GameSession::score() const noexcept { return score_; }

ComputerPlayer::ComputerPlayer() : engine_(std::random_device{}()) {}
ComputerPlayer::ComputerPlayer(std::uint32_t seed) : engine_(seed) {}

Choice ComputerPlayer::choose() {
    std::uniform_int_distribution<int> distribution(0, 2);
    return static_cast<Choice>(distribution(engine_));
}

}  // namespace rps

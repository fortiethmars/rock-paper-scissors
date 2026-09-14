#include "rps/game.hpp"

#include <charconv>
#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

namespace {

std::optional<unsigned> parsePositive(std::string_view value) {
    unsigned result{};
    const auto parsed = std::from_chars(value.data(), value.data() + value.size(), result);
    if (parsed.ec != std::errc{} || parsed.ptr != value.data() + value.size() || result == 0) {
        return std::nullopt;
    }
    return result;
}

void printUsage(const char* program) {
    std::cout << "Usage: " << program << " [--wins N] [--seed N]\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    unsigned targetWins = 3;
    std::optional<std::uint32_t> seed;

    for (int i = 1; i < argc; ++i) {
        const std::string_view argument = argv[i];
        if (argument == "--help") {
            printUsage(argv[0]);
            return 0;
        }
        if ((argument == "--wins" || argument == "--seed") && i + 1 < argc) {
            const auto value = parsePositive(argv[++i]);
            if (!value) {
                std::cerr << "Expected a positive integer after " << argument << ".\n";
                return 2;
            }
            if (argument == "--wins") {
                targetWins = *value;
            } else {
                seed = static_cast<std::uint32_t>(*value);
            }
            continue;
        }
        std::cerr << "Unknown or incomplete argument: " << argument << "\n";
        printUsage(argv[0]);
        return 2;
    }

    rps::ComputerPlayer computer = seed ? rps::ComputerPlayer(*seed) : rps::ComputerPlayer();
    rps::GameSession game(targetWins);

    std::cout << "Welcome to Rock, Paper, Scissors!\n"
              << "First to " << targetWins << " wins. Enter rock/r, paper/p, scissors/s, or quit.\n";

    std::string input;
    while (!game.finished()) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) {
            std::cout << "\nInput closed.\n";
            return 0;
        }
        if (input == "quit" || input == "q") {
            std::cout << "Match abandoned.\n";
            return 0;
        }

        const auto player = rps::parseChoice(input);
        if (!player) {
            std::cout << "Invalid choice. Try rock/r, paper/p, scissors/s, or quit.\n";
            continue;
        }

        const rps::Choice computerChoice = computer.choose();
        const rps::Outcome outcome = game.playRound(*player, computerChoice);
        const rps::Score& score = game.score();

        std::cout << "You: " << rps::toString(*player)
                  << ", computer: " << rps::toString(computerChoice) << "\n"
                  << rps::roundMessage(*player, computerChoice, outcome) << "\n"
                  << "Score " << score.playerWins << ':' << score.computerWins
                  << " (draws: " << score.draws << ")\n";
    }

    std::cout << (game.matchResult() == rps::Outcome::win ? "You won the match!\n"
                                                          : "Computer won the match.\n");
    return 0;
}

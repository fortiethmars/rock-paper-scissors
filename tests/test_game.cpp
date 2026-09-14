#include "rps/game.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

int failures = 0;

void check(bool condition, const std::string& name) {
    if (!condition) {
        std::cerr << "FAIL: " << name << '\n';
        ++failures;
    }
}

void testParsing() {
    check(rps::parseChoice("rock") == rps::Choice::rock, "parse full name");
    check(rps::parseChoice("  PAPER ") == rps::Choice::paper, "trim and ignore case");
    check(rps::parseChoice("s") == rps::Choice::scissors, "parse abbreviation");
    check(!rps::parseChoice("stone"), "reject invalid value");
}

void testAllOutcomes() {
    using rps::Choice;
    using rps::Outcome;
    struct Case {
        Choice player;
        Choice computer;
        Outcome expected;
        const char* name;
    };
    const Case cases[] = {
        {Choice::rock, Choice::rock, Outcome::draw, "rock vs rock"},
        {Choice::rock, Choice::paper, Outcome::loss, "rock vs paper"},
        {Choice::rock, Choice::scissors, Outcome::win, "rock vs scissors"},
        {Choice::paper, Choice::rock, Outcome::win, "paper vs rock"},
        {Choice::paper, Choice::paper, Outcome::draw, "paper vs paper"},
        {Choice::paper, Choice::scissors, Outcome::loss, "paper vs scissors"},
        {Choice::scissors, Choice::rock, Outcome::loss, "scissors vs rock"},
        {Choice::scissors, Choice::paper, Outcome::win, "scissors vs paper"},
        {Choice::scissors, Choice::scissors, Outcome::draw, "scissors vs scissors"},
    };
    for (const auto& item : cases) {
        check(rps::determineOutcome(item.player, item.computer) == item.expected, item.name);
    }
}

void testSessionAndScore() {
    rps::GameSession game(2);
    game.playRound(rps::Choice::rock, rps::Choice::rock);
    game.playRound(rps::Choice::paper, rps::Choice::rock);
    check(!game.finished(), "match continues before target");
    game.playRound(rps::Choice::scissors, rps::Choice::paper);
    check(game.finished(), "match finishes at target");
    check(game.matchResult() == rps::Outcome::win, "player wins match");
    check(game.score().rounds == 3 && game.score().draws == 1, "score tracks rounds and draws");

    bool rejected = false;
    try {
        game.playRound(rps::Choice::rock, rps::Choice::scissors);
    } catch (const std::logic_error&) {
        rejected = true;
    }
    check(rejected, "cannot play after match");
}

void testInvalidTarget() {
    bool rejected = false;
    try {
        const rps::GameSession game(0);
        (void)game;
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    check(rejected, "zero target rejected");
}

void testSeededComputer() {
    rps::ComputerPlayer first(42);
    rps::ComputerPlayer second(42);
    for (int i = 0; i < 20; ++i) {
        check(first.choose() == second.choose(), "same seed gives same sequence");
    }
}

}  // namespace

int main() {
    testParsing();
    testAllOutcomes();
    testSessionAndScore();
    testInvalidTarget();
    testSeededComputer();

    if (failures == 0) {
        std::cout << "All tests passed.\n";
    }
    return failures == 0 ? 0 : 1;
}

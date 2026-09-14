<div align="center">

# Rock Paper Scissors

Консольная игра «Камень, бумага, ножницы» на C++17.

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-064F8C?logo=cmake)
![Tests](https://img.shields.io/badge/tests-CTest-success)

</div>

## Возможности

- матч до заданного количества побед;
- подсчёт побед, поражений и ничьих;
- ввод без учёта регистра;
- сокращения `r`, `p`, `s`;
- досрочный выход по команде `quit` или `q`;
- фиксированный seed для воспроизводимых запусков;
- автоматические тесты игровой логики.

## Требования

- CMake 3.16 или новее;
- компилятор с поддержкой C++17:
  - GCC;
  - Clang;
  - MSVC.

Внешние библиотеки не требуются.

## Сборка

```sh
cmake -S . -B build
cmake --build build
```

По умолчанию тесты запускаются автоматически во время сборки.

### Release-сборка

Для одноконфигурационных генераторов:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Для Visual Studio:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

## Запуск

Linux и macOS:

```sh
./build/rps
```

Windows с Visual Studio:

```powershell
.\build\Release\rps.exe
```

## Параметры

```text
rps [--wins N] [--seed N] [--help]
```

| Параметр | Описание |
|---|---|
| `--wins N` | Количество побед для завершения матча, по умолчанию `3` |
| `--seed N` | Seed генератора случайных чисел |
| `--help` | Показать справку |

Пример матча до пяти побед:

```sh
./build/rps --wins 5
```

Воспроизводимый запуск:

```sh
./build/rps --wins 3 --seed 42
```

## Управление

Доступные ходы:

```text
rock      или r
paper     или p
scissors  или s
```

Завершение игры:

```text
quit      или q
```

Регистр и пробелы при вводе ходов не учитываются.

## Пример

```text
Welcome to Rock, Paper, Scissors!
First to 2 wins. Enter rock/r, paper/p, scissors/s, or quit.

> rock
You: rock, computer: scissors
Rock crushes scissors. You win!
Score 1:0 (draws: 0)

> paper
You: paper, computer: rock
Paper covers rock. You win!
Score 2:0 (draws: 0)

You won the match!
```

## Тесты

Ручной запуск тестов:

```sh
ctest --test-dir build --output-on-failure
```

Для Visual Studio:

```powershell
ctest --test-dir build --output-on-failure -C Release
```

Тесты проверяют:

- разбор пользовательского ввода;
- все девять комбинаций ходов;
- подсчёт результатов;
- завершение матча;
- некорректные состояния;
- воспроизводимость генератора.

Чтобы не запускать тесты автоматически при сборке:

```sh
cmake -S . -B build -DRPS_TESTS_DURING_BUILD=OFF
```

Чтобы не собирать тесты:

```sh
cmake -S . -B build -DRPS_BUILD_TESTS=OFF
```

## Структура проекта

```text
.
├── CMakeLists.txt
├── README.md
├── include/rps/game.hpp
├── src/
│   ├── game.cpp
│   └── main.cpp
└── tests/test_game.cpp
```

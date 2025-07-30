# Search Engine

Простой полнотекстовый поисковый движок на C++, реализующий обратный индекс и ранжирование документов по релевантности.

## 🧩 Возможности
- Построение обратного индекса (`InvertedIndex`) по коллекции документов
- Многопоточный поиск по запросам (`SearchServer`)
- Ранжирование результатов на основе частоты совпадений
- Чтение и запись данных в формате JSON (`ConverterJSON`)
- Юнит-тесты на базе GoogleTest
- CMake-проект с раздельной сборкой:
  - `search_engine` — основное приложение
  - `search_tests` — модульные тесты

## 📁 Структура проекта
search_engine/
├── include/ # Заголовочные файлы
├── src/ # Основная реализация
├── tests/ # Тесты (GoogleTest)
├── resources/ # JSON-файлы: config.json, requests.json и др.
├── CMakeLists.txt # Главный CMake-файл
├── LICENSE
└── README.md

## ⚙️ Сборка проекта
> Требования: CMake 3.21+, компилятор с поддержкой C++17
```bash
cmake -B build
cmake --build build

🚀 Запуск приложения
bash
./build/search_engine
После запуска программа обработает запросы из resources/requests.json и создаст answers.json.

🧪 Запуск тестов
bash
./build/search_tests
Тесты построены на базе GoogleTest и автоматически подтягиваются через CMake.

📦 Зависимости
C++17

nlohmann/json — для работы с JSON (включён в проект)

GoogleTest — подтягивается автоматически через FetchContent в CMake

📄 Лицензия
MIT License — см. файл LICENSE


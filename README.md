# 🔍 Search Engine

Простой поисковый движок на C++, реализующий полнотекстовый поиск по набору документов.

## 🚀 Возможности

- Построение обратного индекса (`InvertedIndex`) для документов
- Поддержка многопоточного поиска (`SearchServer`)
- Подсчёт релевантности документов на основе количества совпадений слов
- Работа с конфигурациями и результатами в формате **JSON** (`ConverterJSON`)
- Удобная структура проекта и модульные тесты на **GoogleTest**

## 📁 Структура проекта

search_engine/
├── include/ # Заголовочные файлы
├── resources/ # Конфигурационные JSON-файлы (документы, запросы и т.д.)
├── src/ # Исходный код
├── tests/ # Модульные тесты
└── CMakeLists.txt # CMake-конфигурация


## ⚙️ Сборка проекта

```bash
cmake -B build
cmake --build build

▶️ Запуск
bash
Копировать
Редактировать
./build/search_engine

🧪 Тестирование
bash
Копировать
Редактировать
./build/search_tests

📦 Зависимости
C++17

nlohmann/json (включён в проект)

GoogleTest (автоматически загружается через CMake)

📄 Лицензия
Проект распространяется под лицензией MIT.

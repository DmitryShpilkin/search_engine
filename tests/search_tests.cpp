#include "gtest/gtest.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

/// Вспомогательная функция для тестирования InvertedIndex
void TestInvertedIndexFunctionality(
    const std::vector<std::string>& docs,
    const std::vector<std::string>& requests,
    const std::vector<std::vector<Entry>>& expected
) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for (const auto& request : requests) {
        auto word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}

/// Простой тест на совпадение слов "london" и "the"
TEST(TestCaseInvertedIndex, TestBasic) {
    const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
    };
    const std::vector<std::string> requests = { "london", "the" };
    const std::vector<std::vector<Entry>> expected = {
        {
            {0, 1}
        }, {
            {0, 1}, {1, 3}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

/// Расширенный тест с разными количествами слов "milk" и "water"
TEST(TestCaseInvertedIndex, TestBasic2) {
    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const std::vector<std::string> requests = { "milk", "water", "cappuccino" };
    const std::vector<std::vector<Entry>> expected = {
        {
            {0, 4}, {1, 1}, {2, 5}
        }, {
            {0, 3}, {1, 2}, {2, 5}
        }, {
            {3, 1}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

/// Проверка обработки слов, которых нет в базе
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const std::vector<std::string> docs = {
        "a b c d e f g h i j k l",
        "statement"
    };
    const std::vector<std::string> requests = { "m", "statement" };
    const std::vector<std::vector<Entry>> expected = {
        {
            // Пусто: слово "m" не встречается
        }, {
            {1, 1}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

/// Тестирование SearchServer — поиск слов по релевантности
TEST(TestCaseSearchServer, TestSimple) {
    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const std::vector<std::string> request = { "milk water", "sugar" };
    const std::vector<std::vector<RelativeIndex>> expected = {
        {
            {2, 1},
            {0, 0.7f},
            {1, 0.3f}
        },
        {
            // Пусто: слово "sugar" не встречается
        }
    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    auto result = srv.search(request);
    ASSERT_EQ(result, expected);
}

/// Проверка, что SearchServer возвращает только топ-5 результатов
TEST(TestCaseSearchServer, TestTop5) {
    const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland",
    };
    const std::vector<std::string> request = { "moscow is the capital of russia" };
    const std::vector<std::vector<RelativeIndex>> expected = {
        {
            {7, 1},
            {14, 1},
            {0, 0.6666667f},
            {1, 0.6666667f},
            {2, 0.6666667f}
        }
    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    auto result = srv.search(request);
    ASSERT_EQ(result, expected);
}

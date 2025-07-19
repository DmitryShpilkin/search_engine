#pragma once

#include "InvertedIndex.h"
#include <string>
#include <vector>

/**
 * @brief Структура, описывающая относительную релевантность документа.
 */
struct RelativeIndex {
    size_t doc_id;  ///< ID документа
    float rank;     ///< Релевантность документа в диапазоне [0;1]

    // Сравнение по идентификатору документа и его релевантности
    bool operator==(const RelativeIndex& other) const {
        return doc_id == other.doc_id && std::abs(rank - other.rank) < 1e-6;
    }
};

/**
 * @brief Класс, реализующий поисковый сервер на основе обратного индекса.
 */
class SearchServer {
public:
    /**
     * @brief Конструктор, принимающий ссылку на готовый инвертированный индекс.
     * @param idx Инвертированный индекс, содержащий информацию о частоте слов в документах.
     */
    explicit SearchServer(InvertedIndex& idx);

    /**
     * @brief Выполняет поиск по заданным запросам.
     * @param queries_input Вектор строковых запросов (поисковых фраз).
     * @return Вектор списков релевантных документов для каждого запроса.
     *         Каждый результат отсортирован по убыванию релевантности.
     */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex& _index;  ///< Ссылка на внешний инвертированный индекс
};

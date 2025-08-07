#pragma once
#include <vector>
#include <string>
#include <utility>

/**
 * @brief Класс для работы с JSON-файлами конфигурации, документов, запросов и ответов.
 *
 * Обеспечивает чтение исходных текстовых документов, поисковых запросов,
 * а также запись результатов поиска в JSON-файл.
 */
class ConverterJSON {
public:
    /**
     * @brief Загружает тексты документов из файлов, указанных в config.json.
     * @return Вектор строк, каждая из которых — содержимое одного документа.
     * @throws std::runtime_error при отсутствии или некорректности config.json.
     */
    std::vector<std::string> GetTextDocuments();

    /**
     * @brief Получает ограничение на максимальное количество ответов для каждого запроса.
     * @return Максимальное количество ответов (если не указано в config.json — возвращает 5).
     * @throws std::runtime_error при отсутствии или некорректности config.json.
     */
    int GetResponsesLimit();

    /**
     * @brief Загружает поисковые запросы из requests.json.
     * @return Вектор строк с запросами.
     * @throws std::runtime_error при отсутствии или некорректности requests.json.
     */
    std::vector<std::string> GetRequests();

    /**
     * @brief Записывает результаты поиска в файл answers.json.
     * @param answers Результаты поиска: вектор для каждого запроса, содержащий пары (doc_id, релевантность).
     */
    void putAnswers(const std::vector<std::vector<std::pair<size_t, float>>>& answers);
};

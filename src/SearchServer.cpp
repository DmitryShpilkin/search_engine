#include "SearchServer.h"
#include <algorithm>
#include <numeric>
#include <thread>
#include <mutex>
#include <sstream>
#include <unordered_map>

SearchServer::SearchServer(InvertedIndex& idx) : _index(idx) {}

// Обрабатываем список запросов и возвращаем результаты поиска
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input, int max_responses) {
    std::vector<std::vector<RelativeIndex>> results(queries_input.size());
    std::mutex result_mutex;
    std::vector<std::thread> workers;

    // RAII-структура для безопасного завершения потоков
    struct ThreadJoiner {
        std::vector<std::thread>& threads;
        ~ThreadJoiner() {
            for (auto& t : threads)
                if (t.joinable()) t.join();
        }
    } joiner{workers};

    // Обработка каждого запроса в отдельном потоке
    for (size_t i = 0; i < queries_input.size(); ++i) {
        workers.emplace_back([this, &queries_input, &results, &result_mutex, i, max_responses]() {
            std::unordered_map<size_t, size_t> doc_relevance;
            std::istringstream stream(queries_input[i]);
            std::string word;

            // Подсчитываем частоту слов из запроса в документах
            while (stream >> word) {
                if (word.length() > 100)
                   continue; // игнорируем слишком длинные слова

               for (const auto& entry : _index.GetWordCount(word)) {
               doc_relevance[entry.doc_id] += entry.count;
               }
            }


            if (doc_relevance.empty())
                return;

            // Находим максимальную релевантность
           size_t max_relevance = std::max_element(
                doc_relevance.begin(), doc_relevance.end(),
                [](const auto& lhs, const auto& rhs) {
                    return lhs.second < rhs.second;
                }
            )->second;

            std::vector<RelativeIndex> rel_indices;
            rel_indices.reserve(doc_relevance.size());

            for (const auto& [doc_id, count] : doc_relevance) {
                float rank = static_cast<float>(count) / max_relevance;
                rel_indices.push_back({ doc_id, rank });
            }

            // Сортируем: сначала по убыванию rank, затем по возрастанию doc_id
            std::sort(rel_indices.begin(), rel_indices.end(), [](const auto& lhs, const auto& rhs) {
                if (lhs.rank == rhs.rank)
                    return lhs.doc_id < rhs.doc_id;
                return lhs.rank > rhs.rank;
            });

            // Ограничим top-N результатов (по умолчанию 5)
            if (rel_indices.size() > static_cast<size_t>(max_responses)) {
                 rel_indices.resize(max_responses);
             }

            // Потокобезопасное сохранение результата
            std::lock_guard<std::mutex> lock(result_mutex);
             results[i] = std::move(rel_indices);
         });
    }

    // Все потоки завершатся через ThreadJoiner
    return results;
}

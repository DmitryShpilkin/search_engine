#include "InvertedIndex.h"
#include <sstream>
#include <map>
#include <thread>
#include <mutex>
#include <algorithm>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    // Заменяем старую базу документов
    docs = std::move(input_docs);

    // Полностью очищаем словарь (вместе с памятью)
    std::map<std::string, std::vector<Entry>>().swap(freq_dictionary);

    std::mutex freq_mutex;
    std::vector<std::thread> workers;

    // RAII-объект для автоматического join всех потоков
    struct ThreadJoiner {
        std::vector<std::thread>& threads;
        ~ThreadJoiner() {
            for (auto& t : threads)
                if (t.joinable()) t.join();
        }
    } joiner{workers};

    // Запускаем параллельную обработку каждого документа
    for (size_t doc_id = 0; doc_id < docs.size(); ++doc_id) {
        workers.emplace_back([this, &freq_mutex, doc_id]() {
            std::istringstream stream(docs[doc_id]);
            std::map<std::string, size_t> word_count;
            std::string word;

            // Подсчитываем частоту слов в документе
            while (stream >> word) {
                ++word_count[word];
            }

            // Безопасно добавляем частоты в общий словарь
            std::lock_guard<std::mutex> lock(freq_mutex);
            for (const auto& [w, count] : word_count) {
                freq_dictionary[w].push_back({ doc_id, count });
            }
        });
    }

    // Потоки автоматически join-нутся через ThreadJoiner
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    if (freq_dictionary.count(word) == 0) {
        return {};  // слово не найдено
    }
    auto result = freq_dictionary[word];

    // Сортируем по doc_id для стабильности
    std::sort(result.begin(), result.end(), [](const Entry& a, const Entry& b) {
        return a.doc_id < b.doc_id;
    });

    return result;
}

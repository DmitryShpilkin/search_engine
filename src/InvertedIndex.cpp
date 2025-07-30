#include "InvertedIndex.h"
#include <sstream>
#include <map>
#include <thread>
#include <mutex>
#include <algorithm>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    // Если документы пусты — очищаем и выходим
    if (input_docs.empty()) {
        docs.clear();
        freq_dictionary.clear();  // Быстрое очищение без лишних аллокаций
        return;
    }

    // Заменяем старую базу документов на новую
    docs = std::move(input_docs);

    // Полностью очищаем словарь с потенциальным освобождением памяти
    std::map<std::string, std::vector<Entry>>().swap(freq_dictionary);

    std::mutex freq_mutex;
    std::vector<std::thread> workers;

    // RAII-структура для корректного завершения всех потоков
    struct ThreadJoiner {
        std::vector<std::thread>& threads;
        ~ThreadJoiner() {
            for (auto& t : threads)
                if (t.joinable()) t.join();
        }
    } joiner{workers};

    // Обработка каждого документа в отдельном потоке
    for (size_t doc_id = 0; doc_id < docs.size(); ++doc_id) {
        workers.emplace_back([this, &freq_mutex, doc_id]() {
            std::istringstream stream(docs[doc_id]);
            std::map<std::string, size_t> word_count;
            std::string word;

            // Подсчитываем частоту слов в документе
            while (stream >> word) {
                ++word_count[word];
            }

            // Потокобезопасное обновление общей частотной таблицы
            std::lock_guard<std::mutex> lock(freq_mutex);
            for (const auto& [w, count] : word_count) {
                freq_dictionary[w].push_back({ doc_id, count });
            }
        });
    }

    // Потоки будут завершены автоматически в деструкторе joiner
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    auto it = freq_dictionary.find(word);
    if (it == freq_dictionary.end()) {
        return {};  // Слово не найдено
    }

    std::vector<Entry> result = it->second;

    // Сортировка по doc_id для стабильности
    std::sort(result.begin(), result.end(), [](const Entry& a, const Entry& b) {
        return a.doc_id < b.doc_id;
    });

    return result;
}

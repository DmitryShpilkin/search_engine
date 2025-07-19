#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
    try {
        ConverterJSON converter;

        // Читаем документы
        auto documents = converter.GetTextDocuments();

        // Индексируем документы
        InvertedIndex index;
        index.UpdateDocumentBase(documents);

        // Читаем поисковые запросы
        auto requests = converter.GetRequests();

        // Запускаем поисковый сервер
        SearchServer search_server(index);
        auto answers = search_server.search(requests);

        // Подготовка данных для записи
        std::vector<std::vector<std::pair<int, float>>> output_answers;
        for (const auto& result_vector : answers) {
            std::vector<std::pair<int, float>> ranked_entries;
            for (const auto& rel_idx : result_vector) {
                ranked_entries.emplace_back(rel_idx.doc_id, rel_idx.rank);
            }
            output_answers.push_back(std::move(ranked_entries));
        }

        // Записываем результаты в answers.json
        converter.putAnswers(output_answers);

        std::cout << "Writing answers to JSON completed.\n";
        std::cout << "Search completed successfully.\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

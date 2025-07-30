#include "ConverterJSON.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Получение документов из config.json
std::vector<std::string> ConverterJSON::GetTextDocuments() {
    // Открываем конфигурационный файл
    std::ifstream config_file("resources/config.json");
    if (!config_file.is_open()) {
        throw std::runtime_error("config file is missing");
    }

    // Загружаем JSON
    json config_json;
    config_file >> config_json;

    // Проверка наличия ключа "config"
    if (!config_json.contains("config")) {
        throw std::runtime_error("config file is empty");
    }

    // Проверка версии файла
    if (!config_json["config"].contains("version") || config_json["config"]["version"] != "0.1") {
        throw std::runtime_error("config.json has incorrect file version");
    }

    std::vector<std::string> documents;

    // Читаем список файлов документов
    if (config_json.contains("files")) {
        for (const auto& filepath_json : config_json["files"]) {
            std::string filepath = filepath_json.get<std::string>();
            std::ifstream doc_file(filepath);

            // Если файл не открывается — предупреждаем и пропускаем
            if (!doc_file.is_open()) {
                std::cerr << "Warning: Failed to open document file: " << filepath << "\n";
                continue;
            }

            // Читаем содержимое файла целиком в строку
            std::string content((std::istreambuf_iterator<char>(doc_file)),
                                 std::istreambuf_iterator<char>());

            documents.push_back(content); // копирование, move не нужен
        }
    }

    return documents;
}

// Получение ограничения количества ответов
int ConverterJSON::GetResponsesLimit() {
    std::ifstream config_file("resources/config.json");
    if (!config_file.is_open()) {
        throw std::runtime_error("config file is missing");
    }

    json config_json;
    config_file >> config_json;

    if (!config_json.contains("config")) {
        throw std::runtime_error("config file is empty");
    }

    // Если указано max_responses — возвращаем его
    if (config_json["config"].contains("max_responses")) {
        return config_json["config"]["max_responses"].get<int>();
    }

    // Иначе по умолчанию — 5
    return 5;
}

// Чтение поисковых запросов из requests.json
std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream requests_file("resources/requests.json");
    if (!requests_file.is_open()) {
        throw std::runtime_error("requests file is missing");
    }

    json requests_json;
    requests_file >> requests_json;

    std::vector<std::string> requests;

    // Получаем массив строковых запросов
    if (requests_json.contains("requests")) {
        for (const auto& req : requests_json["requests"]) {
            requests.push_back(req.get<std::string>());
        }
    }

    return requests;
}

// Сохраняем результаты поиска в файл answers.json
void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    json answers_json;
    answers_json["answers"] = json::object();

    // Обработка каждого запроса
    for (size_t i = 0; i < answers.size(); ++i) {
        // Формируем идентификатор вида "request001"
        std::string prefix = (i < 9) ? "00" : (i < 99) ? "0" : "";
        std::string request_id = "request" + prefix + std::to_string(i + 1);

        // Если на запрос нет ответов
        if (answers[i].empty()) {
            answers_json["answers"][request_id]["result"] = "false";
            continue;
        }

        answers_json["answers"][request_id]["result"] = "true";

        // Один результат — короткая запись
        if (answers[i].size() == 1) {
            answers_json["answers"][request_id]["docid"] = answers[i][0].first;
            answers_json["answers"][request_id]["rank"] = answers[i][0].second;
        } else {
            // Несколько — записываем как массив объектов
            json relevance_array = json::array();
            for (const auto& [docid, rank] : answers[i]) {
                relevance_array.push_back({
                    {"docid", docid},
                    {"rank", rank}
                });
            }
            answers_json["answers"][request_id]["relevance"] = relevance_array;
        }
    }

    // Пишем результат в файл
    std::ofstream answers_file("answers.json");
    if (!answers_file.is_open()) {
        throw std::runtime_error("Unable to open answers.json for writing");
    }

    // Красиво оформляем JSON с отступами
    answers_file << answers_json.dump(2);
    answers_file.flush();
    std::cout << "Written and flushed to answers.json\n";
}

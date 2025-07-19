#include "ConverterJSON.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Функция читает список текстовых документов из файлов, указанных в config.json
std::vector<std::string> ConverterJSON::GetTextDocuments() {
    // Открываем config.json из папки ресурсов
    std::ifstream config_file("resources/config.json");
    if (!config_file.is_open()) {
        throw std::runtime_error("config file is missing");
    }

    json config_json;
    config_file >> config_json;

    // Проверяем, что в файле есть нужный раздел "config"
    if (config_json.find("config") == config_json.end()) {
        throw std::runtime_error("config file is empty");
    }

    // Проверяем версию файла, чтобы избежать проблем с несовместимостью
    if (config_json["config"].find("version") == config_json["config"].end() ||
        config_json["config"]["version"] != "0.1") {
        throw std::runtime_error("config.json has incorrect file version");
    }

    std::vector<std::string> documents;
    // Читаем список файлов документов из поля "files"
    if (config_json.find("files") != config_json.end()) {
        for (const auto& filepath : config_json["files"]) {
            std::ifstream doc_file(filepath.get<std::string>());

            // Считываем весь файл в строку
            std::string content((std::istreambuf_iterator<char>(doc_file)),
                                std::istreambuf_iterator<char>());
            documents.push_back(std::move(content));
        }
    }
    return documents;
}

// Функция возвращает ограничение на количество ответов из config.json
int ConverterJSON::GetResponsesLimit() {
    std::ifstream config_file("resources/config.json");
    if (!config_file.is_open()) {
        throw std::runtime_error("config file is missing");
    }

    json config_json;
    config_file >> config_json;

    if (config_json.find("config") == config_json.end()) {
        throw std::runtime_error("config file is empty");
    }

    // Если в config указано max_responses, возвращаем это число
    if (config_json["config"].find("max_responses") != config_json["config"].end()) {
        return config_json["config"]["max_responses"].get<int>();
    }
    // Иначе возвращаем значение по умолчанию — 5
    return 5;
}

// Функция читает поисковые запросы из файла requests.json
std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream requests_file("resources/requests.json");
    if (!requests_file.is_open()) {
        throw std::runtime_error("requests file is missing");
    }

    json requests_json;
    requests_file >> requests_json;

    std::vector<std::string> requests;
    // Читаем массив запросов из поля "requests"
    if (requests_json.find("requests") != requests_json.end()) {
        for (const auto& req : requests_json["requests"]) {
            requests.push_back(req.get<std::string>());
        }
    }
    return requests;
}

// Функция записывает результаты поиска в файл answers.json в формате JSON
void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    json answers_json;
    answers_json["answers"] = json::object();

    // Формируем идентификаторы запросов в формате request001, request002 и т.д.
    for (size_t i = 0; i < answers.size(); ++i) {
        std::string number_prefix;
        if (i < 9) number_prefix = "00";
        else if (i < 99) number_prefix = "0";
        else number_prefix = "";

        std::string request_id = "request" + number_prefix + std::to_string(i + 1);

        // Если ответов на запрос нет, пишем result = false
        if (answers[i].empty()) {
            answers_json["answers"][request_id]["result"] = "false";
            continue;
        }

        answers_json["answers"][request_id]["result"] = "true";

        // Если ответ один, пишем docid и rank напрямую
        if (answers[i].size() == 1) {
            answers_json["answers"][request_id]["docid"] = answers[i][0].first;
            answers_json["answers"][request_id]["rank"] = answers[i][0].second;
        }
        else {
            // Если ответов несколько, формируем массив relevance с объектами docid и rank
            json relevance_array = json::array();
            for (const auto& [docid, rank] : answers[i]) {
                json entry;
                entry["docid"] = docid;
                entry["rank"] = rank;
                relevance_array.push_back(entry);
            }
            answers_json["answers"][request_id]["relevance"] = relevance_array;
        }
    }

    // Открываем файл answers.json для записи (перезаписываем существующий)
    std::ofstream answers_file("answers.json");


    // Конвертируем JSON в красивую строку с отступами и записываем в файл
    std::string string_builder = answers_json.dump(2);


    answers_file << string_builder;
    answers_file.flush();
    std::cout << "Written and flushed to answers.json\n";
}

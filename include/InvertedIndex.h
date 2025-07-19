#pragma once

#include <string>
#include <vector>
#include <map>

/**
 * @struct Entry
 * Структура, описывающая частоту слова в конкретном документе
 */
struct Entry {
    size_t doc_id;  ///< ID документа
    size_t count;   ///< Количество вхождений слова в документ

    bool operator==(const Entry& other) const {
        return doc_id == other.doc_id && count == other.count;
    }
};

/**
 * @class InvertedIndex
 * Класс для построения и работы с инвертированным индексом.
 */
class InvertedIndex {
public:
    InvertedIndex() = default;

    /**
     * Обновляет базу документов и пересчитывает словарь частот.
     * @param input_docs Вектор строк с содержимым документов.
     */
    void UpdateDocumentBase(std::vector<std::string> input_docs);

    /**
     * Получает список документов, в которых встречается слово, и его частоту.
     * @param word Слово, для которого требуется статистика.
     * @return Вектор структур Entry, отсортированных по doc_id.
     */
    std::vector<Entry> GetWordCount(const std::string& word);

private:
    std::vector<std::string> docs; ///< Оригинальные документы
    std::map<std::string, std::vector<Entry>> freq_dictionary; ///< Частотный словарь слов
};

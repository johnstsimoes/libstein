#include "libstein/csv_reader.h"

#include <vector>

using namespace libstein;

enum class CSVState
{
    UnquotedField,
    QuotedField,
    QuotedQuote
};

CSVReader::CSVReader(std::istream &file) : file (file)
{
}

CSVReader::~CSVReader()
{
}

std::vector<std::string> CSVReader::readCSVRow(const std::string &row)
{
    CSVState state = CSVState::UnquotedField;
    std::vector<std::string> fields {""};
    size_t i = 0; // index of the current field
    for (char c : row)
    {
        switch (state)
        {
            case CSVState::UnquotedField:
                switch (c) {
                    case ',': // end of field
                              fields.push_back(""); i++;
                              break;
                    case '"': state = CSVState::QuotedField;
                              break;
                    default:  fields[i].push_back(c);
                              break; }
                break;
            case CSVState::QuotedField:
                switch (c) {
                    case '"': state = CSVState::QuotedQuote;
                              break;
                    default:  fields[i].push_back(c);
                              break; }
                break;
            case CSVState::QuotedQuote:
                switch (c) {
                    case ',': // , after closing quote
                              fields.push_back(""); i++;
                              state = CSVState::UnquotedField;
                              break;
                    case '"': // "" -> "
                              fields[i].push_back('"');
                              state = CSVState::QuotedField;
                              break;
                    default:  // end of quote
                              state = CSVState::UnquotedField;
                              break; }
                break;
        }
    }
    return fields;
}

/// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes"""
std::vector<std::vector<std::string>> CSVReader::readCSV()
{
    std::vector<std::vector<std::string>> table;
    std::string row;
    while (!this->file.eof())
    {
        std::getline(this->file, row);
        if (this->file.bad() || this->file.fail())
        {
            break;
        }
        auto fields = readCSVRow(row);
        table.push_back(fields);
    }
    return table;
}

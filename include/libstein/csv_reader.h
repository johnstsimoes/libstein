#pragma once

#include <fstream>
#include <istream>

namespace libstein
{
    class CSVReader
    {
    private:
        std::istream &file;
        std::vector<std::string> readCSVRow(const std::string &);

    public:

        CSVReader(std::istream &);
        ~CSVReader();

        std::vector<std::vector<std::string>> readCSV();
    };
}
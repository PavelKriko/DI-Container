#include "ReadCsv.h"

#include <fstream>

void readCsv(const std::string& nameFile, X& x, y& y) {

    std::ifstream csv(nameFile);
    std::string str;
    std::getline(csv, str);
    size_t posEnd = 1;

    if (csv.is_open()) {
        while (!csv.eof()) {

            std::getline(csv, str);

            std::vector<double> object;
            std::vector<bool> feature;

            if (!str.empty()) {
                for (size_t posStart = 0; posStart < str.size(); ) {
                    posEnd = str.find(';', posStart);
                    if (posEnd != std::string::npos) {
                        object.push_back(std::stod(str.substr(posStart, posEnd)));
                        posStart = posEnd + 1;
                    }
                    else {
                        y.push_back(static_cast<bool>(std::stod(str.substr(posStart, str.size()))));
                        break;
                    }

                }
                x.push_back(object);
            }

        }
    }
    else {
        throw std::runtime_error("no file in such directory");
    }
}
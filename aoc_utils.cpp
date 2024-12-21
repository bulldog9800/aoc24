#include "aoc_utils.hpp"
#include "default.hpp"

void InputReader::readFile(const string& fileName) {

    ifstream in{fileName};
    if (!in) {
        cerr << "Cannot open file " << fileName << endl;
        assert(0);
    }

    std::string str;
    while (std::getline(in, str)) {
        m_lines.push_back(str);
    }
}

vector<string>& InputReader::getLines() {
    return m_lines;
}

vector<string> StringTokenizer::sGetTokens(string str, string delimiter) {
    size_t iterator = 0;
    vector<string> tokens;

    while (iterator != string::npos) {
        string slicedStr = str.substr(iterator);
        size_t nextPos = slicedStr.find(delimiter);

        if (nextPos == string::npos) {
            iterator = string::npos;
            tokens.push_back(slicedStr);
            break;
        }

        if (nextPos != 0) {
            tokens.push_back(slicedStr.substr(0, nextPos));
        }
        iterator += nextPos + delimiter.size();
    }

    return tokens;
}

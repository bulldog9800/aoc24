#include "../aoc_utils.hpp"

class Stones {
public:
    void initFromLine(vector<string>& lines) {
        assert(lines.size() == 1);

        vector<string> tokens = StringTokenizer::sGetTokens(lines[0], " ");
        for (string& token : tokens) {
            UINT64 num = std::stoull(token);

            if (m_appearances.find(num) == m_appearances.end()) {
                m_appearances[num] = 1;
            } else {
                m_appearances[num]++;
            }
        }
    }

    UINT64 part1() {
        for (int i = 0; i < 75; i++) {
            blink();
        }

        UINT64 size = 0;
        for (auto& val : m_appearances) {
            size += val.second;
        }

        return size;
    }

private:
    void blink() {
        unordered_map<UINT64, UINT64> tmp;

        for (auto it : m_appearances) {
            UINT64 stone = it.first;
            UINT64 appearances = it.second;

            if (stone == 0) {
                tmp[1] += appearances;
                continue;
            }

            string numAsStr = std::to_string(stone);
            if (numAsStr.size() % 2 == 0) {
                UINT64 firstHalf =
                    std::stoull(numAsStr.substr(0, numAsStr.size() / 2));
                UINT64 secondHalf = std::stoull(
                    numAsStr.substr(numAsStr.size() / 2, numAsStr.size() / 2));

                tmp[firstHalf] += appearances;
                tmp[secondHalf] += appearances;
            } else {
                tmp[stone * 2024] += appearances;
            }
        }

        m_appearances = tmp;
    }

    std::string trimLeadingZeros(const std::string& str) {
        size_t firstNonZero = str.find_first_not_of('0');
        if (firstNonZero == std::string::npos) {
            // The string is all zeros, return "0"
            return "0";
        }
        return str.substr(firstNonZero);
    }

    bool isEvenNumOfDigits(UINT64 num) {
        int numOfDigits = 0;
        int x = num;

        while (x > 0) {
            numOfDigits++;
            x /= 10;
        }

        return numOfDigits % 2 == 0;
    }

    unordered_map<UINT64, UINT64> m_appearances;
};

int main() {
    InputReader inputReader;
    inputReader.readFile("/Users/whysoserious/repos/aoc24/day11/input.txt");
    vector<string>& rLines = inputReader.getLines();

    Stones stones;
    stones.initFromLine(rLines);

    cout << stones.part1() << endl;

    return 0;
}

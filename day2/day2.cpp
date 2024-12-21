#include "../aoc_utils.hpp"

class Report {
public:
    void fillFromLine(string& line) {
        vector<string> tokens = StringTokenizer::sGetTokens(line, " ");

        for (auto token : tokens) {
            m_levels.push_back(std::stoi(token));
        }
    }

    bool isSafe() {
        for (int j = 0; j < m_levels.size(); j++) {
            vi levels(m_levels);
            levels.erase(levels.begin() + j);

            bool isIncreasing = false;
            bool isSafe = true;
            /*
             * Check if the first 2 levels are in increasing or decreasing
             * order.
             */
            if (levels[1] >= levels[0]) {
                isIncreasing = true;
            }

            for (int i = 1; i < levels.size(); i++) {
                /* Check if the increasing/decreasing condition is violated. */
                if (isIncreasing && levels[i] <= levels[i - 1]) {
                    isSafe = false;
                    break;
                }
                if (isIncreasing == false && levels[i] >= levels[i - 1]) {
                    isSafe = false;
                    break;
                }

                /* If not, check if the difference is within the allowed range.
                 */
                int diff = std::abs(levels[i] - levels[i - 1]);
                if (diff < skMinDiffAllowed || diff > skMaxDiffAllowed) {
                    isSafe = false;
                    break;
                }
            }

            if (isSafe) {
                return true;
            }
        }

        return false;
    }

private:
    static constexpr int skMinDiffAllowed{1};
    static constexpr int skMaxDiffAllowed{3};

    vi m_levels;
};

int main() {
    InputReader inputReader;
    inputReader.readFile("/Users/whysoserious/repos/aoc24/day2/input.txt");
    vector<string>& rLines = inputReader.getLines();

    int numOfSafeReports = 0;
    for (auto& line : rLines) {
        Report report;
        report.fillFromLine(line);

        if (report.isSafe()) {
            numOfSafeReports++;
        }
    }

    cout << numOfSafeReports << endl;

    return 0;
}

#include "../aoc_utils.hpp"

class AntennaManager {
public:
    void initFromLines(vector<string>& rLines) {
        M = rLines.size();
        N = rLines[0].size();

        for (int i = 0; i < rLines.size(); i++) {
            for (int j = 0; j < rLines[0].size(); j++) {
                if (rLines[i][j] != skDefaultNode) {
                    m_antennas[rLines[i][j]].push_back(ii(i, j));
                }
            }
        }
    }

    bool isInRange(ii coordinates) {
        return coordinates.first >= 0 && coordinates.first < M &&
               coordinates.second >= 0 && coordinates.second < N;
    }

    void addAntiNodesForTwoAntennas(ii firstLocation, ii secondLocation) {
        /* Insert the two antennas. */
        m_antinodes.insert(firstLocation);
        m_antinodes.insert(secondLocation);

        int x_diff = firstLocation.first - secondLocation.first;
        int y_diff = firstLocation.second - secondLocation.second;

        ii antinode1(firstLocation.first + x_diff,
                     firstLocation.second + y_diff);

        while (isInRange(antinode1)) {
            m_antinodes.insert(antinode1);

            antinode1.first += x_diff;
            antinode1.second += y_diff;
        }

        ii antinode2(secondLocation.first - x_diff,
                     secondLocation.second - y_diff);

        while (isInRange(antinode2)) {
            m_antinodes.insert(antinode2);

            antinode2.first -= x_diff;
            antinode2.second -= y_diff;
        }
    }

    void addAllAntinodes() {
        for (const auto& [antennaSymbol, locations] : m_antennas) {
            for (int i = 0; i < locations.size(); i++) {
                for (int j = i + 1; j < locations.size(); j++) {
                    addAntiNodesForTwoAntennas(locations[i], locations[j]);
                }
            }
        }
    }

    int getNumOfAntinodes() {
        return m_antinodes.size();
    }

private:
    static constexpr char skDefaultNode = '.';

    int M;
    int N;

    unordered_set<ii, PairHash> m_antinodes;

    unordered_map<char, vector<ii>> m_antennas;
};

int main() {
    InputReader inputReader;
    inputReader.readFile("/Users/whysoserious/repos/aoc24/day8/input.txt");
    vector<string>& rLines = inputReader.getLines();

    AntennaManager antennaMgr;
    antennaMgr.initFromLines(rLines);

    antennaMgr.addAllAntinodes();

    cout << antennaMgr.getNumOfAntinodes() << endl;

    return 0;
}

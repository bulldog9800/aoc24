#include "../aoc_utils.hpp"

class Bathroom {
public:
    class Robot {
    public:
        void initFromLine(string line) {
            vector<string> tokens = StringTokenizer::sGetTokens(line, " ");
            string position = tokens[0];
            string velocity = tokens[1];

            m_initialPosition = parseCoordinatesStr(position);
            m_velocity = parseCoordinatesStr(velocity);
        }

        ii parseCoordinatesStr(string coordinatesStr) {
            vector<string> tmp =
                StringTokenizer::sGetTokens(coordinatesStr, "=");
            vector<string> coordinates =
                StringTokenizer::sGetTokens(tmp[1], ",");

            return ii(stoi(coordinates[0]), stoi(coordinates[1]));
        }

        ii m_initialPosition;

        ii m_velocity;
    };

    void calcRobotLocationAfterTime(Robot& rRobot, int numOfSeconds);

    UINT64 calcSafetyScore();

    void print() {
        for (vi& row : m_grid) {
            for (int i : row) {
                char c;
                if (i == 0) {
                    c = '.';
                } else {
                    c = '#';
                }
                cout << c;
            }
            cout << endl;
        }
    }

    void reset() {
        for (int i = 0; i < m_grid.size(); i++) {
            for (int j = 0; j < m_grid[0].size(); j++) {
                m_grid[i][j] = 0;
            }
        }
    }

    Bathroom() : m_grid(vvi(skLength, vi(skWidth, 0))) {
    }

private:
    static constexpr int skWidth{101};
    static constexpr int skLength{103};
    vi m_quadrants{vi(4, 0)};

    vvi m_grid;
};

void Bathroom::calcRobotLocationAfterTime(Robot& rRobot, int numOfSeconds) {
    int resX = (rRobot.m_initialPosition.first +
                rRobot.m_velocity.first * numOfSeconds) %
               skWidth;
    if (resX < 0) {
        resX = skWidth + resX;
    }

    int resY = (rRobot.m_initialPosition.second +
                rRobot.m_velocity.second * numOfSeconds) %
               skLength;
    if (resY < 0) {
        resY = skLength + resY;
    }

    if (resX == (skWidth - 1) / 2 || resY == (skLength - 1) / 2) {
        return;
    }

    int quadrant = ((resX < (skWidth - 1) / 2) ? 0 : 1) +
                   ((resY < (skLength - 1) / 2) ? 0 : 2);
    assert(quadrant >= 0 && quadrant < 4);

    m_quadrants[quadrant]++;

    m_grid[resX][resY]++;
}

UINT64 Bathroom::calcSafetyScore() {
    UINT64 res = 1;
    for (int amount : m_quadrants) {
        res *= amount;
    }

    return res;
}

int main() {
    InputReader inputReader;
    inputReader.readFile("/Users/whysoserious/repos/aoc24/day14/input.txt");
    vector<string>& rLines = inputReader.getLines();
    Bathroom bathroom;
    vector<Bathroom::Robot> robots;

    for (string& line : rLines) {
        Bathroom::Robot robot;
        robot.initFromLine(line);

        robots.push_back(robot);
    }

    for (int second = 31; second < 100000000; second += 103) {
        cout << second << " seconds: " << endl;

        for (auto& robot : robots) {
            bathroom.calcRobotLocationAfterTime(robot, second);
        }

        bathroom.print();
        bathroom.reset();

        cout << endl;
    }

    cout << bathroom.calcSafetyScore() << endl;

    return 0;
}

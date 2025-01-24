#include "../aoc_utils.hpp"

class Warehouse {
public:
    int countBoxes() {
        int res = 0;
        for (auto& s : m_grid) {
            for (char c : s) {
                if (c == skBoxLeft) {
                    res++;
                }
            }
        }

        return res;
    }

    void printGrid() {
        for (auto& s : m_grid) {
            cout << s << endl;
        }
    }

    int initFromLines(vector<string>& rLines) {
        int line;
        for (line = 0; line < rLines.size(); line++) {
            /* We reached the end of the grid input. */
            if (rLines[line].empty()) {
                break;
            }

            /* Try to find the robot in this line, if found save it. */
            int robotColumn = rLines[line].find(skRobot);
            if (robotColumn != string::npos) {
                m_robotPosition = ii(line, robotColumn);
            }

            /* Insert the line. */
            m_grid.push_back(rLines[line]);
        }

        assert(m_robotPosition != ii(-1, -1));

        /* Return the index of the empty line which separates the map from the
         * direction. */
        return line;
    }

    int initFromLines2(vector<string>& rLines) {
        vector<string> newLines(rLines.size());

        int line;
        for (line = 0; line < rLines.size(); line++) {
            /* We reached the end of the grid input. */
            if (rLines[line].empty()) {
                break;
            }

            /* Insert the wide line into the grid. */
            string wideLine = getWideLine(rLines[line]);

            /* Try to find the robot in this line, if found save it. */
            int robotColumn = wideLine.find(skRobot);
            if (robotColumn != string::npos) {
                m_robotPosition = ii(line, robotColumn);
            }

            /* Insert the line. */
            m_grid.push_back(wideLine);
        }

        assert(m_robotPosition != ii(-1, -1));

        /* Return the index of the empty line which separates the map from the
         * direction. */
        return line;
    }

    string getWideLine(string line) {
        string res;
        for (char c : line) {
            switch (c) {
                case skObstacle: {
                    res += skObstacle;
                    res += skObstacle;
                    break;
                }
                case skEmpty: {
                    res += skEmpty;
                    res += skEmpty;
                    break;
                }
                case skBox: {
                    res += skBoxLeft;
                    res += skBoxRight;
                    break;
                }
                case skRobot: {
                    res += skRobot;
                    res += skEmpty;
                    break;
                }
                default: {
                    assert(false);
                }
            }
        }

        return res;
    }

    bool canMove(ii box, char direction) {
        ii directionFwd = directionForward(direction);

        ii destLeft(box.first + directionFwd.first,
                    box.second + directionFwd.second);
        ii destRight(box.first + directionFwd.first,
                     box.second + 1 + directionFwd.second);

        /* Left dest contains an obstacle, cannot move. */
        if (m_grid[destLeft.first][destLeft.second] == skObstacle) {
            return false;
        }

        /* Right dest contains an obstacle, cannot move. */
        if (m_grid[destRight.first][destRight.second] == skObstacle) {
            return false;
        }

        /* We are not moving into an obstacle, check if we have neighbors in
         * that direction and decide based on them. */
        vector<ii> neighbors = getNeighborsInDirection(box, direction);

        bool res = true;
        for (ii neighbor : neighbors) {
            res &= canMove(neighbor, direction);
        }

        return res;
    }

    void moveBox(ii box, char direction) {
        ii directionFwd = directionForward(direction);

        ii destLeft(box.first + directionFwd.first,
                    box.second + directionFwd.second);
        ii destRight(box.first + directionFwd.first,
                     box.second + 1 + directionFwd.second);

        /* Mark the previous location as empty. */
        m_grid[box.first][box.second] = skEmpty;
        m_grid[box.first][box.second + 1] = skEmpty;

        /* Mark the dest with a box. */
        m_grid[destLeft.first][destLeft.second] = skBoxLeft;
        m_grid[destRight.first][destRight.second] = skBoxRight;
    }

    bool DFSMove(ii box, char direction) {
        vector<ii> neighbors = getNeighborsInDirection(box, direction);

        /* If we can't move one of the boxes, we won't move at all. */
        if (canMove(box, direction) == false) {
            return false;
        }

        /* All boxes can move, move the neighbors recursively first. */
        for (ii neighbor : neighbors) {
            bool moved = DFSMove(neighbor, direction);
            assert(moved);
        }

        /* Move this box. */
        moveBox(box, direction);

        return true;
    }

    void moveRobot(char direction) {
        ii directionFwd = directionForward(direction);
        ii robotDest(m_robotPosition.first + directionFwd.first,
                     m_robotPosition.second + directionFwd.second);

        /* Robot moves directly into an obstacle, nothing to do. */
        if (m_grid[robotDest.first][robotDest.second] == skObstacle) {
            return;
        }

        ii firstNeighbor = getRobotNeighborInDirection(direction);
        /* Move the neighbors if there are any. */
        if (firstNeighbor != skNoNeighbor) {
            /* Couldn't move boxes, return. */
            bool couldMove = DFSMove(firstNeighbor, direction);
            if (couldMove == false) {
                return;
            }
        }

        /* Move the robot to its dest position. */
        m_grid[robotDest.first][robotDest.second] = skRobot;
        m_grid[m_robotPosition.first][m_robotPosition.second] = skEmpty;
        m_robotPosition.first = robotDest.first;
        m_robotPosition.second = robotDest.second;
    }

    ii getRobotNeighborInDirection(char direction) {
        ii directionFwd = directionForward(direction);

        ii dest(m_robotPosition.first + directionFwd.first,
                m_robotPosition.second + directionFwd.second);

        if (m_grid[dest.first][dest.second] == skBoxLeft) {
            return ii(dest.first, dest.second);
        }

        if (m_grid[dest.first][dest.second] == skBoxRight) {
            return ii(dest.first, dest.second - 1);
        }

        return skNoNeighbor;
    }

    vector<ii> getNeighborsInDirection(ii box, char direction) {
        ii directionFwd = directionForward(direction);

        ii destLeft(box.first + directionFwd.first,
                    box.second + directionFwd.second);
        ii destRight(box.first + directionFwd.first,
                     box.second + 1 + directionFwd.second);

        vector<ii> neighbors;

        switch (direction) {
            case skRight: {
                if (m_grid[box.first][box.second + 2] == skBoxLeft) {
                    neighbors.push_back({box.first, box.second + 2});
                }
                break;
            }
            case skLeft: {
                if (m_grid[box.first][box.second - 1] == skBoxRight) {
                    neighbors.push_back({box.first, box.second - 2});
                }
                break;
            }
            case skUp: {
                /* A neighbor is above the box like so:
                 * []
                 *  []
                 */
                if (m_grid[box.first - 1][box.second] == skBoxRight) {
                    neighbors.push_back({box.first - 1, box.second - 1});
                }

                /* A neighbor is above the box like so:
                 *   []
                 *  []
                 */
                if (m_grid[box.first - 1][box.second + 1] == skBoxLeft) {
                    neighbors.push_back({box.first - 1, box.second + 1});
                }

                /* A neighbor is above the box like so:
                 * []
                 * []
                 */
                if (m_grid[box.first - 1][box.second] == skBoxLeft) {
                    neighbors.push_back({box.first - 1, box.second});
                }

                break;
            }

            case skDown: {
                /* A neighbor is below the box like so:
                 * []
                 *  []
                 */
                if (m_grid[box.first + 1][box.second + 1] == skBoxLeft) {
                    neighbors.push_back({box.first + 1, box.second + 1});
                }

                /* A neighbor is below the box like so:
                 *   []
                 *  []
                 */
                if (m_grid[box.first + 1][box.second] == skBoxRight) {
                    neighbors.push_back({box.first + 1, box.second - 1});
                }

                /* A neighbor is below the box like so:
                 * []
                 * []
                 */
                if (m_grid[box.first + 1][box.second] == skBoxLeft) {
                    neighbors.push_back({box.first + 1, box.second});
                }

                break;
            }
            default: {
                assert(false);
            }
        }

        return neighbors;
    }

    void moveInDirection(char direction) {
        /* Find the next empty square/obstacle by starting from one step after
         * the robot position and scanning in the desired direction until
         * encountering an obstacle. */
        ii directionFwd = directionForward(direction);
        ii it = ii(m_robotPosition.first + directionFwd.first,
                   m_robotPosition.second + directionFwd.second);

        while (m_grid[it.first][it.second] == skBox) {
            it.first += directionFwd.first;
            it.second += directionFwd.second;
        }

        /* We stopped seeing boxes, if we saw an obstacle immediately after then
         * we can't move anything. */
        if (m_grid[it.first][it.second] == skObstacle) {
            return;
        }

        /* The iterator is on an empty square. Move backwards and move all the
         * boxes and the robot one step in the direction. */
        while (m_grid[it.first][it.second] != skRobot) {

            m_grid[it.first][it.second] =
                m_grid[it.first - directionFwd.first]
                      [it.second - directionFwd.second];
            it.first -= directionFwd.first;
            it.second -= directionFwd.second;
        }

        /* Now the iterator is at the position the robot was in, mark this
         * position as empty and update the new robot location. */
        m_grid[it.first][it.second] = skEmpty;

        m_robotPosition =
            ii(it.first + directionFwd.first, it.second + directionFwd.second);
    }

    UINT64 part1() {
        UINT64 res = 0;

        for (int row = 1; row < m_grid.size() - 1; row++) {
            for (int col = 1; col < m_grid[0].size() - 1; col++) {
                if (m_grid[row][col] == skBoxLeft) {
                    res += calcGPSCor(ii(row, col));
                }
            }
        }

        return res;
    }

private:
    int calcGPSCor(ii boxLocation) {
        return 100 * boxLocation.first + boxLocation.second;
    }

    ii directionForward(char direction) {
        switch (direction) {
            case skLeft: {
                return ii(0, -1);
            }
            case skRight: {
                return ii(0, 1);
            }
            case skUp: {
                return ii(-1, 0);
            }
            case skDown: {
                return ii(1, 0);
            }
            default: {
                assert(false);
            }
        }
    }

    static constexpr ii skNoNeighbor{-1, -1};

    static constexpr char skRobot = '@';
    static constexpr char skObstacle = '#';
    static constexpr char skEmpty = '.';
    static constexpr char skBox = 'O';
    static constexpr char skBoxLeft = '[';
    static constexpr char skBoxRight = ']';

    static constexpr char skLeft = '<';
    static constexpr char skRight = '>';
    static constexpr char skUp = '^';
    static constexpr char skDown = 'v';

    ii m_robotPosition{-1, -1};

    vector<string> m_grid;
};

int main() {
    InputReader inputReader;
    inputReader.readFile("/Users/whysoserious/repos/aoc24/day15/input.txt");
    vector<string>& rLines = inputReader.getLines();

    Warehouse warehouse;
    int emptyLineIdx = warehouse.initFromLines2(rLines);

    for (auto it = rLines.begin() + emptyLineIdx + 1; it != rLines.end();
         ++it) {

        for (char direction : *it) {
            // warehouse.moveInDirection(direction);
            cout << endl << "Moving in direction: " << direction << endl;
            warehouse.printGrid();
            warehouse.moveRobot(direction);
        }
    }

    cout << warehouse.part1() << endl;

    return 0;
}

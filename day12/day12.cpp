#include "../aoc_utils.hpp"

class Garden {
    struct AreaAndPerimeter {
        AreaAndPerimeter(int area, int perimeter)
            : m_area(area),
              m_perimeter(perimeter){};

        int m_area;
        int m_perimeter;
    };

public:
    void initFromLines(vector<string>& rLines) {
        m_grid = rLines;
        M = m_grid.size();
        N = m_grid[0].size();

        m_visited = vector<vector<bool>>(M, vector<bool>(N, false));
    }

    UINT64 part1() {
        UINT64 totalPrice = 0;

        for (int row = 0; row < M; row++) {
            for (int col = 0; col < N; col++) {
                if (m_visited[row][col] == false) {
                    AreaAndPerimeter res = coverRegion(row, col);

                    totalPrice += res.m_perimeter * res.m_area;
                }
            }
        }

        return totalPrice;
    }

    bool hasFenceInDirection(int row, int col, ii direction) {
        char curr = m_grid[row][col];
        int destRow = row + direction.first;
        int destCol = col + direction.second;

        /* Destination is out of bounds, we have a fence there. */
        if (destRow < 0 || destRow >= M || destCol < 0 || destCol >= N) {
            return true;
        }

        /* There is a different plant at the destination, we put a fence. */
        if (m_grid[destRow][destCol] != curr) {
            return true;
        }

        return false;
    }

    bool shouldAddToPerimeter(int row, int col, ii direction) {
        /* If the direction is left or right, we don't add if the above element
         * has a fence in the same direction. */
        if (direction == ii(0, 1) || direction == ii(0, -1)) {
            return row == 0 || m_grid[row - 1][col] != m_grid[row][col] ||
                   !hasFenceInDirection(row - 1, col, direction);
        }

        /* Direction is up or down, if the left element has fence we don't add
         * to the perimeter. */
        assert(direction == ii(1, 0) || direction == ii(-1, 0));

        return col == 0 || m_grid[row][col] != m_grid[row][col - 1] ||
               !hasFenceInDirection(row, col - 1, direction);
    }

    AreaAndPerimeter coverRegion(int row, int col) {
        assert(row >= 0 && row < M && col >= 0 && col < N);
        int area = 1;
        int perimeter = 0;

        m_visited[row][col] = true;

        vector<ii> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        for (ii direction : directions) {
            int destRow = row + direction.first;
            int destCol = col + direction.second;

            if (hasFenceInDirection(row, col, direction)) {
                if (shouldAddToPerimeter(row, col, direction)) {
                    perimeter++;
                }

                continue;
            }

            /* Destination is in bounds and has the same plant. */
            if (m_visited[destRow][destCol] == false) {
                AreaAndPerimeter res = coverRegion(destRow, destCol);
                perimeter += res.m_perimeter;
                area += res.m_area;
            }
        }

        return AreaAndPerimeter(area, perimeter);
    }

private:
    vector<string> m_grid;

    vector<vector<bool>> m_visited;

    int M{-1};

    int N{-1};
};

int main() {
    InputReader inputReader;
    inputReader.readFile("/Users/whysoserious/repos/aoc24/day12/input.txt");
    vector<string>& rLines = inputReader.getLines();

    Garden garden;
    garden.initFromLines(rLines);

    cout << garden.part1() << endl;

    return 0;
}

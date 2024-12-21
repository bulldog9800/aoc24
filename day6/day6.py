import networkx as nx


class Map:
    obstacle = "#"
    direction_step = {
        "up": (-1, 0),
        "down": (1, 0),
        "left": (0, -1),
        "right": (0, 1),
    }

    right_of_direction = {
        "up": "right",
        "down": "left",
        "left": "up",
        "right": "down",
    }

    def getNextStep(self):
        return self.direction_step[self.guard_direction]

    def isPositionInBounds(self, position):
        return (
            position[0] >= 0
            and position[0] < self.M
            and position[1] >= 0
            and position[1] < self.N
        )

    def hasObstacle(self, position):
        return self.grid[position[0]][position[1]] == self.obstacle

    def createGrid(self, grid):
        for rowNum, row in enumerate(grid):
            self.grid.append(list(row.replace("\n", "")))
            for colNum, char in enumerate(row):
                if char == "^":
                    self.guard_position = (rowNum, colNum)

    def initialize(self):
        self.grid = []
        self.createGrid(self.original_grid)

        self.guard_direction = "up"
        self.M = len(self.original_grid)
        self.N = len(self.original_grid[0])
        self.visited = [[0 for _ in range(self.N)] for _ in range(self.M)]

    def __init__(self, grid):
        self.original_grid = []
        for row in grid:
            self.original_grid.append(row.replace('\n', ''))
        self.initialize()
        self.num_of_positions = 0

    def moveStep(self):
        next_step = self.getNextStep()
        next_position = self.guard_position + next_step
        next_position = tuple(
            x + y for x, y in zip(self.guard_position, next_step)
        )

        while self.isPositionInBounds(next_position) and self.hasObstacle(
            next_position
        ):
            self.guard_direction = self.right_of_direction[
                self.guard_direction
            ]
            next_step = self.getNextStep()
            next_position = tuple(
                x + y for x, y in zip(self.guard_position, next_step)
            )

        self.guard_position = next_position

    def moveUntilExit(self):
        num_of_steps = 0
        while self.isPositionInBounds(self.guard_position):
            self.visited[self.guard_position[0]][self.guard_position[1]] = 1
            self.moveStep()
            num_of_steps += 1
            if num_of_steps > 100000:
                self.num_of_positions += 1
                break

        print(f"Took {num_of_steps} steps")
        return

    def part2(self):
        for rowNum, row in enumerate(self.original_grid):
            for colNum, char in enumerate(row):
                if char == self.obstacle:
                    continue
                # No obstacle here, try placing one and run
                self.initialize()
                print(f"Placing obstacle in ({rowNum}, {colNum})")
                self.grid[rowNum][colNum] = self.obstacle
                self.moveUntilExit()

        print(self.num_of_positions)

    def countVisitedPositions(self):
        res = 0

        for row in self.visited:
            for num in row:
                res += num

        return res


def main():
    with open(
        "/Users/whysoserious/repos/aoc24/day6/input.txt", "r"
    ) as inputFile:
        lines = inputFile.readlines()

    map = Map(lines)

    map.moveUntilExit()
    print(map.countVisitedPositions())

    map.part2()


if __name__ == "__main__":
    main()

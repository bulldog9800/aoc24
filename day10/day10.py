import networkx as nx


class TopoMap:
    def __init__(self, lines):
        self.grid = []
        for row in lines:
            self.grid.append(row.replace("\n", ""))

        self.trailheads = []
        self.nines = []
        self.graph = nx.DiGraph()
        self.M = len(self.grid)
        self.N = len(self.grid[0])
        self.addEdges()

    def calcTrailheadScore(self, trailhead):
        lengths = nx.shortest_path_length(self.graph, source=trailhead)
        score = 0

        for dest in self.nines:
            if dest in lengths:
                score += 1

        return score

    def part2(self):
        res = 0
        for trailhead in self.trailheads:
            for dest in self.nines:
                res += len(
                    list(nx.all_simple_paths(self.graph, trailhead, dest))
                )

        return res

    def part1(self):
        res = 0
        for trailhead in self.trailheads:
            res += self.calcTrailheadScore(trailhead)

        return res

    def addEdgesForNode(self, rowNum, colNum):
        val = int(self.grid[rowNum][colNum])
        if val == 0:
            self.trailheads.append((rowNum, colNum))
        if val == 9:
            self.nines.append((rowNum, colNum))

        for direction in [(1, 0), (-1, 0), (0, 1), (0, -1)]:
            dest = tuple(a + b for a, b in zip((rowNum, colNum), direction))
            if (
                0 <= dest[0] < self.M
                and 0 <= dest[1] < self.N
                and val == int(self.grid[dest[0]][dest[1]]) - 1
            ):
                self.graph.add_edges_from(
                    [((rowNum, colNum), (dest[0], dest[1]))]
                )

    def addEdges(self):
        for rowNum, row in enumerate(self.grid):
            for colNum, val in enumerate(row):
                self.addEdgesForNode(rowNum, colNum)


def main():
    with open(
        "/Users/whysoserious/repos/aoc24/day10/input.txt", "r"
    ) as inputFile:
        lines = inputFile.readlines()

    topoMap = TopoMap(lines)

    print(topoMap.part1())
    print(topoMap.part2())


if __name__ == "__main__":
    main()

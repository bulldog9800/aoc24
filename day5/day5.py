import networkx as nx

lines = []
with open("/Users/whysoserious/repos/aoc24/day5/input.txt", "r") as inputFile:
    lines = inputFile.readlines()

dependencies = {}

# Add the dependencies, each dependency is an edge n1 -> n2 where n1 has to
# come before n2
for idx, line in enumerate(lines):
    if len(line) == 1:
        break
    nums = line.replace("\n", "").split("|")
    if nums[0] not in dependencies:
        dependencies[nums[0]] = []
    dependencies[nums[0]].append(nums[1])

res = 0
for line in lines[idx + 1 :]:
    update = line.replace("\n", "").split(",")
    if len(update) % 2 == 0:
        print(f"Update contains even number of nums: {update}")

    graph = nx.DiGraph()
    for num in update:
        if num not in dependencies:
            continue
        for target in dependencies[num]:
            if target in update:
                graph.add_edges_from([(num, target)])

    copy_graph = graph.copy()
    for i in range(1, len(update)):
        copy_graph.add_edges_from([(update[i - 1], update[i])])

    try:
        nx.find_cycle(copy_graph)
    except nx.exception.NetworkXNoCycle:
        # No cycle is found, update is correctly ordered
        continue

    # Cycle was found, update is incorrect, need to reorder based on the
    # topological ordering only
    ordered_nodes = list(nx.topological_sort(graph))
    res += int(ordered_nodes[len(ordered_nodes) // 2])


print(res)

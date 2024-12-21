import re

inputFile = open("/Users/whysoserious/repos/aoc24/day3/input.txt", "r")

mulExprs = re.findall(
    "mul\([0-9]+,[0-9]+\)|do\(\)|don't\(\)", inputFile.read())
print(mulExprs)

res = 0
doing = True

for expression in mulExprs:
    if expression == "don't()":
        doing = False
        continue

    if expression == "do()":
        doing = True
        continue

    if doing is False:
        continue

    nums = re.findall("[0-9]+", expression)
    print(nums)
    res = res + (int(nums[0]) * int(nums[1]))

print(f"RESULT: {res}")

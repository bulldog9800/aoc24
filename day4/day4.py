import re

inputFile = open("/Users/whysoserious/repos/aoc24/day4/input.txt", "r")

hist = {}
letters = ['X', 'M', 'A', 'S']

rows = []
res = 0

for line in inputFile:
    rows.append(line.replace("\n", ""))

M = len(rows)
N = len(rows[0])

columns = {}

for i in range(N):
    columns[i] = ""

# Construct the columns
for row in rows:
    for i, char in enumerate(row):
        columns[i] += char

diagonals = {}
anti_diagonals = {}

for i in range(M + N - 1):
    diagonals[i] = ""
    anti_diagonals[i] = ""

# Construct the diagonals
for n in range(N):
    (i, j) = (0, n)
    while i < M and j < N:
        diagonals[n] += rows[i][j]
        i += 1
        j += 1

for m in range(M):
    (i, j) = (1 + m, 0)
    while i < M and j < N:
        diagonals[N+m] += rows[i][j]
        i += 1
        j += 1

# Construct the anti diagonals
for n in range(N):
    (i, j) = (0, n)
    while i < M and j >= 0:
        anti_diagonals[n] += rows[i][j]
        i += 1
        j -= 1

for m in range(M):
    (i, j) = (1 + m, N - 1)
    while i < M and j >= 0:
        anti_diagonals[N+m] += rows[i][j]
        i += 1
        j -= 1

for row in rows:
    res += len(re.findall("XMAS", row))
    res += len(re.findall("XMAS", row[::-1]))

for col in columns.values():
    res += len(re.findall("XMAS", col))
    res += len(re.findall("XMAS", col[::-1]))

for diag in diagonals.values():
    res += len(re.findall("XMAS", diag))
    res += len(re.findall("XMAS", diag[::-1]))

for anti_diag in anti_diagonals.values():
    res += len(re.findall("XMAS", anti_diag))
    res += len(re.findall("XMAS", anti_diag[::-1]))

print(res)

# Part 2

res = 0

for i in range(M-2):
    for j in range(N-2):
        word = ""
        for n in range(3):
            word += rows[i + n][j + n]
        print(f"word: {word}")
        if word == "MAS" or word == "SAM":
            x_word = ""
            for n in range(3):
                x_word += rows[i + n][j + 2 - n]
            print(f"x_word: {x_word}")
            if x_word == "MAS" or x_word == "SAM":
                res += 1

print(res)

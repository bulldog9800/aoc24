import numpy as np


class ConfigValues:
    def __init__(self, config):
        # Get the values for button A
        buttonALine = config[0]

        x_location = buttonALine.find('X')
        comma_location = buttonALine.find(',')
        self.a_x = int(buttonALine[x_location+2:comma_location])

        y_location = buttonALine.find('Y')
        self.a_y = int(buttonALine[y_location+2:])

        # Get the values for button B
        buttonBLine = config[1]

        x_location = buttonBLine.find('X')
        comma_location = buttonBLine.find(',')
        self.b_x = int(buttonBLine[x_location+2:comma_location])

        y_location = buttonBLine.find('Y')
        self.b_y = int(buttonBLine[y_location+2:])

        # Get the prize values
        prizeLine = config[2]

        x_location = prizeLine.find('X')
        comma_location = prizeLine.find(',')
        self.prize_x = int(
            prizeLine[x_location+2:comma_location]) + 10000000000000

        y_location = prizeLine.find('Y')
        self.prize_y = int(prizeLine[y_location+2:]) + 10000000000000


def is_whole(num):
    rounded_val = np.round(num)
    return rounded_val - 1e-4 < num < rounded_val + 1e-4


def main():
    with open(
        "/Users/whysoserious/repos/aoc24/day13/input.txt", "r"
    ) as inputFile:
        lines = inputFile.readlines()

    tokens = 0
    for i in range(0, len(lines), 4):
        machineConfig = lines[i: i+4]
        config_vals = ConfigValues(machineConfig)

        coefficients = np.array([[config_vals.a_x, config_vals.b_x],
                                [config_vals.a_y, config_vals.b_y]])
        result = np.array([config_vals.prize_x, config_vals.prize_y])

        try:
            sol = np.linalg.solve(coefficients, result)
        except np.linalg.LinAlgError:
            continue

        a_presses = sol[0]
        b_presses = sol[1]

        if (not is_whole(a_presses)) or (not is_whole(b_presses)):
            continue

        tokens += (a_presses * 3) + (b_presses)

    print(tokens)


if __name__ == "__main__":
    main()

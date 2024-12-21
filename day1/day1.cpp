#include "../aoc_utils.hpp"

int main() {
    InputReader inputReader;
    inputReader.readFile("/Users/whysoserious/repos/aoc24/day1/input.txt");
    vector<string>& rLines = inputReader.getLines();

    vi leftList, rightList;

    for (auto& line : rLines) {
        /* For each line, tokenize it and insert the first number into the first
         * list and the second number into the relevant lists. */
        vector<string> tokens = StringTokenizer::sGetTokens(line, "   ");
        assert(tokens.size() == 2);

        int leftVal = std::stoi(tokens[0]);
        int rightVal = std::stoi(tokens[1]);

        leftList.push_back(leftVal);
        rightList.push_back(rightVal);
    }

    std::sort(leftList.begin(), leftList.end());
    std::sort(rightList.begin(), rightList.end());

    /* Now get the total distance. */
    int totalDist = 0;

    for (int i = 0; i < leftList.size(); i++) {
        totalDist += std::max(leftList[i], rightList[i]) -
                     std::min(leftList[i], rightList[i]);
    }

    /********************** Part 2 ************************/
    /* For this part we'll make a histogram of the right list numbers. That is,
     * a map between location IDs and number of appearances. After we construct
     * this histogram, we go over the left list and search for each number in
     * the right list's histogram, if found we multiply and add to the score.
     */
    unordered_map<int, int> rightListHistogram;

    for (int num : rightList) {
        /* Not in the histogram yet, initialize it with 1. */
        if (rightListHistogram.find(num) == rightListHistogram.end()) {
            rightListHistogram[num] = 1;
            continue;
        }
        /* Else increment the value. */
        rightListHistogram[num]++;
    }

    /* Now that the histogram is constructed, calculate the similarity score. */
    int similarityScore = 0;

    for (int num : leftList) {
        if (rightListHistogram.find(num) != rightListHistogram.end()) {
            similarityScore += num * rightListHistogram[num];
        }
    }

    cout << similarityScore << endl;

    return 0;
}

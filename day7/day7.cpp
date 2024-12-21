#include "../aoc_utils.hpp"

class Equation {
public:
    void initFromLine(string rLine) {
        vector<string> tokens = StringTokenizer::sGetTokens(rLine, ":");
        m_result = std::stoull(tokens[0]);

        vector<string> nums = StringTokenizer::sGetTokens(tokens[1], " ");
        for (string num : nums) {
            m_nums.push_back(std::stoi(num));
        }
    }

    bool canBeTrue();

    UINT64 getResult() {
        return m_result;
    }

private:
    enum class Operator { kPlus = 0, kMult, kConcat, kInvalid };
    vector<vector<Operator>> genPermutations();
    bool genPermutationsAux(vector<Operator>& permutation);
    bool checkPermutation(vector<Operator>& permutation);

    UINT64 m_result{0};

    vi m_nums;
};

bool Equation::genPermutationsAux(vector<Operator>& permutation) {
    if (permutation.size() == m_nums.size() - 1) {
        return checkPermutation(permutation);
    }

    for (int i = 0; i < 3; i++) {
        permutation.push_back(Operator(i));
        if (genPermutationsAux(permutation)) {
            return true;
        }
        permutation.pop_back();
    }

    return false;
}

vector<vector<Equation::Operator>> Equation::genPermutations() {
    vector<vector<Operator>> res;

    vector<Operator> perm;
    genPermutationsAux(perm);

    return res;
}

long int concatenate(long int a, long int b) {
    int count = 0;
    long int x = b;
    while (b > 0) {
        count++;
        b /= 10;
    }
    for (int i = 0; i < count; i++)
        a *= 10;
    return a + x;
}

bool Equation::checkPermutation(vector<Operator>& permutation) {
    assert(permutation.size() == m_nums.size() - 1);
    UINT64 res = m_nums[0];
    for (int i = 0; i < permutation.size(); i++) {
        switch (permutation[i]) {
            case Operator::kMult: {
                res *= m_nums[i + 1];
                break;
            }

            case Operator::kPlus: {
                res += m_nums[i + 1];
                break;
            }

            case Operator::kConcat: {
                res = concatenate(res, m_nums[i + 1]);
                break;
            }

            default: {
                assert(0);
            }
        }

        if (res > m_result) {
            return false;
        }
    }

    return res == m_result;
}

bool Equation::canBeTrue() {
    vector<Operator> perm;
    return genPermutationsAux(perm);
}

int main() {
    InputReader inputReader;
    inputReader.readFile("/Users/whysoserious/repos/aoc24/day7/input.txt");
    vector<string>& rLines = inputReader.getLines();

    UINT64 res = 0;
    for (auto line : rLines) {
        Equation equation;
        equation.initFromLine(line);
        if (equation.canBeTrue()) {
            res += equation.getResult();
        }
    }

    cout << res << endl;

    return 0;
}

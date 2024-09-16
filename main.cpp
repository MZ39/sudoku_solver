#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <string_view>

class Sudoku {

private:
    int matrix[3][3][3][3];
    
    struct Pose {
        int i, j, k, l;
        bool operator<(const Pose& other) const {
            if (i != other.i) return i < other.i;
            if (j != other.j) return j < other.j;
            if (k != other.k) return k < other.k;
            return l < other.l;
        }
    };

    std::map<Pose, std::set<int>> candidates_map;

public:
    Sudoku() {
        std::ifstream file("sudoku_example_0");
        std::vector<int> lines;
        std::string line;

        while (file >> line) {
            lines.push_back(std::stoi(line));
        }

        int p = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                for (int k = 0; k < 3; ++k) {
                    for (int l = 0; l < 3; ++l) {
                        matrix[i][j][k][l] = lines[p++];
                    }
                }
            }
        }
        file.close();
    }

    void print() {
        std::cout << " ----------------------- "  << std::endl;
        for (int i = 0; i < 3; ++i) {
            if (i > 0){
                std::cout << "|-------|-------|-------|"  << std::endl;
            }
            for (int k = 0; k < 3; ++k) {
                std::cout << "| ";
                for (int j = 0; j < 3; ++j) {
                    for (int l = 0; l < 3; ++l) {
                        std::cout << matrix[i][j][k][l] << " ";
                        if (l == 2) {
                            std::cout << "|" << " ";
                        }
                    }
                }
                std::cout << std::endl;
            }
        }
        std::cout << " ----------------------- "  << std::endl;
    }

    std::set<int> get_row_candidates(int i, int j, int k, int l) {
        std::set<int> candidates = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (int jj = 0; jj < 3; jj++) {
            for (int ll = 0; ll < 3; ll++) {
                if (matrix[i][jj][k][ll] != 0) {
                    candidates.erase(matrix[i][jj][k][ll]);
                }
            }
        }
        return candidates;
    }

    std::set<int> get_column_candidates(int i, int j, int k, int l) {
        std::set<int> candidates = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (int ii = 0; ii < 3; ii++) {
            for (int kk = 0; kk < 3; kk++) {
                if (matrix[ii][j][kk][l] != 0) {
                    candidates.erase(matrix[ii][j][kk][l]);
                }
            }
        }
        return candidates;
    }

    std::set<int> get_group_candidates(int i, int j, int k, int l) {
        std::set<int> candidates = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (int kk = 0; kk < 3; kk++) {
            for (int ll = 0; ll < 3; ll++) {
                if (matrix[i][j][kk][ll] != 0) {
                    candidates.erase(matrix[i][j][kk][ll]);
                }
            }
        }
        return candidates;
    }

    std::set<int> get_candidates(int i, int j, int k, int l) {
        std::set<int> row_candidates = get_row_candidates(i, j, k, l);
        std::set<int> column_candidates = get_column_candidates(i, j, k, l);
        std::set<int> group_candidates = get_group_candidates(i, j, k, l);

        std::set<int> intersection;
        std::set_intersection(row_candidates.begin(), row_candidates.end(),
                              column_candidates.begin(), column_candidates.end(),
                              std::inserter(intersection, intersection.begin()));

        std::set<int> final_candidates;
        std::set_intersection(intersection.begin(), intersection.end(),
                              group_candidates.begin(), group_candidates.end(),
                              std::inserter(final_candidates, final_candidates.begin()));

        return final_candidates;
    }

    bool solve_recursive() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    for (int l = 0; l < 3; l++) {
                        if (matrix[i][j][k][l] == 0) {
                            std::set<int> candidates = get_candidates(i, j, k, l);
                            for (int num : candidates) {
                                matrix[i][j][k][l] = num;
                                if (solve_recursive()) {
                                    return true;
                                }
                                matrix[i][j][k][l] = 0; // Backtrack
                            }
                            return false;  // If no candidate works
                        }
                    }
                }
            }
        }
        return true;  // If no empty spaces are left, puzzle is solved
    }

    void solve() {
        solve_recursive();
    }
};

int main() {
    Sudoku sudoku;
    std::cout << "Unsolved sudoku:" << std::endl;
    sudoku.print();
    sudoku.solve();
    std::cout << std::endl << "Solved sudoku:" << std::endl;
    sudoku.print();

    return 0;
}

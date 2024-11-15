#include <vector>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>

/**
 * Class NQueenSolve implements the N-Queens problem solution
 * using the Min-Conflicts algorithm.
 */
class NQueenSolve {
    int* row_position;
    int N;
    int* queens_per_row;
    int* queens_per_D1;
    int* queens_per_D2;

    // Initializes an array with zeros
    void init(int*& arr, int size) {
        for (int i = 0; i < size; i++) {
            arr[i] = 0;
        }
    }

    // Calculates initial conflicts for the board
    void calculate_conflicts() {
        for (int i = 0; i < N; i++) {
            queens_per_row[row_position[i]]++;
            queens_per_D1[row_position[i] - i + (N - 1)]++;
            queens_per_D2[i + row_position[i]]++;
        }
    }

    // Updates conflicts when moving a queen
    void move(int column, int old_row, int new_row) {
        queens_per_row[old_row]--;
        queens_per_D1[old_row - column + (N - 1)]--;
        queens_per_D2[column + old_row]--;

        queens_per_row[new_row]++;
        queens_per_D1[new_row - column + (N - 1)]++;
        queens_per_D2[column + new_row]++;
    }

    // Calculates conflicts for a specific position
    int calculate_position(int column, int row) const {
        return queens_per_row[row] +
               queens_per_D1[row - column + (N - 1)] +
               queens_per_D2[column + row];
    }

    // Calculates conflicts for a specific queen
    int calculate_queen(int column) const {
        int row = row_position[column];
        return queens_per_row[row] - 1 +
               queens_per_D1[row - column + (N - 1)] - 1 +
               queens_per_D2[column + row] - 1;
    }

    // Identifies the queen with the maximum conflicts
    int get_max_conflict() const {
        int max_conflict = -1;
        std::vector<int> candidates;
        for (int i = 0; i < N; i++) {
            int conflicts = calculate_queen(i);
            if (conflicts > max_conflict) {
                max_conflict = conflicts;
                candidates = {i};
            } else if (conflicts == max_conflict) {
                candidates.push_back(i);
            }
        }
        return candidates[rand() % candidates.size()];
    }

    // Finds the best row for a column with minimum conflicts
    std::pair<int, int> minConflict_col(int column, int lastPosition) const {
        int min_conflict = N + 1;
        std::vector<int> best_rows;
        for (int i = 0; i < N; i++) {
            if (i == lastPosition) continue;
            int conflicts = calculate_position(column, i);
            if (conflicts < min_conflict) {
                min_conflict = conflicts;
                best_rows = {i};
            } else if (conflicts == min_conflict) {
                best_rows.push_back(i);
            }
        }
        if (!best_rows.empty())
            return {best_rows[rand() % best_rows.size()], min_conflict};
        else
            return {-1, -1};
    }

    // Initializes the board with random placement of queens
    void first_step() {
        for (int i = 0; i < N; i++) {
            row_position[i] = i;
        }
        for (int i = 0; i < N; i++) {
            int swap_with = rand() % N;
            std::swap(row_position[i], row_position[swap_with]);
        }
        init(queens_per_row, N);
        init(queens_per_D1, 2 * N - 1);
        init(queens_per_D2, 2 * N - 1);
        calculate_conflicts();
    }

    // Solves the N-Queens problem using Min-Conflicts algorithm
    void arrange() {
        int iter = 0;
        int current_max = get_max_conflict();
        
        while (calculate_queen(current_max) != 0) {
            if (++iter > 2 * N) {
                first_step();
                iter = 0;
            }

            int last_row = row_position[current_max];
            std::pair<int, int> result = minConflict_col(current_max, last_row);
            int new_row = result.first;
            int currentMin = result.second;
            if (new_row == -1) {
                first_step();
                continue;
            }

            row_position[current_max] = new_row;
            move(current_max, last_row, new_row);
            current_max = get_max_conflict();
        }
    }

    // Prints the solved board
    void print_queens() const {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << (row_position[j] == i ? " * " : " _ ");
            }
            std::cout << std::endl;
        }
    }

public:
    // Constructor
    NQueenSolve(int N1) : N(N1) {
        row_position = new int[N];
        queens_per_row = new int[N];
        queens_per_D1 = new int[2 * N - 1];
        queens_per_D2 = new int[2 * N - 1];
        init(row_position, N);
    }

    // Destructor
    ~NQueenSolve() {
        delete[] row_position;
        delete[] queens_per_row;
        delete[] queens_per_D1;
        delete[] queens_per_D2;
    }

    // Entry point for solving the problem
    void start() {
        first_step();
        arrange();
        print_queens();
    }
};

int main() {
    srand(time(0));
    NQueenSolve solver(5);
    solver.start();
    return 0;
}




#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

const int SIZE = 8;
static char plane[8][8];

char read_input() {
    int character = getchar();
    if (isspace(character)) {
        return read_input();
    } else {
        return (char) character;
    }
}

bool in_bounds(int x, int y) {
    return x >= 0 && y >= 0 && x < SIZE && y < SIZE;
}

bool white_move_possible(int x, int y) {
    char item_at_destination = plane[x][y];
    return in_bounds(x, y) && (item_at_destination == '.' || (islower(item_at_destination)));
}

bool black_move_possible(int x, int y, char simulatedPlane[SIZE][SIZE]) {
    char item_at_destination = simulatedPlane[x][y];
    return in_bounds(x, y) && (item_at_destination == '.' || (isupper(item_at_destination)));
}

bool white_hit(int x, int y) {
    return (bool) islower((char) plane[x][y]);
}

bool black_hit(int x, int y, char simulatedPlane[SIZE][SIZE]) {
    return (bool) isupper((char) simulatedPlane[x][y]);
}

int possibilities_at_point(int x, int y) {
    int possibilities = 0;
    if (white_move_possible(x, y)) {
        possibilities++;
        if (white_hit(x, y)) {
            possibilities++;
        }
    }
    return possibilities;
}

int possibilities_for_straight_move(int x, int y) {
    int possibilities = 0;
    for (int kx = x + 1; kx < SIZE; kx++) {
        int possibilities_part = possibilities_at_point(kx, y);
        possibilities += possibilities_part;
        if (possibilities_part == 0 || possibilities_part == 2) break;
    }
    for (int kx = x - 1; kx >= 0; kx--) {
        int possibilities_part = possibilities_at_point(kx, y);
        possibilities += possibilities_part;
        if (possibilities_part == 0 || possibilities_part == 2) break;
    }
    for (int ky = y + 1; ky < SIZE; ky++) {
        int possibilities_part = possibilities_at_point(x, ky);
        possibilities += possibilities_part;
        if (possibilities_part == 0 || possibilities_part == 2) break;
    }
    for (int ky = y - 1; ky >= 0; ky--) {
        int possibilities_part = possibilities_at_point(x, ky);
        possibilities += possibilities_part;
        if (possibilities_part == 0 || possibilities_part == 2) break;
    }
    return possibilities;
}

int possibilities_for_diagonally_move(int x, int y) {
    int possibilities = 0;
    for (int kx = x + 1, ky = y + 1; kx < SIZE && ky < SIZE; kx++, ky++) {
        int possibilities_part = possibilities_at_point(kx, ky);
        possibilities += possibilities_part;
        if (possibilities_part == 0 || possibilities_part == 2) break;
    }
    for (int kx = x - 1, ky = y + 1; kx < SIZE && ky < SIZE; kx--, ky++) {
        int possibilities_part = possibilities_at_point(kx, ky);
        possibilities += possibilities_part;
        if (possibilities_part == 0 || possibilities_part == 2) break;
    }
    for (int kx = x + 1, ky = y - 1; kx < SIZE && ky < SIZE; kx++, ky--) {
        int possibilities_part = possibilities_at_point(kx, ky);
        possibilities += possibilities_part;
        if (possibilities_part == 0 || possibilities_part == 2) break;
    }
    for (int kx = x - 1, ky = y - 1; kx < SIZE && ky < SIZE; kx--, ky--) {
        int possibilities_part = possibilities_at_point(kx, ky);
        possibilities += possibilities_part;
        if (possibilities_part == 0 || possibilities_part == 2) break;
    }
    return possibilities;
}

bool is_legal_at_point(int x, int y, char simulatedPlane[SIZE][SIZE]) {
    if (black_move_possible(x, y, simulatedPlane)) {
        if (black_hit(x, y, simulatedPlane) && simulatedPlane[x][y] == 'K') {
            false;
        }
    }
    return true;
}

bool is_legal_straight_move(int x, int y, char simulatedPlane[SIZE][SIZE]) {
    for (int kx = x + 1; kx < SIZE; kx++) {
        bool legal = is_legal_at_point(kx, y, simulatedPlane);
        if (!legal) return false;
    }
    for (int kx = x - 1; kx >= 0; kx--) {
        bool legal = is_legal_at_point(kx, y, simulatedPlane);
        if (!legal) return false;
    }
    for (int ky = y + 1; ky < SIZE; ky++) {
        bool legal = is_legal_at_point(x, ky, simulatedPlane);
        if (!legal) return false;
    }
    for (int ky = y - 1; ky >= 0; ky--) {
        bool legal = is_legal_at_point(x, ky, simulatedPlane);
        if (!legal) return false;
    }
    return true;
}

bool is_legal_diagonally_move(int x, int y, char simulatedPlane[SIZE][SIZE]) {
    for (int kx = x + 1, ky = y + 1; kx < SIZE && ky < SIZE; kx++, ky++) {
        bool legal = is_legal_at_point(kx, y, simulatedPlane);
        if (!legal) return false;
    }
    for (int kx = x - 1, ky = y + 1; kx < SIZE && ky < SIZE; kx--, ky++) {
        bool legal = is_legal_at_point(kx, y, simulatedPlane);
        if (!legal) return false;
    }
    for (int kx = x + 1, ky = y - 1; kx < SIZE && ky < SIZE; kx++, ky--) {
        bool legal = is_legal_at_point(kx, y, simulatedPlane);
        if (!legal) return false;
    }
    for (int kx = x - 1, ky = y - 1; kx < SIZE && ky < SIZE; kx--, ky--) {
        bool legal = is_legal_at_point(kx, y, simulatedPlane);
        if (!legal) return false;
    }
    return true;
}

bool is_legal(char simulatedPlane[SIZE][SIZE]) {
    bool legal = true;
    for (int x = 0; x < SIZE; ++x) {
        for (int y = 0; y < SIZE; ++y) {
            char item = simulatedPlane[x][y];
            if (item == '.') continue;
            switch (item) {
                case 'p':
                    //zbicie po skosie
                    if (in_bounds(x + 1, y - 1) && (simulatedPlane[x + 1][y + 1] == 'K')) {
                        return false;
                    }
                    if (in_bounds(x - 1, y - 1) && (simulatedPlane[x - 1][y + 1] == 'K')) {
                        return false;
                    }
                    break;
                case 's':
                    if ((black_move_possible(x + 3, y + 1, simulatedPlane) && simulatedPlane[x + 3][y + 1] == 'K') ||
                        (black_move_possible(x - 3, y + 1, simulatedPlane) && simulatedPlane[x - 3][y + 1] == 'K') ||
                        (black_move_possible(x + 3, y - 1, simulatedPlane) && simulatedPlane[x + 3][y - 1] == 'K') ||
                        (black_move_possible(x - 3, y - 1, simulatedPlane) && simulatedPlane[x - 3][y - 1] == 'K') ||
                        (black_move_possible(x + 1, y + 3, simulatedPlane) && simulatedPlane[x + 1][y + 3] == 'K') ||
                        (black_move_possible(x - 1, y + 3, simulatedPlane) && simulatedPlane[x - 1][y + 3] == 'K') ||
                        (black_move_possible(x + 1, y - 3, simulatedPlane) && simulatedPlane[x + 1][y - 3] == 'K') ||
                        (black_move_possible(x - 1, y - 3, simulatedPlane) && simulatedPlane[x - 1][y - 3] == 'K')) {
                        return false;
                    }
                    break;
                case 'w':
                    if (!is_legal_straight_move(x, y, simulatedPlane)) return false;
                    break;
                case 'g':
                    if (!is_legal_diagonally_move(x, y, simulatedPlane)) return false;
                    break;
                case 'h':
                    if (!is_legal_diagonally_move(x, y, simulatedPlane)) return false;
                    if (!is_legal_straight_move(x, y, simulatedPlane)) return false;
                    break;
                case 'k':
                    if (black_move_possible(x + 1, y + 1, simulatedPlane) &&
                        simulatedPlane[x + 1][y + 1] == 'K')
                        return false;
                    if (black_move_possible(x + 1, y - 1, simulatedPlane) &&
                        simulatedPlane[x + 1][y - 1] == 'K')
                        return false;
                    if (black_move_possible(x - 1, y + 1, simulatedPlane) &&
                        simulatedPlane[x - 1][y + 1] == 'K')
                        return false;
                    if (black_move_possible(x - 1, y - 1, simulatedPlane) &&
                        simulatedPlane[x - 1][y - 1] == 'K')
                        return false;
                    break;
                default:
                    break;
            }
        }
    }
    return legal;
}

int possibilities(char item, int x, int y) {
    int possible_moves = 0;
    switch (item) {
        case 'P':
            //jezeli startuje z poczatkowej linii
            if (y == 1) {
                for (int ky = 2; ky <= 3; ky++) {
                    if (white_move_possible(x, ky)) {
                        possible_moves++;
                    }
                }
            } else if (in_bounds(x, y + 1)) {
                //zwykly ruch do przodu
                possible_moves++;
            }
            //zbicie po skosie
            if (in_bounds(x + 1, y + 1) && islower(plane[x + 1][y + 1])) {
                possible_moves++;
            }
            if (in_bounds(x - 1, y + 1) && islower(plane[x - 1][y + 1])) {
                possible_moves++;
            }
            break;
        case 'S':
            if (white_move_possible(x + 3, y + 1)) possible_moves++;
            if (white_move_possible(x - 3, y + 1)) possible_moves++;
            if (white_move_possible(x + 3, y - 1)) possible_moves++;
            if (white_move_possible(x - 3, y - 1)) possible_moves++;
            if (white_move_possible(x + 1, y + 3)) possible_moves++;
            if (white_move_possible(x - 1, y + 3)) possible_moves++;
            if (white_move_possible(x + 1, y - 3)) possible_moves++;
            if (white_move_possible(x - 1, y - 3)) possible_moves++;
            break;
        case 'W':
            possible_moves += possibilities_for_straight_move(x, y);
            break;
        case 'G':
            possible_moves += possibilities_for_diagonally_move(x, y);
            break;
        case 'H':
            possible_moves += possibilities_for_diagonally_move(x, y);
            possible_moves += possibilities_for_straight_move(x, y);
            break;
        case 'K':
            if (white_move_possible(x + 1, y + 1)) possible_moves++;
            if (white_move_possible(x + 1, y - 1)) possible_moves++;
            if (white_move_possible(x - 1, y + 1)) possible_moves++;
            if (white_move_possible(x - 1, y - 1)) possible_moves++;
            break;
        default:
            break;
    }
    return possible_moves;
}

int main() {
    for (int i = SIZE - 1; i >= 0; --i) {
        for (int j = SIZE - 1; j >= 0; --j) {
            plane[j][i] = read_input();
        }
    }

    int possible_moves = 0;
    for (int x = 0; x < SIZE; ++x) {
        for (int y = 0; y < SIZE; ++y) {
            char item = plane[x][y];
            if (item == '.') continue;
            possible_moves += possibilities(item, x, y);
        }
    }

    printf("%d", possible_moves);

    return 0;
}
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

const int SIZE = 8;
static char plane[8][8];
static char simulatedPlane[8][8];

bool white_move_possible(int x, int y, int x_from, int y_from, char item);

bool white_move_possible_return_if_legal(int x, int y, int x_from, int y_from, char item, bool *is_legal);

bool white_move_possible_if_hit(int x, int y, int x_from, int y_from, char item);

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

bool black_move_possible(int x, int y) {
    char item_at_destination = simulatedPlane[x][y];
    return in_bounds(x, y) && (item_at_destination == '.' || (isupper(item_at_destination)));
}

bool white_hit(int x, int y) {
    return (bool) islower(plane[x][y]);
}

bool black_hit(int x, int y) {
    return (bool) isupper(simulatedPlane[x][y]);
}

int possibilities_at_point(int x, int y, int x_from, int y_from, char item, bool *hit, bool *legal) {
    int possibilities = 0;
    if (white_move_possible_return_if_legal(x, y, x_from, y_from, item, legal)) {
        possibilities++;
        if (white_hit(x, y)) {
            *hit = true;
        }
    }
    return possibilities;
}

int possibilities_for_straight_move(int x, int y, char item) {
    int possibilities = 0;
    bool hit = false;
    bool legal = true;
    for (int kx = x + 1; kx < SIZE; kx++) {
        int possibilities_part = possibilities_at_point(kx, y, x, y, item, &hit, &legal);
        if (!legal) continue;
        possibilities += possibilities_part;
        if (!possibilities_part) break;
        if (hit) break;
    }
    hit = false;
    for (int kx = x - 1; kx >= 0; kx--) {
        int possibilities_part = possibilities_at_point(kx, y, x, y, item, &hit, &legal);
        if (!legal) continue;
        possibilities += possibilities_part;
        if (!possibilities_part) break;
        if (hit) break;
    }
    hit = false;
    for (int ky = y + 1; ky < SIZE; ky++) {
        int possibilities_part = possibilities_at_point(x, ky, x, y, item, &hit, &legal);
        if (!legal) continue;
        possibilities += possibilities_part;
        if (!possibilities_part) break;
        if (hit) break;
    }
    hit = false;
    for (int ky = y - 1; ky >= 0; ky--) {
        int possibilities_part = possibilities_at_point(x, ky, x, y, item, &hit, &legal);
        if (!legal) continue;
        possibilities += possibilities_part;
        if (!possibilities_part) break;
        if (hit) break;
    }
    return possibilities;
}

int possibilities_for_diagonally_move(int x, int y, char item) {
    int possibilities = 0;
    bool hit = false;
    bool legal = true;
    for (int kx = x + 1, ky = y + 1; kx < SIZE && ky < SIZE && ky >= 0 && kx >= 0; kx++, ky++) {
        int possibilities_part = possibilities_at_point(kx, ky, x, y, item, &hit, &legal);
        if (!legal) continue;
        possibilities += possibilities_part;
        if (!possibilities_part) break;
        if (hit) break;
    }
    hit = false;
    for (int kx = x - 1, ky = y + 1; kx < SIZE && ky < SIZE && ky >= 0 && kx >= 0; kx--, ky++) {
        int possibilities_part = possibilities_at_point(kx, ky, x, y, item, &hit, &legal);
        if (!legal) continue;
        possibilities += possibilities_part;
        if (!possibilities_part) break;
        if (hit) break;
    }
    hit = false;
    for (int kx = x + 1, ky = y - 1; kx < SIZE && ky < SIZE && ky >= 0 && kx >= 0; kx++, ky--) {
        int possibilities_part = possibilities_at_point(kx, ky, x, y, item, &hit, &legal);
        if (!legal) continue;
        possibilities += possibilities_part;
        if (!possibilities_part) break;
        if (hit) break;
    }
    hit = false;
    for (int kx = x - 1, ky = y - 1; kx < SIZE && ky < SIZE && ky >= 0 && kx >= 0; kx--, ky--) {
        int possibilities_part = possibilities_at_point(kx, ky, x, y, item, &hit, &legal);
        if (!legal) continue;
        possibilities += possibilities_part;
        if (!possibilities_part) break;
        if (hit) break;
    }
    return possibilities;
}

bool is_legal_at_point(int x, int y, bool *hit) {
    if (black_move_possible(x, y) && black_hit(x, y)) {
        *hit = true;
        if (simulatedPlane[x][y] == 'K') {
            //printf("checking (%d,%d)[%c] ?hit%d\n", x, y, simulatedPlane[x][y], *hit);
            return false;
        }
    }
    //printf("checking (%d,%d)[%c] ?hit%d\n", x, y, simulatedPlane[x][y], *hit);
    return true;
}

bool is_legal_straight_move(int x, int y) {
    bool hit = false;
    for (int kx = x + 1; kx < SIZE; kx++) {
        bool legal = is_legal_at_point(kx, y, &hit);
        if (!legal) return false;
        if (hit)break;
    }
    hit = false;
    for (int kx = x - 1; kx >= 0; kx--) {
        bool legal = is_legal_at_point(kx, y, &hit);
        if (!legal) return false;
        if (hit)break;
    }
    hit = false;
    for (int ky = y + 1; ky < SIZE; ky++) {
        hit = false;
        bool legal = is_legal_at_point(x, ky, &hit);
        if (!legal) return false;
        if (hit)break;
    }
    hit = false;
    for (int ky = y - 1; ky >= 0; ky--) {
        hit = false;
        bool legal = is_legal_at_point(x, ky, &hit);
        if (!legal) return false;
        if (hit)break;
    }
    return true;
}

bool is_legal_diagonally_move(int x, int y) {
    bool hit = false;
    for (int kx = x + 1, ky = y + 1; kx < SIZE && ky < SIZE && ky >= 0 && kx >= 0; kx++, ky++) {
        bool legal = is_legal_at_point(kx, ky, &hit);
        if (!legal) return false;
        if (hit)break;
    }
    hit = false;
    for (int kx = x - 1, ky = y + 1; kx < SIZE && ky < SIZE && ky >= 0 && kx >= 0; kx--, ky++) {
        bool legal = is_legal_at_point(kx, ky, &hit);
        if (!legal) return false;
        if (hit)break;
    }
    hit = false;
    for (int kx = x + 1, ky = y - 1; kx < SIZE && ky < SIZE && ky >= 0 && kx >= 0; kx++, ky--) {
        bool legal = is_legal_at_point(kx, ky, &hit);
        if (!legal) return false;
        if (hit)break;
    }
    hit = false;
    for (int kx = x - 1, ky = y - 1; kx < SIZE && ky < SIZE && ky >= 0 && kx >= 0; kx--, ky--) {
        bool legal = is_legal_at_point(kx, ky, &hit);
        if (!legal) return false;
        if (hit)break;
    }
    return true;
}

bool is_legal() {
    bool legal = true;
    for (int x = 0; x < SIZE; ++x) {
        for (int y = 0; y < SIZE; ++y) {
            char item = simulatedPlane[x][y];
            if (item == '.') continue;
            if (islower(item))
                //printf("started with [%c](%d,%d)\n", simulatedPlane[x][y],x,y);
                switch (item) {
                    case 'p':
                        //zbicie po skosie
                        if (in_bounds(x + 1, y - 1) && (simulatedPlane[x + 1][y - 1] == 'K')) {
                            return false;
                        }
                        if (in_bounds(x - 1, y - 1) && (simulatedPlane[x - 1][y - 1] == 'K')) {
                            return false;
                        }
                        break;
                    case 's':
                        if ((black_move_possible(x + 2, y + 1) && simulatedPlane[x + 2][y + 1] == 'K') ||
                            (black_move_possible(x - 2, y + 1) && simulatedPlane[x - 2][y + 1] == 'K') ||
                            (black_move_possible(x + 2, y - 1) && simulatedPlane[x + 2][y - 1] == 'K') ||
                            (black_move_possible(x - 2, y - 1) && simulatedPlane[x - 2][y - 1] == 'K') ||
                            (black_move_possible(x + 1, y + 2) && simulatedPlane[x + 1][y + 2] == 'K') ||
                            (black_move_possible(x - 1, y + 2) && simulatedPlane[x - 1][y + 2] == 'K') ||
                            (black_move_possible(x + 1, y - 2) && simulatedPlane[x + 1][y - 2] == 'K') ||
                            (black_move_possible(x - 1, y - 2) && simulatedPlane[x - 1][y - 2] == 'K')) {
                            return false;
                        }
                        break;
                    case 'w':
                        if (!is_legal_straight_move(x, y)) return false;
                        break;
                    case 'g':
                        if (!is_legal_diagonally_move(x, y)) return false;
                        break;
                    case 'h':
                        if (!is_legal_diagonally_move(x, y)) return false;
                        if (!is_legal_straight_move(x, y)) return false;
                        break;
                    case 'k':
                        if (black_move_possible(x + 1, y + 1) &&
                            simulatedPlane[x + 1][y + 1] == 'K')
                            return false;
                        if (black_move_possible(x + 1, y - 1) &&
                            simulatedPlane[x + 1][y - 1] == 'K')
                            return false;
                        if (black_move_possible(x - 1, y + 1) &&
                            simulatedPlane[x - 1][y + 1] == 'K')
                            return false;
                        if (black_move_possible(x - 1, y - 1) &&
                            simulatedPlane[x - 1][y - 1] == 'K')
                            return false;
                        if (black_move_possible(x, y + 1) &&
                            simulatedPlane[x][y + 1] == 'K')
                            return false;
                        if (black_move_possible(x + 1, y) &&
                            simulatedPlane[x + 1][y] == 'K')
                            return false;
                        if (black_move_possible(x - 1, y) &&
                            simulatedPlane[x - 1][y] == 'K')
                            return false;
                        if (black_move_possible(x, y - 1) &&
                            simulatedPlane[x][y - 1] == 'K')
                            return false;
                        break;
                    default:
                        break;
                }
        }
    }
    return legal;
}

bool promotion(int y) {
    return y == SIZE - 1;
}

int possibilities(char item, int x, int y) {
    int possible_moves = 0;
    switch (item) {
        case 'P':
            //jezeli startuje z poczatkowej linii
            if (y == 1) {
                if (white_move_possible(x, 2, x, y, item)) possible_moves++;
                if (white_move_possible(x, 3, x, y, item)) possible_moves++;
            } else if (white_move_possible(x, y + 1, x, y, item) && !islower(plane[x][y + 1])) {
                //zwykly ruch do przodu
                if (promotion(y + 1)) possible_moves += 3;
                possible_moves++;
            }
            //zbicie po skosie
            if (white_move_possible_if_hit(x + 1, y + 1, x, y, item)) {
                if (promotion(y + 1)) possible_moves += 3;
                possible_moves++;
            }
            if (white_move_possible_if_hit(x - 1, y + 1, x, y, item)) {
                if (promotion(y + 1)) possible_moves += 3;
                possible_moves++;
            }
            break;
        case 'S':
            if (white_move_possible(x + 2, y + 1, x, y, item)) possible_moves++;
            if (white_move_possible(x - 2, y + 1, x, y, item)) possible_moves++;
            if (white_move_possible(x + 2, y - 1, x, y, item)) possible_moves++;
            if (white_move_possible(x - 2, y - 1, x, y, item)) possible_moves++;
            if (white_move_possible(x + 1, y + 2, x, y, item)) possible_moves++;
            if (white_move_possible(x - 1, y + 2, x, y, item)) possible_moves++;
            if (white_move_possible(x + 1, y - 2, x, y, item)) possible_moves++;
            if (white_move_possible(x - 1, y - 2, x, y, item)) possible_moves++;
            break;
        case 'W':
            possible_moves += possibilities_for_straight_move(x, y, item);
            break;
        case 'G':
            possible_moves += possibilities_for_diagonally_move(x, y, item);
            break;
        case 'H':
            possible_moves += possibilities_for_diagonally_move(x, y, item);
            possible_moves += possibilities_for_straight_move(x, y, item);
            break;
        case 'K':
            if (white_move_possible(x + 1, y + 1, x, y, item)) possible_moves++;
            if (white_move_possible(x + 1, y - 1, x, y, item)) possible_moves++;
            if (white_move_possible(x - 1, y + 1, x, y, item)) possible_moves++;//
            if (white_move_possible(x - 1, y - 1, x, y, item)) possible_moves++;//
            if (white_move_possible(x + 1, y, x, y, item)) possible_moves++;
            if (white_move_possible(x, y - 1, x, y, item)) possible_moves++;
            if (white_move_possible(x - 1, y, x, y, item)) possible_moves++;//
            if (white_move_possible(x, y - 1, x, y, item)) possible_moves++;
            break;
        default:
            break;
    }
    return possible_moves;
}

int main() {
    for (int i = SIZE - 1; i >= 0; --i) {
        for (int j = 0; j < SIZE; ++j) {
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

bool __white_move_possible(int x, int y, int x_from, int y_from, char item, bool count_only_if_hit, bool *legal) {
    char item_at_destination = plane[x][y];

    *legal = true;
    bool is_possible =
            in_bounds(x, y) && ((item_at_destination == '.' && !count_only_if_hit) || (islower(item_at_destination)));
    if (!is_possible) return false;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            simulatedPlane[j][i] = plane[j][i];
        }
    }
    simulatedPlane[x_from][y_from] = '.';
    simulatedPlane[x][y] = item;
    *legal = is_legal();
    //printf("(%d,%d)[%c]->(%d,%d)[%c] %d\n", x_from, y_from, item, x, y, item_at_destination, *legal);
    return *legal;
}

bool _white_move_possible(int x, int y, int x_from, int y_from, char item, bool count_only_if_hit) {
    bool tmp = true;
    return __white_move_possible(x, y, x_from, y_from, item, count_only_if_hit, &tmp);
}

bool white_move_possible(int x, int y, int x_from, int y_from, char item) {
    return _white_move_possible(x, y, x_from, y_from, item, false);
}

bool white_move_possible_if_hit(int x, int y, int x_from, int y_from, char item) {
    return _white_move_possible(x, y, x_from, y_from, item, true);
}

bool white_move_possible_return_if_legal(int x, int y, int x_from, int y_from, char item, bool *is_legal) {
    return __white_move_possible(x, y, x_from, y_from, item, false, is_legal);
}
#include \u003Ciostream>
#include \u003Cvector>
#include \u003Cthread>
#include \u003Cchrono>
#include \u003Ctermios.h>
#include \u003Cunistd.h>
#include \u003Cfcntl.h>
#include \u003Calgorithm>
#include \u003Crandom>

using namespace std;

const int W = 10, H = 20;
const string BLK = "██", EMP = "  ", BORD = "▒▒;

struct Piece {
    int x, y, size;
    vector\u003Cvector\u003Cint>> shape;

    void rotate() {
        vector\u003Cvector\u003Cint>> next(size, vector\u003Cint>(size, 0));
        for (int i = 0; i \u003C size; i++)
            for (int j = 0; j \u003C size; j++)
                next[j][size - 1 - i] = shape[i][j];
        shape = next;
    }
};

vector\u003Cint> bag;
int grid[H + 1][W + 2];
long long score = 0;
int level = 1, speed = 600;
bool is_slow = false, cheat_mode = false;
string status_msg = "PRESS 'C' ДЛЯ ЧИТОВ";
vector\u003Cint> cheat_buffer;

vector\u003CPiece> forms = {
    {0, 0, 4, {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}},
    {0, 0, 2, {{1,1}, {1,1}}},
    {0, 0, 3, {{0,1,0}, {1,1,1}, {0,0,0}}},
    {0, 0, 3, {{1,0,0}, {1,1,1}, {0,0,0}}},
    {0, 0, 3, {{0,0,1}, {1,1,1}, {0,0,0}}},
    {0, 0, 3, {{0,1,1}, {1,1,0}, {0,0,0}}},
    {0, 0, 3, {{1,1,0}, {0,1,1}, {0,0,0}}}
};

Piece get_balanced_piece() {
    if (bag.empty()) {
        for (int i = 0; i \u003C 7; i++) bag.push_back(i);
        shuffle(bag.begin(), bag.end(), mt19937(random_device()()));
    }
    int id = bag.back();
    bag.pop_back();
    Piece p = forms[id];
    p.x = W / 2 - p.size / 2;
    p.y = 0;
    return p;
}

struct Cheat { string name; vector\u003Cint> seq; };
vector\u003CCheat> my_cheats = {
    {"NUKER", {66, 66, 66, 65}},
    {"SLOW_MO", {68, 68, 67, 67}},
    {"BACK_TO_REALITY", {65, 67, 66, 68}}
};

int get_key() {
    struct termios oldt, newt;
    tcgetattr(0, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &newt);
    int oldf = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, oldf | O_NONBLOCK);
    int ch = getchar();
    if (ch == 27) {
        if (getchar() == 91) ch = getchar(); 
    }
    tcsetattr(0, TCSANOW, &oldt);
    fcntl(0, F_SETFL, oldf);
    return ch;
}

bool collide(const Piece& p, int dx, int dy) {
    for (int i = 0; i \u003C p.size; i++) {
        for (int j = 0; j \u003C p.size; j++) {
            if (p.shape[i][j]) {
                int nx = p.x + j + dx + 1;
                int ny = p.y + i + dy;
                if (nx \u003C 0 || nx >= W + 2 || ny \u003C 0 || ny >= H + 1) return true;
                if (grid[ny][nx] != 0) return true;
            }
        }
    }
    return false;
}

void draw(const Piece& p) {
    printf("\033[H");
    printf("  SCORE: %lld  LVL: %d  %s\n", score, level, is_slow ? "[SLOW]" : "");
    printf("  \033[1;32m%-20s\033[0m\n", status_msg.c_str());
    for (int i = 0; i \u003C= H; i++) {
        for (int j = 0; j \u003C W + 2; j++) {
            if (grid[i][j] == -1) {
                printf("%s", BORD.c_str());
                continue;
            }
            bool is_p = false;
            int px = j - 1 - p.x;
            int py = i - p.y;
            if (px >= 0 && px \u003C p.size && py >= 0 && py \u003C p.size && p.shape[py][px]) {
                is_p = true;
            }
            if (is_p || grid[i][j] == 1) printf("%s", BLK.c_str());
            else printf("%s", EMP.c_str());
        }
        printf("\n");
    }
}

int main() {
    for (int i = 0; i \u003C= H; i++) 
        for (int j = 0; j \u003C W + 2; j++) 
            grid[i][j] = (j == 0 || j == W + 1 || i == H) ? -1 : 0;

    printf("\033[?25l\033[2J");
    Piece cur = get_balanced_piece();
    auto last_drop = chrono::steady_clock::now();

    while (true) {
        int current_speed = is_slow ? speed * 3 : speed;
        if (chrono::steady_clock::now() - last_drop > chrono::milliseconds(current_speed)) {
            if (!collide(cur, 0, 1)) {
                cur.y++;
            } else {
                for (int i = 0; i \u003C cur.size; i++)
                    for (int j = 0; j \u003C cur.size; j++)
                        if (cur.shape[i][j] && cur.y + i >= 0) 
                            grid[cur.y + i][cur.x + j + 1] = 1;
                for (int i = 0; i \u003C H; i++) {
                    bool full = true;
                    for (int j = 1; j \u003C= W; j++) if (grid[i][j] \u003C= 0) full = false;
                    if (full) {
                        for (int k = i; k > 0; k--)
                            for (int j = 1; j \u003C= W; j++) grid[k][j] = grid[k-1][j];
                        score += 100;
                    }
                }
                level = (score / 1000) + 1;
                speed = max(100, 600 - (level * 40));
                cur = get_balanced_piece();
                if (collide(cur, 0, 0)) break;
            }
            last_drop = chrono::steady_clock::now();
        }
        int k = get_key();
        if (k != -1) {
            if (k == 'c') {
                cheat_mode = !cheat_mode;
                cheat_buffer.clear();
                status_msg = cheat_mode ? "ВВОДИ КОМБО..." : "PRESS 'C' ДЛЯ ЧИТОВ";
            } else if (cheat_mode && k >= 65 && k \u003C= 68) {
                cheat_buffer.push_back(k);
                bool possible = false;
                for (auto& c : my_cheats) {
                    if (cheat_buffer.size() \u003C= c.seq.size()) {
                        bool match = true;
                        for (size_t i = 0; i \u003C cheat_buffer.size(); i++) 
                            if (cheat_buffer[i] != c.seq[i]) match = false;
                        if (match) {
                            possible = true;
                            if (cheat_buffer.size() == c.seq.size()) {
                                if (c.name == "NUKER") {
                                    for(int i=H-1; i>H-6; i--) for(int j=1; j\u003C=W; j++) grid[i][j]=0;
                                    status_msg = "ЯДЕРНЫЙ УДАР!";
                                } else if (c.name == "SLOW_MO") {
                                    is_slow = true; status_msg = "ЗАМЕДЛЕНИЕ";
                                } else if (c.name == "BACK_TO_REALITY") {
                                    is_slow = false; status_msg = "РЕАЛЬНОСТЬ";
                                }
                                cheat_mode = false; cheat_buffer.clear();
                            }
                        }
                    }
                }
                if (!possible) {
                    status_msg = "ОШИБКА!"; cheat_mode = false; cheat_buffer.clear();
                }
            } else {
                if (k == 68 && !collide(cur, -1, 0)) cur.x--;
                if (k == 67 && !collide(cur, 1, 0)) cur.x++;
                if (k == 66 && !collide(cur, 0, 1)) cur.y++;
                if (k == 65) {
                    Piece next = cur; next.rotate();
                    if (!collide(next, 0, 0)) cur = next;
                }
                if (k == 'q') break;
            }
        }
        draw(cur);
        this_thread::sleep_for(chrono::milliseconds(30));
    }
    printf("\033[?25h\033[2J\033[H  GAME OVER\n  SCORE: %lld\n", score);
    return 0;
}


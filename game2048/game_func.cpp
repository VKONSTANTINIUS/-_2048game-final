#include "game2048.h"

// Ініціалізація глобальних змінних
HANDLE h;
const char* menuItems[MENU_SIZE] = { "New Game", "Continue", "Exit" };
int board[BOX_SIZE][BOX_SIZE]{};
int score = 0;

void ConsoleSett() {
    system("mode con cols=50 lines=20");
    system("title GAME2048");
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = false;
    cci.dwSize = 100;
    SetConsoleCursorInfo(h, &cci);
    system("cls");
}

void setCursorPos(int x, int y) {
    COORD pos = { (int)x, (int)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setColor(int value) {
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (value) {
    case 1: SetConsoleTextAttribute(h, 48); break;
    case 2: SetConsoleTextAttribute(h, 2); break;      // light gray
    case 4: SetConsoleTextAttribute(h, 11); break;     // light cyan
    case 8: SetConsoleTextAttribute(h, 14); break;     // yellow
    case 16: SetConsoleTextAttribute(h, 10); break;    // green
    case 32: SetConsoleTextAttribute(h, 13); break;    // magenta
    case 64: SetConsoleTextAttribute(h, 12); break;    // red
    case 128: SetConsoleTextAttribute(h, 9); break;    // blue
    case 256: SetConsoleTextAttribute(h, 6); break;    // brown/yellow
    case 512: SetConsoleTextAttribute(h, 3); break;    // aqua
    case 1024: SetConsoleTextAttribute(h, 5); break;   // purple
    case 2048: SetConsoleTextAttribute(h, 15); break;  // white
    default: SetConsoleTextAttribute(h, 8); break;     // dark gray
    }
}

void drawFrame(int x, int y, int width, int height, int color) {
    setColor(8);
    for (int i = 0; i < width; i++) {
        setCursorPos(x + i, y);
        std::cout << char(196);
        setCursorPos(x + i, y + height - 1);
        std::cout << char(196);
    }
    for (int i = 0; i < height; i++) {
        setCursorPos(x, y + i);
        std::cout << char(179);
        setCursorPos(x + width - 1, y + i);
        std::cout << char(179);
    }
    setCursorPos(x, y);                     std::cout << char(218);
    setCursorPos(x + width - 1, y);         std::cout << char(191);
    setCursorPos(x, y + height - 1);        std::cout << char(192);
    setCursorPos(x + width - 1, y + height - 1); std::cout << char(217);
    setColor(7);
}

void drawMenu(int selected) {
    system("cls");

    int startX = 2;
    int startY = 2;
    int width = 20;
    int height = 3;

    for (int i = 0; i < MENU_SIZE; i++) {
        int x = startX;
        int y = startY + i * height;

        if (i == selected) {
            drawFrame(x - 2, y - 1, width, height, 10);
            setColor(8);
        }
        else {
            setColor(2048);
        }

        setCursorPos(x, y);
        std::cout << menuItems[i];
    }
    setColor(7);
}

void menu_init() {
    int selected = 0;
    bool running = true;

    while (running) {
        drawMenu(selected);

        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == UP) {
                selected = (selected - 1 + MENU_SIZE) % MENU_SIZE;
                Beep(700, 10);
            }
            else if (key == DOWN) {
                selected = (selected + 1) % MENU_SIZE;
                Beep(700, 10);
            }
        }
        else if (key == ENTER) {
            system("cls");
            setColor(14);
            std::cout << "Selected option: " << menuItems[selected] << std::endl;
            setColor(7);
            if (selected == 2) {
                system("cls");
                std::cout << "ESCAPE!\n";
                exit(0);
            }
            else if (selected == 0) {
                set_board();
                new_game();
            }
            else if (selected == 1) {
                set_board();
                loadsave("autosave.txt");
                new_game();
            }
            else {
                system("pause");
            }
        }
    }
}

void set_board() {
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            board[i][j] = 0;
        }
    }
    add_number();
    add_number();
}

void add_number() {
    int i = rand() % BOX_SIZE;
    int j = rand() % BOX_SIZE;
    int value = (rand() % 2 + 1) * 2;
    if (board[i][j] == 0) {
        board[i][j] = value;
    }
    else {
        add_number();
    }
}

void draw_board() {
    system("cls");
    std::cout << "Score: " << score << "\n";

    std::cout << char(218);
    for (int i = 0; i < BOX_SIZE - 1; i++) {
        std::cout << std::string(6, char(196)) << char(194);
    }
    std::cout << std::string(6, char(196)) << char(191) << std::endl;

    for (int y = 0; y < BOX_SIZE; y++) {
        std::cout << char(179);
        for (int x = 0; x < BOX_SIZE; x++) {
            setColor(board[y][x]);
            std::cout << std::setw(6) << board[y][x];
            setColor(2048);
            std::cout << char(179);
        }
        std::cout << std::endl;

        if (y != BOX_SIZE - 1) {
            std::cout << char(195);
            for (int i = 0; i < BOX_SIZE - 1; i++) {
                std::cout << std::string(6, char(196)) << char(197);
            }
            std::cout << std::string(6, char(196)) << char(180) << std::endl;
        }
    }

    std::cout << char(192);
    for (int i = 0; i < BOX_SIZE - 1; i++) {
        std::cout << std::string(6, char(196)) << char(193);
    }
    std::cout << std::string(6, char(196)) << char(217) << std::endl;
}

void move_up() {
    for (int j = 0; j < BOX_SIZE; j++) {
        for (int i = 1; i < BOX_SIZE; i++) {
            if (board[i][j] != 0) {
                int k = i;
                while (k > 0 && board[k - 1][j] == 0) {
                    board[k - 1][j] = board[k][j];
                    board[k][j] = 0;
                    k--;
                }
                if (k > 0 && board[k - 1][j] == board[k][j]) {
                    board[k - 1][j] *= 2;
                    score += board[k - 1][j];
                    board[k][j] = 0;
                }
            }
        }
    }
    add_number();
    Beep(700, 10);
    draw_board();
}

void move_down() {
    for (int j = 0; j < BOX_SIZE; j++) {
        for (int i = BOX_SIZE - 2; i >= 0; i--) {
            if (board[i][j] != 0) {
                int k = i;
                while (k < BOX_SIZE - 1 && board[k + 1][j] == 0) {
                    board[k + 1][j] = board[k][j];
                    board[k][j] = 0;
                    k++;
                }
                if (k < BOX_SIZE - 1 && board[k + 1][j] == board[k][j]) {
                    board[k + 1][j] *= 2;
                    score += board[k + 1][j];
                    board[k][j] = 0;
                }
            }
        }
    }
    add_number();
    Beep(700, 10);
    draw_board();
}

void move_left() {
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 1; j < BOX_SIZE; j++) {
            if (board[i][j] != 0) {
                int k = j;
                while (k > 0 && board[i][k - 1] == 0) {
                    board[i][k - 1] = board[i][k];
                    board[i][k] = 0;
                    k--;
                }
                if (k > 0 && board[i][k - 1] == board[i][k]) {
                    board[i][k - 1] *= 2;
                    score += board[i][k - 1];
                    board[i][k] = 0;
                }
            }
        }
    }
    add_number();
    Beep(700, 10);
    draw_board();
}

void move_right() {
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = BOX_SIZE - 2; j >= 0; j--) {
            if (board[i][j] != 0) {
                int k = j;
                while (k < BOX_SIZE - 1 && board[i][k + 1] == 0) {
                    board[i][k + 1] = board[i][k];
                    board[i][k] = 0;
                    k++;
                }
                if (k < BOX_SIZE - 1 && board[i][k + 1] == board[i][k]) {
                    board[i][k + 1] *= 2;
                    score += board[i][k + 1];
                    board[i][k] = 0;
                }
            }
        }
    }
    add_number();
    Beep(700, 10);
    draw_board();
}

bool is_full() {
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            if (board[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool is_won() {
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            if (board[i][j] == WIN_COUNT) {
                return true;
            }
        }
    }
    return false;
}

bool no_move() {
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            if (board[i][j] == 0) {
                return false;
            }
            if (j > 0 && board[i][j] == board[i][j - 1]) {
                return false;
            }
            if (j < BOX_SIZE - 1 && board[i][j] == board[i][j + 1]) {
                return false;
            }
            if (i > 0 && board[i][j] == board[i - 1][j]) {
                return false;
            }
            if (i < BOX_SIZE - 1 && board[i][j] == board[i + 1][j]) {
                return false;
            }
        }
    }
    return true;
}

void new_game() {
    srand(time(0));
    draw_board();

    while (true) {
        int direct = _getch();
        if (direct == 224) {
            direct = _getch();
        }

        if (direct == RIGHT) {
            move_right();
        }
        else if (direct == LEFT) {
            move_left();
        }
        else if (direct == UP) {
            move_up();
        }
        else if (direct == DOWN) {
            move_down();
        }
        else if (direct == ESC) {
            autosave("autosave.txt");
            menu_init();
        }

        if (is_won()) {
            system("cls");
            std::cout << "You won!\n";
            system("pause");
            menu_init();
        }

        if (is_full() || no_move()) {
            system("cls");
            std::cout << "Game over!\n";
            system("pause");
            menu_init();
        }
    }
}

void autosave(const char* filename) {
    FILE* out = fopen(filename, "w");
    if (!out) {
        std::cerr << "---" << std::endl;
        system("pause");
        return;
    }

    for (int y = 0; y < BOX_SIZE; y++) {
        for (int x = 0; x < BOX_SIZE; x++) {
            fprintf(out, "%d ", board[y][x]);
        }
        fprintf(out, "\n");
    }
    fclose(out);
}

void loadsave(const char* filename) {
    FILE* in = fopen(filename, "r");
    if (!in) {
        system("cls");
        std::cerr << "Can't open file" << std::endl;
        system("pause");
        menu_init();
        return;
    }

    for (int y = 0; y < BOX_SIZE; y++) {
        for (int x = 0; x < BOX_SIZE; x++) {
            if (fscanf(in, "%d", &board[y][x]) != 1) {
                std::cerr << "Error reading file data" << std::endl;
                fclose(in);
                system("pause");
                menu_init();
                return;
            }
        }
    }
    fclose(in);
}

void loading() {
    system("cls");
    for (int i = 0; i < 5; i++) {
        setCursorPos(2, i);
        for (int j = 0; j < 4; j++) {
            if (i == 0 || (i == 1 && j == 3) || i == 2 || (i == 3 && j == 0) || i == 4) {
                setColor(1);
                Sleep(10);
                Beep(700, 10);
                std::cout << " ";
            }
            else {
                setColor(2);
                Sleep(10);
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    for (int i = 0; i < 5; i++) {
        setCursorPos(7, i);
        for (int j = 0; j < 4; j++) {
            if (i == 0 || i == 4 || j == 0 || j == 3) {
                setColor(1);
                Sleep(10);
                Beep(700, 10);
                std::cout << " ";
            }
            else {
                setColor(2);
                Sleep(10);
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    for (int i = 0; i < 5; i++) {
        setCursorPos(12, i);
        for (int j = 0; j < 4; j++) {
            if ((i == 0 && (j == 0 || j == 3)) || (i == 1 && (j == 0 || j == 3)) ||
                i == 2 || (i == 3 && j == 3) || (i == 4 && j == 3)) {
                setColor(1);
                Sleep(10);
                Beep(700, 10);
                std::cout << " ";
            }
            else {
                setColor(2);
                Sleep(10);
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    for (int i = 0; i < 5; i++) {
        setCursorPos(17, i);
        for (int j = 0; j < 4; j++) {
            if (i == 0 || (i == 1 && (j == 0 || j == 3)) ||
                i == 2 || (i == 3 && (j == 0 || j == 3)) || i == 4) {
                setColor(1);
                Sleep(10);
                Beep(700, 10);
                std::cout << " ";
            }
            else {
                setColor(2);
                Sleep(10);
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    setColor(2);
    system("pause");
}
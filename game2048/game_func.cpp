#include "game2048.h"  // Включаємо хедер-файл, щоб отримати доступ до оголошень
using namespace std; //  для використання імен з std без префікса.

// --- Визначення змінних ---
// Ці змінні оголошені як 'extern' у хедері, а тут вони визначаються (виділяється пам'ять)
HANDLE h;
const char* menuItems[MENU_SIZE] = { "New Game", "Continue", "Exit" };
int board[BOX_SIZE][BOX_SIZE]{};
int score = 0;

// --- Реалізації функцій налаштувань консолі ---

// Функція для початкового налаштування консолі:
// - Встановлює розмір вікна консолі.
// - Встановлює заголовок вікна.
// - Отримує дескриптор стандартного виводу консолі.
// - Приховує курсор.
// - Очищає екран.

void ConsoleSett() {
    system("mode con cols=50 lines=20"); // Встановлення розміру консолі
    system("title GAME2048");  // Встановлення заголовка вікна
    h = GetStdHandle(STD_OUTPUT_HANDLE);  // Отримання дескриптора для виводу
    CONSOLE_CURSOR_INFO cci;  // Структура для інформації про курсор
    cci.bVisible = false;   // Приховати курсор
    cci.dwSize = 100;  // Розмір курсора
    SetConsoleCursorInfo(h, &cci);   // Застосувати налаштування курсора
    system("cls"); // Очистити екран
}

// Функція для встановлення позиції курсора в консолі за заданими координатами (x, y)
void setCursorPos(int x, int y) {
    COORD pos = { (int)x, (int)y }; // Створення структури COORD
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);  // Встановлення позиції
}

// Функція для встановлення кольору тексту консолі залежно від значення.
// Використовує SetConsoleTextAttribute для зміни кольору символів.
void setColor(int value) {
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (value) {
    case 1: SetConsoleTextAttribute(h, 48); break;     // Спеціальний колір для "завантаження"
    case 2: SetConsoleTextAttribute(h, 2); break;      // Світло-сірий
    case 4: SetConsoleTextAttribute(h, 11); break;     // Світло-блакитний
    case 8: SetConsoleTextAttribute(h, 14); break;     // Жовтий
    case 16: SetConsoleTextAttribute(h, 10); break;    // Зелений
    case 32: SetConsoleTextAttribute(h, 13); break;    // Пурпурний
    case 64: SetConsoleTextAttribute(h, 12); break;    // Червоний
    case 128: SetConsoleTextAttribute(h, 9); break;    // Синій
    case 256: SetConsoleTextAttribute(h, 6); break;    // Коричневий/Жовтий
    case 512: SetConsoleTextAttribute(h, 3); break;    // Аква
    case 1024: SetConsoleTextAttribute(h, 5); break;   // Фіолетовий
    case 2048: SetConsoleTextAttribute(h, 15); break;  // Білий 
    default: SetConsoleTextAttribute(h, 8); break;     // Темно-сірий (за замовчуванням)
    }
}

// --- Реалізації функцій для роботи з меню ---

// Малює рамку навколо обраного пункту меню.
void drawFrame(int x, int y, int width, int height, int color) {
    setColor(8);  // Встановлюємо колір рамки
    // Горизонтальні лінії
    for (int i = 0; i < width; i++) {
        setCursorPos(x + i, y);
        std::cout << char(196);  // Горизонтальна лінія
        setCursorPos(x + i, y + height - 1);
        std::cout << char(196);
    }
    // Вертикальні лінії
    for (int i = 0; i < height; i++) {
        setCursorPos(x, y + i);
        std::cout << char(179);  // Вертикальна лінія
        setCursorPos(x + width - 1, y + i);
        std::cout << char(179);
    }
    // Кути рамки
    setCursorPos(x, y);                     
    cout << char(218);  // Верхній лівий
    setCursorPos(x + width - 1, y);         
    cout << char(191);  // Верхній правий
    setCursorPos(x, y + height - 1);        
    cout << char(192);  // Нижній лівий
    setCursorPos(x + width - 1, y + height - 1); 
    cout << char(217);  // Нижній правий
    setColor(7);
}

// Відображає ігрове меню.
// Виділяє обраний пункт рамкою та змінює колір тексту.
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
            drawFrame(x - 2, y - 1, width, height, 10);  // Малюємо рамку навколо вибраного пункту
            setColor(8); // Колір тексту для вибраного пункту
        }
        else {
            setColor(2048);  // Колір тексту для не вибраних пунктів
        }

        setCursorPos(x, y);  // Встановлюємо курсор
        cout << menuItems[i];  // Виводимо пункт меню
    }
    setColor(7);  // Повертаємо стандартний колір
}

// Ініціалізує ігрове меню та обробляє вибір користувача.
// Дозволяє переміщатися по меню за допомогою стрілок та вибирати пункти за допомогою Enter.
void menu_init() {
    int selected = 0;  // Поточний вибраний пункт меню
    bool running = true;

    while (running) {
        drawMenu(selected);  // Малюємо меню

        int key = _getch();  // Зчитуємо натискання клавіші
        if (key == 0 || key == 224) {  // Перевірка на спеціальні клавіші (стрілки)
            key = _getch();  // Зчитуємо фактичний код стрілки
            if (key == UP) {  // Рух по меню вгору
                selected = (selected - 1 + MENU_SIZE) % MENU_SIZE;
                Beep(700, 10);  // Звуковий сигнал
            }
            else if (key == DOWN) {   // Рух по меню вниз
                selected = (selected + 1) % MENU_SIZE;
                Beep(700, 10);
            }
        }
        else if (key == ENTER) {  // Вибір пункту меню
            system("cls"); // Очищаємо екран
            setColor(14);
            cout << "Selected option: " << menuItems[selected] << endl;
            setColor(7);
            if (selected == 2) {  // Вибрано пункт "Exit"
                system("cls");
                std::cout << "ESCAPE!\n";
                exit(0);  // Завершення програми
            }
            else if (selected == 0) {  // Вибрано пункт "New Game"
                set_board();  // Ініціалізуємо нове поле
                new_game();  // Починаємо нову гру
            }
            else if (selected == 1) {  // Вибрано пункт "Continue"
                set_board();  // Очищаємо поле перед завантаженням
                loadsave("autosave.txt");  // Завантажуємо збережену гру
                new_game();  // Продовжуємо гру
            }
            else {
                system("pause");  // Пауза, якщо пункт не оброблений
            }
        }
    }
}

// --- Реалізації функцій гри ---

// Ініціалізує ігрове поле, заповнюючи його нулями,
// а потім додає два початкові числа (2 або 4)

void set_board() {
    // Заповнюємо поле нулями
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            board[i][j] = 0;
        }
    }
    // Додаємо два початкові числа
    add_number();
    add_number();
}

// Додає випадкове число (2 або 4) в випадкову порожню клітинку (зі значенням 0).
// Якщо вибрана клітинка не порожня, функція викликається рекурсивно,
// доки не знайдеться вільне місце.
void add_number() {
    int i = rand() % BOX_SIZE; // Випадковий рядок
    int j = rand() % BOX_SIZE; // Випадковий стовпець
    int value = (rand() % 2 + 1) * 2; // Випадкове значення (2 або 4)
    if (board[i][j] == 0) {  // Якщо клітинка порожня
        board[i][j] = value;  // Додаємо число
    }
    else {
        add_number();  // Якщо клітинка зайнята, шукаємо іншу
    }
}

// Виводить поточний стан ігрового поля на екран, включаючи рахунок.
// Використовує символи псевдографіки для малювання сітки та встановлює
// різні кольори для чисел на полі.
void draw_board() {
    system("cls");  // Очищаємо екран
    cout << "Score: " << score << "\n";  // Виводимо поточний рахунок

     // Верхня межа поля
    cout << char(218);  // Верхній лівий кут
    for (int i = 0; i < BOX_SIZE - 1; i++) {
        cout << string(6, char(196)) << char(194); // Горизонтальні лінії та перетини
    }
    cout << string(6, char(196)) << char(191) << endl; // Верхній правий кут
    
    // Рядки поля
    for (int y = 0; y < BOX_SIZE; y++) {
        cout << char(179);  // Ліва вертикальна межа
        for (int x = 0; x < BOX_SIZE; x++) {
            setColor(board[y][x]);  // Встановлюємо колір для числа
            // Виводимо число, вирівняне по ширині (setw)
            cout << setw(6) << board[y][x];
            setColor(2048);  // Повертаємо колір рамки/сітки
            cout << char(179);  // Права вертикальна межа комірки
        }
        cout << endl;  // Перехід на новий рядок

        // Горизонтальні роздільники між рядками (крім останнього)
        if (y != BOX_SIZE - 1) {
            cout << char(195);  // Лівий перетин
            for (int i = 0; i < BOX_SIZE - 1; i++) {
                cout << string(6, char(196)) << char(197);  // Горизонтальні лінії та перетини
            }
            cout << string(6, char(196)) << char(180) << endl;  // Правий перетин
        }
    }

    // Нижня межа поля
    cout << char(192);  // Нижній лівий кут
    for (int i = 0; i < BOX_SIZE - 1; i++) {
        cout << string(6, char(196)) << char(193);  // Горизонтальні лінії та перетини
    }
    cout << string(6, char(196)) << char(217) << endl;  // Нижній правий кут
}

// Переміщує всі плитки вгору та об'єднує однакові числа.
// Після руху додає нове число та перемальовує дошку.
void move_up() {
    for (int j = 0; j < BOX_SIZE; j++) {
        for (int i = 1; i < BOX_SIZE; i++) {
            if (board[i][j] != 0) {
                int k = i;
                 // Переміщуємо плитку вгору, поки є порожні клітинки
                while (k > 0 && board[k - 1][j] == 0) {
                    board[k - 1][j] = board[k][j];
                    board[k][j] = 0;
                    k--;
                }
                // Якщо наступна клітинка вгорі має таке ж значення, об'єднуємо їх
                if (k > 0 && board[k - 1][j] == board[k][j]) {
                    board[k - 1][j] *= 2;  // Подвоюємо значення
                    score += board[k - 1][j];  // Додаємо до рахунку
                    board[k][j] = 0;  // Очищаємо початкову клітинку
                }
            }
        }
    }
    add_number();  // Додаємо нове число
    Beep(700, 10);  // Звуковий сигнал
    draw_board();  // Перемальовуємо поле
}

// Переміщує всі плитки вниз та об'єднує однакові числа.
// Аналогічно move_up, але рух відбувається у зворотному напрямку.
void move_down() {
    for (int j = 0; j < BOX_SIZE; j++) {
        for (int i = BOX_SIZE - 2; i >= 0; i--) {
            if (board[i][j] != 0) {
                int k = i;
                // Переміщуємо плитку вниз, поки є порожні клітинки
                while (k < BOX_SIZE - 1 && board[k + 1][j] == 0) {
                    board[k + 1][j] = board[k][j];
                    board[k][j] = 0;
                    k++;
                }
                // Якщо наступна клітинка внизу має таке ж значення, об'єднуємо їх
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
// Переміщує всі плитки вліво та об'єднує однакові числа.
// Аналогічно move_up, але рух відбувається по горизонталі.
void move_left() {
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 1; j < BOX_SIZE; j++) {
            if (board[i][j] != 0) {
                int k = j;
                // Переміщуємо плитку вліво, поки є порожні клітинки
                while (k > 0 && board[i][k - 1] == 0) {
                    board[i][k - 1] = board[i][k];
                    board[i][k] = 0;
                    k--;
                }
                // Якщо наступна клітинка зліва має таке ж значення, об'єднуємо їх
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

// Переміщує всі плитки вправо та об'єднує однакові числа.
// Аналогічно move_left, але рух відбувається у зворотному напрямку по горизонталі.
void move_right() {
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = BOX_SIZE - 2; j >= 0; j--) {
            if (board[i][j] != 0) {
                int k = j;
                // Переміщуємо плитку вправо, поки є порожні клітинки
                while (k < BOX_SIZE - 1 && board[i][k + 1] == 0) {
                    board[i][k + 1] = board[i][k];
                    board[i][k] = 0;
                    k++;
                }
                // Якщо наступна клітинка справа має таке ж значення, об'єднуємо їх
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

// --- Реалізації логічних функцій для визначення стану гри ---

// Перевіряє, чи всі клітинки на ігровому полі зайняті (не містять 0).
// Повертає 'true', якщо поле повністю заповнене, і 'false' в іншому випадку.
bool is_full() {
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            if (board[i][j] == 0) {  // Якщо знайдено порожню клітинку
                return false;  // Поле не повне
            }
        }
    }
    return true;  // Всі клітинки зайняті
}

// Перевіряє, чи гравець досяг переможного значення (WIN_COUNT) на будь-якій плитці.
// Повертає 'true', якщо знайдено переможну плитку, і 'false' в іншому випадку.
bool is_won() {
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            if (board[i][j] == WIN_COUNT) {  // Якщо знайдено плитку з переможним значенням
                return true;  // Гра виграна
            }
        }
    }
    return false;  // Переможного значення не досягнуто
}

// Перевіряє, чи можливі подальші ходи.
// Це включає перевірку наявності порожніх клітинок та можливість об'єднання сусідніх плиток.
// Повертає 'true', якщо ходів більше немає (гра закінчилася), і 'false' в іншому випадку.
bool no_move() {

    // Перевіряємо можливість об'єднання по горизонталі
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
             // Перевіряємо можливість об'єднання по вертикалі
            if (i > 0 && board[i][j] == board[i - 1][j]) {
                return false;
            }
            if (i < BOX_SIZE - 1 && board[i][j] == board[i + 1][j]) {
                return false;
            }
        }
    }
    return true;  // Рухів немає
}

// --- Реалізація основної функції гри ---

// Основна функція ігрового процесу.
// Ініціалізує генератор випадкових чисел, відображає поле,
// і в нескінченному циклі обробляє введення користувача
// та перевіряє стан гри (виграш/програш).
void new_game() {
    srand(time(0)); // Ініціалізація генератора випадкових чисел
    draw_board();  // Початкове відображення поля

    while (true) {  // Нескінченний цикл гри
        int direct = _getch();  // Зчитуємо натискання клавіші
        if (direct == 224) {  // Якщо це спеціальна клавіша (стрілка), зчитуємо її код
            direct = _getch();
        }
        
        // Обробка рухів
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
        else if (direct == ESC) {  // Якщо натиснуто ESC, зберегти гру та повернутися в меню
            autosave("autosave.txt");
            menu_init();
        }
        
        // Перевірка умов виграшу
        if (is_won()) {
            system("cls");
            std::cout << "You won!\n";
            system("pause");
            menu_init();
        }
        
        // Перевірка умов програшу (поле повне і немає можливих ходів)
        if (is_full() || no_move()) {
            system("cls");
            std::cout << "Game over!\n";
            system("pause");
            menu_init();
        }
    }
}

// --- Реалізації функцій збереження/завантаження ---

// Зберігає поточний стан ігрового поля у вказаний файл.
// Кожна клітинка поля записується через пробіл, рядок - через перенос рядка.
void autosave(const char* filename) {
    FILE* out = fopen(filename, "w");  // Відкриття файлу для запису
    if (!out) {  // Перевірка на помилку відкриття файлу
        cerr << "---" << endl;
        system("pause");
        return;
    }

    for (int y = 0; y < BOX_SIZE; y++) {
        for (int x = 0; x < BOX_SIZE; x++) {
            fprintf(out, "%d ", board[y][x]);  // Запис значення клітинки
        }
        fprintf(out, "\n");  // Перехід на новий рядок після кожного ряду поля
    }
    fclose(out);  // Закриття файлу
}

// Завантажує стан ігрового поля з вказаного файлу.
// Зчитує значення клітинок і заповнює ними масив board.
// Обробляє помилки відкриття файлу та зчитування даних.
void loadsave(const char* filename) {
    FILE* in = fopen(filename, "r");  // Відкриття файлу для читання
    if (!in) { // Перевірка на помилку відкриття файлу
        system("cls");
        cerr << "Can't open file" << endl;
        system("pause");
        // Якщо файл не знайдено, можна або почати нову гру, або повернутися в меню.
        // Поточна реалізація повертається в меню, щоб користувач міг вибрати "New Game"
        menu_init();
        return;
    }

    for (int y = 0; y < BOX_SIZE; y++) {
        for (int x = 0; x < BOX_SIZE; x++) {
            // Зчитування значення клітинки. Перевірка, чи успішно зчитано 1 елемент.
            if (fscanf(in, "%d", &board[y][x]) != 1) {
                cerr << "Error reading file data" << endl;
                fclose(in);
                system("pause");
                menu_init();  // Повернення в меню у випадку помилки читання
                return;
            }
        }
    }
    fclose(in);  // Закриття файлу
}

// Функція, що відображає стилізовану анімацію завантаження "2048".
// Використовує встановлення позиції курсора, кольори та затримки для анімаційного ефекту.
void loading() {
    system("cls");  // Очищаємо екран
    // Анімація цифри '2'
    for (int i = 0; i < 5; i++) {
        setCursorPos(2, i);
        for (int j = 0; j < 4; j++) {
            if (i == 0 || (i == 1 && j == 3) || i == 2 || (i == 3 && j == 0) || i == 4) {
                setColor(1);
                Sleep(10);
                Beep(700, 10);
                cout << " ";
            }
            else {
                setColor(2);
                Sleep(10);
                cout << " ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
    // Анімація цифри '0'
    for (int i = 0; i < 5; i++) {
        setCursorPos(7, i);
        for (int j = 0; j < 4; j++) {
            if (i == 0 || i == 4 || j == 0 || j == 3) {
                setColor(1);
                Sleep(10);
                Beep(700, 10);
                cout << " ";
            }
            else {
                setColor(2);
                Sleep(10);
                cout << " ";
            }
        }
        cout << "\n";
    }
    // Анімація цифри '4'
    for (int i = 0; i < 5; i++) {
        setCursorPos(12, i);
        for (int j = 0; j < 4; j++) {
            if ((i == 0 && (j == 0 || j == 3)) || (i == 1 && (j == 0 || j == 3)) ||
                i == 2 || (i == 3 && j == 3) || (i == 4 && j == 3)) {
                setColor(1);
                Sleep(10);
                Beep(700, 10);
                cout << " ";
            }
            else {
                setColor(2);
                Sleep(10);
                cout << " ";
            }
        }
        cout << "\n";
    }
    // Анімація цифри '8'
    for (int i = 0; i < 5; i++) {
        setCursorPos(17, i);
        for (int j = 0; j < 4; j++) {
            if (i == 0 || (i == 1 && (j == 0 || j == 3)) ||
                i == 2 || (i == 3 && (j == 0 || j == 3)) || i == 4) {
                setColor(1);
                Sleep(10);
                Beep(700, 10);
                cout << " ";
            }
            else {
                setColor(2);
                Sleep(10);
                cout << " ";
            }
        }
        cout << "\n";
    }
    setColor(2);  // Повертаємо стандартний колір
    system("pause");  // Чекаємо на натискання будь-якої клавіші
}

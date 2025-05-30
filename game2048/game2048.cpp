//Гра "2048" на C++
//Ціль: Створити консольну гру "2048", де гравець має об'єднувати числа, щоб досягти 2048.

// Структура проєкту:
// project/
// ├── game2048.cpp - головний файл
// ├── game_func.cpp - файл з функціями гри
// └── game2048.h - хедер файл

// Логіка гри:
// Нові числа (2 або 4) з'являються в випадкових комірках 
// Плитки одного значення об'єднуються 
// Умова виграшу: досягти 2048 
// Програш: немає доступних ходів 

// Приклад виводу в консолі:

// +------+------+------+------+
// |    2 |    4 |      |      |
// +------+------+------+------+
// |      |   16 |    8 |      |
// +------+------+------+------+
// |      |      |      |      |
// +------+------+------+------+
// |      |      |      |      |
// +------+------+------+------+
// Score: 28

// Управління: стрілки клавіатури

#include "game2048.h"  // Включаємо хедер-файл, який містить усі оголошення функцій та елементів

// Основна функція програми.
// Викликає функції для налаштування консолі, відображення анімації завантаження
// та ініціалізації головного меню гри.

int main() {
    ConsoleSett();   // Налаштування консолі
    loading();  // Відображення анімації завантаження
    menu_init();  // Запуск ігрового меню
    return 0;   // Завершення програми з кодом успіху
}

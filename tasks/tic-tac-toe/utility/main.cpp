#include <iostream>
#include <dlfcn.h>
#include "../strategy.h"
#include "checker.h"

// Прототип функции CreateObj
typedef Strategy* (*CreateStrategy)();

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " <путь к библиотеке1> <путь к библиотеке2>" << std::endl;
        return 1;
    }

    void* handle1 = dlopen(argv[1], RTLD_LAZY);
    if (!handle1) {
        std::cerr << "Ошибка загрузки библиотеки: " << dlerror() << std::endl;
        return 1;
    }

    auto createObj1 = reinterpret_cast<CreateStrategy>(dlsym(handle1, "CreateStrategy"));
    const char* dlsym_error1 = dlerror();
    if (dlsym_error1) {
        std::cerr << "Ошибка получения адреса функции CreateObj: " << dlsym_error1 << std::endl;
        dlclose(handle1);
        return 1;
    }

    Strategy* firstStrategy = createObj1();
    if (!firstStrategy) {
        std::cerr << "Ошибка вызова функции CreateObj для первой библиотеки" << std::endl;
        dlclose(handle1);
        return 1;
    }

    void* handle2 = dlopen(argv[2], RTLD_LAZY);
    if (!handle2) {
        std::cerr << "Ошибка загрузки библиотеки: " << dlerror() << std::endl;
        dlclose(handle1); // Закрываем первую библиотеку перед завершением программы
        return 1;
    }

    auto createObj2 = (CreateStrategy)dlsym(handle2, "CreateStrategy");
    const char* dlsym_error2 = dlerror();
    if (dlsym_error2) {
        std::cerr << "Ошибка получения адреса функции CreateObj: " << dlsym_error2 << std::endl;
        dlclose(handle1);
        dlclose(handle2);
        return 1;
    }

    Strategy* secondStrategy = createObj2();
    if (!secondStrategy) {
        std::cerr << "Ошибка вызова функции CreateObj для второй библиотеки" << std::endl;
        dlclose(handle1);
        dlclose(handle2);
        return 1;
    }

    Checker checker(firstStrategy, secondStrategy);
    int winner = checker.CheckWinner();

    dlclose(handle1);
    dlclose(handle2);

    return winner;
}
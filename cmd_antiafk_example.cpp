#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>
#include <random>

class AntiAFK {
private:
    bool running;
    int interval; // интервал в секундах
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;

public:
    AntiAFK(int sec = 60) : running(false), interval(sec), gen(rd()), dis(0, 100) {}

    void start() {
        running = true;
        std::cout << "Анти-АФК запущен. Интервал: " << interval << " сек.\n";
        std::cout << "Нажмите Ctrl+C для остановки.\n";
        
        while (running) {
            performAction();
            std::this_thread::sleep_for(std::chrono::seconds(interval));
        }
    }

    void stop() {
        running = false;
        std::cout << "\nАнти-АФК остановлен.\n";
    }

private:
    void performAction() {
        int action = dis(gen) % 3; // 0-движение, 1-клик, 2-клавиша
        
        switch(action) {
            case 0:
                moveMouse();
                break;
            case 1:
                clickMouse();
                break;
            case 2:
                pressKey();
                break;
        }
    }

    void moveMouse() {
        POINT pos;
        GetCursorPos(&pos);
        
        int dx = dis(gen) % 10 - 5;
        int dy = dis(gen) % 10 - 5;
        
        SetCursorPos(pos.x + dx, pos.y + dy);
        std::cout << "Движение мыши: (" << dx << ", " << dy << ")\n";
    }

    void clickMouse() {
        // Имитация клика левой кнопкой
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        std::cout << "Клик мыши\n";
    }

    void pressKey() {
        // Имитация нажатия клавиши (например, пробел)
        keybd_event(VK_SPACE, 0, 0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
        std::cout << "Нажатие клавиши SPACE\n";
    }
};

// Обработчик Ctrl+C
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        std::cout << "\nПолучен сигнал остановки...\n";
        exit(0);
    }
    return TRUE;
}

int main() {
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
    
    std::cout << "=== Анти-АФК v1.0 ===\n";
    std::cout << "Введите интервал в секундах (по умолчанию 60): ";
    
    int interval = 60;
    std::string input;
    std::getline(std::cin, input);
    
    if (!input.empty()) {
        try {
            interval = std::stoi(input);
            if (interval < 5) interval = 5;
            if (interval > 300) interval = 300;
        } catch (...) {
            interval = 60;
        }
    }
    
    AntiAFK antiAFK(interval);
    antiAFK.start();
    
    return 0;
}
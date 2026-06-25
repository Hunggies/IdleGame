#ifndef CONSOLE_UTILS_HPP
#define CONSOLE_UTILS_HPP

#include <string>

namespace ConsoleUtils {
    // Xóa toàn bộ màn hình Console và đưa con trỏ về góc trên cùng bên trái
    void clearScreen();

    // Thay đổi màu chữ hiển thị (Dùng mã màu ANSI cơ bản)
    // 31: Đỏ, 32: Xanh lá, 33: Vàng, 34: Xanh dương, 35: Tím, 36: Xanh ngọc, 37: Trắng
    void setTextColor(int colorCode);

    // Trả màu chữ về mặc định của hệ thống
    void resetColor();
}

#endif // CONSOLE_UTILS_HPP
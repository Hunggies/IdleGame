#include "core/GameEngine.hpp"

int main() {
    GameEngine engine;
    
    // Khởi tạo engine hệ thống (nạp dữ liệu cấu hình)
    if (engine.initialize()) {
        // Bắt đầu kích hoạt Game Loop thời gian thực
        engine.run();
    }
    
    return 0;
}
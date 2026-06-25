cmake_minimum_required(VERSION 3.10)
project(IncrementalRPG VERSION 1.0 LANGUAGES CXX)

# Yêu cầu chuẩn biên dịch C++11 trở lên để hỗ trợ std::chrono và enum class
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Khai báo đường dẫn chứa các file header (.hpp)
include_directories(include)

# Tự động quét đệ quy toàn bộ file nguồn (.cpp) trong thư mục src/
file(GLOB_RECURSE GAME_SOURCES "src/*.cpp")

# Chỉ định tạo ra file thực thi từ mã nguồn quét được
add_executable(IncrementalGame ${GAME_SOURCES})
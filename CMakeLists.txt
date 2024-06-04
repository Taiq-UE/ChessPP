cmake_minimum_required(VERSION 3.27)
project(ChessCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

list(APPEND CMAKE_PREFIX_PATH "D:\\Programy\\Qt\\6.7.1\\mingw_64\\lib\\cmake")

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(ChessCpp main.cpp
        Piece.cpp
        Board.cpp
        chessboard.h
        ChessBoard.h
        ChessBoard.cpp)

target_link_libraries(ChessCpp Qt6::Widgets)
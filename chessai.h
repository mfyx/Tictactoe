#ifndef CHESSAI_H
#define CHESSAI_H

#include <QDebug>
#include <QPoint>

#define INT_MAX 2147483640

// 位置状态（空， 黑， 白）
#define C_NONE 0
#define C_BLACK 1
#define C_WHITE 2

enum gameMode {PLAYER, AI, AI2}; // 游戏模式（人人/人机）
enum gameStatus {UNDERWAY, FINISH}; // 在下/结束
enum gameTurn {T_BLACK, T_WHITE};   // 轮到谁下
enum gameResult {R_BLACK, R_WHITE, R_DRAW, R_DEADGAME}; //黑赢，白赢，胜负未定

struct DECISION {
    QPoint pos;
    int eval;
};

class ChessAi
{
public:
    int chesses[3][3];    // 棋盘
    DECISION decision;

public:
    ChessAi();
    gameResult evaluate(int board[3][3]);

    void copyBoard(int A[3][3], int B[3][3]);
    void reverseBoard(int A[3][3], int B[3][3]);
    //int analyse(int board[3][3], int depth, int alpha, int beta, bool r);
    //int analyse(int board[3][3], int depth, int alpha, int beta);
    int analyse(int board[3][3], int depth, int alpha, int beta, int C_ME);
};

#endif // CHESSAI_H

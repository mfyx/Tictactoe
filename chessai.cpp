#include "chessai.h"

ChessAi::ChessAi()
{

}

void ChessAi::copyBoard(int A[3][3], int B[3][3]) {
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++) {
            B[i][j] = A[i][j];
        }
}

void ChessAi::reverseBoard(int A[3][3], int B[3][3]) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(A[i][j] == C_NONE) B[i][j] = C_NONE;
            else if(A[i][j] == C_BLACK) B[i][j] = C_WHITE;
            else if(A[i][j] == C_WHITE) B[i][j] = C_BLACK;
            else qDebug()<<"reverseBoard棋盘非法";
        }
    }
}

gameResult ChessAi::evaluate(int board[3][3]) {
    for(int i = 0; i < 3; i++) {
        if(board[i][0]==board[i][1] && board[i][0]==board[i][2] && board[i][0]==C_BLACK)
            return R_BLACK;

        if(board[0][i]==board[1][i] && board[0][i]==board[2][i] && board[0][i]==C_BLACK)
            return R_BLACK;


        if(board[i][0]==board[i][1] && board[i][0]==board[i][2] && board[i][0]==C_WHITE)
            return R_WHITE;

        if(board[0][i]==board[1][i] && board[0][i]==board[2][i] && board[0][i]==C_WHITE)
            return R_WHITE;
    }

    if(board[0][0]==board[1][1] && board[0][0]==board[2][2] && board[0][0]==C_BLACK)
        return R_BLACK;
    if(board[0][2]==board[1][1] && board[0][2]==board[2][0] && board[0][2]==C_BLACK)
        return R_BLACK;

    if(board[0][0]==board[1][1] && board[0][0]==board[2][2] && board[0][0]==C_WHITE)
        return R_WHITE;
    if(board[0][2]==board[1][1] && board[0][2]==board[2][0] && board[0][2]==C_WHITE)
        return R_WHITE;

    //return R_DRAW;

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(board[i][j] == C_NONE)
                return R_DRAW;
        }
    }

    return R_DEADGAME;
}

int ChessAi::analyse(int board[3][3], int depth, int alpha, int beta, int C_ME = C_WHITE) {
    if(depth > 1) C_ME = C_NONE;

    gameResult EVAL = evaluate(board);
    if(EVAL == R_WHITE) {
        return 1;
    } else if(EVAL == R_DEADGAME) {
        return 0;
    } else if(EVAL == R_BLACK){
        return -1;
    } else if(depth > 9) {
        qDebug()<<"depth > 9";
        return 0;
    }

    if(depth % 2 == 0) {
        //int sameBoard[3][3];
        //copyBoard(board, sameBoard);
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++) {
                int sameBoard[3][3];
                copyBoard(board, sameBoard);
                if(sameBoard[i][j] == C_NONE) {
                    sameBoard[i][j] = C_WHITE;
                    int a = analyse(sameBoard, depth+1, alpha, beta);
                    if(a > alpha) {
                        alpha = a;
                        if(C_ME == C_WHITE && depth == 0) {
                            qDebug()<<"ai执白准备落子";
                            decision.pos.setX(i);         // row
                            decision.pos.setY(j);         // col
                            decision.eval = a;
                        }
                    }
                    if(beta <= alpha) break;
                }
            }
        return alpha;
    } else {
        //int rBoard[3][3];
        //reverseBoard(board, rBoard);
        //int sameBoard[3][3];
        //copyBoard(board, sameBoard);
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++) {
                int sameBoard[3][3];
                copyBoard(board, sameBoard);
                if(sameBoard[i][j] == C_NONE) {   // if(sameBoard == C_NONE) {
                    sameBoard[i][j] = C_BLACK;
                    int a = analyse(sameBoard, depth+1, alpha, beta);
                    if(a < beta) {
                        beta = a;
                        if(C_ME == C_BLACK && depth == 1) {
                            qDebug()<<"ai执黑准备落子";
                            decision.pos.setX(i);
                            decision.pos.setY(j);
                            decision.eval = a;
                        }
                    }
                    if(beta <= alpha) break;
                }
            }
        return beta;
    }
}

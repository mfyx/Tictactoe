#include "gamewidget.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    ui->lcd_black->hide();
    ui->lcd_deadgame->hide();
    ui->lcd_white->hide();
    ui->label->hide();
    //ui—>lcd_white->hide();
    ui->label_2->hide();
    ui->label_3->hide();

    this->setStyleSheet("QLabel, QPushButton { color : rgba(0, 0, 0, 230); }");

    for(int i = 0; i < 3; i++) {   // 棋盘左上角点为(20,20)，每格间距为30
        for(int  j = 0; j < 3; j++) {
            chessboard[i][j].setX(80 + 150*i);
            chessboard[i][j].setY(80 + 150*j);
        }
    }

    connect(this->ui->btn_return, SIGNAL(clicked(bool)), this, SLOT(returnPush()));

    setMouseTracking(true);
    //initialzeGame();
}

GameWidget::~GameWidget() {
    delete ui;
}

void GameWidget::returnPush() {
    emit returnSignal();
    this->hide();
    //initialzeGame();
}

void GameWidget::initialzeGame() {
    qDebug()<<"游戏重新初始化";
    if(mode == PLAYER) qDebug()<<"玩家模式";
    else qDebug()<<"AI模式";

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            ai.chesses[i][j] = C_NONE;

    status = UNDERWAY;  // 游戏正在进行
    turn = T_BLACK;     // 轮到黑方
    cursorRow = -1;     // 光标默认在窗口外
    cursorCol = -1;

    if(mode == AI2)
        chessOneByAi();
}

bool GameWidget::isDeadGame() {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(ai.chesses[i][j] == C_NONE)
                return false;
        }
    }
    return true;
}

bool GameWidget::isLegalMove(int x, int y) {
    if(ai.chesses[x][y] == C_NONE) return true;
    else return false;
}

void GameWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor("#E7CDA4")));
    painter.drawRect(QRect{QPoint(0, 0),QPoint(692, 468)});

    // 画棋盘
    painter.setPen(Qt::black);
    for(int i = 0; i < 3; i++) {
        painter.drawLine(chessboard[0][i], chessboard[3-1][i]);
        painter.drawLine(chessboard[i][0], chessboard[i][3-1]);
    }

    // 画鼠标光标
    if(turn == T_BLACK) painter.setBrush(Qt::black);
    else painter.setBrush(Qt::white);
    if(cursorRow != -1 && cursorCol != -1) {
        const int size = 26;
        QRect rec(chessboard[cursorCol][cursorRow].x()-size/2, chessboard[cursorCol][cursorRow].y()-size/2, size, size);
        painter.drawRect(rec);
    }

    // 画棋子
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(ai.chesses[i][j] != C_NONE) {
                if(ai.chesses[i][j] == C_BLACK) painter.setBrush(Qt::black);
                else painter.setBrush(Qt::white);
                const int size = 50;
                painter.drawEllipse(chessboard[j][i].x() - size/2, chessboard[j][i].y() - size/2, size, size);
            }
        }
    }
}

void GameWidget::mouseMoveEvent(QMouseEvent *event) {
    if(event->x()>=5 && event->x()<=455 && event->y()>=5 && event->y()<=455) {
        // 5=20-30/2, 455=20+14*30+30/2
        // 5=80-150/2 455=80+2*150+150/2
        setCursor(Qt::BlankCursor);     // 设置为空白光标
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                float x=event->x(), y=event->y();
                // 判断鼠标落在那一个点的正方形范围内
                if( (x >= chessboard[i][j].x()-150/2) && (x < chessboard[i][j].x()+150/2) &&
                    (y >= chessboard[i][j].y()-150/2) && (y < chessboard[i][j].y()+150/2) ) {
                    cursorRow = j;
                    cursorCol = i;
                    if(ai.chesses[cursorRow][cursorCol] != C_NONE)
                        setCursor(Qt::ForbiddenCursor);

                }
            }
        }
    }
    else setCursor(Qt::ArrowCursor);

    update();
}

void GameWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(mode == PLAYER) {
        if(chessOneByPlayer()) {
            if(status == FINISH) initialzeGame();
        }
    } else {
        if(chessOneByPlayer()) {
            if(status == UNDERWAY) {
                chessOneByAi();
                if(status == FINISH) initialzeGame();
            }
            else initialzeGame();
        }
    }
}

void GameWidget::chessOneByAi() {
    qDebug()<<"ai chess";

    if(mode == AI)
        ai.analyse(ai.chesses, 0, -INT_MAX, INT_MAX, C_WHITE);
    else if(mode == AI2)
        ai.analyse(ai.chesses, 1, -INT_MAX, INT_MAX, C_BLACK);
    else
        qDebug()<<"chessOneByAi 无法识别当前模式";

    QPoint p = ai.decision.pos;
    qDebug()<<"ai 落子:"<<p.x()<<", "<<p.y();

    if(isLegalMove(p.x(), p.y())) {
        oneChessMove(p.x(), p.y());
    } else {
        qDebug()<<"ai落子不合法！";
    }
}

bool GameWidget::chessOneByPlayer() {
    if(ai.chesses[cursorRow][cursorCol] == C_NONE) {
        qDebug()<<"player chess";
        oneChessMove(cursorRow, cursorCol);

        return true;
    }
    return false;
}

void GameWidget::oneChessMove(int row, int col) {
    qDebug()<<row<<","<<col;

    if(turn == T_BLACK) {
        turn = T_WHITE;
        ai.chesses[row][col] = C_BLACK;
    } else {
        turn = T_BLACK;
        ai.chesses[row][col] = C_WHITE;
    }

    gameResult result = ai.evaluate(ai.chesses);

    QMessageBox msg;
    //meg.setIcon(QMessageBox::Ctitical);
    msg.setStandardButtons(QMessageBox::Yes);
    if(result == R_BLACK || result == R_WHITE) {
        status = FINISH;
        if(result == R_BLACK) {
            qDebug()<<"黑棋赢";
            msg.setText("黑棋赢");
            score_black++;
        } else if(result == R_WHITE) {
            qDebug()<<"白棋赢";
            msg.setText("白棋赢");
            score_white++;
        }
        msg.exec();
        ui->lcd_black->display(score_black);
        ui->lcd_white->display(score_white);
    } else if(isDeadGame()) {
        status = FINISH;
        msg.setText("平局");
        score_deadgame++;
        msg.exec();
        ui->lcd_deadgame->display(score_deadgame);
    }
    update();
}





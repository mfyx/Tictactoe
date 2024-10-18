#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setStyleSheet("QLabel, QPushButton { color : rgba(0, 0, 0, 180);  font-size: 25px; }");

    game = new GameWidget();
    game->hide();

    connect(this->ui->btn_player, SIGNAL(clicked(bool)), this, SLOT(playerButtonPush()));
    connect(this->ui->btn_ai, SIGNAL(clicked(bool)), this, SLOT(aiButtonPush()));
    connect(game, &GameWidget::returnSignal, this, &Widget::show);

    connect(this->ui->btn_ai_2, SIGNAL(clicked(bool)), this, SLOT(ai2ButtonPush()));
}

Widget::~Widget() {
    delete ui;
}

void Widget::playerButtonPush() {
    this->hide();
    game->show();
    game->setGameMode(PLAYER);

    //game->chessOneByAi(true);
}

void Widget::aiButtonPush() {
    this->hide();
    game->show();
    game->setGameMode(AI);

    game->initialzeGame();
}

void Widget::ai2ButtonPush() {
    this->hide();
    game->show();
    game->setGameMode(AI2);

    game->initialzeGame();

    //game->chessOneByAi();
}

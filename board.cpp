/**
 * @Name: Fanchen Kong
 * @UID: 404506008
 * @This is a cpp file for class board. It take controls all the changes on the board.
*/

#include "board.h"
#include "ui_board.h"
#include <QGraphicsItem>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QMessageBox>
#include <QGridLayout>
#include <QPixmap>
#include <QVector>
#include <QKeyEvent>
#include <QtGlobal>
#include <cstdlib>
#include <QObject>
#include <QDebug>
#include <QVector>

/**
 * @brief This is a constructor for Board
 * @param parent connects the board to the mainwindow
 * @param level_diff is the level the board will set up
*/
Board::Board(QWidget *parent, Level level_diff) :
    QWidget(parent),
    ui(new Ui::Board),level_difficulty(level_diff)
{

    QHBoxLayout* top = new QHBoxLayout;
    top->addSpacing(50);
    //set up the timer
    mylabel *time = new mylabel(300); //300 seconds
    //time->setFixedWidth(300);
    top -> addWidget(time,0);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),time,SLOT(showTime()));
    timer->start(1000);

    //set up the label
    if(level_diff == easy ){
        QLabel* diff_label = new QLabel("Difficulty: easy");
        top->addWidget(diff_label);
    }
    else if(level_diff == medium){
        QLabel* diff_label = new QLabel("Difficulty: medium");
        top->addWidget(diff_label);
    }
    else if(level_diff == hard){
        QLabel* diff_label = new QLabel("Difficulty: hard");
        top->addWidget(diff_label);
    }
    else if(level_diff == surprise){
        QLabel* diff_label = new QLabel("Difficulty: surprise");
        top->addWidget(diff_label);
    }
    //diff_label->setFixedWidth(150);
    //set up the label
    undo = new QPushButton("Undo");
    top->addWidget(undo);
    QObject::connect(undo,SIGNAL(clicked()),this,SLOT(undo_step()));

    back = new QPushButton("Menu");
    top->addWidget(back);
    QObject::connect(back,SIGNAL(clicked()),parent,SLOT(back_menu()));
    //QObject::connect(back,SIGNAL(clicked()),timer,SLOT(stop()));

    restart = new QPushButton("Restart");
    top->addWidget(restart);
    QObject::connect(restart,SIGNAL(clicked()),parent,SLOT(restart_game()));

    QVBoxLayout* vertical = new QVBoxLayout;
    vertical->addLayout(top);

    //set up the land and secret land
    tank_land = new QGridLayout();
    tank_land_label = new QLabel*[196];

    //create a 14*14 secret land
    secret_land.resize(14);
    for(int i = 0;i<14;++i){
        secret_land[i].resize(14);
    }

    tank_land->setContentsMargins(0,0,0,0);
    tank_land->setAlignment(Qt::AlignCenter);


    QPixmap* stone = new QPixmap("://stone.png");
    stone1 = new QPixmap(stone->scaled(40,40,Qt::KeepAspectRatio));


    QPixmap* wall = new QPixmap("://box.png");
    wall1 = new QPixmap(wall->scaled(40,40,Qt::KeepAspectRatio));
    QPixmap* wall_cover = new QPixmap("://wall_cover.png");
    wall1_cover = new QPixmap(wall_cover->scaled(40,40,Qt::KeepAspectRatio));

    QPixmap* star = new QPixmap("://star.png");
    star1 = new QPixmap(star->scaled(40,40,Qt::KeepAspectRatio));

    //set up the four directions of the tank
    QPixmap *player_1 = new QPixmap("://player_up.png");
    player_one_up = new QPixmap(player_1->scaled(40,40,Qt::KeepAspectRatio));
    QPixmap *player_2 = new QPixmap("://player_down.png");
    player_one_down = new QPixmap(player_2->scaled(40,40,Qt::KeepAspectRatio));
    QPixmap *player_3 = new QPixmap("://player_right.png");
    player_one_right = new QPixmap(player_3->scaled(40,40,Qt::KeepAspectRatio));
    QPixmap *player_4 = new QPixmap("://player_left.png");
    player_one_left = new QPixmap(player_4->scaled(40,40,Qt::KeepAspectRatio));

    //set up the star
    if (level_diff == easy){
        tank_number = 4;
        tanks = new QPoint*[4];
        goal = new QPoint*[4];
        goal_cover.resize(4);
        for(int i =0;i<tank_number;i++)
            goal_cover[i] = false;
        setup_easy_board();
    }
    else if(level_diff == medium){
        tank_number = 5;
        tanks = new QPoint*[5];
        goal = new QPoint*[5];
        goal_cover.resize(5);
        for(int i =0;i<tank_number;i++)
            goal_cover[i] = false;
        setup_medium_board();
    }
    else if(level_diff == hard){
        tank_number = 6;
        tanks = new QPoint*[6];
        goal = new QPoint*[6];
        goal_cover.resize(6);
        for(int i =0;i<tank_number;i++)
            goal_cover[i] = false;
        setup_hard_board();
    }
    else if(level_diff == surprise){
        tank_number = 7;
        tanks = new QPoint*[7];
        goal = new QPoint*[7];
        goal_cover.resize(7);
        for(int i =0;i<6;i++)
            goal_cover[i] = false;
        goal_cover[6] = true;
        setup_surprise_board();

    }


    //std::cout << time->get_time();
    if(level_diff != surprise ){//if it is the last diff level
        connect(this,SIGNAL(game_over()),parent,SLOT(next_diff()));
        connect(this, SIGNAL(gameover()),timer,SLOT(stop()));
    }
    else if (level_diff == surprise){//if it is not the last diff level
        connect(this, SIGNAL(game_over()), parent, SLOT(last_level()));
        connect(this, SIGNAL(gameover()),timer,SLOT(stop()));
        //timer->stop();
    }
    //connect(this, SIGNAL(game_over()),timer,SLOT(stop()));
    connect(time, SIGNAL(noTime()), parent, SLOT(out_of_time()));

    vertical->addLayout(tank_land);
    this->setLayout(vertical);

}
/**
 * @brief this is the desctrutor of the board
 */
Board::~Board()
{
    delete ui;
}

/**
 * @brief This how to set up the board for easy level
 */
void Board::setup_easy_board(){
    for(int i = 0; i <47 ;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        //assign a number to the position;
        secret_land[i%14][i/14] = 0;
    }
    for(int i =47; i < 50;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i = 50; i < 61 ;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }

    for(int i =61; i < 62;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i = 62; i < 63 ;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*star1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 9;
        goal[0] = new QPoint(i%14,i/14);
    }
    for(int i =63; i < 64;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i = 64; i < 75 ;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =75; i < 76;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =76; i < 77;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*wall1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 1;
        tanks[0] = new QPoint(i%14,i/14);
    }
    for(int i =77; i < 81;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =81; i < 87;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =87; i < 90;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =90; i < 92;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =92; i < 93;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*wall1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 1;
        tanks[1] = new QPoint(i%14,i/14);
    }
    for(int i =93; i < 94;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*star1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 9;
        goal[1] = new QPoint(i%14,i/14);
    }
    for(int i =94; i < 95;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =95; i < 101;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =101; i < 102;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =102; i < 103;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*star1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 9;
        goal[2] = new QPoint(i%14,i/14);
    }
    for(int i =103; i < 104;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =104; i < 106;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*wall1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 1;
        tanks[i-102] = new QPoint(i%14,i/14);
    }
    for(int i =106; i < 109;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =109; i < 115;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =115; i < 119;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =119; i < 120;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =120; i < 121;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =121; i < 132;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =132; i < 133;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =133; i < 134;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*star1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 9;
        goal[3] = new QPoint(i%14,i/14);
    }
    for(int i =134; i < 135;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =135; i < 146;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =146; i < 149;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =149; i < 196;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }

    for(int i =0; i < 196;i++){
        tank_land_label[i]->setFixedHeight(40);
        tank_land_label[i]->setFixedWidth(40);
    }

    players = new QPoint*[1];
    players[0] = new QPoint(7,6);
    tank_land_label[91]->setPixmap(*player_one_up);
    secret_land[7][6] = 0;


}

/**
 * @brief This is how to set up the board for medium level.
 */
void Board::setup_medium_board(){
    for(int i = 0; i <36 ;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        //assign a number to the position;
        secret_land[i%14][i/14] = 0;
    }
    for(int i =36; i < 39;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i = 39; i < 50 ;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }

    for(int i =50; i < 51;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i = 51; i < 52 ;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*star1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 9;
        goal[0] = new QPoint(i%14,i/14);
    }
    for(int i =52; i < 53;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i = 53; i < 60 ;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =60; i < 65;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =65; i < 66;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*star1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 9;
        goal[1] = new QPoint(i%14,i/14);
    }
    for(int i =66; i < 67;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =67; i < 73;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =73; i < 75;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =75; i < 76;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =76; i < 77;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*wall1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 1;
        tanks[0] = new QPoint(i%14,i/14);
    }
    for(int i =77; i < 79;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =79; i < 80;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*star1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 9;
        goal[2] = new QPoint(i%14,i/14);
    }
    for(int i =80; i < 81;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =81; i < 87;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =87; i < 88;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =88; i < 91;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =91; i < 93;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*wall1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 1;
        tanks[i-90] = new QPoint(i%14,i/14);
    }
    for(int i =93; i < 94;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*star1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 9;
        goal[3] = new QPoint(i%14,i/14);
    }
    for(int i =94; i < 95;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =95; i < 101;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =101; i < 102;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =102; i < 104;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =104; i < 105;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*wall1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 1;
        tanks[3] = new QPoint(i%14,i/14);
    }
    for(int i =105; i < 106;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =106; i < 107;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =107; i < 108;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*star1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 9;
        goal[4] = new QPoint(i%14,i/14);
    }
    for(int i =108; i < 109;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =109; i < 115;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =115; i < 118;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =118; i < 119;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =119; i < 121;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =121; i < 122;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =122; i < 123;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =123; i < 129;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =129; i < 130;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =130; i < 132;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =132; i < 133;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*wall1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 1;
        tanks[4] = new QPoint(i%14,i/14);
    }
    for(int i =133; i < 136;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =136; i < 137;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =137; i < 143;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =143; i < 144;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =144; i < 148;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =148; i < 151;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =151; i < 157;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }
    for(int i =157; i < 163;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 5;
    }
    for(int i =163; i < 196;i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        secret_land[i%14][i/14] = 0;
    }

    for(int i =0; i < 196;i++){
        tank_land_label[i]->setFixedHeight(40);
        tank_land_label[i]->setFixedWidth(40);
    }


    players = new QPoint*[1];
    players[0] = new QPoint(4,10);
    tank_land_label[144]->setPixmap(*player_one_up);
    secret_land[4][10] = 0;



}

/**
 * @brief This is how to set up the board for surprise level.
 */
void Board::setup_surprise_board(){
    for(int i = 0; i < 196; i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        //assign a number to the position;
        secret_land[i%14][i/14] = 0;
    }

    QVector<int> stone_pos = {18,19,20,21,22,23,31,32,37,45,49,51,52,53,58,59,
                              61,67,72,75,76,77,79,81,86,95,100,103,105,107,108,
                              109,114,115,117,121,129,134,135,143,144,145,148,159,160,161,162
                             };
    for(int i = 0;i < stone_pos.length();i++){
        //tank_land_label[stone_pos[i]] = new QLabel;
        tank_land_label[stone_pos[i]]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[stone_pos[i]],stone_pos[i]%14,stone_pos[i]/14);
        tank_land->setSpacing(0);
        secret_land[stone_pos[i]%14][stone_pos[i]/14] = 5;

    }

    QVector<int> box = {48,50,65,88,104,116};
    for(int i = 0; i < 6; i++){
       //tank_land_label[i] = new QLabel;
       tank_land_label[box[i]]->setPixmap(*wall1);
       tank_land->addWidget(tank_land_label[box[i]],box[i]%14,box[i]/14);
       tank_land->setSpacing(0);
       secret_land[box[i]%14][box[i]/14] = 1;
       tanks[i] = new QPoint(box[i]%14,box[i]/14);
    }
    secret_land[91%14][91/14] = 4;
    tanks[6] = new QPoint(91%14,91/14);
    tank_land_label[91]->setPixmap(*wall1_cover);
    tank_land->addWidget(tank_land_label[91],91%14,91/14);
    goal[6] = new QPoint(91%14,91/14);

    QVector<int> star_position = {62,63,64,78,90,92};
    for(int i =0; i<6;i++){
        //tank_land_label[star_position[i]] = new QLabel;
        tank_land_label[star_position[i]]->setPixmap(*star1);
        tank_land->addWidget(tank_land_label[star_position[i]],star_position[i]%14,star_position[i]/14);
        tank_land->setSpacing(0);
        secret_land[star_position[i]%14][star_position[i]/14] = 9;
        goal[i] = new QPoint(star_position[i]%14,star_position[i]/14);

    }

    for(int i =0; i < 196;i++){
        tank_land_label[i]->setFixedHeight(40);
        tank_land_label[i]->setFixedWidth(40);
    }

    players = new QPoint*[1];
    players[0] = new QPoint(4,5);
    tank_land_label[74]->setPixmap(*player_one_up);
    secret_land[4][5] = 0;
}

/**
 * @brief This is how to set up the board for hard level.
 */
void Board::setup_hard_board(){
    for(int i = 0; i < 196; i++){
        tank_land_label[i] = new QLabel;
        tank_land_label[i]->setStyleSheet("QLabel { background-color : black; }");
        tank_land->addWidget(tank_land_label[i],i%14,i/14);
        tank_land->setSpacing(0);
        //assign a number to the position;
        secret_land[i%14][i/14] = 0;
    }

    QVector<int> stone_pos = {34,35,36,37,38,48,52,62,66,73,74,74,75,76,79,80,87,94,101,
                             105,107,108,115,122,129,130,131,132,133,135,136,147,148,149};
    for(int i = 0;i < stone_pos.length();i++){
        //tank_land_label[stone_pos[i]] = new QLabel;
        tank_land_label[stone_pos[i]]->setPixmap(*stone1);
        tank_land->addWidget(tank_land_label[stone_pos[i]],stone_pos[i]%14,stone_pos[i]/14);
        tank_land->setSpacing(0);
        secret_land[stone_pos[i]%14][stone_pos[i]/14] = 5;

    }

    QVector<int> box = {64,92,89,103,117,119};
    for(int i = 0; i < 6; i++){
       //tank_land_label[i] = new QLabel;
       tank_land_label[box[i]]->setPixmap(*wall1);
       tank_land->addWidget(tank_land_label[box[i]],box[i]%14,box[i]/14);
       tank_land->setSpacing(0);
       secret_land[box[i]%14][box[i]/14] = 1;
       tanks[i] = new QPoint(box[i]%14,box[i]/14);
    }
    QVector<int> star_position = {78,91,93,106,121,134};
    for(int i =0; i<6;i++){
        //tank_land_label[star_position[i]] = new QLabel;
        tank_land_label[star_position[i]]->setPixmap(*star1);
        tank_land->addWidget(tank_land_label[star_position[i]],star_position[i]%14,star_position[i]/14);
        tank_land->setSpacing(0);
        secret_land[star_position[i]%14][star_position[i]/14] = 9;
        goal[i] = new QPoint(star_position[i]%14,star_position[i]/14);

    }

    for(int i =0; i < 196;i++){
        tank_land_label[i]->setFixedHeight(40);
        tank_land_label[i]->setFixedWidth(40);
    }

    players = new QPoint*[1];
    players[0] = new QPoint(4,7);
    tank_land_label[102]->setPixmap(*player_one_up);
    secret_land[4][7] = 0;
}
/**
 * @brief Board::updateplayer_1, update the player from the previous position to next position.
 * @param before_x is the x axis of the previous position for the player
 * @param before_y is the y axis of the previous position for the player
 * @param after_x is the x axis of the after position for the player
 * @param after_y is the y axis of the after position for the player
 */
void Board::updateplayer_1(int before_x,int before_y,int after_x,int after_y){
    if(secret_land[before_x][before_y]==0){
        tank_land_label[before_y*14+before_x]->clear();
        tank_land_label[before_y*14+before_x]->setStyleSheet("QLabel { background-color : black; }");
        if(secret_land[after_x][after_y] == 0){
            players[0]->setX(after_x);
            players[0]->setY(after_y);
        }
        else if(secret_land[after_x][after_y] == 9){
            //find which goal;
            int number;
            for(number = 0; number <tank_number;number++){
                if(goal[number]->rx() == after_x && goal[number]->ry() == after_y)
                    break;
            }
            goal_cover[number] = true;
            players[0]->setX(after_x);
            players[0]->setY(after_y);
            tank_land_label[after_x+after_y*14]->clear();
        }

    }
    else if(secret_land[before_x][before_y] == 9){
        //find which goal is the position before;
        int number;
        for(number = 0; number <tank_number;number++){
            if(goal[number]->rx() == before_x && goal[number]->ry() == before_y)
                break;
        }
        goal_cover[number] = false;
        tank_land_label[before_y*14+before_x]->clear();
        if(secret_land[after_x][after_y] == 0){
            players[0]->setX(after_x);
            players[0]->setY(after_y);
            secret_land[after_x][after_y] = 0;
        }
        else if(secret_land[after_x][after_y] == 9){
            //find which goal;
            int number;
            for(number = 0; number <tank_number;number++){
                if(goal[number]->rx() == after_x && goal[number]->ry() == after_y)
                    break;
            }
            goal_cover[number] = true;
            players[0]->setX(after_x);
            players[0]->setY(after_y);
            tank_land_label[after_x+after_y*14]->clear();
        }

    }
    /*
    for(int i = 0;i<14;i++){
        for(int j = 0;j<14;j++)
            std::cout << secret_land[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
    */

}

/**
 * @brief Board::updatebox is the function to update the box
 * @param before_x is the x axis of the previous position for the box
 * @param before_y is the y axis of the previous position for the box
 * @param after_x is the x axis of the after position for the box
 * @param after_y is the y axis of the after position for the box
 */
void Board::updatebox(int before_x,int before_y,int after_x,int after_y){
    /*
    for(int i = 0;i<14;i++){
        for(int j = 0;j<14;j++)
            std::cout << secret_land[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
    */
    tank_land_label[before_y*14+before_x]->clear();
    tank_land_label[after_y*14+after_x]->clear();
    //find the box that it will move
    int number;
    for(number = 0; number <tank_number;number++){
        if(tanks[number]->rx() == before_x && tanks[number]->ry() == before_y)
            break;
    }
    //if the before position is a star
    int i =0;
    for(i = 0; i <tank_number;i++){
        if(goal[i]->rx() == before_x && goal[i]->ry() == before_y)
            break;
    }
    //if yes, to make the before position a star
    if(i < tank_number){
        goal_cover[i] = false;
        secret_land[before_x][before_y] = 9;
    }
    //if no, set black sheet
    else{
        tank_land_label[before_x+before_y*14]->setStyleSheet("QLabel { background-color : black; }");
        secret_land[before_x][before_y] = 0;
    }
    //check the position after is a star
    int j =0;
    for(j = 0; j <tank_number;j++){
        if(goal[j]->rx() == after_x && goal[j]->ry() == after_y)
            break;
    }
    //if yes, set the box position to that position, and update the photo.
    if(j < tank_number){
        goal_cover[j] = true;
        tanks[number]->setX(after_x);
        tanks[number]->setY(after_y);
        secret_land[after_x][after_y] = 4;//change to 4, if it after position is a star
    }
    else{
        tanks[number]->setX(after_x);
        tanks[number]->setY(after_y);
        secret_land[after_x][after_y] = 1;

    }
    /*
    for(int i = 0;i<14;i++){
        for(int j = 0;j<14;j++)
            std::cout << secret_land[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
    */
}

/**
 * @brief Board::keyPressEvent is the function to update after the key pressed
 * @param event is the default key press event.
 */
void Board::keyPressEvent(QKeyEvent *event){

    int x = players[0]->rx();
    int y = players[0]->ry();
    int check_move = 0;
    switch (event->key()) {
    case Qt::Key_Left:
        if(y != 0 && (secret_land[x][y-1] == 0 || secret_land[x][y-1] == 9)){
            updateplayer_1(x,y,x,y-1);
        }
        //if that direction is a box
        if(y != 0 && (secret_land[x][y-1] == 1 || secret_land[x][y-1]==4)){
            if((y-1)!=0 && (secret_land[x][y-2] == 0 || secret_land[x][y-2] == 9)){
                updatebox(x,y-1,x,y-2);
                check_move = 1;
                updateplayer_1(x,y,x,y-1);
                update_after_shoot();
            }
        }

        direction.push_back("Left");

        break;
    case Qt::Key_Right:
        if(y != 13 && (secret_land[x][y+1] == 0 || secret_land[x][y+1] == 9)){
            updateplayer_1(x,y,x,y+1);
        }
        //if that direction is a box
        if(y != 13 && (secret_land[x][y+1] == 1|| secret_land[x][y+1]==4)){
            if((y-1)!=0 && (secret_land[x][y+2] == 0 || secret_land[x][y+2] == 9)){
                updatebox(x,y+1,x,y+2);
                check_move = 1;
                updateplayer_1(x,y,x,y+1);
                update_after_shoot();
            }
        }
        direction.push_back("Right");
        break;
    case Qt::Key_Up:
        if(x != 0 && (secret_land[x-1][y] == 0 || secret_land[x-1][y] == 9)){
            updateplayer_1(x,y,x-1,y);
        }
        if(x != 0 && (secret_land[x-1][y] == 1 || secret_land[x-1][y]==4)){
            if((x-1)!=0 && (secret_land[x-2][y] == 0 || secret_land[x-2][y] == 9 )){
                updatebox(x-1,y,x-2,y);
                check_move = 1;
                updateplayer_1(x,y,x-1,y);
                update_after_shoot();
            }
        }
        direction.push_back("Up");
        break;
    case Qt::Key_Down:
        if(x != 13 && (secret_land[x+1][y] == 0 || secret_land[x+1][y] == 9)){
            updateplayer_1(x,y,x+1,y);
        }
        if(x != 13 && (secret_land[x+1][y] == 1 || secret_land[x+1][y]==4)){
            if((x+1)!=0 && (secret_land[x+2][y] == 0 || secret_land[x+2][y] == 9)){
                updatebox(x+1,y,x+2,y);
                check_move = 1;
                updateplayer_1(x,y,x+1,y);
                update_after_shoot();
            }
        }
        direction.push_back("Down");
        break;
    default:
        QWidget::keyPressEvent(event);
    }
    move.push_back(check_move);
    return;
}

/**
 * @brief Board::paintEvent is the redefined function for the paintevent
 * @param e is the default paint event.
 */
void Board::paintEvent(QPaintEvent *e) {


    int x = players[0]->rx();
    int y = players[0]->ry();

    if(direction.length() > 0){
        QString last_direction = direction[direction.length()-1];
        if(last_direction == "Up")
            tank_land_label[y*14+x]->setPixmap(*player_one_up);
        else if(last_direction == "Down")
            tank_land_label[y*14+x]->setPixmap(*player_one_down);
        else if(last_direction == "Left")
            tank_land_label[y*14+x]->setPixmap(*player_one_left);
        else if(last_direction == "Right")
            tank_land_label[y*14+x]->setPixmap(*player_one_right);
     }
    else
        tank_land_label[y*14+x]->setPixmap(*player_one_up);

    for(int i=0;i<tank_number;i++) {
        int px = tanks[i]->rx();
        int py = tanks[i]->ry();
        //check the tank is on the star position, if yes, change the box color.
        if(check_cover(px,py))
            tank_land_label[py*14+px]->setPixmap(*wall1_cover);
        else
            tank_land_label[py*14+px]->setPixmap(*wall1);
    }
    for(int i =0;i<tank_number;i++){
        int px = goal[i]->rx();
        int py = goal[i]->ry();
        if(goal_cover[i] == false)
            tank_land_label[py*14+px]->setPixmap(*star1);
    }

}
/**
 * @brief Board::check_cover is a function to check whether the tank cover any star position
 * @param px is the position of the tank
 * @param py is the position of the tank
 * @return true if this tank cover some star position, otherwise return false
 */
bool Board::check_cover(int px, int py){
    for(int i = 0;i<tank_number;i++){
        int x = goal[i]->rx();
        int y = goal[i]->ry();
        if(px == x && py == y)
            return true;
    }
    return false;

}
/**
 * @brief Board::showEvent is a redefined function for the showevent.
 * @param e is the default show event.
 */
void Board::showEvent(QShowEvent *e) {

    this->activateWindow();
    this->setFocus();
    QWidget::showEvent(e);
}

/**
 * @brief Board::update_after_shoot is a function to check if all the stars are covered after each move.
 */
void Board::update_after_shoot(){
    int position_check = 0;
    for(int i =0;i<tank_number;i++){
        int goal_x = goal[i]->rx();
        int goal_y = goal[i]->ry();
        //std::cout << goal_x << goal_y<<secret_land[goal_x][goal_y]<< std::endl;
        if (secret_land[goal_x][goal_y]==4)
            position_check++;
    }
    //std::cout << position_check << "Tank number" << tank_number<<std::endl;
    if (position_check == tank_number){
        emit gameover();
        this->game_over();
    }
}

/**
 * @brief mylabel::showTime is a function to show time.
 */
void mylabel::showTime(){
    int min = sec/60;
    QString minString = QString::number(min);
    QString secString;
    if(sec%60 == 0)
        secString = "00";
    else if(sec%60 > 9)
        secString = QString::number(sec%60);
    else
        secString = "0"+QString::number(sec%60);

    this->setText("Time Remaining: " + minString + ":" + secString);
    if(sec > 0)
        sec--;
    else
        emit noTime();
    std::cout << sec;
}

/**
 * @brief Board::undo_step is a function about how to undo one step if the player click the undo
 */
void Board::undo_step(){
    if(undo_times != 0 && direction.size() > 0){
        int x = players[0]->rx();
        int y = players[0]->ry();
        QString last_step = direction[direction.size()-1];
        int check_move = move[move.size()-1];
        direction.pop_back();
        move.pop_back();
        undo_times--;

        if(last_step == "Right"){
            updateplayer_1(x,y,x,y-1);//move the player to the left
            if(check_move == 1)//then update the box, move to the place that the player have
                updatebox(x,y+1,x,y);
            update_after_shoot();
            }
        else if(last_step == "Left"){
            updateplayer_1(x,y,x,y+1);
            if(check_move == 1)
                updatebox(x,y-1,x,y);
             update_after_shoot();
            }
        else if(last_step == "Down"){
            updateplayer_1(x,y,x-1,y);
            if(check_move ==1)
                updatebox(x+1,y,x,y);
             update_after_shoot();
            }
        else if(last_step == "Up"){
            updateplayer_1(x,y,x+1,y);
            if(check_move == 1)
                updatebox(x-1,y,x,y);
             update_after_shoot();
            }
        }
    else if(undo_times == 0 && direction.size() > 0){ //show up a window say there is no more chance to undo
        QMessageBox msgBox;
        msgBox.setText("There is no more chance to undo.");
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int result = msgBox.exec();
        switch (result) {
            case QMessageBox::Yes:
                break;
            default:
                break;
            }

    }

}

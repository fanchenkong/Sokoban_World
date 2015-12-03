/**
 * @Name: Fanchen Kong
 * @UID: 404506008
 * @This is a header file for board.h
*/
#ifndef BOARD_H
#define BOARD_H

#include <cstdlib>
#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QVector>
#include <QPoint>
#include <random>
#include <chrono>
#include <QTimer>
#include <iostream>
#include <QGridLayout>
#include <QPushButton>
#include <QString>
#include <QTransform>
//it defines the level
enum Level{easy,medium,hard,surprise};

namespace Ui {
class Board;
}
/**
 * @brief The Board class designs the game board and take control of the game running.
 */
class Board : public QWidget
{
    Q_OBJECT

public slots:
       void undo_step();

signals:
       void game_over();
       void gameover();
       void time_end();

public:
    explicit Board(QWidget *parent = 0, Level level_diff = easy);
    ~Board();
    void keyPressEvent(QKeyEvent *event);
    void updateplayer_1(int before_x,int before_y,int after_x,int after_y);
    void updatebox(int before_x,int before_y, int after_x, int after_y);
    void showEvent(QShowEvent *e);
    void paintEvent(QPaintEvent *e);

    void setup_easy_board();
    void setup_medium_board();
    void setup_hard_board();
    void setup_surprise_board();
    //helper function to check whether the star is covered by the box
    bool check_cover(int px, int py);
    //bool valid_space(int x, int y);
    //void shoot(int x, int y, QString direction);
    void update_after_shoot();
    //void bullet_update();

private:
    Ui::Board *ui;
    QPushButton* back;
    QPushButton* undo;
    QPushButton* restart;
    Level level_difficulty;
    int tank_number;

    QLabel* diff_label;
    QPixmap* stone1;
    QPixmap* wall1;
    QPixmap* wall1_cover;
    QPixmap* star1;

    QPixmap* player_one_up;
    QPixmap* player_one_down;
    QPixmap* player_one_left;
    QPixmap* player_one_right;

    QGridLayout *tank_land;
    QLabel** tank_land_label;
    QVector< QVector< int > > secret_land;//keep track the actual position for different objects
    QVector<bool> goal_cover;

    QVector<QString> direction;//Use QString to keep track with the direction
    QVector<int> move;//use move to check whether the box move in each step. 1 move, 0 not move
    int undo_times = 3;

     //set up the qpoints for the raod and wall
    QPoint** tanks;
    QPoint** players;
    QPoint** goal;

    QString *player1_direction;

    int shoot_x;
    int shoot_y;

    QTimer* timer;
};

/**
 * @brief The mylabel class defines the label for show time.
 */
class mylabel:public QLabel{
    Q_OBJECT
public:
    mylabel(int a){sec = a;};
    int get_time(){return sec;};
public slots:
    void showTime();
signals:
     void noTime();
private:
    int sec;
};
#endif // BOARD_H

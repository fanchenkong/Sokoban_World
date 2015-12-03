/**
 * @Name: Fanchen Kong
 * @UID: 404506008
 * @This is a header file for mainwindow.
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include <QPushButton>
#include <board.h>


namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class is the main menu of this game.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    void display();
    void closeEvent(QCloseEvent *event);
    ~MainWindow();

public slots:
    void start_game();
    void game_over();
    void next_diff();
    void last_level();
    void out_of_time();
    void back_menu();
    void restart_game();
    void show_instruction();
private:
    int diff;
    Ui::MainWindow *ui;
    Board *board;
    QRadioButton* Easy_radioButton;
    QRadioButton* Medium_radioButton;
    QRadioButton* Hard_radioButton;
    QRadioButton* Suprise_radioButton;
    QPushButton* descr;
};

#endif // MAINWINDOW_H

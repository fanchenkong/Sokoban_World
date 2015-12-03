/**
 * @Name: Fanchen Kong
 * @UID: 404506008
 * @This is a cpp file for the mainwindow class.
 *  It defines all the public slots and take control of all the changes of mainwindow.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "board.h"
#include <QRadioButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <iostream>
#include <cstdlib>

/**
 * @brief MainWindow::MainWindow is a constructor for the mainwindow.
 * @param parent defines the parent of the the mainwindow
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    display();
}
/**
 * @brief MainWindow::~MainWindow is a destructor for the mainwindow.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::start_game is a function to start game based on which button is checked.
 */
void MainWindow::start_game(){
    if(Easy_radioButton->isChecked()){
        diff = 1;
        board = new Board(this,easy);
        this->setCentralWidget(board);
    }

    else if (Medium_radioButton->isChecked()){
        diff = 2;
        board = new Board(this,medium);
        this->setCentralWidget(board);
    }
    else if(Hard_radioButton->isChecked()){
        diff = 3;
        board = new Board(this,hard);
        this->setCentralWidget(board);
    }
    else if(Suprise_radioButton->isChecked()){
        diff = 4;
        board = new Board(this,surprise);
        this->setCentralWidget(board);

    }
}

/**
 * @brief MainWindow::game_over is a public slot for game over.
 */
void MainWindow::game_over(){

    // Reset the MainWindow with the initial startup screen
    ui->setupUi(this);
    this->setFixedSize(600,650);
    display();
}
/**
 * @brief MainWindow::next_diff is a public slot for change to the next difficulty.
 */
void MainWindow::next_diff(){
    diff += 1;
    QMessageBox msgBox;
    msgBox.setText("Congratulations! \nDo you want to go to the next level?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int result = msgBox.exec();
    switch (result) {
        case QMessageBox::Yes:
            if(diff == 2)
                board = new Board(this,medium);
            else if (diff == 3)
                board = new Board(this,hard);
            else if(diff == 4)
                board = new Board(this,surprise);
            this->setCentralWidget(board);
            break;
        case QMessageBox::No:
            ui->setupUi(this);
            display();
            break;
        default:
            if(diff == 2)
                board = new Board(this,medium);
            else if (diff == 3)
                board = new Board(this,hard);
            else if(diff == 4)
                board = new Board(this,surprise);
            this->setCentralWidget(board);
            break;
        }
}

/**
 * @brief MainWindow::restart_game is a plublic slot for restart that level
 */
void MainWindow::restart_game(){
    QMessageBox msgBox;
    msgBox.setText("Do you want to restart this game?");
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int result = msgBox.exec();
    switch (result) {
        case QMessageBox::Yes:
            if(diff == 1)
                board = new Board(this,easy);
            else if(diff == 2)
                board = new Board(this,medium);
            else if (diff == 3)
                board = new Board(this,hard);
            else if(diff == 4)
                board = new Board(this,surprise);
            this->setCentralWidget(board);
            break;
        case QMessageBox::No:
            break;
        default:
            if(diff == 1)
                board = new Board(this,easy);
            else if(diff == 2)
                board = new Board(this,medium);
            else if (diff == 3)
                board = new Board(this,hard);
            else if(diff == 4)
                board = new Board(this,surprise);
            this->setCentralWidget(board);
            break;
    }
}

/**
 * @brief MainWindow::out_of_time is a public slot if the timer out ouf time.
 */
void MainWindow::out_of_time(){
    QMessageBox msgBox;
    msgBox.setText("Sorry, you run out of the time.");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int result = msgBox.exec();
    switch (result) {
        case QMessageBox::Yes:
            ui->setupUi(this);
            display();
            break;
        default:
            ui->setupUi(this);
            display();
            break;
        }

}

/**
 * @brief MainWindow::last_level is a public slot if the last_level is complished.
 */
void MainWindow::last_level(){
    QMessageBox msgBox;
    msgBox.setText("Congratulations on completing all the difficulty levels!\nThe window will go back to the main page of this game.");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int result = msgBox.exec();
    switch (result) {
        case QMessageBox::Yes:
            ui->setupUi(this);
            display();
            break;
        default:
            ui->setupUi(this);
            display();
            break;
    }
}

/**
 * @brief MainWindow::back_menu is a public slot if the menu pushbutton is clicked.
 */
void MainWindow::back_menu(){
    QMessageBox msgBox;
    msgBox.setText("Do you want to go back to the menu?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int result = msgBox.exec();
    switch (result) {
        case QMessageBox::Yes:
            ui->setupUi(this);
            display();
            break;
        case QMessageBox::No:
            break;
        default:
            break;
        }
}
/**
 * @brief MainWindow::closeEvent is a redifined function for close event.
 * @param event is a default close event.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to close?");
    msgBox.setStandardButtons(QMessageBox::Close | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Close);
    int result = msgBox.exec();
    switch (result) {
        case QMessageBox::Close:
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        default:
            QMainWindow::closeEvent(event);
            break;
        }
}

/**
 * @brief MainWindow::show_instruction is a slot function to show instruction if the description is clicked.
 */
void MainWindow::show_instruction()
{
    QWidget* instruction=new QWidget();
    QVBoxLayout* instr_central=new QVBoxLayout();
    QLabel *instr_title = new QLabel("Instruction");
    instr_title->setAlignment(Qt::AlignCenter);
    QFont instr_title_font=instr_title->font();
    instr_title_font.setPointSize(20);
    instr_title->setFont(instr_title_font);
    instr_central->addSpacing(50);
    instr_central->addWidget(instr_title);
    instr_central->addSpacing(15);
    QLabel* instr_label=new QLabel();
    QString instr_text = "This game gives user five minutes to push the boxes from its original position to the star position.\nThe user can select four levels to start the game. After starting the game, the user uses direction \nkeyboard 'up', 'down', 'right', 'left' to move the people. If all the boxes hits the star position in the \nlimited time, it will go to the next level. Also, the user can click the button 'undo' to go back for \n three times. The solution for the hard level and surprise level will be in the website: http://\nwww.aipai.com/c25/PDohICoiICVqJWQvLg.html, level 37 and level 44, respectively.\nEnjoy the game!";
    instr_label->setText(instr_text);
    instr_central->addWidget(instr_label);
    instr_central->addSpacing(50);
    instruction->setLayout(instr_central);
    instruction->show();
}

/**
 * @brief MainWindow::display is a function to display the main window.
 */
void MainWindow::display(){
    setWindowTitle("Sokoban");
    setFixedSize(600,650);
    QVBoxLayout* central=new QVBoxLayout;
    QLabel* title=new QLabel("Welcome to Sokoban World!");
    //set up the style fo title
    title->setAlignment(Qt::AlignCenter);
    QFont title_font=title->font();
    title_font.setPointSize(25);
    title->setFont(title_font);
    central->addSpacing(75);
    central->addWidget(title);
    central->addSpacing(25);
    //Add the image
    QPixmap sokoban("://sokoban.jpg");
    QPixmap sokoban1=sokoban.scaled(250,250,Qt::KeepAspectRatio);
    QLabel* image = new QLabel;
    image->setPixmap(sokoban1);
    image->setAlignment(Qt::AlignCenter);
    central->addWidget(image);
    central->addSpacing(15);

    QHBoxLayout *instr = new QHBoxLayout;
    instr->addSpacing(150);
    descr = new QPushButton("instruction");
    instr->addWidget(descr);
    instr->addSpacing(150);
    central->addLayout(instr);
    QObject::connect(descr,SIGNAL(clicked()),this,SLOT(show_instruction()));

    QHBoxLayout* under_title = new QHBoxLayout;
    QVBoxLayout* game_model=new QVBoxLayout;
    QVBoxLayout* difficulty = new QVBoxLayout;

    QLabel *GameModel = new QLabel("Start the Game");
    GameModel->setAlignment(Qt::AlignCenter);
    game_model->addWidget(GameModel);
    game_model->addSpacing(100);

    QHBoxLayout *play = new QHBoxLayout;
    play->addSpacing(50);
    QPushButton* one_player = new QPushButton("Play");
    play->addWidget(one_player);
    play->addSpacing(50);
    game_model->addLayout(play);
    game_model->addSpacing(100);

    //central->addSpacing();
    under_title->addLayout(game_model);
    //set up the difficulty
    QLabel *diff_level = new QLabel("Difficulty");
    diff_level->setAlignment(Qt::AlignCenter);
    difficulty->addWidget(diff_level);
    difficulty->addSpacing(10);

    QHBoxLayout* easy_model = new QHBoxLayout;
    easy_model->addSpacing(40);
    Easy_radioButton=new QRadioButton("");
    QLabel *easy_label = new QLabel("Easy");
    easy_model->addWidget(Easy_radioButton);
    easy_model->addWidget(easy_label);
    easy_model->addSpacing(50);

    QHBoxLayout* medium_model = new QHBoxLayout;
    medium_model->addSpacing(40);
    Medium_radioButton=new QRadioButton("");
    QLabel *medium_label = new QLabel("Medium");
    medium_model->addWidget(Medium_radioButton);
    medium_model->addWidget(medium_label);
    medium_model->addSpacing(50);

    QHBoxLayout* hard_model = new QHBoxLayout;
    hard_model->addSpacing(40);
    Hard_radioButton=new QRadioButton("");
    QLabel *hard_label = new QLabel("Hard");
    hard_model->addWidget(Hard_radioButton);
    hard_model->addWidget(hard_label);
    hard_model->addSpacing(50);

    QHBoxLayout* surprise_model = new QHBoxLayout;
    surprise_model->addSpacing(40);
    Suprise_radioButton=new QRadioButton("");
    QLabel *suprise_label = new QLabel("Surprise");
    surprise_model->addWidget(Suprise_radioButton);
    surprise_model->addWidget(suprise_label);
    surprise_model->addSpacing(50);

    difficulty->addLayout(easy_model);
    difficulty->addSpacing(10);
    difficulty->addLayout(medium_model);
    difficulty->addSpacing(10);
    difficulty->addLayout(hard_model);
    difficulty->addSpacing(10);
    difficulty->addLayout(surprise_model);
    difficulty->addSpacing(10);

    under_title->addLayout(difficulty);
    central->addLayout(under_title);

    QObject::connect(one_player,SIGNAL(clicked()),this,SLOT(start_game()));

    centralWidget()->setLayout(central);

}


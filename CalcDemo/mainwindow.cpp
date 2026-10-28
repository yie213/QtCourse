#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //状态变量初始化
    m_num1 = 0;
    m_num2 = 0;
    m_op = "";
    m_waitSecondNum = false;
    m_afterCalc = false;

    updateDisplay("0");

    //绑定全部按钮信号槽，全部绑定，但槽是空实现
    QList<QPushButton*> numBtns = {
        ui->btn_0,ui->btn_1,ui->btn_2,ui->btn_3,ui->btn_4,
        ui->btn_5,ui->btn_6,ui->btn_7,ui->btn_8,ui->btn_9
    };
    for(auto btn : numBtns){
        connect(btn,&QPushButton::clicked,this,&MainWindow::slot_numClick);
    }

    connect(ui->btn_add,&QPushButton::clicked,this,&MainWindow::slot_opClick);
    connect(ui->btn_sub,&QPushButton::clicked,this,&MainWindow::slot_opClick);
    connect(ui->btn_mul,&QPushButton::clicked,this,&MainWindow::slot_opClick);
    connect(ui->btn_div,&QPushButton::clicked,this,&MainWindow::slot_opClick);

    connect(ui->btn_dot,&QPushButton::clicked,this,&MainWindow::slot_dotClick);
    connect(ui->btn_eq,&QPushButton::clicked,this,&MainWindow::slot_eqClick);
    connect(ui->btn_clear,&QPushButton::clicked,this,&MainWindow::slot_clearClick);
    connect(ui->btn_backspace,&QPushButton::clicked,this,&MainWindow::slot_backspaceClick);
    connect(ui->btn_sign,&QPushButton::clicked,this,&MainWindow::slot_signClick);
    connect(ui->btn_square,&QPushButton::clicked,this,&MainWindow::slot_squareClick);
    connect(ui->btn_sqrt,&QPushButton::clicked,this,&MainWindow::slot_sqrtClick);
    connect(ui->btn_reciprocal,&QPushButton::clicked,this,&MainWindow::slot_reciprocalClick);
    connect(ui->btn_per,&QPushButton::clicked,this,&MainWindow::slot_perClick);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateDisplay(QString str)
{
    ui->display->setText(str);
}

//全部空实现
void MainWindow::processInput(QString text){
    Q_UNUSED(text);
}
void MainWindow::slot_numClick(){}
void MainWindow::slot_dotClick(){}
void MainWindow::slot_opClick(){}
void MainWindow::slot_eqClick(){}
void MainWindow::slot_clearClick(){}
void MainWindow::slot_backspaceClick(){}
void MainWindow::slot_signClick(){}
void MainWindow::slot_squareClick(){}
void MainWindow::slot_sqrtClick(){}
void MainWindow::slot_reciprocalClick(){}
void MainWindow::slot_perClick(){}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);
}

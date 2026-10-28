#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_num1 = 0;
    m_num2 = 0;
    m_op = "";
    m_waitSecondNum = false;
    m_afterCalc = false;
    updateDisplay("0");

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

//操作数输入，禁止重复小数点
void MainWindow::processInput(QString text)
{
    QString curText = ui->display->text();
    if(m_afterCalc){
        if((text >= "0" && text <= "9") || text == "."){
            curText = "0";
            m_afterCalc = false;
        }
    }
    if(text >= "0" && text <= "9"){
        if(curText == "0"){
            curText = text;
        }else{
            curText += text;
        }
    }else if(text == "."){
        //关键：判断当前字符串已经包含小数点，则不再追加，避免重复小数点
        if(!curText.contains('.')){
            curText += ".";
        }
    }
    updateDisplay(curText);
}

void MainWindow::slot_numClick()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    processInput(btn->text());
}

void MainWindow::slot_dotClick()
{
    processInput(".");
}

//运算符、等于暂未实现
void MainWindow::slot_opClick(){}
void MainWindow::slot_eqClick(){}

//清除全部状态
void MainWindow::slot_clearClick()
{
    m_num1 = 0;
    m_num2 = 0;
    m_op.clear();
    m_waitSecondNum = false;
    m_afterCalc = false;
    updateDisplay("0");
}

//退格功能
void MainWindow::slot_backspaceClick()
{
    QString cur = ui->display->text();
    if(cur.length() <= 1){
        cur = "0";
    }else{
        cur.chop(1);
    }
    updateDisplay(cur);
}

//单目运算暂空
void MainWindow::slot_signClick(){}
void MainWindow::slot_squareClick(){}
void MainWindow::slot_sqrtClick(){}
void MainWindow::slot_reciprocalClick(){}
void MainWindow::slot_perClick(){}

//键盘事件暂空
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);
}

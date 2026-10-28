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
    m_expStr = ""; //初始化表达式
    updateExpLabel();
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

    //刚算完结果，输入数字则重置屏幕
    if(m_afterCalc){
        if((text >= "0" && text <= "9") || text == "."){
            curText = "0";
            m_afterCalc = false;
        }
    }

    // 等待第二个操作数：清空屏幕用于输入第二个数，⚠️不修改 m_waitSecondNum！！
    bool needClearInput = m_waitSecondNum;
    if(needClearInput)
    {
        curText = "";
    }

    if(text >= "0" && text <= "9"){
        if(curText == "0" || curText.isEmpty()){
            curText = text;
        }else{
            curText += text;
        }
    }else if(text == "."){
        if(!curText.contains('.')){
            if(curText.isEmpty()) curText = "0";
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

//等于
void MainWindow::slot_opClick()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QString op = btn->text();
    QString disp = ui->display->text();

    if(!m_waitSecondNum){
        //第一次点击运算符，把当前数字+运算符存入表达式
        m_num1 = disp.toDouble();
        m_expStr = disp + " " + op + " ";
    }else{
        //连续按运算符，先计算上一次
        slot_eqClick();
        m_num1 = ui->display->text().toDouble();
        m_expStr = QString::number(m_num1) + " " + op + " ";
    }
    m_op = op;
    m_waitSecondNum = true;
    m_afterCalc = false;
    updateExpLabel(); //刷新上方算式
}



void MainWindow::slot_eqClick()
{
    if(!m_waitSecondNum || m_op.isEmpty()) return;
    m_num2 = ui->display->text().toDouble();
    double res = 0;
    bool divZero = false;

    if(m_op == "+"){
        res = m_num1 + m_num2;
    }else if(m_op == "-"){
        res = m_num1 - m_num2;
    }else if(m_op == "×"){   // 中文乘号！！和UI按钮一致
        res = m_num1 * m_num2;
    }else if(m_op == "÷"){   // 中文除号！！
        if(qAbs(m_num2) < 1e-9){
            divZero = true;
        }else{
            res = m_num1 / m_num2;
        }
    }

    if(divZero){
        updateDisplay("错误-除零");
        m_expStr += QString::number(m_num2) + " =";
        updateExpLabel();
        m_waitSecondNum = false;
        m_afterCalc = true;
        m_op.clear();
        return;
    }

    m_expStr += QString::number(m_num2) + " =";
    updateExpLabel();
    updateDisplay(QString::number(res));
    m_num1 = res;
    m_afterCalc = true;
    m_waitSecondNum = false;
    m_op.clear();
}




void MainWindow::slot_clearClick()
{
    m_num1 = 0;
    m_num2 = 0;
    m_op.clear();
    m_waitSecondNum = false;
    m_afterCalc = false;
    m_expStr.clear(); //清空算式
    updateExpLabel();
    updateDisplay("0");
}


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

void MainWindow::updateExpLabel()
{
    ui->label_exp->setText(m_expStr);
}

//单目运算空实现
void MainWindow::slot_signClick(){}
void MainWindow::slot_squareClick(){}
void MainWindow::slot_sqrtClick(){}
void MainWindow::slot_reciprocalClick(){}
void MainWindow::slot_perClick(){}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);
}

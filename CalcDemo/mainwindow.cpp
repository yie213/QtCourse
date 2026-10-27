#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QKeyEvent>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 状态初始化
    m_num1 = 0;
    m_num2 = 0;
    m_op = "";
    m_waitSecondNum = false;
    m_afterCalc = false;
    updateDisplay("0");

    // 数字按钮绑定 0~9
    QList<QPushButton*> numBtns = {
        ui->btn_0,ui->btn_1,ui->btn_2,ui->btn_3,ui->btn_4,
        ui->btn_5,ui->btn_6,ui->btn_7,ui->btn_8,ui->btn_9
    };
    for(auto btn : numBtns){
        connect(btn,&QPushButton::clicked,this,&MainWindow::slot_numClick);
    }

    // 二元运算符 + - * /
    connect(ui->btn_add,&QPushButton::clicked,this,&MainWindow::slot_opClick);
    connect(ui->btn_sub,&QPushButton::clicked,this,&MainWindow::slot_opClick);
    connect(ui->btn_mul,&QPushButton::clicked,this,&MainWindow::slot_opClick);
    connect(ui->btn_div,&QPushButton::clicked,this,&MainWindow::slot_opClick);

    // 其余按钮
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

// 公共输入处理，鼠标键盘共用
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

void MainWindow::slot_opClick()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QString op = btn->text();
    QString disp = ui->display->text();

    if(!m_waitSecondNum){
        m_num1 = disp.toDouble();
        m_waitSecondNum = true;
    }else{
        slot_eqClick();
        m_num1 = ui->display->text().toDouble();
        m_waitSecondNum = true;
    }
    m_op = op;
    m_afterCalc = false;
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
    }else if(m_op == "*"){
        res = m_num1 * m_num2;
    }else if(m_op == "/"){
        if(qAbs(m_num2) < 1e-9){
            divZero = true;
        }else{
            res = m_num1 / m_num2;
        }
    }

    if(divZero){
        updateDisplay("错误-除零");
        m_waitSecondNum = false;
        m_afterCalc = true;
        m_op.clear();
        return;
    }
    updateDisplay(QString::number(res));
    m_num1 = res;
    m_afterCalc = true;
}

void MainWindow::slot_clearClick()
{
    m_num1 = 0;
    m_num2 = 0;
    m_op.clear();
    m_waitSecondNum = false;
    m_afterCalc = false;
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

// +/- 正负切换
void MainWindow::slot_signClick()
{
    double val = ui->display->text().toDouble();
    double res = -val;
    updateDisplay(QString::number(res));
}

// x² 平方
void MainWindow::slot_squareClick()
{
    double val = ui->display->text().toDouble();
    double res = val * val;
    updateDisplay(QString::number(res));
    m_afterCalc = true;
}

// √x 平方根
void MainWindow::slot_sqrtClick()
{
    double val = ui->display->text().toDouble();
    if(val < 0){
        updateDisplay("错误");
        m_afterCalc = true;
        return;
    }
    double res = sqrt(val);
    updateDisplay(QString::number(res));
    m_afterCalc = true;
}

// 1/x 倒数
void MainWindow::slot_reciprocalClick()
{
    double val = ui->display->text().toDouble();
    if(qAbs(val) < 1e-9){
        updateDisplay("错误");
        m_afterCalc = true;
        return;
    }
    double res = 1.0 / val;
    updateDisplay(QString::number(res));
    m_afterCalc = true;
}

// % 百分号：当前数字 /100
void MainWindow::slot_perClick()
{
    double val = ui->display->text().toDouble();
    double res = val / 100.0;
    updateDisplay(QString::number(res));
    m_afterCalc = true;
}

// 键盘事件
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    QString text = event->text();

    if((key >= Qt::Key_0 && key <= Qt::Key_9) || text == "."){
        processInput(text);
    }else if(key == Qt::Key_Plus || key == Qt::Key_Minus
             || key == Qt::Key_Asterisk || key == Qt::Key_Slash){
        if(key == Qt::Key_Plus) ui->btn_add->click();
        else if(key == Qt::Key_Minus) ui->btn_sub->click();
        else if(key == Qt::Key_Asterisk) ui->btn_mul->click();
        else if(key == Qt::Key_Slash) ui->btn_div->click();
    }else if(key == Qt::Key_Return || key == Qt::Key_Enter){
        slot_eqClick();
    }else if(key == Qt::Key_Escape){
        slot_clearClick();
    }else if(key == Qt::Key_Backspace){
        slot_backspaceClick();
    }else{
        QMainWindow::keyPressEvent(event);
    }
}

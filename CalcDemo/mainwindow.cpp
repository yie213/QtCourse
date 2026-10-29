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
    m_num1 = 0;
    m_num2 = 0;
    m_op = "";
    m_waitSecondNum = false;
    m_afterCalc = false;
    m_isFirstInputSecond = false;
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

    //四则运算
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
    connect(ui->btn_ce,&QPushButton::clicked,this,&MainWindow::slot_ceClick);


    // 全局样式
    this->setStyleSheet(R"(
    QWidget{
        background-color: #f0f0f0;
    }
    QLabel#label_exp{
        color:#666666;
        font-size:18px;
        padding-right:8px;
    }
    QLineEdit#display{
        font-size:48px;
        border:none;
        padding-right:8px;
    }
    QPushButton{
        background-color: #ffffff;
        border-radius:4px;
        font-size:20px;
    }
    QPushButton:pressed{
        background-color:#dddddd;
    }
    QPushButton#btn_eq{
        background-color:#009688;
        color:white;
    }
    QPushButton#btn_eq:pressed{
        background-color:#00796b;
    }
    )");


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

    if(m_waitSecondNum && m_isFirstInputSecond)
        {
            curText = "";
            m_isFirstInputSecond = false; //清空之后，后续再输入不再清空
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
        m_num1 = disp.toDouble();
        m_expStr = disp + " " + op + " ";
    }else{
        slot_eqClick();
        m_num1 = ui->display->text().toDouble();
        m_expStr = QString::number(m_num1) + " " + op + " ";
    }
    m_op = op;
    m_waitSecondNum = true;
    m_isFirstInputSecond = true; //标记：接下来是第二个数的首次输入
    m_afterCalc = false;
    updateExpLabel();
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
        updateDisplay("除数不能为零");
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

//退格
void MainWindow::slot_backspaceClick()
{
    if(m_afterCalc) return;
    QString text = ui->display->text();
    if(text.size() <= 1)
    {
        updateDisplay("0");
    }else{
        text.chop(1);
        updateDisplay(text);
    }
}

//C 清空
void MainWindow::slot_clearClick()
{
    m_num1 = 0;
    m_num2 = 0;
    m_op.clear();
    m_waitSecondNum = false;
    m_afterCalc = false;
    m_isFirstInputSecond = false;
    m_expStr.clear(); //清空算式
    updateExpLabel();
    updateDisplay("0");
}


// CE：只清空当前输入，不清历史算式和m_num1
void MainWindow::slot_ceClick()
{
    updateDisplay("0");
    m_afterCalc = false;

    if(m_waitSecondNum)
        {
            m_isFirstInputSecond = true;
        }
}

// ± 正负切换
void MainWindow::slot_signClick()
{
    QString text = ui->display->text();
    double val = text.toDouble();
    val = -val;
    updateDisplay(QString::number(val));
}

// x² 平方
void MainWindow::slot_squareClick()
{
    QString text = ui->display->text();
    double val = text.toDouble();
    val = val * val;
    updateDisplay(QString::number(val));
    m_afterCalc = true;
}

// √x 开平方
void MainWindow::slot_sqrtClick()
{
    QString text = ui->display->text();
    double val = text.toDouble();
    if(val < 0)
    {
        updateDisplay("错误");
    }else{
        val = sqrt(val);
        updateDisplay(QString::number(val));
    }
    m_afterCalc = true;
}

// 1/x 倒数
void MainWindow::slot_reciprocalClick()
{
    QString text = ui->display->text();
    double val = text.toDouble();
    if(qAbs(val) < 1e-9)
    {
        updateDisplay("错误");
    }else{
        val = 1.0 / val;
        updateDisplay(QString::number(val));
    }
    m_afterCalc = true;
}

// % 百分比
void MainWindow::slot_perClick()
{
    QString text = ui->display->text();
    double val = text.toDouble();
    val = val / 100.0;
    updateDisplay(QString::number(val));
    m_afterCalc = true;
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);
}

void MainWindow::updateExpLabel()
{
    ui->label_exp->setText(m_expStr);
}

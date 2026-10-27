#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    //重写键盘事件
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void slot_numClick();
    void slot_dotClick();
    void slot_opClick();
    void slot_eqClick();
    void slot_clearClick();
    void slot_backspaceClick();
    void slot_signClick();
    void slot_squareClick();
    void slot_sqrtClick();
    void slot_reciprocalClick();
    void slot_perClick();


private:
    Ui::MainWindow *ui;

    //====计算器状态变量====
    double m_num1;         //第一个操作数
    double m_num2;         //第二个操作数
    QString m_op;          //保存运算符
    bool m_waitSecondNum;  //true=等待输入第二个操作数
    bool m_afterCalc;      //true=刚刚完成一次计算，下次输入数字清空屏幕
    //核心公共处理函数：鼠标按钮、键盘事件全部调用这个函数，实现逻辑复用
    void processInput(QString text);
    void updateDisplay(QString str);
};
#endif // MAINWINDOW_H

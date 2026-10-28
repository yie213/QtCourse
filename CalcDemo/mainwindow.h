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
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    //空槽声明，尚未实现
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
    //状态变量定义
    double m_num1;
    double m_num2;
    QString m_op;
    bool m_waitSecondNum;
    bool m_afterCalc;

    void processInput(QString text);
    void updateDisplay(QString str);
};
#endif // MAINWINDOW_H

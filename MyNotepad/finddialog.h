#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();

signals:
    //发送查找参数给主窗口
    void findNext(QString keyword, bool caseSensitive, bool searchDown);

private slots:
    void on_btFindNext_clicked();
    void on_btCancel_clicked();

private:
    Ui::FindDialog *ui;
};

#endif // FINDDIALOG_H

#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ReplaceDialog(QWidget *parent = nullptr);
    ~ReplaceDialog();

signals:
    void findNext(QString keyword, bool caseSensitive, bool down);
    void replaceOne(QString keyword, QString replaceText, bool caseSensitive, bool down);
    void replaceAll(QString keyword, QString replaceText, bool caseSensitive);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::ReplaceDialog *ui;
};

#endif // REPLACEDIALOG_H

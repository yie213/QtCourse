#include "replacedialog.h"
#include "ui_replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    ui->radioButton_2->setChecked(true); // 默认向下查找
    setAttribute(Qt::WA_DeleteOnClose, false);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

//查找下一个
void ReplaceDialog::on_pushButton_clicked()
{
    QString key = ui->lineEdit->text();
    bool caseSens = ui->checkBox->isChecked();
    bool down = ui->radioButton_2->isChecked();
    emit findNext(key, caseSens, down);
}

//替换当前选中
void ReplaceDialog::on_pushButton_2_clicked()
{
    QString key = ui->lineEdit->text();
    QString repText = ui->lineEdit_2->text();
    bool caseSens = ui->checkBox->isChecked();
    bool down = ui->radioButton_2->isChecked();
    emit replaceOne(key, repText, caseSens, down);
}

//全部替换
void ReplaceDialog::on_pushButton_3_clicked()
{
    QString key = ui->lineEdit->text();
    QString repText = ui->lineEdit_2->text();
    bool caseSens = ui->checkBox->isChecked();
    emit replaceAll(key, repText, caseSens);
}

//取消，关闭窗口
void ReplaceDialog::on_pushButton_4_clicked()
{
    this->close();
}

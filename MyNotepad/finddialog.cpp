#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    ui->rbDown->setChecked(true);
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::on_btFindNext_clicked()
{
    QString key = ui->SearchNext->text();
    bool caseSensitive = ui->cbCaseSensetive->isChecked();
    bool down = ui->rbDown->isChecked();
    emit findNext(key, caseSensitive, down);
}

void FindDialog::on_btCancel_clicked()
{
    this->close();
}

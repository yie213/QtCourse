#include "masterview.h"
#include "ui_masterview.h"

MasterView::MasterView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);
}

MasterView::~MasterView()
{
    delete ui;
}


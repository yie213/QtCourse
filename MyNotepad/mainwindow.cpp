#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "finddialog.h"
#include "replacedialog.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusLabel.setMaximumWidth(150);
    statusLabel.setText("length: " + QString::number(0) + "   lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln: " + QString::number(0) + "   Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("谢嘉仪"));
    ui->statusbar->addPermanentWidget(author);

    m_curFilePath = "";
    m_isModified = false;
    // 绑定文本编辑框内容变化信号
    connect(ui->textEdit, &QPlainTextEdit::textChanged, this, &MainWindow::onTextChanged);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionFind_triggered()
{
    FindDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg;
    dlg.exec();
}

void MainWindow::on_actionNew_triggered()
{
    // 如果有未保存修改，提示
    if(m_isModified)
    {
        QMessageBox::StandardButton ret = QMessageBox::question(this, tr("提示"),
                                          tr("当前文档有修改，是否保存？"),
                                          QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(ret == QMessageBox::Yes)
        {
            on_actionSave_triggered();
        }
        else if(ret == QMessageBox::Cancel)
        {
            return;
        }
    }
    ui->textEdit->clear();
    m_curFilePath = "";
    m_isModified = false;
    this->setWindowTitle(tr("无标题 - 小小文本编辑器"));
}

void MainWindow::on_actionOpen_triggered()
{
    if(m_isModified)
    {
        QMessageBox::StandardButton ret = QMessageBox::question(this, tr("提示"),
                                          tr("当前文档有修改，是否保存？"),
                                          QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(ret == QMessageBox::Yes)
        {
            on_actionSave_triggered();
        }
        else if(ret == QMessageBox::Cancel)
        {
            return;
        }
    }
    QString filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), "./", tr("文本文件(*.txt)"));
    if(filePath.isEmpty()) return;

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("打开失败"), tr("无法读取该文件！"));
        return;
    }
    QTextStream in(&file);
    ui->textEdit->setPlainText(in.readAll());
    file.close();

    m_curFilePath = filePath;
    m_isModified = false;
    this->setWindowTitle(tr("小小文本编辑器 - %1").arg(m_curFilePath));
}

void MainWindow::on_actionSave_triggered()
{
    if(m_curFilePath.isEmpty())
    {
        // 没有路径，跳另存为
        on_actionSaveAs_triggered();
        return;
    }
    QFile file(m_curFilePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("保存失败"), tr("文件无法写入！"));
        return;
    }
    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();
    m_isModified = false;
    this->setWindowTitle(tr("小小文本编辑器 - %1").arg(m_curFilePath));
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("另存为"), "./", tr("文本文件(*.txt)"));
    if(filePath.isEmpty()) return;

    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("另存失败"), tr("文件无法写入！"));
        return;
    }
    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();

    m_curFilePath = filePath;
    m_isModified = false;
    this->setWindowTitle(tr("小小文本编辑器 - %1").arg(m_curFilePath));
}

void MainWindow::onTextChanged()
{
    // 文本一变，打上修改标记，标题加*
    m_isModified = true;
    QString title;
    if(m_curFilePath.isEmpty())
    {
        title = tr("无标题 - 小小文本编辑器 *");
    }
    else
    {
        title = tr("小小文本编辑器 - %1 *").arg(m_curFilePath);
    }
    this->setWindowTitle(title);
}

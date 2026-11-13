#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "finddialog.h"
#include "replacedialog.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <QFontDialog>
#include <QColorDialog>
#include <QTextCharFormat>

#include <QClipboard>
#include <QGuiApplication>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionAutoLineWrap->setChecked(ui->textEdit->lineWrapMode() == QPlainTextEdit::WidgetWidth);
    ui->actionToolBar->setChecked(true);
    ui->actionStatusBar->setChecked(true);

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

    connect(ui->actionUndo, &QAction::triggered, ui->textEdit, &QPlainTextEdit::undo);
    connect(ui->actionRedo, &QAction::triggered, ui->textEdit, &QPlainTextEdit::redo);
    connect(ui->actionCut, &QAction::triggered, ui->textEdit, &QPlainTextEdit::cut);
    connect(ui->actionCopy, &QAction::triggered, ui->textEdit, &QPlainTextEdit::copy);
    connect(ui->actionPaste, &QAction::triggered, ui->textEdit, &QPlainTextEdit::paste);
    connect(ui->actionSelectAll, &QAction::triggered, ui->textEdit, &QPlainTextEdit::selectAll);

    connect(ui->textEdit, &QPlainTextEdit::textChanged, this, &MainWindow::updateStatusInfo);
    connect(ui->textEdit, &QPlainTextEdit::cursorPositionChanged, this, &MainWindow::updateStatusInfo);

    updateStatusInfo();

    connect(ui->textEdit, &QPlainTextEdit::selectionChanged, this, &MainWindow::updateEditActionEnable);
    //剪贴板内容发生变化时，更新粘贴按钮状态
    connect(QGuiApplication::clipboard(),&QClipboard::dataChanged,this,&MainWindow::updateEditActionEnable);

    //程序启动初始化一次按钮状态
    updateEditActionEnable();

    //修改编辑框光标颜色为蓝色
    ui->textEdit->setStyleSheet(R"(
    QPlainTextEdit {
        color: black;
        background-color: white;
        selection-color: white;
        selection-background-color: #217ad9;
        /* 设置闪烁输入光标（插入光标）蓝色 */
        qproperty-cursorColor: #0044ff;
    }
    )");

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


    if(!m_findDlg)
        {
            m_findDlg = new FindDialog(this);
            connect(m_findDlg, &FindDialog::findNext, this, &MainWindow::onFindNext);
        }
        m_findDlg->show();
        m_findDlg->raise();
        m_findDlg->activateWindow();
}

void MainWindow::onFindNext(QString keyword, bool caseSensitive, bool searchDown)
{
    if(keyword.isEmpty())
        return;

    QTextDocument::FindFlags flag = {};
    if(caseSensitive)
        flag |= QTextDocument::FindCaseSensitively;
    if(!searchDown)
        flag |= QTextDocument::FindBackward;

    bool isFound = ui->textEdit->find(keyword, flag);
    if(!isFound)
    {
        QMessageBox::information(this, tr("查找"), tr("未找到【%1】").arg(keyword));
    }
}

void MainWindow::on_actionReplace_triggered()
{
    if(!m_replaceDlg)
        {
            m_replaceDlg = new ReplaceDialog(this);
            connect(m_replaceDlg, &ReplaceDialog::findNext, this, &MainWindow::onReplaceFind);
            connect(m_replaceDlg, &ReplaceDialog::replaceOne, this, &MainWindow::onReplaceOne);
            connect(m_replaceDlg, &ReplaceDialog::replaceAll, this, &MainWindow::onReplaceAll);
        }
        m_replaceDlg->show();
        m_replaceDlg->raise();
        m_replaceDlg->activateWindow();
}

void MainWindow::onReplaceFind(QString keyword, bool caseSensitive, bool searchDown)
{
    if(keyword.isEmpty()) return;
    QTextDocument::FindFlags flag;
    if(caseSensitive) flag |= QTextDocument::FindCaseSensitively;
    if(!searchDown) flag |= QTextDocument::FindBackward;

    bool found = ui->textEdit->find(keyword, flag);
    if(!found)
    {
        QMessageBox::information(this, tr("查找"), tr("未找到【%1】").arg(keyword));
    }
}

void MainWindow::onReplaceOne(QString keyword, QString replaceText, bool caseSensitive, bool searchDown)
{
    QTextCursor cur = ui->textEdit->textCursor();
    //当前选中内容等于目标文本才替换
    if(cur.hasSelection())
    {
        QString selected = cur.selectedText();
        Qt::CaseSensitivity cs = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
        if(selected.compare(keyword, cs) == 0)
        {
            cur.insertText(replaceText);
            ui->textEdit->setTextCursor(cur);
        }
    }
    //替换完自动查找下一处
    onReplaceFind(keyword, caseSensitive, searchDown);
}

void MainWindow::onReplaceAll(QString keyword, QString replaceText, bool caseSensitive)
{
    if(keyword.isEmpty()) return;
    QTextDocument::FindFlags flag;
    if(caseSensitive) flag |= QTextDocument::FindCaseSensitively;

    ui->textEdit->moveCursor(QTextCursor::Start); //回到文档开头
    int count = 0;
    while(ui->textEdit->find(keyword, flag))
    {
        QTextCursor cur = ui->textEdit->textCursor();
        cur.insertText(replaceText);
        count++;
    }
    QMessageBox::information(this, tr("全部替换"), tr("一共替换了 %1 处").arg(count));
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


//自动换行
void MainWindow::on_actionAutoLineWrap_triggered()
{
    if(ui->actionAutoLineWrap->isChecked())
    {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    }
    else
    {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    }
}


//字体
void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont curFont = ui->textEdit->currentCharFormat().font();
    QFont font = QFontDialog::getFont(&ok, curFont, this, tr("选择字体"));
    if (ok)
    {
        QTextCharFormat fmt;
        fmt.setFont(font);
        ui->textEdit->mergeCurrentCharFormat(fmt);
    }
}

//字体颜色
void MainWindow::on_actionFontColor_triggered()
{
    QColor curColor = ui->textEdit->currentCharFormat().foreground().color();
    QColor color = QColorDialog::getColor(curColor, this, tr("选择字体颜色"));
    if(color.isValid())
    {
        QTextCharFormat fmt;
        fmt.setForeground(color);
        ui->textEdit->mergeCurrentCharFormat(fmt);
    }
}

//字体背景色
void MainWindow::on_actionFontBackgroundColor_triggered()
{
    QColor curBg = ui->textEdit->currentCharFormat().background().color();
    QColor color = QColorDialog::getColor(curBg, this, tr("选择字体背景色"));
    if(color.isValid())
    {
        QTextCharFormat fmt;
        fmt.setBackground(color);
        ui->textEdit->mergeCurrentCharFormat(fmt);
    }
}

//编辑器背景色
void MainWindow::on_actionEditorBackgroundColor_triggered()
{
    QColor color = QColorDialog::getColor(ui->textEdit->palette().color(QPalette::Base), this, tr("选择编辑器背景色"));
    if(color.isValid())
    {
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit { background-color: %1; }").arg(color.name()));
    }
}


void MainWindow::updateStatusInfo()
{
    //获取全部文本
    QString allText = ui->textEdit->toPlainText();
    int textLength = allText.length();
    int lineCount = ui->textEdit->blockCount();

    //获取光标位置
    QTextCursor cur = ui->textEdit->textCursor();
    int lineNo = cur.blockNumber() + 1;
    int colNo = cur.positionInBlock() + 1;

    //更新状态栏两个标签
    statusLabel.setText(QString("length: %1   lines: %2").arg(textLength).arg(lineCount));
    statusCursorLabel.setText(QString("Ln: %1   Col: %2").arg(lineNo).arg(colNo));
}

void MainWindow::on_actionToolBar_triggered()
{
    bool visible = ui->actionToolBar->isChecked();
    ui->toolBar->setVisible(visible);
}

void MainWindow::on_actionStatusBar_triggered()
{
    bool visible = ui->actionStatusBar->isChecked();
    ui->statusbar->setVisible(visible);
}


void MainWindow::updateEditActionEnable()
{
    //是否存在选中文本
    bool hasSelection = ui->textEdit->textCursor().hasSelection();
    bool hasClipText = !QGuiApplication::clipboard()->text().isEmpty();

    //有选中，剪切复制才可用，否则置灰
    ui->actionCut->setEnabled(hasSelection);
    ui->actionCopy->setEnabled(hasSelection);

    //剪贴板有文字，粘贴才可用
    ui->actionPaste->setEnabled(hasClipText);
}


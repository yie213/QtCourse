#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QCloseEvent>

#include "finddialog.h"
#include "replacedialog.h"
#include "codeeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionFind_triggered();
    void onFindNext(QString keyword, bool caseSensitive, bool searchDown);

    void on_actionReplace_triggered();
    void onReplaceFind(QString keyword, bool caseSensitive, bool searchDown);
    void onReplaceOne(QString keyword, QString replaceText, bool caseSensitive, bool searchDown);
    void onReplaceAll(QString keyword, QString replaceText, bool caseSensitive);

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    // 文本内容变化触发，用来更新修改标记
    void onTextChanged();

    void on_actionAutoLineWrap_triggered();
    void on_actionFont_triggered();
    void on_actionFontColor_triggered();
    void on_actionFontBackgroundColor_triggered();
    void on_actionEditorBackgroundColor_triggered();

    void updateStatusInfo();
    void on_actionToolBar_triggered();
    void on_actionStatusBar_triggered();

    void updateEditActionEnable();

    void on_actionExit_triggered();
    void updateUndoRedoEnable(bool canUndo, bool canRedo);

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;  

    QString m_curFilePath;
    bool m_isModified;

    FindDialog* m_findDlg = nullptr;
    ReplaceDialog* m_replaceDlg = nullptr;
};
#endif // MAINWINDOW_H

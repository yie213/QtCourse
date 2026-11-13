#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = nullptr);

    //绘制行号区域
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    //获取行号控件宽度
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    //文本块更新，更新行号宽度
    void updateLineNumberAreaWidth(int newBlockCount);
    //滚动时更新行号区域
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *m_lineNumberArea;
};

//行号侧边小部件
class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumberArea(CodeEditor *editor) : QWidget(editor), m_codeEditor(editor){}
protected:
    void paintEvent(QPaintEvent *event) override
    {
        m_codeEditor->lineNumberAreaPaintEvent(event);
    }
private:
    CodeEditor *m_codeEditor;
};

#endif // CODEEDITOR_H

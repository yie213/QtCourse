#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MasterView; }
QT_END_NAMESPACE

class MasterView : public QMainWindow
{
    Q_OBJECT

public:
    MasterView(QWidget *parent = nullptr);
    ~MasterView();

private:
    Ui::MasterView *ui;
};
#endif // MASTERVIEW_H

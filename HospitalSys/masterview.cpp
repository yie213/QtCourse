#include "masterview.h"
#include "ui_masterview.h"
#include <QDebug>

MasterView::MasterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);
    goLoginView();
}

MasterView::~MasterView()
{
    delete ui;
}

void MasterView::goLoginView()
{
    qDebug()<<"goLoginView";
    loginView = new LoginView(this);
    pushWidgetToStackView(loginView);
    connect(loginView,SIGNAL(loginSuccess()),this,SLOT(goWelcomeView()));
}

void MasterView::goWelcomeView()
{
    qDebug()<<"goWelcomeView";
    welcomeView = new WelcomeView(this);
    pushWidgetToStackView(welcomeView);
    connect(welcomeView,SIGNAL(goDoctorView()),this,SLOT(goDoctorView()));
    connect(welcomeView,SIGNAL(goPatientView()),this,SLOT(goPatientView()));
    connect(welcomeView,SIGNAL(goDepartmentView()),this,SLOT(goDepartmentView()));
}

void MasterView::goDoctorView()
{
    qDebug()<<"goDoctorView";
    doctorView = new DoctorView(this);
    pushWidgetToStackView(doctorView);
}

void MasterView::goDepartmentView()
{
    qDebug()<<"goDepartmentView";
    departmentView = new DepartmentView(this);
    pushWidgetToStackView(departmentView);
}

void MasterView::goPatientEditView()
{
    qDebug()<<"goPatientEditView";
    patientEditView = new PatientEditView(this);
    pushWidgetToStackView(patientEditView);
}

void MasterView::goPatientView()
{
    qDebug()<<"goPatientView";
    patientView = new PatientView(this);
    pushWidgetToStackView(patientView);
    connect(patientView,SIGNAL(goPatientEditView()),this,SLOT(goPatientEditView()));
}

void MasterView::goPreviousView()
{
    int count = ui->stackedWidget->count();
    if(count > 1){
        ui->stackedWidget->setCurrentIndex(count - 2);
        ui->labelTitle->setText(ui->stackedWidget->currentWidget()->windowTitle());
        QWidget *widget = ui->stackedWidget->widget(count-1);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }
}

void MasterView::pushWidgetToStackView(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
    int count = ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(count - 1);
    ui->labelTitle->setText(widget->windowTitle());
}

void MasterView::on_btBack_clicked()
{
    goPreviousView();
}

void MasterView::on_stackedWidget_currentChanged(int arg1)
{
    QWidget *currentWid = ui->stackedWidget->currentWidget();
        if (!currentWid)
        {
            ui->btBack->setEnabled(false);
            ui->btLogout->setEnabled(false);
            return;
        }

        LoginView *login = qobject_cast<LoginView*>(currentWid);
        WelcomeView *welcome = qobject_cast<WelcomeView*>(currentWid);
        int count = ui->stackedWidget->count();

        if (login)
        {
            // 根登录页：返回、登出全部禁用
            ui->btBack->setEnabled(false);
            ui->btLogout->setEnabled(false);
        }
        else if (welcome)
        {
            // 欢迎页：返回禁用，登出启用
            ui->btBack->setEnabled(false);
            ui->btLogout->setEnabled(true);
        }
        else
        {
            // 其他业务子页面：栈长度大于1才允许返回，登出禁用
            ui->btBack->setEnabled(count > 1);
            ui->btLogout->setEnabled(false);
        }

        ui->labelTitle->setText(currentWid->windowTitle());
}

void MasterView::on_btLogout_clicked()
{
    goPreviousView();
}

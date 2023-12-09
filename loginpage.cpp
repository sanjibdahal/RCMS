#include "loginpage.h"
#include "ui_loginpage.h"
#include <QMessageBox>

LoginPage::LoginPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);
}

LoginPage::~LoginPage()
{
    delete ui;
}


void LoginPage::on_loginBtn_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if(username == "admin@gmail.com" && password == "admin@123") {
        this->hide();
        AdminDashboard *admindashboard = new AdminDashboard();
        admindashboard->showMaximized();
    }
    else {
        //QMessageBox::warning(this,"Dhulikhel Recreational Center","Username or password incorrect!");
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setText("Username or password incorrect!");
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setWindowTitle("Dhulikhel Recreational Center");
        msgBox->setStyleSheet("QLabel{font-size: 18px; color: #fff; font-weight: 400; font-family: 'Poppins';} QPushButton{"
                              "color: #fff; font-family: 'Poppins' }");
        msgBox->exec();
    }
}


void LoginPage::on_signupBtn_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}


void LoginPage::on_loginBtn_2_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}


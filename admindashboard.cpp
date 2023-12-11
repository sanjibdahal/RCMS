#include "admindashboard.h"
#include "ui_admindashboard.h"

AdminDashboard::AdminDashboard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminDashboard)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

}

AdminDashboard::~AdminDashboard()
{
    delete ui;
}

void AdminDashboard::on_logoutBtn_clicked()
{
    LoginPage *loginpage = new LoginPage();
    loginpage->showMaximized();
    this->close();
}


void AdminDashboard::on_dashboardBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void AdminDashboard::on_membersBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void AdminDashboard::on_bookingBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void AdminDashboard::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void AdminDashboard::on_pushButton_addBooking_clicked()
{

}


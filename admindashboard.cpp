#include "admindashboard.h"
#include "ui_admindashboard.h"

#define path_to_database "E:/Qt Projects/RCMS2/database/database.db"

AdminDashboard::AdminDashboard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminDashboard)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->comboBox->setItemData(0, 0, Qt::UserRole - 1);

    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(path_to_database);

    if(DB.open()) {
        qDebug() << "Database connected.";
    }
    else {
        qDebug() << "Database not connected.";
        qDebug() << "Error: " << DB.lastError();
    }
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


void AdminDashboard::on_refreshtableBtn_clicked()
{
    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM user_details");
    int numberofRowstoShow = 10;

    if (query.exec()) {
        ui->tableWidget->setRowCount(numberofRowstoShow);
        qDebug() << "Rows: " << query.size();
        int RowNumber = 0;

        while(query.next())
        {
            ui->tableWidget->setItem(RowNumber, 0, new QTableWidgetItem(QString(query.value("id").toString())));
            ui->tableWidget->setItem(RowNumber, 1, new QTableWidgetItem(QString(query.value("name").toString())));
            ui->tableWidget->setItem(RowNumber, 2, new QTableWidgetItem(QString(query.value("gender").toString())));
            ui->tableWidget->setItem(RowNumber, 3, new QTableWidgetItem(QString(query.value("phone_number").toString())));
            ui->tableWidget->setItem(RowNumber, 4, new QTableWidgetItem(QString(query.value("address").toString())));
            ui->tableWidget->setItem(RowNumber, 5, new QTableWidgetItem(QString(query.value("service").toString())));
            ui->tableWidget->setItem(RowNumber, 6, new QTableWidgetItem(QString(query.value("date").toString())));
            RowNumber++;
        }


    } else {
        // Login failed, show error message
        qDebug() << "Data loading to table error.";
    }

    DB.close();
}










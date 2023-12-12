#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QMainWindow>
#include "loginpage.h"
#include <QtSql>

namespace Ui {
class AdminDashboard;
}

class AdminDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminDashboard(QWidget *parent = nullptr);
    ~AdminDashboard();

private slots:
    void on_logoutBtn_clicked();

    void on_dashboardBtn_clicked();

    void on_membersBtn_clicked();

    void on_bookingBtn_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_addBooking_clicked();

    void on_refreshtableBtn_clicked();

private:
    Ui::AdminDashboard *ui;
    QSqlDatabase DB;
};

#endif // ADMINDASHBOARD_H

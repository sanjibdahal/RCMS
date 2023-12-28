#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QMainWindow>
#include "loginpage.h"
#include <QtSql>
#include <QMessageBox>
#include <QIntValidator>

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

    void refreshTable();

    void on_pushButton_clicked();

    void on_pushButton_updateBooking_clicked();

    void on_pushButton_updateInfo_clicked();

    void selectedPushButton(QPushButton *button);

    void deselectedPushButton(QPushButton *button);

    void on_showBookingsBtn_clicked();

    bool checkCapacity(const QString& service, const QString& date, const QString& time);

private:
    Ui::AdminDashboard *ui;
    QSqlDatabase DB;
    int swimmingCapacity = 40;
    int spaCapacity = 5;
    int saunaCapacity = 15;
};

#endif // ADMINDASHBOARD_H

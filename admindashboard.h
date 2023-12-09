#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QMainWindow>
#include "loginpage.h"

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

private:
    Ui::AdminDashboard *ui;
};

#endif // ADMINDASHBOARD_H

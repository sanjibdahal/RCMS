#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QMainWindow>
#include "loginpage.h"

namespace Ui {
class RegisterPage;
}

class RegisterPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr);
    ~RegisterPage();

private slots:
    void on_loginBtn_clicked();

private:
    Ui::RegisterPage *ui;
};

#endif // REGISTERPAGE_H

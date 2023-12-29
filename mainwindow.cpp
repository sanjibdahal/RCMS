#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //qDebug() << QCoreApplication::applicationDirPath();
    //qDebug() << QCoreApplication::applicationFilePath();
    QString directoryPath = QCoreApplication::applicationDirPath() + QDir::separator() + "database";
    QString databasePath = directoryPath + QDir::separator() + "database.db";

    // Check if the directory exists
    QDir directory(directoryPath);
    if (!directory.exists()) {
        // Create the directory
        if (directory.mkpath(".")) {
            qDebug() << "Directory created successfully.";
        } else {
            qWarning() << "Failed to create the directory.";
        }
    } else {
        qDebug() << "Directory already exists.";
    }

    // Check if the database file exists
    QFile databaseFile(databasePath);
    if (!databaseFile.exists()) {
        // Create the database file
        if (databaseFile.open(QIODevice::ReadWrite)) {
            qDebug() << "Database file created successfully.";
            databaseFile.close();
        } else {
            qWarning() << "Failed to create the database file.";
        }
    } else {
        qDebug() << "Database file already exists.";
    }

    // Set up the SQLite database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);

    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
    } else {
        qDebug() << "Database opened successfully.";
    }

    // Create tables if they do not exist

    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS credentials (id INTEGER PRIMARY KEY AUTOINCREMENT, email TEXT NOT NULL UNIQUE, password TEXT NOT NULL);");

    if (query.exec()) {
        qDebug() << "Table credentials created successfully.";
    } else {
        qWarning() << "Failed to create table credentials" << ":" << query.lastError().text();
    }

    query.prepare("CREATE TABLE IF NOT EXISTS user_details (id INTEGER PRIMARY KEY AUTOINCREMENT, name	TEXT NOT NULL, gender TEXT NOT NULL, "
                  "phone_number TEXT NOT NULL, address TEXT NOT NULL, service TEXT NOT NULL, date TEXT NOT NULL, time TEXT NOT NULL);");

    if (query.exec()) {
        qDebug() << "Table user_details created successfully.";
    } else {
        qWarning() << "Failed to create table user_details"<< ":" << query.lastError().text();
    }

    query.prepare("CREATE TABLE IF NOT EXISTS staff_details (id INTEGER PRIMARY KEY AUTOINCREMENT, name	TEXT NOT NULL, gender TEXT NOT NULL, "
                  "phone_number TEXT NOT NULL, address TEXT NOT NULL, proficiency TEXT NOT NULL, dateofjoining TEXT NOT NULL);");

    if (query.exec()) {
        qDebug() << "Table staff_details created successfully.";
    } else {
        qWarning() << "Failed to create table user_details"<< ":" << query.lastError().text();
    }
    query.prepare("CREATE TABLE IF NOT EXISTS service_capacity (service TEXT NOT NULL, capacity TEXT NOT NULL);");

    if (query.exec()) {
        qDebug() << "Table service_capacity created successfully.";
    } else {
        qWarning() << "Failed to create table user_details"<< ":" << query.lastError().text();
    }

    QStringList services = {"Swimming", "Spa", "Sauna"};
    int capacityList[] = {40, 15, 15};

    for(int i = 0; i < services.size(); i++) {
        query.prepare("SELECT * FROM service_capacity WHERE service = :service");
        query.bindValue(":service", services.at(i));

        if (query.exec() && query.next()) {
            qDebug() << "AITS";
        } else {
            query.prepare("INSERT INTO service_capacity (service, capacity) VALUES (:service, :capacity)");
            query.bindValue(":service", services.at(i));
            //                query.bindValue(":password", password);
            query.bindValue(":capacity", capacityList[i]);

            if (query.exec()) {
                qDebug() << "IS";
            } else {
                qDebug() << "Error: " << query.lastError().text();
            }
        }
    }


    db.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    this->hide();
    LoginPage *loginpage = new LoginPage();
    loginpage->showMaximized();

}


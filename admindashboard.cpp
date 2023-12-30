#include "admindashboard.h"
#include "ui_admindashboard.h"
using namespace std;

AdminDashboard::AdminDashboard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminDashboard)
{
    ui->setupUi(this);
    //ui->stackedWidget->setCurrentIndex(0);
    //ui->comboBox->setItemData(0, 0, Qt::UserRole - 1);
    ui->date->setDate(QDate::currentDate());
    ui->bookDate->setDate(QDate::currentDate());
    ui->sdate->setDate(QDate::currentDate());

    this->setStyleSheet("QMessageBox::QLabel{font-size: 18px; color: #fff; font-weight: 400; font-family: 'Poppins';} QMessageBox::QPushButton{color: #fff; font-family: 'Poppins' } QCalendarWidget { background: #777; }");

    QValidator* intValidator = new QIntValidator(this);
    ui->phone_number->setValidator(intValidator);
    ui->updatephone_number->setValidator(intValidator);


    QString path_to_database = QCoreApplication::applicationDirPath() + QDir::separator() + "database" + QDir::separator() + "database.db";

    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(path_to_database);

    if(DB.open()) {
        qDebug() << "Database connected.";
    }
    else {
        qDebug() << "Database not connected.";
        qDebug() << "Error: " << DB.lastError();
    }
    on_dashboardBtn_clicked();
}

AdminDashboard::~AdminDashboard()
{
    delete ui;
}

void AdminDashboard::refreshDashboard() {
    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }

    QSqlQuery query;
    int swimmingCapacity=0, saunaCapacity=0, spaCapacity=0, totalCapacity=0;
    query.prepare("SELECT * FROM service_capacity WHERE service = :service");
    query.bindValue(":service", "Swimming");
    if(query.exec() && query.next()) {
        swimmingCapacity = query.value("capacity").toInt();
    }
    query.prepare("SELECT * FROM service_capacity WHERE service = :service");
    query.bindValue(":service", "Sauna");
    if(query.exec() && query.next()) {
        saunaCapacity = query.value("capacity").toInt();
    }
    query.prepare("SELECT * FROM service_capacity WHERE service = :service");
    query.bindValue(":service", "Spa");
    if(query.exec() && query.next()) {
        spaCapacity = query.value("capacity").toInt();
    }
    totalCapacity = swimmingCapacity + saunaCapacity + spaCapacity;
    ui->total_capacity->setText(QString::number(totalCapacity));
    ui->available_services->setText("3");

    QString date = QString(QDate::currentDate().toString("yyyy/M/d"));
    qDebug() << "Date: " << date;
    query.prepare("SELECT * FROM user_details WHERE date = :date");
    query.bindValue(":date", date);
    if (query.exec()) {
        int RowNumber = 0;

        while(query.next()) {
            RowNumber++;
        }
        ui->booked_services->setText(QString::number(RowNumber));
    } else {
        qDebug() << "Data loading to table error.";
    }

    query.prepare("SELECT * FROM staff_details");
    if (query.exec()) {
        int RowNumber = 0;

        while(query.next()) {
            RowNumber++;
        }
        ui->total_staffs->setText(QString::number(RowNumber));
    } else {
        qDebug() << "Data loading to table error.";
    }
    DB.close();

}

void AdminDashboard::refreshTable() {
    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM user_details");
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget->setColumnWidth(1,200);
    ui->tableWidget->setColumnWidth(2,120);
    ui->tableWidget->setColumnWidth(3,200);
    ui->tableWidget->setColumnWidth(4,180);
    ui->tableWidget->setColumnWidth(5,125);
    ui->tableWidget->setColumnWidth(6,125);

    if (query.exec()) {
        int RowNumber = 0;

        while(query.next())
        {
            ui->tableWidget->insertRow(RowNumber);
            ui->tableWidget->setRowHeight(RowNumber,40);
            ui->tableWidget->setItem(RowNumber, 0, new QTableWidgetItem(QString("%1-%2").arg(QDate::currentDate().year()).arg(query.value("id").toString())));
            ui->tableWidget->setItem(RowNumber, 1, new QTableWidgetItem(QString(query.value("name").toString())));
            ui->tableWidget->setItem(RowNumber, 2, new QTableWidgetItem(QString(query.value("gender").toString())));
            ui->tableWidget->setItem(RowNumber, 3, new QTableWidgetItem(QString(query.value("phone_number").toString())));
            ui->tableWidget->setItem(RowNumber, 4, new QTableWidgetItem(QString(query.value("address").toString())));
            ui->tableWidget->setItem(RowNumber, 5, new QTableWidgetItem(QString(query.value("service").toString())));
            ui->tableWidget->setItem(RowNumber, 6, new QTableWidgetItem(QString(query.value("date").toString())));
            ui->tableWidget->setItem(RowNumber, 7, new QTableWidgetItem(QString(query.value("time").toString())));
            RowNumber++;
        }
    } else {
        // Login failed, show error message
        qDebug() << "Data loading to table error.";
    }

    DB.close();
}

void AdminDashboard::on_logoutBtn_clicked()
{
    deselectedPushButton(ui->dashboardBtn);
    deselectedPushButton(ui->bookingBtn);
    deselectedPushButton(ui->membersBtn);
    deselectedPushButton(ui->miscellaneousBtn);
    selectedPushButton(ui->logoutBtn);
    LoginPage *loginpage = new LoginPage();
    loginpage->showMaximized();
    this->close();
}


void AdminDashboard::on_dashboardBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    refreshDashboard();
    selectedPushButton(ui->dashboardBtn);
    deselectedPushButton(ui->bookingBtn);
    deselectedPushButton(ui->membersBtn);
    deselectedPushButton(ui->miscellaneousBtn);
    deselectedPushButton(ui->logoutBtn);
}


void AdminDashboard::on_membersBtn_clicked()
{
    refreshTable();
    ui->stackedWidget->setCurrentIndex(3);
    deselectedPushButton(ui->dashboardBtn);
    deselectedPushButton(ui->bookingBtn);
    selectedPushButton(ui->membersBtn);
    deselectedPushButton(ui->miscellaneousBtn);
    deselectedPushButton(ui->logoutBtn);
}


void AdminDashboard::on_bookingBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    on_showBookingsBtn_clicked();
    deselectedPushButton(ui->dashboardBtn);
    selectedPushButton(ui->bookingBtn);
    deselectedPushButton(ui->membersBtn);
    deselectedPushButton(ui->miscellaneousBtn);
    deselectedPushButton(ui->logoutBtn);
}


void AdminDashboard::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void AdminDashboard::on_pushButton_updateInfo_clicked()
{
    QModelIndexList selectedIndexes = ui->tableWidget->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        qDebug() << "No row selected.";
        QMessageBox::warning(this, "Invalid selection","No row is selected.",QMessageBox::Ok);
        return;
    }
    ui->stackedWidget->setCurrentIndex(2);
    int selectedRow = selectedIndexes.first().row();
    string id = ui->tableWidget->item(selectedRow, 0)->text().toStdString();
    id = id.substr(id.find("-") + 1);

    ui->updateid->setText(QString::fromStdString(id));
    ui->updatename->setText(ui->tableWidget->item(selectedRow, 1)->text());
    ui->updatephone_number->setText(ui->tableWidget->item(selectedRow, 3)->text());
    ui->updateaddress->setText(ui->tableWidget->item(selectedRow, 4)->text());

    if(ui->tableWidget->item(selectedRow, 2)->text() == "Male") {
        ui->maleradioButton1->setChecked(true);
    }
    else if(ui->tableWidget->item(selectedRow, 2)->text() == "Female") {
        ui->femaleradioButton1->setChecked(true);
    }
    else if(ui->tableWidget->item(selectedRow, 2)->text() == "Others") {
        ui->othersradioButton1->setChecked(true);
    }
    ui->updateservice->setCurrentText(ui->tableWidget->item(selectedRow, 5)->text());
    ui->updateDate->setDate(QDate::fromString(ui->tableWidget->item(selectedRow, 6)->text(), "yyyy/M/d"));
    ui->updatetimeSlot->setCurrentText(ui->tableWidget->item(selectedRow, 7)->text());
}

void AdminDashboard::on_pushButton_addBooking_clicked()
{

    QString name, gender, phone_number, address, service, date, time;

    name = ui->name->text();
    if(ui->femaleradioButton->isChecked()) {
        gender = ui->femaleradioButton->text();
    }
    else if(ui->maleradioButton->isChecked()) {
        gender = ui->maleradioButton->text();
    }
    else if(ui->othersradioButton->isChecked()) {
        gender = ui->othersradioButton->text();
    }

    phone_number = ui->phone_number->text();
    address = ui->address->text();
    service = ui->service->currentText();
    date = ui->date->text();
    time = ui->timeSlot->currentText();


    qDebug() << name << gender << phone_number << address << service << date << time;

    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }


    if(!name.isEmpty() && !gender.isEmpty() && !phone_number.isEmpty() && !address.isEmpty() && !service.isEmpty() && !date.isEmpty() && !time.isEmpty() && phone_number.length()==10) {
        if(checkCapacity(service, date, time)) {
            QSqlQuery query;
            query.prepare("INSERT INTO user_details (name, gender, phone_number, address, service, date, time) VALUES (:name, :gender, "
                          ":phone_number, :address, :service, :date, :time)");
            query.bindValue(":name", name);
            query.bindValue(":gender", gender);
            query.bindValue(":phone_number", phone_number);
            query.bindValue(":address", address);
            query.bindValue(":service", service);
            query.bindValue(":date", date);
            query.bindValue(":time", time);

            if (query.exec()) {
                qDebug() << "Data inserted successfully";
                QMessageBox::information(this,"Successful", "User inserted successfully.", QMessageBox::Ok);
                ui->name->clear();
                ui->phone_number->clear();
                ui->address->clear();
                ui->maleradioButton->setChecked(false);
                ui->femaleradioButton->setChecked(false);
                ui->othersradioButton->setChecked(false);
                ui->service->setCurrentIndex(-1);
                ui->date->setDate(QDate::currentDate());
                ui->timeSlot->setCurrentIndex(-1);
            } else {
                qDebug() << "Error: " << query.lastError().text();
            }
        } else {
            QMessageBox::warning(this, "Booking error", "The selected time slot is full for the chosen service.", QMessageBox::Ok);
        }
    } else {
        QMessageBox::warning(this,"Booking error", "All fields are required and cannot be empty.", QMessageBox::Ok);
    }

    DB.close();

}


bool AdminDashboard::checkCapacity(const QString &service, const QString &date, const QString &time) {
    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
    }

    // Query to get the counts for each service, date, and time
    QSqlQuery query;
    query.prepare("SELECT * FROM user_details WHERE service = :service AND date = :date AND time = :time");
    query.bindValue(":service", service);
    query.bindValue(":date", date);
    query.bindValue(":time", time);

    int count = 0;
    if (query.exec()) {
        while(query.next())
        {
            count++;
        }
    } else {
        qDebug() << "Error: " << query.lastError().text();
        return false; // Error in querying the database
    }

    // Set the maximum capacity based on service type
    query.prepare("SELECT * FROM service_capacity WHERE service = :service");
    query.bindValue(":service", service);
    int maxCapacity = 0;
    if(query.exec() && query.next()) {
        maxCapacity = query.value("capacity").toInt();
    }

    // checking the capacity is full or not.
    if(count < maxCapacity) {
        return true;
    } else {
        return false;
    }
}


void AdminDashboard::selectedPushButton(QPushButton *button)
{
    button->setStyleSheet("QPushButton { background: #307970; color:#fff; text-align: left; padding-left:20; border: none; border-left:6px solid #2fc5b3; outline: none; } QPushButton:hover { background: #307970; }");
}

void AdminDashboard::deselectedPushButton(QPushButton *button)
{
    button->setStyleSheet("QPushButton { background: #2fc5b3; color:#fff; text-align: left; padding-left:20; border: none; outline: none; } QPushButton:hover { background: #307970; }");
}


void AdminDashboard::on_refreshtableBtn_clicked()
{
    refreshTable();
}

//delete record
void AdminDashboard::on_pushButton_clicked()
{
    QModelIndexList selectedIndexes = ui->tableWidget->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        qDebug() << "No row selected.";
        QMessageBox::warning(this, "Invalid selection","No row is selected.",QMessageBox::Ok);
        return;
    }

    int selectedRow = selectedIndexes.first().row();

    string id = ui->tableWidget->item(selectedRow, 0)->text().toStdString();
    id = id.substr(id.find("-") + 1);
    QString idToDelete = QString::fromStdString(id);
    qDebug() << idToDelete;

    if (QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete this record?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        // Perform the deletion here using the ID or other identifying information
        if (!DB.open()) {
            qDebug() << "Failed to open the database.";
            return;
        }

        QSqlQuery query;
        query.prepare("DELETE FROM user_details WHERE id = :id");
        query.bindValue(":id", idToDelete);

        if (!query.exec()) {
            qDebug() << "Error deleting record:" << query.lastError().text();
        }

        DB.close();

        // Refresh the table after deletion
        refreshTable();
    }
}


void AdminDashboard::on_pushButton_updateBooking_clicked()
{
    QString name, gender, phone_number, address, service, date, id, time;

    name = ui->updatename->text();
    if(ui->femaleradioButton1->isChecked()) {
        gender = ui->femaleradioButton1->text();
    }
    else if(ui->maleradioButton1->isChecked()) {
        gender = ui->maleradioButton1->text();
    }
    else if(ui->othersradioButton1->isChecked()) {
        gender = ui->othersradioButton1->text();
    }

    phone_number = ui->updatephone_number->text();
    address = ui->updateaddress->text();
    service = ui->updateservice->currentText();
    date = ui->updateDate->text();
    id = ui->updateid->text();
    time = ui->updatetimeSlot->currentText();

    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }


    if(!name.isEmpty() && !gender.isEmpty() && !phone_number.isEmpty() && !address.isEmpty() && !service.isEmpty() && !date.isEmpty() && !time.isEmpty() && phone_number.length()==10) {
        if(checkCapacity(service,date,time)) {
            QSqlQuery query;

            query.prepare("UPDATE user_details SET name = :name, gender = :gender, phone_number = :phone_number, address = :address, service = :service, date = :date, time = :time WHERE id = :id");
            query.bindValue(":name", name);
            query.bindValue(":gender", gender);
            query.bindValue(":phone_number", phone_number);
            query.bindValue(":address", address);
            query.bindValue(":service", service);
            query.bindValue(":date", date);
            query.bindValue(":id",id);
            query.bindValue(":time",time);

            if (query.exec()) {
                qDebug() << "Data updated successfully";
                QMessageBox::information(this,"Successful", "User information updated successfully.", QMessageBox::Ok);
                ui->updatename->clear();
                ui->updateid->clear();
                ui->updatephone_number->clear();
                ui->updateaddress->clear();
                ui->maleradioButton1->setChecked(false);
                ui->femaleradioButton1->setChecked(false);
                ui->othersradioButton1->setChecked(false);
                ui->updateservice->setCurrentIndex(-1);
                ui->updateDate->setDate(QDate::currentDate());
                ui->updatetimeSlot->setCurrentIndex(-1);
                refreshTable();
                ui->stackedWidget->setCurrentIndex(3);

            } else {
                qDebug() << "Error: " << query.lastError().text();
            }
        } else {
            QMessageBox::warning(this, "Booking error", "The selected time slot is full for the chosen service.", QMessageBox::Ok);
        }
    } else {
        QMessageBox::warning(this,"Updating error", "All fields are required and cannot be empty.", QMessageBox::Ok);
    }

    DB.close();
}

void AdminDashboard::on_showBookingsBtn_clicked()
{
    QString date, time;
    date = ui->bookDate->text();
    time = ui->bookTimeSlot->currentText();
    int count_Swimming = 0, count_Sauna = 0, count_Spa = 0;

    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM user_details WHERE date = :date AND time = :time");
    query.bindValue(":date", date);
    query.bindValue(":time", time);

    if (query.exec()) {

        while(query.next())
        {
            if (query.value("service").toString() == "Swimming") {
                count_Swimming++;
            }
            else if(query.value("service").toString() == "Sauna") {
                count_Sauna++;
            }
            else if(query.value("service").toString() == "Spa") {
                count_Spa++;
            }
        }

        int swimmingCapacity=0, saunaCapacity=0, spaCapacity=0;
        query.prepare("SELECT * FROM service_capacity WHERE service = :service");
        query.bindValue(":service", "Swimming");
        if(query.exec() && query.next()) {
            swimmingCapacity = query.value("capacity").toInt();
        }
        query.prepare("SELECT * FROM service_capacity WHERE service = :service");
        query.bindValue(":service", "Sauna");
        if(query.exec() && query.next()) {
            saunaCapacity = query.value("capacity").toInt();
        }
        query.prepare("SELECT * FROM service_capacity WHERE service = :service");
        query.bindValue(":service", "Spa");
        if(query.exec() && query.next()) {
            spaCapacity = query.value("capacity").toInt();
        }

        ui->countSwimming->setText("Booked: " + QString::number(count_Swimming));
        ui->countSauna->setText("Booked: " + QString::number(count_Sauna));
        ui->countSpa->setText("Booked: " + QString::number(count_Spa));
        ui->remainingSwimming->setText("Remaining: " + QString::number(swimmingCapacity - count_Swimming));
        ui->remainingSauna->setText("Remaining: " + QString::number(saunaCapacity - count_Sauna));
        ui->remainingSpa->setText("Remaining: " + QString::number(spaCapacity - count_Spa));

    } else {
        // Login failed, show error message
        qDebug() << "Data loading to bookings error.";
    }

    DB.close();
}


void AdminDashboard::on_miscellaneousBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    deselectedPushButton(ui->dashboardBtn);
    deselectedPushButton(ui->bookingBtn);
    deselectedPushButton(ui->membersBtn);
    selectedPushButton(ui->miscellaneousBtn);
    deselectedPushButton(ui->logoutBtn);
    on_staffrefreshtableBtn_clicked();
}


void AdminDashboard::on_staffrefreshtableBtn_clicked()
{
    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM staff_details");
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnWidth(0,100);
    ui->tableWidget_2->setColumnWidth(1,200);
    ui->tableWidget_2->setColumnWidth(2,120);
    ui->tableWidget_2->setColumnWidth(3,200);
    ui->tableWidget_2->setColumnWidth(4,180);
    ui->tableWidget_2->setColumnWidth(5,125);

    if (query.exec()) {
        int RowNumber = 0;

        while(query.next())
        {
            ui->tableWidget_2->insertRow(RowNumber);
            ui->tableWidget_2->setRowHeight(RowNumber,40);
            ui->tableWidget_2->setItem(RowNumber, 0, new QTableWidgetItem(QString("%1-%2").arg(QDate::currentDate().year()).arg(query.value("id").toString())));
            ui->tableWidget_2->setItem(RowNumber, 1, new QTableWidgetItem(QString(query.value("name").toString())));
            ui->tableWidget_2->setItem(RowNumber, 2, new QTableWidgetItem(QString(query.value("gender").toString())));
            ui->tableWidget_2->setItem(RowNumber, 3, new QTableWidgetItem(QString(query.value("phone_number").toString())));
            ui->tableWidget_2->setItem(RowNumber, 4, new QTableWidgetItem(QString(query.value("address").toString())));
            ui->tableWidget_2->setItem(RowNumber, 5, new QTableWidgetItem(QString(query.value("proficiency").toString())));
            ui->tableWidget_2->setItem(RowNumber, 6, new QTableWidgetItem(QString(query.value("dateofjoining").toString())));
            RowNumber++;
        }
    } else {
        // Login failed, show error message
        qDebug() << "Data loading to table error.";
    }

    DB.close();
}


void AdminDashboard::on_addStaff_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}


void AdminDashboard::on_deleteStaff_clicked()
{
    QModelIndexList selectedIndexes = ui->tableWidget_2->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        qDebug() << "No row selected.";
        QMessageBox::warning(this, "Invalid selection","No row is selected.",QMessageBox::Ok);
        return;
    }

    int selectedRow = selectedIndexes.first().row();

    string id = ui->tableWidget_2->item(selectedRow, 0)->text().toStdString();
    id = id.substr(id.find("-") + 1);
    QString idToDelete = QString::fromStdString(id);
    qDebug() << idToDelete;

    if (QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete this record?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        // Perform the deletion here using the ID or other identifying information
        if (!DB.open()) {
            qDebug() << "Failed to open the database.";
            return;
        }

        QSqlQuery query;
        query.prepare("DELETE FROM staff_details WHERE id = :id");
        query.bindValue(":id", idToDelete);

        if (!query.exec()) {
            qDebug() << "Error deleting record:" << query.lastError().text();
        }

        DB.close();

        // Refresh the table after deletion
        on_staffrefreshtableBtn_clicked();
    }
}


void AdminDashboard::on_updateStaff_clicked()
{
    QModelIndexList selectedIndexes = ui->tableWidget_2->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        qDebug() << "No row selected.";
        QMessageBox::warning(this, "Invalid selection","No row is selected.",QMessageBox::Ok);
        return;
    }
    ui->stackedWidget->setCurrentIndex(7);
    int selectedRow = selectedIndexes.first().row();
    string id = ui->tableWidget_2->item(selectedRow, 0)->text().toStdString();
    id = id.substr(id.find("-") + 1);

    ui->supdateid->setText(QString::fromStdString(id));
    ui->supdatename->setText(ui->tableWidget_2->item(selectedRow, 1)->text());
    ui->supdatephone_number->setText(ui->tableWidget_2->item(selectedRow, 3)->text());
    ui->supdateaddress->setText(ui->tableWidget_2->item(selectedRow, 4)->text());

    if(ui->tableWidget_2->item(selectedRow, 2)->text() == "Male") {
        ui->smaleradioButton1->setChecked(true);
    }
    else if(ui->tableWidget_2->item(selectedRow, 2)->text() == "Female") {
        ui->sfemaleradioButton1->setChecked(true);
    }
    else if(ui->tableWidget_2->item(selectedRow, 2)->text() == "Others") {
        ui->sothersradioButton1->setChecked(true);
    }
    ui->supdateservice->setCurrentText(ui->tableWidget_2->item(selectedRow, 5)->text());
    ui->supdateDate->setDate(QDate::fromString(ui->tableWidget_2->item(selectedRow, 6)->text(), "yyyy/M/d"));
}


void AdminDashboard::on_updateCapacity_clicked()
{
    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }
    QString service;
    int capacity;
    service = ui->Mservice->currentText();
    capacity = ui->capacity->text().toInt();
    QSqlQuery query;
    query.prepare("UPDATE service_capacity SET service = :service, capacity = :capacity WHERE service = :service");
    query.bindValue(":service", service);
    query.bindValue(":capacity", capacity);
    if (query.exec()) {
        QMessageBox::information(this,"Successful", "Service's capacity updated successfully.", QMessageBox::Ok);
    } else {
        qDebug() << "Error: " << query.lastError().text();
    }

}


void AdminDashboard::on_Mservice_currentTextChanged(const QString &arg1)
{
    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM service_capacity WHERE service = :service");
    query.bindValue(":service", arg1);
    int Capacity = 0;
    if(query.exec() && query.next()) {
        Capacity = query.value("capacity").toInt();
    } else {
        qDebug() << "Hi";
    }
    ui->capacity->setValue(Capacity);
}


void AdminDashboard::on_s_addStaff_clicked()
{
    QString name, gender, phone_number, address, service, date;

    name = ui->sname->text();
    if(ui->sfemaleradioButton->isChecked()) {
        gender = ui->sfemaleradioButton->text();
    }
    else if(ui->smaleradioButton->isChecked()) {
        gender = ui->smaleradioButton->text();
    }
    else if(ui->sothersradioButton->isChecked()) {
        gender = ui->sothersradioButton->text();
    }

    phone_number = ui->sphone_number->text();
    address = ui->saddress->text();
    service = ui->sservice->currentText();
    date = ui->sdate->text();


    qDebug() << name << gender << phone_number << address << service << date;

    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }


    if(!name.isEmpty() && !gender.isEmpty() && !phone_number.isEmpty() && !address.isEmpty() && !service.isEmpty() && !date.isEmpty() && phone_number.length()==10) {
            QSqlQuery query;
            query.prepare("INSERT INTO staff_details (name, gender, phone_number, address, proficiency, dateofjoining) VALUES (:name, :gender, "
                          ":phone_number, :address, :proficiency, :dateofjoining)");
            query.bindValue(":name", name);
            query.bindValue(":gender", gender);
            query.bindValue(":phone_number", phone_number);
            query.bindValue(":address", address);
            query.bindValue(":proficiency", service);
            query.bindValue(":dateofjoining", date);

            if (query.exec()) {
                qDebug() << "Data inserted successfully";
                QMessageBox::information(this,"Insertion Successful", "Staff inserted successfully.", QMessageBox::Ok);
                ui->sname->clear();
                ui->sphone_number->clear();
                ui->saddress->clear();
                ui->smaleradioButton->setChecked(false);
                ui->sfemaleradioButton->setChecked(false);
                ui->sothersradioButton->setChecked(false);
                ui->sservice->setCurrentIndex(-1);
                ui->sdate->setDate(QDate::currentDate());
                on_staffrefreshtableBtn_clicked();
                ui->stackedWidget->setCurrentIndex(5);
            } else {
                qDebug() << "Error: " << query.lastError().text();
            }
    } else {
        QMessageBox::warning(this,"Insertion error", "All fields are required and cannot be empty.", QMessageBox::Ok);
    }

    DB.close();
}


void AdminDashboard::on_supdateStaff_clicked()
{
    QString name, gender, phone_number, address, service, date, id;

    name = ui->supdatename->text();
    if(ui->sfemaleradioButton1->isChecked()) {
        gender = ui->sfemaleradioButton1->text();
    }
    else if(ui->smaleradioButton1->isChecked()) {
        gender = ui->smaleradioButton1->text();
    }
    else if(ui->sothersradioButton1->isChecked()) {
        gender = ui->sothersradioButton1->text();
    }

    phone_number = ui->supdatephone_number->text();
    address = ui->supdateaddress->text();
    service = ui->supdateservice->currentText();
    date = ui->supdateDate->text();
    id = ui->supdateid->text();

    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }


    if(!name.isEmpty() && !gender.isEmpty() && !phone_number.isEmpty() && !address.isEmpty() && !service.isEmpty() && !date.isEmpty() && phone_number.length()==10) {
        QSqlQuery query;

        query.prepare("UPDATE staff_details SET name = :name, gender = :gender, phone_number = :phone_number, address = :address, proficiency = :proficiency, dateofjoining = :dateofjoining WHERE id = :id");
        query.bindValue(":name", name);
        query.bindValue(":gender", gender);
        query.bindValue(":phone_number", phone_number);
        query.bindValue(":address", address);
        query.bindValue(":proficiency", service);
        query.bindValue(":dateofjoining", date);
        query.bindValue(":id",id);

        if (query.exec()) {
                qDebug() << "Staff information updated successfully";
                QMessageBox::information(this,"Successful", "Staff's information updated successfully.", QMessageBox::Ok);
                ui->supdatename->clear();
                ui->supdateid->clear();
                ui->supdatephone_number->clear();
                ui->supdateaddress->clear();
                ui->smaleradioButton1->setChecked(false);
                ui->sfemaleradioButton1->setChecked(false);
                ui->sothersradioButton1->setChecked(false);
                ui->supdateservice->setCurrentIndex(-1);
                ui->supdateDate->setDate(QDate::currentDate());
                on_staffrefreshtableBtn_clicked();
                ui->stackedWidget->setCurrentIndex(5);

        } else {
                qDebug() << "Error: " << query.lastError().text();
        }
    } else {
        QMessageBox::warning(this,"Updating error", "All fields are required and cannot be empty.", QMessageBox::Ok);
    }

    DB.close();
}


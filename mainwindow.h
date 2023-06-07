#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <iostream>
#include <fstream>
#include <future>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include "libs/sqlite3.h"
#include "graficasresultadosform.h"
#include "graficadiaform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    static QString execCommand(std::string command);
    void copyFiles(std::string source, std::string target);
    std::string getNameOfPath(std::string path);
    static int callback(void *NotUsed, int argc, char **argv, char **azColName);
    static int callbackSelect(void *NotUsed, int argc, char **argv, char **azColName);
    void parsing_result(QString result);
    std::vector<QString> parse_line(QString line);
    std::vector<std::vector<QString>> parse_Table_ResultadoDB(QString);
    void openDB();
    void closeDB();
    void createTableDB();
    void insertResultDB(std::vector<QString>, std::string);
    void selectByHoraDB(std::string);
    void refreshTable();
    ~MainWindow();

private slots:
    void on_toolButton_3_clicked();
    void on_toolButton_4_clicked();
    void on_toolButton_5_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
    void on_toolButton_4_triggered(QAction *arg1);
    void on_calendarWidget_clicked(const QDate &date);
    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void on_toolButton_6_clicked();
    void on_toolButton_7_clicked();

    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::MainWindow *ui;
    QString nombre_fichero_unitario;
    QString ruta_Folder;
    std::vector<QString> imagen_list;
    std::vector<QString> image_To_Analize;
    QString imagen_seleccionada;
    QTableWidgetItem *item_seleccionado;
    std::vector<std::vector<std::string>> callBackSelectTable;
    sqlite3 *db;
    char *zErrMsg;
    int rc;
    QDate m_date;
    int last_item;
};
#endif // MAINWINDOW_H

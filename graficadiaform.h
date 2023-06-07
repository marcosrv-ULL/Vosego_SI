#ifndef GRAFICADIAFORM_H
#define GRAFICADIAFORM_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QtCharts>
#include "bargrapdiaform.h"
#include "libs/sqlite3.h"

namespace Ui {
class GraficaDiaForm;
}

class GraficaDiaForm : public QWidget
{
    Q_OBJECT

public:
    explicit GraficaDiaForm(std::vector<std::vector<std::string> > tablaSelect, QWidget *parent = nullptr);
    void selectByHoraDB(std::string);
    void createCharts();
    void openDB();
    void closeDB();
    ~GraficaDiaForm();

private slots:
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

private:
    Ui::GraficaDiaForm *ui;
    sqlite3 *db;
    char *zErrMsg;
    QChart *chart;
    QPieSeries *series;
    QBarSeries *series1;
    QChartView *chartView;
    QChart *chart1;
    QChartView *chartView1;
    int rc;
    std::vector<std::vector<std::string>> callBackSelectTable;
};

#endif // GRAFICADIAFORM_H

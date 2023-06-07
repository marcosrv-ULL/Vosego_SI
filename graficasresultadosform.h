#ifndef GRAFICASRESULTADOSFORM_H
#define GRAFICASRESULTADOSFORM_H

#include <QWidget>
#include <QtCharts>
#include <QRandomGenerator>
#include <stdlib.h>     /* srand, rand */

namespace Ui {
class GraficasResultadosForm;
}

class GraficasResultadosForm : public QWidget
{
    Q_OBJECT

public:
    explicit GraficasResultadosForm(std::vector<std::vector<std::string>> tablaSelect, QWidget *parent = nullptr);
    ~GraficasResultadosForm();
    void createPieChart(std::vector<std::vector<std::string>> tablaSelect);

private:
    Ui::GraficasResultadosForm *ui;
    std::vector<std::vector<std::string>> tablaSelect;
};

#endif // GRAFICASRESULTADOSFORM_H

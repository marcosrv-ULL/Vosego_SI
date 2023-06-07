#include "graficasresultadosform.h"
#include "ui_graficasresultadosform.h"

GraficasResultadosForm::GraficasResultadosForm(std::vector<std::vector<std::string> > tablaSelect, QWidget *parent):
    QWidget(parent),
    ui(new Ui::GraficasResultadosForm)
{
    ui->setupUi(this);
    QBarSet *set0 = new QBarSet("Diseased");
    QBarSet *set1 = new QBarSet("Healthy");
    int accSaludable = 0;
    int accEnfermo = 0;
    qDebug() << tablaSelect.size();
    for(auto row: tablaSelect) {
        if (row[2] == "Enfermo") {
            accSaludable++;
        } else {
            accEnfermo++;
        }
    }
    *set0 << accSaludable ;
    *set1 << accEnfermo;
    set0->setColor(qRgb(180,0,0));
    set1->setColor(qRgb(0,180,0));
    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Diseased vs Healthy");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QStringList categories;
    categories << "Diseased / Healthy";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    QChartView *chartView = new QChartView(chart);
    chartView->setParent(ui->horizontalFrame);
    QString totalenfermos(std::to_string(accSaludable).c_str());
    QString totalSaludable(std::to_string(accEnfermo).c_str());
    ui->listWidget->addItem("Total of Diseased Crops: " + totalenfermos);
    ui->listWidget->addItem("Total of Healthy Crops: " + totalSaludable);
    createPieChart(tablaSelect);
}

GraficasResultadosForm::~GraficasResultadosForm()
{
    delete ui;
}

void GraficasResultadosForm::createPieChart(std::vector<std::vector<std::string> > tablaSelect)
{
    QPieSeries *series = new QPieSeries();
    QString classWithHighestFrecuency = "NULL";
    int maxInt = 0;
    std::map<std::string, int> podenraciones;
    for(auto row: tablaSelect) {
        podenraciones[row[4]] += 1;
    }

    for (auto const& x : podenraciones)
    {
        series->append(*(new QString(x.first.c_str())), x.second);
        classWithHighestFrecuency = (x.second > maxInt) ? x.first.c_str(): classWithHighestFrecuency;
    }

    srand(std::time(NULL));
    for (auto slice: series->slices())
    {
        QColor color = QColor(rand()%255, rand()%255, rand()%255);
        slice->setLabelVisible();
        slice->setPen(QPen(color, 2));
        slice->setBrush(QBrush(color));
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Class Ratio");
    chart->legend()->hide();
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFramePieChart);
    ui->listWidget->addItem("Class With The Highest Frecuency: " + classWithHighestFrecuency);
}

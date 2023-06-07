#include "bargrapdiaform.h"
#include "ui_bargrapdiaform.h"

barGrapDiaForm::barGrapDiaForm(std::vector<std::vector<std::string>> callbackSelectTable,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::barGrapDiaForm)
{
    ui->setupUi(this);
    QPieSeries *series = new QPieSeries();
    QString classWithHighestFrecuency = "NULL";
    int maxInt = 0;
    std::map<std::string, int> podenraciones;
    this->setWindowTitle(*(new QString((" Diagrams: " + callbackSelectTable[0][3]).c_str())));
    for(auto row: callbackSelectTable) {
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
    chartView->setParent(ui->horizontalFrame_2);
    QBarSet *set0 = new QBarSet("Diseased");
    QBarSet *set1 = new QBarSet("Healthy");
    int accSaludable = 0;
    int accEnfermo = 0;
    qDebug() << callbackSelectTable.size();
    for(auto row: callbackSelectTable) {
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
    QBarSeries *series1 = new QBarSeries();
    series1->append(set0);
    series1->append(set1);
    QChart *chart1 = new QChart();
    chart1->addSeries(series1);
    chart1->setTitle("Diseased vs Healthy");
    chart1->setAnimationOptions(QChart::SeriesAnimations);
    QStringList categories;
    categories << "Diseased / Healthy";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart1->createDefaultAxes();
    chart1->setAxisX(axis, series);

    QChartView *chartView1 = new QChartView(chart1);
    chartView1->setParent(ui->horizontalFrame);
    chart->removeSeries(series);
    chart->addSeries(series);
    chart1->removeSeries(series1);
    chart1->addSeries(series1);
}

barGrapDiaForm::~barGrapDiaForm()
{
    delete ui;
}

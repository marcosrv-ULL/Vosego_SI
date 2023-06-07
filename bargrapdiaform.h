#ifndef BARGRAPDIAFORM_H
#define BARGRAPDIAFORM_H

#include <QWidget>
#include <QtCharts>

namespace Ui {
class barGrapDiaForm;
}

class barGrapDiaForm : public QWidget
{
    Q_OBJECT

public:
    explicit barGrapDiaForm(std::vector<std::vector<std::string>>, QWidget *parent = nullptr);
    ~barGrapDiaForm();

private:
    Ui::barGrapDiaForm *ui;
};

#endif // BARGRAPDIAFORM_H

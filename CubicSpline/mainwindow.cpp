#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    CreateVectors(x, f_x);
    a.resize(points);
    b.resize(points - 1);
    c.resize(points);
    d.resize(points - 1);
    QVector<double> alpha(points - 1);
    QVector<double> delta_x(points - 1);
    QVector<double> delta_y(points - 1);
    for (int i = 0; i < points - 1; i++) {
        delta_x[i] = x[i + 1] - x[i];
        delta_y[i] = f_x[i + 1] - f_x[i];
    }

    for (int i = 0; i < points; i++) {
        a[i] = f_x[i];
    }

    CreateAlpha(a, alpha, delta_x, points);
    CreateC(c, alpha, delta_x, points);
    CreateB(b, c, delta_x, delta_y, points);
    CreateD(d, c, delta_x, points);

    QVector<double> vec;
    for (int i = 0; i < points; i++) {
        vec.push_back(x[i]);
    }
    double i = x[0] + 0.2;
    while(i < x[points - 1]){
        vec.push_back(i);
        i += 0.2;
    }


    double spline = 0;
    QVector<double> cs;
    double min = __DBL_MAX_10_EXP__;
    double max = __DBL_MIN_10_EXP__;
    for(int i = 0; i < vec.size(); i++){
        Interpolate(vec[i], spline);
        cs.push_back(spline);
        if(spline < min) { min = spline; }
        if(spline > max) { max = spline; }
    }
    ui->widget->xAxis->setRange(x[0], x[points - 1]);
    ui->widget->yAxis->setRange(min, max);

    ui->widget->addGraph();
    ui->widget->graph(0)->addData(vec, cs);
    ui->widget->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateVectors(QVector<double> &x, QVector<double> &f_x){
    QFile file("test.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw "Error opening file!";
        }

         QTextStream in(&file);
         in >> points;

         double arg, func;
         int i = 0;
         while (i < points)
         {
             in >> arg;
             if (in.status() != QTextStream::Ok)
                 break;
             x.push_back(arg);

             in >> func;
             if (in.status() != QTextStream::Ok)
                 break;
             f_x.push_back(func);

             i++;
         }
}

QVector<double> MainWindow::CreateAlpha(QVector<double> a, QVector<double> &alpha, QVector<double> delta_x, int points){
    alpha[0] = 0;
        for (int i = 1; i < points - 1; i++) {
            alpha[i] = 3 * ((a[i + 1] - a[i]) / delta_x[i] - (a[i] - a[i - 1]) / delta_x[i - 1]);
        }

        return alpha;
}

QVector<double> MainWindow::CreateC(QVector<double> &c, QVector<double> alpha, QVector<double> delta_x, int points){
    QVector<double> left(points - 1);
    QVector<double> diag(points - 1);
    QVector<double> right(points - 1);

        diag[0] = 1;
        right[0] = 0;

        for (int i = 1; i < points - 1; i++) {
            left[i - 1] = delta_x[i - 1];
            diag[i] = 2 * (delta_x[i - 1] + delta_x[i]);
            right[i] = delta_x[i];
        }

        for (int i = 1; i < points - 1; i++) {
            diag[i] = diag[i] - left[i - 1] * right[i - 1] / diag[i - 1];
            alpha[i] = alpha[i] - left[i - 1] * alpha[i - 1] / diag[i - 1];
        }

        c[points - 1] = 0;
        for (int i = points - 2; i >= 0; i--) {
            c[i] = (alpha[i] - right[i] * c[i + 1]) / diag[i];
        }

        return c;
}

QVector<double> MainWindow::CreateB(QVector<double> &b, QVector<double> c, QVector<double> delta_x, QVector<double> delta_y, int points){
    for (int i = 0; i < points - 1; i++) {
            b[i] = delta_y[i] / delta_x[i] - delta_x[i] * (c[i + 1] + 2 * c[i]) / 3;
        }
        return b;
}

QVector<double> MainWindow::CreateD(QVector<double> &d, QVector<double> c, QVector<double> delta_x, int points){
    for (int i = 0; i < points - 1; i++) {
            d[i] = (c[i + 1] - c[i]) / (3 * delta_x[i]);
        }
        return d;
}

void MainWindow::Interpolate(double value, double& spline){
    if (value < x[0] || value > x[points - 1]) {
            throw std::logic_error("ERROR!!!!!!!!");
        }

        for (int i = 0; i < points - 1; i++) {
            if (value == x[i]) {
                spline = f_x[i];
            }
            else if (value == x[i + 1]) {
                spline = f_x[i + 1];
            }
            else if (value > x[i] && value < x[i + 1]) {
                spline = a[i] +
                    b[i] * (value - x[i]) +
                    c[i] * pow((value - x[i]), 2) +
                    d[i] * pow((value - x[i]), 3);
            }
        }
}

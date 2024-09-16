#include "dialog.hpp"
#include "./ui_dialog.h"
#include <QFileDialog>
#include <QStandardPaths>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , m_schema{this}
{
    ui->setupUi(this);
    setWindowTitle("ZMK Antecedent Morph Config Generator");

    ui->schemaValidity->clear();

    connect(&m_schema, &Schema::logEvent, this, [this](QString const& message){
        ui->log->appendPlainText(message);
    });

    connect(ui->closeButton, &QPushButton::clicked, qApp, &QApplication::quit);

    connect(ui->csvSelectorButton, &QToolButton::clicked, this, [this]{
        QString path = QFileDialog::getOpenFileName(this, "Select CSV", QStandardPaths::standardLocations(QStandardPaths::HomeLocation).value(0, ""), "CSV (*.csv)");
        ui->csvPath->setText(path);
    });

    connect(ui->outputSelectorButton, &QToolButton::clicked, this, [this]{
        QString path = QFileDialog::getSaveFileName(this, "Select output", QStandardPaths::standardLocations(QStandardPaths::HomeLocation).value(0, ""));
        ui->outputPath->setText(path);
    });

    connect(ui->processButton, &QPushButton::clicked, this, [this]{
        if (!m_schema.load(ui->csvPath->text())) {
            ui->schemaValidity->setText("CSV parsing failed");
            return;
        }
        ui->schemaValidity->setText(QString("Schema %1 is %2. Version %3").arg(m_schema.name(), m_schema.isValid() ? "valid" : "invalid", m_schema.version()));

        if (!m_schema.save(ui->outputPath->text())) {
            ui->schemaValidity->setText("Output generation failed");
            return;
        }

        ui->schemaValidity->setText("Done!");
    });
}

Dialog::~Dialog()
{
    delete ui;
}

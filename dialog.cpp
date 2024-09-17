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

    ui->finalResult->clear();

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
        ui->finalResult->clear();
        ui->log->clear();

        if (ui->csvPath->text().trimmed().isEmpty()) {
            ui->log->appendPlainText("No input file is set, so stopping here");
            ui->finalResult->setText("No input file set");
            return;
        }
        if (!m_schema.load(ui->csvPath->text())) {
            ui->log->appendPlainText("Loading has failed");
            ui->finalResult->setText("Loading failed");
            return;
        }

        ui->log->appendPlainText("Loading was successful");

        ui->log->appendPlainText(QString{"Got schema name %1, version %2, %3"}.arg(m_schema.name(), m_schema.version(), m_schema.isValid() ? "valid" : "INVALID"));

        if (!m_schema.isValid()) {
            ui->finalResult->setText("The schema is INVALID");
            return;
        }

        if (ui->outputPath->text().trimmed().isEmpty()) {
            ui->log->appendPlainText("No output file is set, so stopping here");
            ui->finalResult->setText("No output file set");
            return;
        }

        if (!m_schema.save(ui->outputPath->text())) {
            ui->finalResult->setText("Output generation failed");
            return;
        }

        ui->finalResult->setText("Done!");
    });
}

Dialog::~Dialog()
{
    delete ui;
}

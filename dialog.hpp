#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QDialog>
#include "schema.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
    Schema m_schema;
};
#endif // DIALOG_HPP

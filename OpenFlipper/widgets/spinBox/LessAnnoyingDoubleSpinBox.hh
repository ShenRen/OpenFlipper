//=============================================================================
//
//  CLASS LessAnnoyingQDoubleSpinBox
//
//=============================================================================


#ifndef LESSANNOYINGDOUBLESPINBOX_HH
#define LESSANNOYINGDOUBLESPINBOX_HH

#include <QDoubleSpinBox>
#include <QWidget>

class LessAnnoyingDoubleSpinBox : public QDoubleSpinBox
{

public:


  LessAnnoyingDoubleSpinBox(QWidget* _qwidget = Q_NULLPTR ) : QDoubleSpinBox(_qwidget)
  {
  }

  virtual QValidator::State validate(QString& text, int&) const override
  {
    QString copy = strip_prefix_suffix(text) + "0";

    bool ok;
    copy.toDouble(&ok);
    if ( ok )
      return QValidator::Acceptable;
    else
      return QValidator::Invalid;
  }

  virtual double valueFromText(const QString &text) const override
  {
    QString copy = strip_prefix_suffix(text);

    bool ok;
    double value = copy.toDouble(&ok);

    double factor = pow(10.0, decimals() );
    value = double( long(value * double(factor) + 0.5) ) / factor;

    if ( !ok )
      return 0;

    return value;
  }

private:

  QString strip_prefix_suffix(const QString& text) const
  {
    int lenpre = prefix().length();
    int lensuf = suffix().length();

    return text.mid( lenpre, text.length() - lenpre -lensuf ).simplified();
  }

};

//=============================================================================
#endif // LESSANNOYINGDOUBLESPINBOX_HH defined
//=============================================================================


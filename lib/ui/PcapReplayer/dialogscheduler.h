#ifndef DIALOGSCHEDULER_H
#define DIALOGSCHEDULER_H

#include <QDialog>
#include <QtCore/qdatetime.h>
#include <QtCore/qvector.h>
#include <QMessageBox>

#include "Common.h"

namespace Ui {
class DialogScheduler;
}

class DialogScheduler : public QDialog
{
    Q_OBJECT

public:
	QDate m_schedularDate;
	QTime m_schedularTime;
	bool m_bOneTimeOnly;
	bool m_bSchularEnable;
	QVector<bool> m_vecWeekDaysChecked; // from monday to sunday
public:
    explicit DialogScheduler(QWidget *parent = 0);
    ~DialogScheduler();

	void Init_UIScreen();
	bool HandleCheckBoxWeekday();
	// Doc
	void Serialize(bool bStore, QDataStream& fStream);
private:
    Ui::DialogScheduler *ui;
protected:
	QObjectList m_listWeekDays;
signals:
	void onConfirm();
private slots:
	void onConfirmEvent();
	void onSchedularEnable(int state);
};

#endif // DIALOGSCHEDULER_H

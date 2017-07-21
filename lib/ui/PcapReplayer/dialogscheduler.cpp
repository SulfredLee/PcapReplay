#include "dialogscheduler.h"
#include "ui_dialogscheduler.h"

DialogScheduler::DialogScheduler(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogScheduler)
{
    ui->setupUi(this);

	// connect from dialogscheduler.ui
	connect(ui->pushButton_Confirm, SIGNAL(pressed()), this, SLOT(onConfirmEvent()));
	connect(ui->pushButton_Cancel, SIGNAL(pressed()), this, SLOT(close()));
	connect(ui->checkBox_SchedularEnable, SIGNAL(stateChanged(int)), this, SLOT(onSchedularEnable(int)));
	connect(this, SIGNAL(onConfirm()), parentWidget(), SLOT(Scheduler_onConfirm()));

	// init self variable
	m_vecWeekDaysChecked.resize(7); // init for 7 day
	m_listWeekDays = ui->groupBox_Weekday->children();
	m_listWeekDays.removeFirst(); // remove the layout child
	m_bSchularEnable = false;
}

DialogScheduler::~DialogScheduler()
{
    delete ui;
}

void DialogScheduler::onConfirmEvent()
{
	bool bOk = false;
	// get user input config

	// handle calender widget
	m_schedularDate = ui->calendarWidget->selectedDate();
	// handle time edit
	m_schedularTime = ui->timeEdit->time();
	// handle radio box
	m_bOneTimeOnly = ui->radioButton_OneTimeOnly->isChecked();
	// handle check box
	m_bSchularEnable = ui->checkBox_SchedularEnable->isChecked();
	if (m_bSchularEnable)
	{
		bOk = HandleCheckBoxWeekday();
	}
	else
	{
		bOk = true;
	}

	if (bOk)
		emit onConfirm();
}

bool DialogScheduler::HandleCheckBoxWeekday()
{
	bool bWeekDaySet = false;

	for (int i = 0; i < m_listWeekDays.size(); i++)
	{
		bWeekDaySet |= ((QCheckBox*)m_listWeekDays[i])->isChecked();
		m_vecWeekDaysChecked[i] = ((QCheckBox*)m_listWeekDays[i])->isChecked();
	}

	if (!m_bOneTimeOnly && !bWeekDaySet)
	{
		QMessageBox::StandardButton resBtn = QMessageBox::question(this, QString::fromStdString(g_AppName),
			tr("You need to select a weekday, Are you sure you want to leave?\n"),
			QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
			QMessageBox::Yes);
		if (resBtn != QMessageBox::Yes)
		{
			return false;
		}
		else
		{
			m_bSchularEnable = false;
			return true;
		}
	}
	
}

void DialogScheduler::Init_UIScreen()
{
	// handle calender widget
	ui->calendarWidget->setEnabled(m_bSchularEnable);
	// handle time edit
	ui->timeEdit->setEnabled(m_bSchularEnable);
	ui->timeEdit->setTime(m_schedularTime);
	// handle group box
	ui->groupBox->setEnabled(m_bSchularEnable);
	ui->groupBox_Weekday->setEnabled(m_bSchularEnable);
	// handle radio box
	m_bOneTimeOnly ? ui->radioButton_OneTimeOnly->setChecked(true) : ui->radioButton_Weekly->setChecked(true);
	// handle check box
	ui->checkBox_SchedularEnable->setChecked(m_bSchularEnable);
	for (int i = 0; i < m_vecWeekDaysChecked.size(); i++)
	{
		((QCheckBox*)m_listWeekDays[i])->setChecked(m_vecWeekDaysChecked[i]);
	}
}

void DialogScheduler::onSchedularEnable(int state)
{
	m_bSchularEnable = state == Qt::Checked ? true : false;
	Init_UIScreen();
}

void DialogScheduler::Serialize(bool bStore, QDataStream& fStream)
{
	if (bStore)
	{
		fStream << m_schedularDate;
		fStream << m_schedularTime;
		fStream << m_bOneTimeOnly;
		fStream << m_bSchularEnable;
		fStream << m_vecWeekDaysChecked;
	}
	else
	{
		fStream >> m_schedularDate;
		fStream >> m_schedularTime;
		fStream >> m_bOneTimeOnly;
		fStream >> m_bSchularEnable;
		fStream >> m_vecWeekDaysChecked;
	}
}
#ifndef CONFIGURATIONMAKER_H
#define CONFIGURATIONMAKER_H
class MainWindow;
class PcapReplayCtrl;
class DialogScheduler;
class ConfigurationMaker
{
public:
	PcapReplayCtrl* m_pMainCtrl;
public:
	ConfigurationMaker();
	~ConfigurationMaker();

	void GenConfig_MainWin(MainWindow* pMainWindow);
	void GenConfig_SchDialog(DialogScheduler* pDiaScheduler);
protected:
	void SetAdapter_Output();
};
#endif

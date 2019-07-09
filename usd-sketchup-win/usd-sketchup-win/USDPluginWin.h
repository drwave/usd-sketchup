#ifndef SKPTOUSD_WIN_USDPLUGINWIN_H
#define SKPTOUSD_WIN_USDPLUGINWIN_H

#include "USDSketchUpUtilities.h"

class USDExporterPluginWin : public USDExporterPlugin {
public:
	static USDExporterPluginWin *GetInstance();
	static void DestroyInstance();

	USDExporterPluginWin();
	virtual ~USDExporterPluginWin() {};

	void ShowOptionsDialog(bool model_has_selection);
protected:
	void ShowSummaryDialog(const std::string& summary);

	static USDExporterPluginWin *s_pInstance;
};

#endif // !SKPTOUSD_WIN_USDPLUGINWIN_H

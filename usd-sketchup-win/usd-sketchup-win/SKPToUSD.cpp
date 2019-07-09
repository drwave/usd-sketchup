#include "pch.h"
#include "resource.h"
#include "USDPluginWin.h"

class CSkp2XmlApp : public CWinApp {
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkp2XmlApp)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSkp2XmlApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CSkp2XmlApp, CWinApp)
	//{{AFX_MSG_MAP(CSkp2XmlApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CSkp2XmlApp theApp;

BOOL CSkp2XmlApp::InitInstance() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWinApp::InitInstance();
	return TRUE;
}

int CSkp2XmlApp::ExitInstance() {
	// Delete our exporter
	USDExporterPluginWin::DestroyInstance();
	return CWinApp::ExitInstance();
}

// This is the only exported function.  It simply returns a pointer to the
// exporter interface.
SketchUpModelExporterInterface* GetSketchUpModelExporterInterface() {
	return USDExporterPluginWin::GetInstance();
}
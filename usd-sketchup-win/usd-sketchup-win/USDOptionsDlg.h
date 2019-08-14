#ifndef SKPTOUSD_WIN_USDOPTIONSDLG_H
#define SKPTOUSD_WIN_USDOPTIONSDLG_H

#include "resource.h"

class USDOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(USDOptionsDlg)

public:
	USDOptionsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~USDOptionsDlg();

	void SetModelHasSelectionSet(bool hasSelectionSet);

// Dialog Data
	enum { IDD = IDD_DIALOG_EXPORT_OPTIONS };
	double _aspectRatio;
	BOOL _exportNormals;
	BOOL _exportCurves;
	BOOL _exportEdges;
	BOOL _exportLines;
	BOOL _exportToSingleFile;
	BOOL _exportMaterials;
	BOOL _exportMeshes;
	BOOL _exportCameras;
	BOOL _exportARKitCompatible;
	BOOL _exportDoubleSided;
	BOOL _exportSelectionSet;
	CButton _exportSelectionSetButton;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	bool _modelHasSelectionSet;
};

#endif // !SKPTOUSD_WIN_USDOPTIONSDLG_H

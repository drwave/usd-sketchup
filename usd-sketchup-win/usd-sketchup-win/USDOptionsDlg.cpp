// USDOptionsDlg.cpp : implementation file
//

#include "pch.h"

#include "afxdialogex.h"
#include "USDOptionsDlg.h"

// USDOptionsDlg dialog

IMPLEMENT_DYNAMIC(USDOptionsDlg, CDialog)
USDOptionsDlg::USDOptionsDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_EXPORT_OPTIONS, pParent) {
	_aspectRatio = 0.0;
	_exportNormals = true;
	_exportCurves = true;
	_exportEdges = true;
	_exportLines = true;
	_exportToSingleFile = true;
	_exportMaterials = true;
	_exportMeshes = true;
	_exportCameras = true;
	_exportARKitCompatible = false;
	_exportDoubleSided = true;
	_exportSelectionSet = false;
}

USDOptionsDlg::~USDOptionsDlg() {
}

void USDOptionsDlg::SetModelHasSelectionSet(bool hasSelectionSet) {
	_modelHasSelectionSet = hasSelectionSet;
}

void USDOptionsDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_EXPORT_NORMALS, _exportNormals);
	DDX_Check(pDX, IDC_CHECK_EXPORT_CURVES, _exportCurves);
	DDX_Check(pDX, IDC_CHECK_EXPORT_EDGES, _exportEdges);
	DDX_Check(pDX, IDC_CHECK_EXPORT_LINES, _exportLines);
	DDX_Check(pDX, IDC_CHECK_EXPORT_SINGLE_FILE, _exportToSingleFile);
	DDX_Check(pDX, IDC_CHECK_EXPORT_MATERIALS, _exportMaterials);
	DDX_Check(pDX, IDC_CHECK_EXPORT_MESHES, _exportMeshes);
	DDX_Check(pDX, IDC_CHECK_EXPORT_CAMERAS, _exportCameras);
	DDX_Check(pDX, IDC_CHECK_EXPORT_ARKIT_COMPATIBLE, _exportARKitCompatible);
	DDX_Check(pDX, IDC_CHECK_EXPORT_DOUBLE_SIDED, _exportDoubleSided);
	DDX_Check(pDX, IDC_CHECK_EXPORT_SELECTION, _exportSelectionSet);
	DDX_Control(pDX, IDC_CHECK_EXPORT_SELECTION, _exportSelectionSetButton);
}

BOOL USDOptionsDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	if (_modelHasSelectionSet == false) {
		_exportSelectionSet = false;
		_exportSelectionSetButton.EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(USDOptionsDlg, CDialog)
END_MESSAGE_MAP()


// USDOptionsDlg message handlers

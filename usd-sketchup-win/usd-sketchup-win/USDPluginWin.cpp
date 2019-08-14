#include "pch.h"
#include "usdpluginwin.h"
#include "usdoptionsdlg.h"
#include "usdexportsresultdlg.h"

USDExporterPluginWin *USDExporterPluginWin::s_pInstance = NULL;

USDExporterPluginWin::USDExporterPluginWin() {
}

USDExporterPluginWin *USDExporterPluginWin::GetInstance() {
	if (s_pInstance == NULL) {
		s_pInstance = new USDExporterPluginWin();
	}
	return s_pInstance;
}

void USDExporterPluginWin::DestroyInstance() {
	if (s_pInstance) {
		delete s_pInstance;
		s_pInstance = NULL;
	}
}

void USDExporterPluginWin::ShowOptionsDialog(bool model_has_selection) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// Init variables
	SetExportNormals(true);
	SetExportCurves(true);
	SetExportEdges(true);
	SetExportLines(true);
	SetExportToSingleFile(true);
	SetExportMaterials(true);
	SetExportMeshes(true);
	SetExportCameras(true);
	SetExportARKitCompatible(true);
	SetExportDoubleSided(true);
	//SetExportSelectionSet(true);

	// Create dialog and set preferences on dialog
	USDOptionsDlg dlg;
	dlg._exportNormals = GetExportNormals();
	dlg._exportCurves = GetExportCurves();
	dlg._exportEdges = GetExportEdges();
	dlg._exportLines = GetExportLines();
	dlg._exportToSingleFile = GetExportToSingleFile();
	dlg._exportMaterials = GetExportMaterials();
	dlg._exportMeshes = GetExportMeshes();
	dlg._exportCameras = GetExportCameras();
	dlg._exportARKitCompatible = GetExportARKitCompatible();
	dlg._exportDoubleSided = GetExportDoubleSided();
	dlg.SetModelHasSelectionSet(model_has_selection);

	// Display dialog
	if (dlg.DoModal() == IDOK) {
		// Save preferences
		SetExportNormals(dlg._exportNormals);
		SetExportCurves(dlg._exportCurves);
		SetExportEdges(dlg._exportEdges);
		SetExportLines(dlg._exportLines);
		SetExportToSingleFile(dlg._exportToSingleFile);
		SetExportMaterials(dlg._exportMaterials);
		SetExportMeshes(dlg._exportMeshes);
		SetExportCameras(dlg._exportCameras);
		SetExportARKitCompatible(dlg._exportARKitCompatible);
		SetExportDoubleSided(dlg._exportDoubleSided);
		dlg.SetModelHasSelectionSet(model_has_selection);
	}
}

static void AppendMessage(int number, UINT msgId, CString& to) {
	if (number > 0) {
		CString tmp;
		tmp.FormatMessage(msgId, number);
		to += tmp;
	}
}

void USDExporterPluginWin::ShowSummaryDialog(const std::string& summary) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString exported_msg = summary.c_str();
	exported_msg.Replace(_T("\n"), _T("\r\n"));
	USDExportResultDlg dlg;
	dlg.set_message(exported_msg);
	dlg.DoModal();
}

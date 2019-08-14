// Copyright 2013 Trimble Navigation Limited. All Rights Reserved.

#ifndef USDTOXML_WIN32_USDEXPORTERRESULTSDLG_H
#define USDTOXML_WIN32_USDEXPORTERRESULTSDLG_H

#include "resource.h"

class USDExportResultDlg : public CDialog {
	DECLARE_DYNAMIC(USDExportResultDlg)

public:
	USDExportResultDlg(CWnd* parent_ = NULL);   // standard constructor
	virtual ~USDExportResultDlg();
	void set_message(CString msg);

	// Dialog Data
	enum { IDD = IDD_DIALOG_EXPORT_RESULTS };

protected:

	CString	message_;
	virtual void DoDataExchange(CDataExchange* dx);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

#endif // USDTOXML_WIN32_USDEXPORTERRESULTSDLG_H
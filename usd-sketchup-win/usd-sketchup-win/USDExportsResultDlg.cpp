// Copyright 2013 Trimble Navigation Limited. All Rights Reserved.

#include "pch.h"
#include "USDExportsResultDlg.h"


IMPLEMENT_DYNAMIC(USDExportResultDlg, CDialog)
USDExportResultDlg::USDExportResultDlg(CWnd* parent_ /*=NULL*/)
	: CDialog(USDExportResultDlg::IDD, parent_) {
}

USDExportResultDlg::~USDExportResultDlg() {
}

void USDExportResultDlg::DoDataExchange(CDataExchange* dx) {
	CDialog::DoDataExchange(dx);
	DDX_Text(dx, IDC_EDIT_STATS_MESSAGE, message_);
}

BEGIN_MESSAGE_MAP(USDExportResultDlg, CDialog)
END_MESSAGE_MAP()

void USDExportResultDlg::set_message(CString msg) {
	message_ = msg;
}

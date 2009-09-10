/*  v4l2ucp - A universal control panel for all V4L2 devices
    Copyright (C) 2009 Vasily Khoruzhick (anarsoul@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 */

#include "previewSettings.h"
#include <QSettings>


PreviewSettingsDialog::PreviewSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    QSettings settings(APP_ORG, APP_NAME);
    if (settings.contains(SETTINGS_APP_BINARY_NAME))
    {
        ui.appNameEdit->setText(settings.value(SETTINGS_APP_BINARY_NAME).toString());
    }
    else
    {
        ui.appNameEdit->setText("mplayer");
    }
}

PreviewSettingsDialog::~PreviewSettingsDialog()
{

}

void PreviewSettingsDialog::saveSettings()
{
    QSettings settings(APP_ORG, APP_NAME);
    settings.setValue(SETTINGS_APP_BINARY_NAME, ui.appNameEdit->text());
    settings.sync();
}

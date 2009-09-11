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
#include <QMessageBox>
#include <QSettings>


PreviewSettingsDialog::PreviewSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    QObject::connect(ui.envList, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
        this, SLOT(envItemDoubleClicked(QListWidgetItem *)));
    QObject::connect(ui.envList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
        this, SLOT(listCurItemChanged(QListWidgetItem *, QListWidgetItem *)));
    QObject::connect(ui.argList, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
        this, SLOT(argItemDoubleClicked(QListWidgetItem *)));
    QObject::connect(ui.argList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
        this, SLOT(listCurItemChanged(QListWidgetItem *, QListWidgetItem *)));
    QObject::connect(ui.addEnvBut, SIGNAL(clicked()),
        this, SLOT(addEnvItemClicked()));
    QObject::connect(ui.addArgBut, SIGNAL(clicked()),
        this, SLOT(addArgItemClicked()));
    QObject::connect(ui.removeEnvBut, SIGNAL(clicked()),
        this, SLOT(delEnvItemClicked()));
    QObject::connect(ui.removeArgBut, SIGNAL(clicked()),
        this, SLOT(delArgItemClicked()));

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

void PreviewSettingsDialog::envItemDoubleClicked(QListWidgetItem *item)
{
    ui.envList->openPersistentEditor(item);
}

void PreviewSettingsDialog::argItemDoubleClicked(QListWidgetItem *item)
{
    ui.argList->openPersistentEditor(item);
}

void PreviewSettingsDialog::addEnvItemClicked()
{
    if (!ui.envEdit->text().isEmpty())
    {
        QListWidgetItem *item = NULL;
        item = new QListWidgetItem(ui.envEdit->text(), ui.envList);
        ui.envEdit->clear();
    }
    else
    {
        QMessageBox::warning(NULL, "v4l2ucp", "Empty values seems to be meaningless!", "OK");
    }
}

void PreviewSettingsDialog::addArgItemClicked()
{
    if (!ui.argEdit->text().isEmpty())
    {
        QListWidgetItem *item = NULL;
        item = new QListWidgetItem(ui.argEdit->text(), ui.argList);
        ui.argEdit->clear();
    }
    else
    {
        QMessageBox::warning(NULL, "v4l2ucp", "Empty values seems to be meaningless!", "OK");
    }
}
void PreviewSettingsDialog::listCurItemChanged(QListWidgetItem *newItem, QListWidgetItem *oldItem)
{
    if (oldItem)
    {
        oldItem->listWidget()->closePersistentEditor(oldItem);
    }
}

void PreviewSettingsDialog::delEnvItemClicked()
{
    if (ui.envList->currentItem())
    {
        delete ui.envList->currentItem();
    }
    else
    {
        QMessageBox::warning(NULL, "v4l2ucp", "Select entry to delete.", "OK");
    }
}

void PreviewSettingsDialog::delArgItemClicked()
{
    if (ui.argList->currentItem())
    {
        delete ui.argList->currentItem();
    }
    else
    {
        QMessageBox::warning(NULL, "v4l2ucp", "Select entry to delete.", "OK");
    }
}

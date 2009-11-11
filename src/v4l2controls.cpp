/*  v4l2ucp - A universal control panel for all V4L2 devices
    Copyright (C) 2005 Scott J. Bertin (scottbertin@yahoo.com)
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
#include <sys/ioctl.h>
#include <cerrno>
#include <cstring>
#include <libv4l2.h>

#include <QPushButton>
#include <QLabel>
#include <QValidator>
#include <QMessageBox>

#include "v4l2controls.h"

V4L2Control::V4L2Control(int fd, const struct v4l2_queryctrl &ctrl,
                         QWidget *parent) :
    QWidget(parent), cid(ctrl.id), default_value(ctrl.default_value)
{
    this->fd = fd;
    strncpy(name, (const char *)ctrl.name, sizeof(name));
    name[sizeof(name)-1] = '\0';
    this->setLayout(&layout);
}

void V4L2Control::updateHardware()
{
    struct v4l2_control c;
    c.id = cid;
    c.value = getValue();
    if(v4l2_ioctl(fd, VIDIOC_S_CTRL, &c) == -1) {
        QString msg;
	msg.sprintf("Unable to set %s\n%s", name, strerror(errno));
	QMessageBox::warning(this, "Unable to set control", msg, "OK");
    }
    updateStatus();
}

void V4L2Control::updateStatus()
{
    struct v4l2_control c;
    c.id = cid;
    if(v4l2_ioctl(fd, VIDIOC_G_CTRL, &c) == -1) {
        QString msg;
	msg.sprintf("Unable to get %s\n%s", name,
	            strerror(errno));
	QMessageBox::warning(this, "Unable to get control", msg, "OK");
    } else {
        if(c.value != getValue())
	    setValue(c.value);
    }
    struct v4l2_queryctrl ctrl;
    ctrl.id = cid;
    if(v4l2_ioctl(fd, VIDIOC_QUERYCTRL, &ctrl) == -1) {
        QString msg;
	msg.sprintf("Unable to get the status of %s\n%s", name,
	            strerror(errno));
	QMessageBox::warning(this, "Unable to get control status", msg, "OK");
    } else {
        setEnabled(ctrl.flags == 0);
    }
}

void V4L2Control::resetToDefault()
{
    if(isEnabled()) {
        setValue(default_value);
        updateHardware();
    }
}

/*
 * V4L2IntegerControl
 */
V4L2IntegerControl::V4L2IntegerControl
    (int fd, const struct v4l2_queryctrl &ctrl, QWidget *parent) :
    V4L2Control(fd, ctrl, parent),
    minimum(ctrl.minimum), maximum(ctrl.maximum), step(ctrl.step)
{
    int pageStep = (maximum-minimum)/10;
    if(step > pageStep)
        pageStep = step;
    sl = new QSlider(Qt::Horizontal, this);
    sl->setMinimum(minimum);
    sl->setMaximum(maximum);
    sl->setPageStep(pageStep);
    sl->setValue(default_value);
    //sl->setLineStep(step);
    sl->setVisible(true);
    this->layout.addWidget(sl);

    QString defStr;
    defStr.setNum(default_value);
    le = new QLineEdit(this);
    le->setText(defStr);
    le->setValidator(new QIntValidator(minimum, maximum, this));
    this->layout.addWidget(le);
    
    QObject::connect( sl, SIGNAL(valueChanged(int)),
                      this, SLOT(SetValueFromSlider()) );
    QObject::connect( sl, SIGNAL(sliderReleased()),
                      this, SLOT(SetValueFromSlider()) );
    QObject::connect( le, SIGNAL(returnPressed()),
                      this, SLOT(SetValueFromText()) );
    updateStatus();
}

void V4L2IntegerControl::setValue(int val)
{
    if(val < minimum)
        val = minimum;
    if(val > maximum)
        val = maximum;
    if(step > 1) {
        int mod = (val-minimum)%step;
        if(mod > step/2) {
            val += step-mod;
        } else {
            val -= mod;
        }
    }
    QString str;
    str.setNum(val);
    le->setText(str);

	/* FIXME: find clean solution to prevent infinite loop */
	sl->blockSignals(true);
    sl->setValue(val);
	sl->blockSignals(false);
}

int V4L2IntegerControl::getValue()
{
    return sl->value();
}

void V4L2IntegerControl::SetValueFromSlider()
{
    setValue(sl->value());
    updateHardware();
}

void V4L2IntegerControl::SetValueFromText()
{
    if(le->hasAcceptableInput()) {
        setValue(le->text().toInt());
        updateHardware();
    } else {
        SetValueFromSlider();
    }
}

/*
 * V4L2BooleanControl
 */
V4L2BooleanControl::V4L2BooleanControl
    (int fd, const struct v4l2_queryctrl &ctrl, QWidget *parent) :
    V4L2Control(fd, ctrl, parent),
    cb(new QCheckBox(this))
{
    this->layout.addWidget(cb);
    QObject::connect( cb, SIGNAL(clicked()), this, SLOT(updateHardware()) );
    updateStatus();
}

void V4L2BooleanControl::setValue(int val)
{
    cb->setChecked(val != 0);
}

int V4L2BooleanControl::getValue()
{
    return cb->isChecked();
}

/*
 * V4L2MenuControl
 */
V4L2MenuControl::V4L2MenuControl
    (int fd, const struct v4l2_queryctrl &ctrl, QWidget *parent) :
    V4L2Control(fd, ctrl, parent)
{
    cb = new QComboBox(this);
    this->layout.addWidget(cb);
    
    for(int i=ctrl.minimum; i<=ctrl.maximum; i++) {
        struct v4l2_querymenu qm;
        qm.id = ctrl.id;
        qm.index = i;
        if(v4l2_ioctl(fd, VIDIOC_QUERYMENU, &qm) == 0) {
            cb->insertItem(i, (const char *)qm.name);
        } else {
            QString msg;
            msg.sprintf("Unable to get menu item for %s, index=%d\n"
	                "Will use Unknown", name, qm.index);
            QMessageBox::warning(this, "Unable to get menu item", msg, "OK");
            cb->insertItem(i, "Unknown");
        }
    }
    cb->setCurrentIndex(default_value);
    QObject::connect( cb, SIGNAL(activated(int)),
                      this, SLOT(menuActivated(int)) );
    updateStatus();
}

void V4L2MenuControl::setValue(int val)
{
    cb->setCurrentIndex(val);
}

int V4L2MenuControl::getValue()
{
    return cb->currentIndex();
}

void V4L2MenuControl::menuActivated(int val)
{
    setValue(val);
    updateHardware();
}

/*
 * V4L2ButtonControl
 */
V4L2ButtonControl::V4L2ButtonControl
    (int fd, const struct v4l2_queryctrl &ctrl, QWidget *parent) :
    V4L2Control(fd, ctrl, parent)
{
    QPushButton *pb = new QPushButton((const char *)ctrl.name, this);
    this->layout.addWidget(pb);
    QObject::connect( pb, SIGNAL(clicked()), this, SLOT(updateHardware()) );
}

void V4L2ButtonControl::updateStatus()
{
    struct v4l2_queryctrl ctrl;
    ctrl.id = cid;
    if(v4l2_ioctl(fd, VIDIOC_QUERYCTRL, &ctrl) == -1) {
        QString msg;
	msg.sprintf("Unable to get the status of %s\n%s", name,
	            strerror(errno));
	QMessageBox::warning(this, "Unable to get control status", msg, "OK");
    } else {
        setEnabled(ctrl.flags == 0);
    }
}

void V4L2ButtonControl::resetToDefault()
{
}

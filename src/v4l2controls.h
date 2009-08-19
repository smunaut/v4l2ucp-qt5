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
#include <sys/time.h>
#include <linux/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>

#include <QHBoxLayout>
#include <QCheckBox>
#include <QSlider>
#include <QComboBox>
#include <QLineEdit>

class V4L2Control : public QWidget
{
    Q_OBJECT
public slots:
    void updateHardware();
    virtual void updateStatus();
    virtual void resetToDefault();
    virtual void setValue(int val) = 0;

public:
    virtual int getValue() = 0;

protected:
    V4L2Control(int fd, const struct v4l2_queryctrl &ctrl, QWidget *parent);
    int fd;
    int cid;
    int default_value;
    char name[32];
    QHBoxLayout layout;
};

class V4L2IntegerControl : public V4L2Control
{
    Q_OBJECT
public:
    V4L2IntegerControl(int fd, const struct v4l2_queryctrl &ctrl, QWidget *parent);

public slots:
    void setValue(int val);

public:
    int getValue();

private slots:
    void SetValueFromSlider(void);
    void SetValueFromText(void);

private:
    int minimum;
    int maximum;
    int step;
    QSlider *sl;
    QLineEdit *le;
};

class V4L2BooleanControl : public V4L2Control
{
    Q_OBJECT
public:
    V4L2BooleanControl(int fd, const struct v4l2_queryctrl &ctrl, QWidget *parent);

public slots:
    void setValue(int val);

public:
    int getValue();

private:
    QCheckBox *cb;
};

class V4L2MenuControl : public V4L2Control
{
    Q_OBJECT
public:
    V4L2MenuControl(int fd, const struct v4l2_queryctrl &ctrl, QWidget *parent);

public slots:
    void setValue(int val);

public:
    int getValue();

private:
    QComboBox *cb;

private slots:
    void menuActivated(int val);
};

class V4L2ButtonControl : public V4L2Control
{
    Q_OBJECT
public slots:
    void updateStatus();
    void resetToDefault();

public:
    V4L2ButtonControl(int fd, const struct v4l2_queryctrl &ctrl, QWidget *parent);

public slots:
    void setValue(int) {};
    int getValue() { return 0; };
};

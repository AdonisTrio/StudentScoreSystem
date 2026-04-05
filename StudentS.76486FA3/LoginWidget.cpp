#include "LoginWidget.h"

#include<QInputDialog>

LoginWidget::LoginWidget(QWidget* parent)
{
	initialize_widget();
}

void LoginWidget::initialize_widget()
{
	this->setWindowTitle("NJUST学生成绩管理系统");

	QScreen* myDesktop = QGuiApplication::primaryScreen();
	QRect myAvailableDesktop = myDesktop->availableGeometry();
	this->setGeometry((myAvailableDesktop.width() - this->width()) / 2, (myAvailableDesktop.height() - this->height()) / 2, 500, 400);

	//按钮
	bt* b1 = new bt("以教师身份登录", this);
	connect(b1, &bt::clicked, this, &LoginWidget::On_bt1_Clicked);
	bt* b2 = new bt("以访客模式打开", this);
	connect(b2, &bt::clicked, this, &LoginWidget::On_bt2_Clicked);
	
	b1->setFont(QFont("SimSun", 15));
	b2->setFont(QFont("SimSun", 15));
	b1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	b2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	//标签
	lb* l1 = new lb("欢迎使用学生成绩管理系统", this);
	l1->setFont(QFont("Microsoft YaHei", 25));
	l1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	l1->setAlignment(Qt::AlignCenter);

	//布局设计
	hb* layout1 = new hb();
	vb* layout2 = new vb(this);
	layout1->setSpacing(50);
	layout1->setContentsMargins(30, 30, 30, 60);
	layout2->setContentsMargins(50, 70, 50, 30);
	layout1->addWidget(b1);
	layout1->addWidget(b2);
	layout2->addWidget(l1);
	layout2->addLayout(layout1);
	layout2->setAlignment(l1, Qt::AlignHCenter);
}


void LoginWidget::On_bt1_Clicked()
{
	// 弹出登录对话框
	bool ok;
	QString username = QInputDialog::getText(this, "登录", "用户名:", QLineEdit::Normal, "", &ok);
	bool isTeacher = false;
	if (ok && !username.isEmpty()) {
		QString password = QInputDialog::getText(this, "登录", "密码:", QLineEdit::Password, "", &ok);
		if (ok && !password.isEmpty()) {
			DatabaseHelper* db = new DatabaseHelper;
			if (db->verifyTeacher(username, password)) {
				InitialWidget* sub = new InitialWidget(this);
				this->hide();
				sub->show();
			}
			else {
				QMessageBox::warning(this, "", "用户名或密码错误！请重试！");
			}
		}
	}
}

void LoginWidget::On_bt2_Clicked()
{
	try
	{
		QString path = QFileDialog::getOpenFileName(
			this,
			"选择本地数据库",
			QDir::currentPath() + "/Database/students.db",
			"SQLite数据库 (*.db )"
		);
		if (path.isEmpty())
			return;

		DatabaseHelper* db = new DatabaseHelper;
		db->OpenDatabase(path);
		if (db->get_All_Students().size() == 0) {
			delete db;
			throw"数据库中没有学生记录！请重新选择或导入CSV文件建库。";
		}
		MainWindow* sub = new MainWindow(this);
		sub->setParent(this);
		sub->setTeacherMode(false);
		sub->connect_table_and_db(db);
		sub->initable();
		this->hide();
		sub->show();
	}
	catch (const char* msg)
	{
		QMessageBox::critical(this, " ", msg);
	}
}

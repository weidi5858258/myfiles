'''
自己的想法：
1.先认识几个框架
2.列出常用的库

数据库的设计是重中之重
Django的使用步骤：

1.
先创建工程目录
在任何一个目录下执行下面代码，
django-admin startproject TestDjango
然后会创建一个TestDjango目录
（可以认为是工程目录，标记为A目录），
TestDjango目录里面有：
manage.py文件
(经常要使用到的)
和
TestDjango目录
（可以认为工程目录的配置工程，标记为B目录）。
TestDjango目录里面有：
__init__.py   
settings.py   
urls.py   
wsgi.py 

2.
设计表结构(针对于mysql)
show databases;
use 数据库名;
drop database 数据库名;
create database 数据库名;
select * from 表名;
desc 表名;
要查看有多少个表，输入.table
查看单个表的结构，输入.schema TableName

查看起来有些吃力，为了让数据看起来更美观，
输入.head on，
再输入.mode column，
然后输入select * from table;

配置数据库
Django默认使用的是SQLite数据库
python3安装的是PyMySQL
2-1.
__init__.py这个文件的配置：
import pymysql
pymysql.install_as_MySQLdb()
2-2.
操作这步之前，数据库必须要先创建好
settings.py这个文件的配置：
DATABASES = {
	 'default': {
	     'ENGINE'   : 'django.db.backends.mysql',
	     'NAME'     : 'sunck',要操作的数据库名
	     'USER'     : 'root',用户名
	     'PASSWORD' : 'weidi',密码
	     'HOST'     : 'localhost',服务器ip地址
	     'PORT'     : '3306'端口
	 }
}

3.
进入A目录，创建应用，
在一个项目中创建多个应用，每个应用进行一种业务处理。
如:
python3 manage.py startapp myApp
python3 manage.py startapp loginModule
python3 manage.py startapp searchModule
python3 manage.py startapp payModule
每个应用下有：
__init__.py 
migrations 	是一个目录
admin.py 	站点配置
models.py 	模型
views.py 	视图
apps.py 
tests.py 

migrations目录中有：
__init__.py文件

4.
想用应用，就要激活应用
settings.py这个文件的配置：
往INSTALLED_APPS这个列表中加入应用'loginModule'

5.
定义模型(models)
一张表对应一个模型
往models.py这个文件中加入模型：
from django.db import models
所有的模型类都需要继承models.Model这个类
class Grades(models.Model):
	gname = models.CharField(max_length=20)
	gdate = models.DateTimeField()
	ggirnum = models.IntegerField()
	idDelete = 
	models.BooleanGField(default=False)
	# 关联外键（也就是关联对象），
	# 写在“一对多”的那个“多”的模型类中
	# 赋值时需要School对象赋给sgrade
	sgrade = models.ForeignKey('School')
不需要定义主键，在生成时自动添加，
并且这个值是自动增加的。

6.
在数据库中生成表
models.py中定义的类都会生成一张表.
生成的表的表名都是小写,以"模块名_小写的类名"命名.
最好不要使用AutoField,生成表的时候会自动生成一个"id"的字段,当数据一条条插入时其值是自动增加的.
在A目录进行
6-1.
生成迁移文件
python3 manage.py makemigrations
此时数据库中还没有生成表
6-2.
执行迁移
python3 manage.py migrate
此时会根据模型类到数据库中生成相对应的表
说明:
以后操作数据库里面的数据都是
通过模型类的对象去操作的。
如果要修改表字段，那么最好先把迁移文件给删除掉。
如果只是增加表，那么不需要把迁移文件给删除掉。
测试数据的使用：
python3 manage.py shell
测试时需要先导入有关模块
from myApp.models import Grade, Student
from django.utils import timezone
from datetime import *

查询某个模型类的全部数据
Grades这个模型类下有一个隐藏的objects属性，
通过这个属性的all()方法就能查出Grades表中的
所有数据。
覆写一下__str__(self)方法，
那么调用print函数输出内容时就不是一个地址了
Grades.objects.all()

# 增、删、改、查
创建数据
grade = Grades()
grade.gname = ''
grade.gdate = 
datetime(year=2017,month=7,day=17)
grade.save()

删除数据
grade.delete()

修改数据
grade.gname = ''
grade.save()

查询某条具体的数据
Grades.objects.get(pk=2) 也就是id=2

获得关联对象的集合（查询“多”的数据）
对象名.关联的类名(小写)_set.all()
grade.students_set.all()

“一对多”中“多”的数据创建有两种方式：
一：
直接保存到数据库中去了，
不需要赋给一个变量，
然后调用save()方法
grade.students_set.create(sname='',sgender=True,scontend=u'',sage=45)
二：
创建空对象，然后赋值，
把关联对象的那个属性指向“一”的对象，
最后调用save()方法

7.
启动服务器
ip可以不写，不写的话代表本机ip
端口号默认是8000
这是一个纯python写的轻量级web服务器，
仅仅在开发测试中使用这个
（net start mysql157）
进入A目录
python3 manage.py runserver ip:port

---------------------------------------------

8.
Admin站点管理：（主要就是管理数据库）
8-1.
内容发布：
负责添加、修改、删除内容（由页面维护数据库中的数据，这样操作起来更方便）
8-2.
公告访问
8-3.
配置Admin应用
在settings.py这个文件的INSTALLED_APPS中添加
'django.contrib.admin'（默认已添加）
8-4.
创建管理员用户
python3 manage.py createsuperuser
然后就可以输入一个用户名、邮箱、密码
8-5.
访问站点网站
http://127.0.0.1:8000/admin/login/?next=/admin/
8-6.
汉化
在settings.py这个文件把原来的英文配置改为：
LANGUAGE_CODE = 'zh-Hans'
TIME_ZONE = 'Asia/Shanghai'
8-7.
管理数据表
修改admin.py这个文件，添加：
# models.py这个文件中有哪些类，才可以把那些类导入
from .models import Grade
# 注册
admin.site.register(Grade)
网站上默认会在Grades这个类名后面再加一个“s”
8-8.
自定义管理页面
class GradeAdmin(admin.ModelAdmin):
    # 列表页属性
    # 显示字段
    list_display = ['id', 'gname', 'gdate', 'gboynum', 'ggirlnum', 'isDelete']
    # 根据什么条件进行过滤
    list_filter = ['gname']
    # 分页，每5条进行分页
    list_per_page = 5
    # 按gname进行搜索
    search_fields = ['gname']

    # 添加、修改页属性
    # 增加grade时，可以规定属性的先后顺序
    # fields = ['ggirlnum', 'gname', 'gdate', 'gboynum', 'isDelete']
    # 给属性分组 fields与fieldsets不能同时使用
    fieldsets = [
        ('num', {'fields': ['gboynum', 'ggirlnum']}),
        ('base', {'fields': ['gname', 'gdate', 'isDelete']})
    ]
# 注册
admin.site.register(Grade, GradeAdmin)

---------------------------------------------

视图层

10.
视图的基本使用
10-1.
在django中，视图对web请求进行回应
视图就是一个python函数，在views.py文件中定义
from django.http import HttpResponse
def index(request):
    return HttpResponse('Django index.')
def index(request):
    return render(
    request, 'myApp/index.html')    
10-2.
然后在B目录下的urls.py这个文件中配置路径
from django.conf.urls import url, include
from django.contrib import admin
urlpatterns = [
    # url匹配规则
    url(r'^admin/', admin.site.urls),
    url(r'^', include('myApp.urls'))
]
10-3.
接着在myApp/urls.py(没有这个文件就创建)这个文件中配置路径
from django.conf.urls import url
from . import views
urlpatterns = [
    # url匹配规则
    url(r'^$', views.index)
]

11.
模板的基本使用
模板是html页面，可以根据视图中传递过来的数据进行填充
11-1.
创建模板
模板的下面对应每个应用的视图
在A目录下创建templates目录，
在templates目录下创建对应项目的模板目录
11-2.
配置路径
修改settings.py文件下的TEMPLATES列表
添加模板目录
'DIRS': [os.path.join(BASE_DIR, 'templates')]
11-3.
定义Grade.html和Student.html两个模板
模板语法有两种：
{{输出值，可以是变量，可以是对象.属性}}
{%执行代码段%}
11-4.
形如：
http://127.0.0.1:8000/grades/

模型层

模型类(很关键的一步内容)
模型类中不能再使用__init__(self)方法
Django对各种数据库提供了很好的支持，
Django为这些数据库提供了统一的API接口。
可以根据不同的业务需求选择不同的数据库。

定义属性

概述
	·django根据属性的类型确定以下信息
	·当前选择的数据库支持字段的类型
	·渲染管理表单时使用的默认html控件
	·在管理站点最低限度的验证
	·django会为表增加自动增长的主键列，
	 每个模型只能有一个主键列，
	 如果使用选项设置某属性为主键列后，
	 则django不会再生成默认的主键列。
	·属性命名限制
		·遵循标识符规则
		·由于django的查询方式，不允许使用连续的下划线

库
	·定义属性时，需要字段类型，
	 字段类型被定义在django.db.models.fields
	 目录下，为了方便使用，
	 被导入到django.db.models中
	·使用方式
		·导入from django.db import models
		·通过models.Field创建字段类型的对象，
		 赋值给属性

逻辑删除
	·对于重要数据都做逻辑删除，不做物理删除，
	 实现方法是定义isDelete属性，
	 类型为BooleanField，默认值为False

字段类型
	·AutoField
		·一个根据实际ID自动增长的IntegerField，
		 通常不指定。如果不指定，一个主键字段将自动添加到模型中.
		 默认情况下,Django会给每个模型添加下面这个字段:
		 id = models.AutoField(primary_key=True)
		 如果想要指定一个自定义主键字段,只要在某个字段上指定primary_key=True即可.
		 如果Django看到显式地设置了primary_key=True,就不会自动添加id列了.
		 每个模型类只能有一个字段指定primary_key=True(无论是显式声明还是自动添加).
	·CharField(max_length=字符长度)
		·字符串，默认的表单样式是 TextInput
	·TextField
		·大文本字段，一般超过4000使用，
		 默认的表单控件是Textarea
	·IntegerField
		·整数
	·DecimalField(max_digits=None, 
	 			  decimal_places=None)
		·使用python的Decimal实例表示十进制浮点数
		·参数说明
			·DecimalField.max_digits
				·位数总数
			·DecimalField.decimal_places
				·小数点后的数字位数
	·FloatField
		·用Python的float实例来表示的浮点数
	·BooleanField
		·true/false
		 此字段的默认表单控制是CheckboxInput
	·NullBooleanField
		·支持null、true、false三种值
	·DateField([auto_now=False,
				auto_now_add=False])
		·使用Python的datetime.date实例表示的日期
		·参数说明
			·DateField.auto_now
				·每次保存对象时，
				 自动设置该字段为当前时间，
				 用于"最后一次修改"的时间戳，
				 它总是使用当前日期，默认为false
			·DateField.auto_now_add
				·当对象第一次被创建时自动设置
				 当前时间，用于创建的时间戳，
				 它总是使用当前日期，默认为false
		·说明
			·该字段默认对应的表单控件是一个TextInput.在管理员站点添加了一个JavaScript
			 写的日历控件，和一个“Today"的快捷按钮，包含了一个额外的invalid_date错误消息键
		·注意
			·auto_now_add, auto_now, and 
			 default 这些设置是相互排斥的，
			 他们之间的任何组合将会发生错误的结果(也就是说只能出现一个限制条件)
	·TimeField
		·使用Python的datetime.time实例表示的时间，
		 参数同DateField
	·DateTimeField
		·使用Python的datetime.datetime
		 实例表示的日期和时间，参数同DateField
	·FileField
		·一个上传文件的字段
	·ImageField
		·继承了FileField的所有属性和方法，
		 但对上传的对象进行校验，确保它是个有效的image
字段选项
	·概述
		·通过字段选项，可以实现对字段的约束
		·在字段对象时通过关键字参数指定
	·null
		·如果为True(如:idCard = models.FileField(null=True))，
		 Django将空值以NULL存储到数据库中，默认值是 False
	·blank
		·如果为True，则该字段允许为空白，
		 默认值是False.如果一个字段的blank=True,
		 表单的验证将允许该字段是空值。如果
		 字段的blank=False,该字段就是必填的。
		·注意
			·null是数据库范畴的概念
			.blank是表单验证证范畴的
	·db_column
		·字段的名称，如果未指定，则使用属性的名称
	·db_index
		·若为True, 则在表中会为此字段创建索引
	·primary_key
		·若为True, 则该字段会成为模型的主键字段
		 如果没有指定一个字段的primary_key=True,Django就会自动添加
		 一个IntegerField字段作为主键,所以除非想覆盖默认的主键行为,
		 否则没必要设置任何一个字段的primary_key=True.
		 主键字段是只读的.如果在一个已存在的对象上面更改主键的值
		 并且保存,一个新的对象将会在原有对象之外创建出来.
	·unique
		·若为True, 则该字段在表中必须有唯一值
	·default
		·默认值(如:sex = models.BooleanField(default=True))
		 
关系
	·分类
		·ForeignKey：
		 一对多，将字段定义在多的那端中
		·ManyToManyField：
		 多对多，将字段定义在两端中的某一端
		·OneToOneField：
		 一对一，将字段定义在任意一端中
	·用一访问多
		·格式
			对象.模型类的小写_set
		·示例
			grade.students_set
	·用一访问一
		·格式
			对象.模型类小写
		·示例
			grade.students
	·访问id
		·格式
			对象.属性_id
		·示例
			student.sgrade_id

class People(models.Model):
	# 不要使用AutoField
	# _id = models.AutoField()
	SHIRT_SIZES = (
		("S", "Small"),
		("M", "Medium"),
		("L", "Large"),
	)
	name = models.CharField(max_length=20, choices=SHIRT_SIZES)
	experience = models.TextField()
	age = models.IntegerField()
	salary = models.DecimalField(max_digits=10, decimal_places=2)
	bonus = models.FloatField()
	sex = models.BooleanField(default=True)
	marry = models.NullBooleanField()
	timeOfEntry = models.DateField(auto_now=True)
	timeOfWork = models.TimeField()
	birthday = models.DateTimeField(auto_now_add=True)
	idCard = models.FileField(null=True)
	headPortrait = models.ImageField(blank=True)
choices由二元组组成的一个可迭代对象(如列表或元组),用来给字段提供选择项.
如果设置了choices,默认的表单将是一个选择框而不是标准的文本框,而且这个选择框
的选项就是choices中的选项.SHIRT_SIZES中每个元组的第一个元素,是存储在数据库中的值;
第二个元素是在管理界面或ModelChoiceField中用㑅显示的内容.在一个给定的model类的实例中,
想得到某个choices字段的显示值,就调用get_name_display()方法(name就是choices字段的名称).

字段的自述名
除ForeignKey、ManyToManyField和OneToOneField之外,每个字段类型都可以接受一个可选的位置
参数---字段的自述名.如果没有给定自述名,
Django将根据字段的属性名称自动创建自述名(将属性名称的下划线替换成空格).
first_name = models.CharField("person's	first name", max_length=30)
自述名是"person's first name".
first_name = models.CharField(max_length=30)
自述名是"first name"
ForeignKey、ManyToManyField和OneToOneField都要求第一个参数是一个模型类,
所以要使用verbose_name关键字参数才能指定自述名.
poll = models.ForeignKey(Poll, verbose_name="the related poll")
习惯上, verbose_name的首字母不用大写.Django在必要的时候会自动大写首字母.

数据库中的"关系"
关系数据库的威力体现在表之间的相互关联.Django提供了三种最常见的数据库关系:
多对一(many-to-one)
多对多(many-to-many)
一对一(one-to-one)

多对一关系
比如:
一个学校有多个教室,多个老师,多个学生.
那么多个教室,多个老师,多个学生就是多的那一方,学校是一的那一方.
设计表时多的那个类中的某个属性需要这样写:
XXX = models.ForeignKey("一"的那个类名)

多对多关系
在哪个模型类中设置ManyToManyField并不重要,在两个模型中任选一个即可(不要两个模型都设置)

元选项
使用内部的class Meta定义模型的元数据.
from django.db import models
class Ox(models.Model):
	forn_length = models.IntegerField()

	class Meta:
		ordering = ["horn_length"]
		verbose_name_plural = "oxen"
在模型类内部定义Meta类，用于设置元信息
db_table定义数据表名，推荐使用小写字母，数据表名默认为项目名小写_类名小写
ordering对象的默认排序字段，获取对象的列表时使用
ordering['id']升序
ordering['-id']降序
排序会增加数据库的开销

模型成员
类属性:
objects是Manager类型的一个对象，
作用是与数据库进行交互

ORM
对象---关系---映射
根据对象的类型生成表结构
将对象、列表的操作转换为sql语句
将sql语句查询到的结果转换为对象、列表

数据库设计及其附加的元数据
模型是有关数据的唯一且明确的数据源.
它包含了所要存储的数据的基本字段和行为.

import datetime
from django.utils import timezone
from polls.models import Poll, Choice
from django.utils import timezone
p = Poll(question="What are you doing?", pub_date=timezone.now())
p.save()
p.question
p.pub_date
p.question = "Hello World"
p.save()
Poll.objects.all()
Poll.objects.filter(id=1)
Poll.objects.filter(question__startswith='what')
Poll.objects.get(id=1)
p = Poll.objects.get(pk=1)
p.choice_set.all()

p.choice_set.create(choice_text='Not much', votes=0)
p.choice_set.create(choice_text='The sky', votes=0)
c = p.choice_set.create(choice_text='Just hacking again', votes=0)
c.poll
p.choice_set.count()

current_year = timezone.now().year
Choice.objects.filter(poll__pub_date__year=current_year)

c = p.choice_set.filter(choice_text__startswith='Just hacking')
c.delete()

from django.contrib import admin
from polls.models import Poll
admin.site.register(Poll)

from polls.models import Poll, Choice
from django.utils import timezone
import datetime

查询全部
Poll.objects.all()
创建新的对象
p = Poll(question="What's your name?", pub_date=timezone.now())
保存对象的数据到数据库中
p.save()
查看刚存入的数据的id是多少
p.id
查询刚存入的对象的各个属性值
p.question
p.pub_date
修改某个属性值后再次保存
p.question = "mama"
p.save()
查询id为1的那条数据(数据不存在时不会产生异常)
filter得到的p对象不能调用属性,
get   得到的p对象能够调用属性
p = Poll.objects.filter(id=1)
p = Poll.objects.get(id=1)
p = Poll.objects.get(pk=1)
查询question属性的开始部分是"What"的数据
Poll.objects.filter(question__startswith="What")

current_year = timezone.now().year
Poll.objects.get(pub_date__year=current_year)

p.choice_set.all()
调用create函数后就直接把数据保存到了数据库中去了
c = p.choice_set.create(choice_text="Not much", votes=0)
c = p.choice_set.create(choice_text="The sky", votes=0)
c = p.choice_set.create(choice_text="battery=u", votes=0)
c.poll
p.choice_set.count()

c = Choice.objects.filter(poll__pub_date__year=current_year)
c = p.choice_set.filter(choice_text__startswith="Just hacking")
c.delete()

<form action="{% url 'polls:vote' poll.id %}" method="post">
使用method="post"(而不是method="get")是非常重要的,因为这种提交表单的方式会改变服务器端的数据。
当你创建一个表单为了修改服务器端的数据时,请使用method="post"。
forloop.counter表示for标签在循环中已经循环过的次数。
所有的POST form针对内部的URLs时都应该使用{% csrf_token %}模板标签。
request.POST是一个类似字典的对象,可以让你通过关键字名称来获取提交的数据。
request.POST的值永远是字符串形式的。

覆盖预定义的模型类方法
如:save(), delete()
from django.db import models
class Blog(models.Model):
	name = models.CharField(max_length=100)
	tagline = models.TextField()

	def save(self, *args, **kwargs):
		do_something()
		# Call the "real" save() method.
		super(Blog, self).save(*args, **kwargs)
		do_something_else()

抽象基类
当你编写完基类之后,在Meta类中设置abstract=True,该类就不能
创建任何数据表.取而代之的是,当它被用来作为一个其他model的
基础类时,它将被加入那一子类中.如果抽象化基础类和它的子类有
相同的项,那么将会出现error(并且Django将返回一个exception).
from django.db import models
class CommonInfo(models.Model):
	name = models.CharField(max_length=100)		
	age = models.PositiveIntegerField()

	class Meta:
		abstract=True

class Student(CommonInfo):
	home_group = models.CharField(max_length=5)

元继承
当一个抽象类被创建的时候,Django会自动把你在基类中定义的Meta
作为子类一个属性.如果子类没有声明自己的Meta类,它将会继承父类的
Meta.如果子类想要扩展父类的Meta,继承父类的Meta即可.
from django.db import models
class CommonInfo(models.Model):
	# ...
	class Meta:
		abstract = True
		ordering = ["name"]

class Student(CommonInfo):
	# ...
	class Meta(CommonInfo.Meta):
		db_table = "student_info"
继承时,Django会对基类的Meta类做一个调整:
在安装Meta属性之前,Django会先设置abstract = False.
这意味着抽象基类的子类不会自动变成抽象类.
当然,你可以让一个抽象类去继承另一个抽象基类,不过
每次都要显式地设置abstract = True.
对于抽象基类而言,有些属性放在Meta内嵌类里面是没有意义的.










































































































'''

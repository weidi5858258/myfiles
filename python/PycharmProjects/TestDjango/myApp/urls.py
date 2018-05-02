from django.conf.urls import url
from . import views

urlpatterns = [
    # url匹配规则
    url(r'^$', views.index),
    # 加入了括号是为了得到这个参数
    url(r'^(\d+)/$', views.detail),
    # 浏览器中输入地址按了“回车”后，
    # 首先会到url控制器这里，
    # 进行地址的匹配，
    # 如果匹配上了，那么就会去调用后面的视图
    url(r'^grades/$', views.grades),
    url(r'^students/$', views.students),

    url(r'^grades/(\d+)$', views.gradeStudent),
]

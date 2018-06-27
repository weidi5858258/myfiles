from django.conf.urls import url
from django.views.generic import DetailView, ListView
from polls import views
from polls.models import Poll

'''
url(regex, view, kwargs, name)
url()函数有四个参数,
两个是必须的:regex和view,
两个是可选的:kwargs和name.
view:
当Django	匹配了一个正则表达式就会调用指定的视图功能,
包含一个HttpRequest实例作为第一个参数和正则表达式
“捕获”的一些值的作为其他参数。
如果使用简单的正则捕获,将按顺序位置传参数;
如果按命名的正则捕获,将按关键字传参数值。
kwargs:
任意关键字参数可传一个字典至目标视图。
name:
命名你的URL,让你在Django的其他地方明确地引用它,特别是在模板中。
这一强大的功能可允许你通过一个文件就可全局修改项目中的URL模式。
'''


urlpatterns = [
        # http://127.0.0.1:8000/polls/
        url(r'^$', views.index, name='index'),
        # http://127.0.0.1:8000/polls/2/
        url(r'^(?P<poll_id>\d+)/$', views.detail, name='detail'),
        # http://127.0.0.1:8000/polls/2/results/
        url(r'^(?P<poll_id>\d+)/results/$', views.results, name='results'),
        # http://127.0.0.1:8000/polls/2/vote/
        url(r'^(?P<poll_id>\d+)/vote/$', views.vote, name='vote'),
]

'''
urlpatterns = [
    url(r'^$',
        ListView.as_view(
            queryset=Poll.objects.order_by('-pub_date')[:5],
            context_object_name='latest_poll_list',
            template_name='polls/index.html'),
        name='index'),
    url(r'^(?P<pk>\d+)/$',
        DetailView.as_view(
            model=Poll,
            template_name='polls/detail.html'),
        name='detail'),
    url(r'^(?P<pk>\d+)/results/$',
        DetailView.as_view(
            model=Poll,
            template_name='polls/results.html'),
        name='results'),
    url(r'^(?P<poll_id>\d+)/vote/$', views.vote, name='vote'),
]
'''
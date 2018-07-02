"""
mysite URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.11/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
# from django.conf.urls import url, patterns, include
from django.conf.urls import url, include
from django.contrib import admin
admin.autodiscover()

urlpatterns = [
    url(r'^admin/', admin.site.urls),
    # http://127.0.0.1:8000/admin/
    url(r'^admin/', include(admin.site.urls)),
    # http://127.0.0.1:8000/polls/ namespace测试没成功
    # url(r'^polls/', include('polls.urls', namespace="polls")),
    url(r'^polls/', include('polls.urls')),
    # url(r'^$', '{{ project_name }}.views.home', name='home'),
    # url(r'^{{ project_name }}/', include('{{ project_name }}.foo.urls')),
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),
]

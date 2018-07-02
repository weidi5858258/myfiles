from django.shortcuts import render

# Create your views here.

from django.http import HttpResponse
from .models import Grade, Student


def index(request):
    return HttpResponse('Django index.')


def detail(request, num):
    return HttpResponse('Detail-%s' % (num))


# url控制器转发到这里
def grades(request):
    # 去模板里取数据
    gradeList = Grade.objects.all()
    # 将数据传递给模板，模板在渲染页面，将渲染好的页面返回给浏览器
    return render(request, 'myApp/Grade.html', {'grades': gradeList})


def students(request):
    # 去模板里取数据
    studentList = Student.objects.all()
    # 将数据传递给模板，模板在渲染页面，将渲染好的页面返回给浏览器
    return render(request, 'myApp/Student.html', {'students': studentList})

def gradeStudent(request, num):
    grade = Grade.objects.get(pk=num)
    studentList = grade.student_set.all()
    return render(request, 'myApp/Student.html', {'students':studentList})
    pass
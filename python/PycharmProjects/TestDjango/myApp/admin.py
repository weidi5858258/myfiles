from django.contrib import admin

# Register your models here.

from .models import Grade, Student


# class StudentInfo(admin.StackedInline):
class StudentInfo(admin.TabularInline):
    model = Student
    extra = 2

# @admin.register(Grade)
class GradeAdmin(admin.ModelAdmin):
    # 在增加Grade对象时，同时增加两个Student类
    inlines = [StudentInfo]
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

# @admin.register(Student)
class StudentAdmin(admin.ModelAdmin):
    def gender(self):
        if self.sgender:
            return "男"
        else:
            return "女"

    # 设置页面列的名称
    gender.short_description = "性别"

    def getName(self):
        return self.sname

    getName.short_description = "姓名"

    # 执行动作的位置
    actions_on_top = True
    actions_on_bottom = False

    # 列表页属性
    # 显示字段
    list_display = ['id', getName, 'scontend', gender, 'sage', 'isDelete', 'sgrade']
    # 根据什么条件进行过滤
    list_filter = ['sname']
    # 分页，每5条进行分页
    list_per_page = 5
    # 按gname进行搜索
    search_fields = ['sname']

    # 添加、修改页属性
    # 增加grade时，可以规定属性的先后顺序
    # fields = ['ggirlnum', 'gname', 'gdate', 'gboynum', 'isDelete']
    # 给属性分组 fields与fieldsets不能同时使用
    fieldsets = [
        ('num', {'fields': ['sgender', 'isDelete']}),
        ('base', {'fields': ['sname', 'scontend', 'sage', 'sgrade']})
    ]


# 注册（实际不是这样注册的，是用装饰器完成注册的）
admin.site.register(Grade, GradeAdmin)
admin.site.register(Student, StudentAdmin)

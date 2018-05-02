# 为了使用models.Model
# 模型类要继承models.Model
from django.db import models


# Create your models here.

# 一对多
# 一
class Grade(models.Model):
    gname = models.CharField(max_length=20)
    gdate = models.DateTimeField()
    gboynum = models.IntegerField()
    ggirlnum = models.IntegerField()
    isDelete = models.BooleanField(default=False)

    # def __str__(self):
    #     # return super().__str__()
    #     print('gname: %s, gdate: %s, gboynum: %d, ggirlnum: %d, isDelete: %s'
    #           % (self.gname, self.gdate.strftime("%Y-%m-%d"), self.gboynum, self.ggirlnum, str(self.isDelete)))


# 多
class Student(models.Model):
    sname = models.CharField(max_length=20)
    scontend = models.CharField(max_length=100)
    sgender = models.BooleanField(default=True)
    sage = models.IntegerField()
    isDelete = models.BooleanField(default=False)
    # 关联外键，关联Grade类
    sgrade = models.ForeignKey('Grade')

    class Meta:
        db_table = "student"
        ordering = ['id']

    # def __str__(self):
    #     # return super().__str__()
    #     print('sname: %s, scontend: %s, sgender: %s, sage: %d, isDelete: %s'
    #           % (self.sname, self.scontend, str(self.sgender), self.sage, str(self.isDelete)))

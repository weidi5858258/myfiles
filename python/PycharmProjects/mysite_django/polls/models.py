import datetime
from django.utils import timezone
from django.db import models

# Create your models here.

# 一
class Poll(models.Model):
	# ~django.db.models.CharField.max_length
	question = models.CharField(max_length=200)
	pub_date = models.DateTimeField('date published')

	def __unicode__(self):
		# print("Poll __unicode__")
		return self.question
	
	def __str__(self):
		# print("Poll __str__")
		return self.question

        # def was_published_recently(self):
        #      return self.pub_date >= timezone.now() - datetime.timedelta(days=1)
# 多
class Choice(models.Model):
	poll = models.ForeignKey(Poll)
	choice_text = models.CharField(max_length=200)
	votes = models.IntegerField(default=0)

	def __unicode__(self):
		# print("Choice __unicode__")
		return self.choice_text

	def __str__(self):
		# print("Choice __str__")
		return self.choice_text

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

'''
CREATE TABLE IF NOT EXISTS "polls_people" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "name" varchar(20) NOT NULL, "experience" text NOT NULL, "age" integer NOT NULL, "salary" decimal NOT NULL, "bonus" real NOT NULL, "sex" bool NOT NULL, "marry" bool NULL, "timeOfEntry" date NOT NULL, "timeOfWork" time NOT NULL, "birthday" datetime NOT NULL, "idCard" varchar(100) NOT NULL, "headPortrait" varchar(100) NOT NULL);
'''






































# -*- coding: utf-8 -*-
# Generated by Django 1.11.4 on 2018-03-18 04:44
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Grade',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('gname', models.CharField(max_length=20)),
                ('gdate', models.DateTimeField()),
                ('gboynum', models.IntegerField()),
                ('ggirlnum', models.IntegerField()),
                ('isDelete', models.BooleanField(default=False)),
            ],
        ),
        migrations.CreateModel(
            name='Student',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('sname', models.CharField(max_length=20)),
                ('scontend', models.CharField(max_length=100)),
                ('sgender', models.BooleanField(default=True)),
                ('sage', models.IntegerField()),
                ('isDelete', models.BooleanField(default=False)),
                ('sgrade', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='myApp.Grade')),
            ],
        ),
    ]

# Generated by Django 3.0.3 on 2020-06-01 10:48

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('trading', '0010_auto_20200601_1051'),
    ]

    operations = [
        migrations.AlterField(
            model_name='order',
            name='dealDateTime',
            field=models.DateTimeField(default=None, null=True),
        ),
    ]
# Generated by Django 3.0.3 on 2020-06-01 11:13

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('trading', '0011_auto_20200601_1848'),
    ]

    operations = [
        migrations.AlterField(
            model_name='order',
            name='price',
            field=models.DecimalField(decimal_places=2, default=500, max_digits=6),
        ),
    ]

# Generated by Django 3.0.3 on 2020-05-04 13:25

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('trading', '0002_auto_20200504_2106'),
    ]

    operations = [
        migrations.AlterField(
            model_name='gaa',
            name='state',
            field=models.ForeignKey(default=0, on_delete=django.db.models.deletion.CASCADE, to='trading.GAAState'),
        ),
    ]

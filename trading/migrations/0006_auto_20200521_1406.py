# Generated by Django 3.0.3 on 2020-05-21 06:06

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('trading', '0005_auto_20200517_1622'),
    ]

    operations = [
        migrations.AlterField(
            model_name='gaa',
            name='state',
            field=models.ForeignKey(default=1, on_delete=django.db.models.deletion.CASCADE, to='trading.GAAState'),
        ),
        migrations.AlterField(
            model_name='order',
            name='price',
            field=models.DecimalField(decimal_places=2, max_digits=6),
        ),
        migrations.AlterField(
            model_name='palinfo',
            name='price',
            field=models.DecimalField(decimal_places=2, max_digits=6),
        ),
    ]
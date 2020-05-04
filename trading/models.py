from django.db import models
from datetime import datetime

# Create your models here.
class PAL(models.Model):
    name = models.CharField(max_length=50)

    def __str__(self):
        return self.name

class PALInfo(models.Model):
    PAL = models.ForeignKey(PAL, on_delete=models.CASCADE, default=None)
    numberOfRemaining = models.IntegerField()
    price = models.IntegerField()
    updateDateTime = models.DateTimeField(default=datetime.now)

    def __str__(self):
        return self.PAL.name

class GAAState(models.Model):
    name = models.CharField(max_length=50)

    def __str__(self):
        return self.name

class GAA(models.Model):
    name = models.CharField(max_length=50)
    state = models.ForeignKey(GAAState, default=0, on_delete=models.CASCADE)
    preference = models.ForeignKey(PAL, on_delete=models.CASCADE, default=None, blank=True, null=True)
    def __str__(self):
        return self.name

class Order(models.Model):
    GAA = models.ForeignKey(GAA, on_delete=models.CASCADE)
    price = models.IntegerField()
    orderDatetime = models.DateTimeField(default=datetime.now)
    isFinish = models.BooleanField(default=False)
    dealWith = models.ForeignKey(PALInfo, on_delete=models.CASCADE, default=None, blank=True, null=True)
    dealDateTimw = models.DateTimeField(default=None, blank=True)

    def __str__(self):
        return self.GAA.name
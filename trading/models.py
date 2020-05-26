from django.db import models
from datetime import datetime
from django.contrib.auth.models import User

# Create your models here.
class PAL(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    numberOfRemaining = models.IntegerField()
    price = models.DecimalField(max_digits=6, decimal_places=2)
    def __str__(self):
        return self.user.username

class PALHistory(models.Model):
    PAL = models.ForeignKey(PAL, on_delete=models.CASCADE, default=None)
    price = models.DecimalField(max_digits=6, decimal_places=2)
    updateDateTime = models.DateTimeField(default=datetime.now)

    def __str__(self):
        return self.PAL.user.username

class GAAState(models.Model):
    name = models.CharField(max_length=50)

    def __str__(self):
        return self.name

class GAA(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    state = models.ForeignKey(GAAState, default=1, on_delete=models.CASCADE)
    preference = models.ForeignKey(PAL, on_delete=models.CASCADE, default=None, blank=True, null=True)
    def __str__(self):
        return self.user.username

class Order(models.Model):
    GAA = models.ForeignKey(GAA, on_delete=models.CASCADE)
    price = models.DecimalField(max_digits=6, decimal_places=2)
    orderDatetime = models.DateTimeField(default=datetime.now)
    isFinish = models.BooleanField(default=False)
    dealWith = models.ForeignKey(PAL, on_delete=models.CASCADE, default=None, blank=True, null=True)
    dealDateTime = models.DateTimeField(default=None, blank=True)

    def __str__(self):
        return self.GAA.name
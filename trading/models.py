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

    # def save(self, force_insert=False, force_update=False, using=None, update_fields=None):
    #     history = PALHistory.objects.create(PAL=self, price=self.price)
    #     PALHistory.save(history)
    #     return super().save(force_insert=force_insert, force_update=force_update, using=using, update_fields=update_fields)


class PALHistory(models.Model):
    PAL = models.ForeignKey(PAL, on_delete=models.CASCADE, default=None)
    price = models.DecimalField(max_digits=6, decimal_places=2)
    updateDateTime = models.DateTimeField(default=datetime.now)

    def __str__(self):
        return self.PAL.user.username + ":" + str(self.price)

class GAAState(models.Model):
    name = models.CharField(max_length=50)

    def __str__(self):
        return self.name

class GAA(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    status = models.ForeignKey(GAAState, default=1, on_delete=models.CASCADE)
    preference = models.ForeignKey(PAL, on_delete=models.CASCADE, default=None, blank=True, null=True)
    
    def __str__(self):
        return self.user.username

class Order(models.Model):
    GAA = models.ForeignKey(GAA, on_delete=models.CASCADE)
    PAL = models.ForeignKey(PAL, on_delete=models.CASCADE, default=None, blank=True, null=True)
    price = models.DecimalField(max_digits=6, decimal_places=2, default=500)
    orderDatetime = models.DateTimeField(default=datetime.now)
    isFinish = models.BooleanField(default=False)
    dealDateTime = models.DateTimeField(default=None, null=True)

    def __str__(self):
        return self.GAA.user.username
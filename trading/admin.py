from django.contrib import admin
from .models import PAL, PALHistory, GAA, GAAState, Order

# Register your models here.
admin.site.register(PAL)
admin.site.register(PALHistory)
admin.site.register(GAA)
admin.site.register(GAAState)
admin.site.register(Order)
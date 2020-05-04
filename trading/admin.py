from django.contrib import admin
from .models import PAL, PALInfo, GAA, GAAState, Order

# Register your models here.
admin.site.register(PAL)
admin.site.register(PALInfo)
admin.site.register(GAA)
admin.site.register(GAAState)
admin.site.register(Order)
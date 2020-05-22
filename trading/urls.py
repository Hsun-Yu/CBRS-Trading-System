from django.conf.urls import url
from . import views
from .views import line_chart, line_chart_json
from django.urls import path, include

urlpatterns = [
    url(r'^$', views.index, name='index'),
    url(r'/login', views.loginView, name='loginView'),
    url(r'/signup', views.signupView, name='signupView'),
    url(r'/logout', views.logoutView, name='logoutView'),
    url(r'/setprice', views.setPriceView, name='setPriceView'),
    
    path('chart', line_chart, name='line_chart'),
    path('chartJSON', line_chart_json, name='line_chart_json'),
];
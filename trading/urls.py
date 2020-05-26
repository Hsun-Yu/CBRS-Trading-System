from django.conf.urls import url
from . import views
from django.urls import path, include

urlpatterns = [
    url(r'^$', views.index, name='index'),
    url(r'/login', views.loginView, name='loginView'),
    url(r'/signup', views.signupView, name='signupView'),
    url(r'/logout', views.logoutView, name='logoutView'),
    url(r'/setprice', views.setPriceView, name='setPriceView'),
];